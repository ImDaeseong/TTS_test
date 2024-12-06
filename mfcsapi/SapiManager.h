#pragma once

#pragma warning(disable:4996)

#include <atlbase.h>
#include <atlcom.h>
#include <vector>

#include <sapi.h>
#include <sphelper.h>
#pragma comment(lib, "sapi.lib")


class SapiManager
{
public:
    SapiManager();
    ~SapiManager();

    BOOL CreateVoice();
    void DestroyVoice();
    BOOL IsExistVoice();
    BOOL SetSpeak(CString strMessage);
    BOOL SetSpeakA(CString strMessage, int nRate = 0, int nPitch = 0);
    BOOL SetSpeechRate(int nRate);
    
private:
    CComPtr<ISpVoice> m_pVoice;
    CComPtr<ISpAudio> m_cpOutAudio;
    CCriticalSection m_cs;
};

