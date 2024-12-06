#include "pch.h"
#include "framework.h"
#include "MFCApplication1.h"
#include "MFCApplication1Dlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CMFCApplication1Dlg::CMFCApplication1Dlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_MFCAPPLICATION1_DIALOG, pParent)
{
}

void CMFCApplication1Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CMFCApplication1Dlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BUTTON1, &CMFCApplication1Dlg::OnBnClickedButton1)
END_MESSAGE_MAP()

BOOL CMFCApplication1Dlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	
	return TRUE;  
}

void CMFCApplication1Dlg::OnPaint()
{
	CPaintDC dc(this);
}

void CMFCApplication1Dlg::OnDestroy()
{
	CDialogEx::OnDestroy();
}

void CMFCApplication1Dlg::OnBnClickedButton1()
{
    SapiManager sapiManager;

    if (sapiManager.CreateVoice())
    {
        // 단순 음성 출력
        sapiManager.SetSpeak(_T("안녕하세요."));

        // 속도와 음높이 설정
        sapiManager.SetSpeakA(_T("안녕하세요."), 3, -2);

        // 속도 변경 테스트
        sapiManager.SetSpeechRate(5);

        sapiManager.SetSpeak(_T("안녕하세요."));

        sapiManager.DestroyVoice();
    }
}
