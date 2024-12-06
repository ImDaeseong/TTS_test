#include "pch.h"
#include "SapiManager.h"

SapiManager::SapiManager() : m_pVoice(nullptr), m_cpOutAudio(nullptr)
{
}

SapiManager::~SapiManager()
{
    DestroyVoice();
}

//초기화
BOOL SapiManager::CreateVoice()
{
    // COM 라이브러리 초기화
    HRESULT hr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED);
    if (FAILED(hr))
    {
        return FALSE;
    }

    // 음성 합성 객체 생성
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

    // 기본 오디오 출력 장치 생성
    hr = SpCreateDefaultObjectFromCategoryId(SPCAT_AUDIOOUT, &m_cpOutAudio);
    if (FAILED(hr))
    {
        DestroyVoice();
        return FALSE;
    }

    // 오디오 출력 설정
    hr = m_pVoice->SetOutput(m_cpOutAudio, TRUE);
    if (FAILED(hr))
    {
        DestroyVoice();
        return FALSE;
    }

    return TRUE;
}

//해제
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

//라이브러리 사용 가능 여부
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

//음성 호출
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

    // 속도 조절 (-10 ~ 10)
    nRate = max(-10, min(nRate, 10));

    // 음높이 조절 (-10 ~ 10)
    nPitch = max(-10, min(nPitch, 10));

    //속도, 음높이 조절
    const CString strTemp =
        _T("<speak>")
        _T("<prosody rate='%d' pitch='%d'>%s</prosody>")
        _T("</speak>");

    //최종 결과
    CString strOutput;
    strOutput.Format(strTemp, nRate, nPitch, strMessage);

    HRESULT hr = m_pVoice->Speak(strOutput, SPF_IS_XML, NULL);
    if (FAILED(hr))
    {
        return FALSE;
    }
   
    return TRUE;
}

//음성 속도
BOOL SapiManager::SetSpeechRate(int nRate)
{
    //기본 속도: 0
    //느린 속도: -10 ~ -1
    //빠른 속도: 1 ~ 10
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
