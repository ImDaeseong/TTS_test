#include "pch.h"
#include "SapiManager.h"

SapiManager::SapiManager() : m_pVoice(nullptr), m_cpOutAudio(nullptr)
{
}

SapiManager::~SapiManager()
{
    DestroyVoice();
}

//�ʱ�ȭ
BOOL SapiManager::CreateVoice()
{
    // COM ���̺귯�� �ʱ�ȭ
    HRESULT hr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED);
    if (FAILED(hr))
    {
        return FALSE;
    }

    // ���� �ռ� ��ü ����
    hr = CoCreateInstance(
        CLSID_SpVoice,
        NULL,
        CLSCTX_ALL,
        IID_ISpVoice,
        (void**)&m_pVoice
    );

    if (FAILED(hr))
    {
        CoUninitialize();
        return FALSE;
    }

    // �⺻ ����� ��� ��ġ ����
    hr = SpCreateDefaultObjectFromCategoryId(SPCAT_AUDIOOUT, &m_cpOutAudio);
    if (FAILED(hr))
    {
        DestroyVoice();
        return FALSE;
    }

    // ����� ��� ����
    hr = m_pVoice->SetOutput(m_cpOutAudio, TRUE);
    if (FAILED(hr))
    {
        DestroyVoice();
        return FALSE;
    }

    return TRUE;
}

//����
void SapiManager::DestroyVoice()
{
    CSingleLock lock(&m_cs, TRUE);

    if (m_pVoice)
    {
        m_pVoice.Release();
        m_pVoice = nullptr;
    }

    if (m_cpOutAudio)
    {
        m_cpOutAudio.Release();
        m_cpOutAudio = nullptr;
    }

    CoUninitialize();
}

//���̺귯�� ��� ���� ����
BOOL SapiManager::IsExistVoice()
{
    CSingleLock lock(&m_cs, TRUE);

    if (!m_pVoice)
    {
        CComPtr<ISpVoice> testVoice;
        HRESULT hr = testVoice.CoCreateInstance(CLSID_SpVoice);
        return SUCCEEDED(hr);
    }
    return TRUE;
}

//���� ȣ��
BOOL SapiManager::SetSpeak(CString strMessage)
{
    CSingleLock lock(&m_cs, TRUE);

    if (!m_pVoice)
    {
        return FALSE;
    }

    HRESULT hr = m_pVoice->Speak(strMessage, SPF_IS_XML, NULL);
    if (FAILED(hr))
    {
        return FALSE;
    }

    return TRUE;
}

BOOL SapiManager::SetSpeakA(CString strMessage, int nRate, int nPitch)
{
    CSingleLock lock(&m_cs, TRUE);

    if (!m_pVoice)
    {
        return FALSE;
    }

    // �ӵ� ���� (-10 ~ 10)
    nRate = max(-10, min(nRate, 10));

    // ������ ���� (-10 ~ 10)
    nPitch = max(-10, min(nPitch, 10));

    //�ӵ�, ������ ����
    const CString strTemp =
        _T("<speak>")
        _T("<prosody rate='%d' pitch='%d'>%s</prosody>")
        _T("</speak>");

    //���� ���
    CString strOutput;
    strOutput.Format(strTemp, nRate, nPitch, strMessage);

    HRESULT hr = m_pVoice->Speak(strOutput, SPF_IS_XML, NULL);
    if (FAILED(hr))
    {
        return FALSE;
    }
   
    return TRUE;
}

//���� �ӵ�
BOOL SapiManager::SetSpeechRate(int nRate)
{
    //�⺻ �ӵ�: 0
    //���� �ӵ�: -10 ~ -1
    //���� �ӵ�: 1 ~ 10
    CSingleLock lock(&m_cs, TRUE);

    if (!m_pVoice)
    {
        return FALSE;
    }

    HRESULT hr = m_pVoice->SetRate(nRate);
    if (FAILED(hr))
    {
        return FALSE;
    }

    return TRUE;
}
