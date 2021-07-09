
// New Client.cpp : 응용 프로그램에 대한 클래스 동작을 정의합니다.
//

#include "stdafx.h"
#include "New Client.h"
#include "New ClientDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CNewClientApp

BEGIN_MESSAGE_MAP(CNewClientApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CNewClientApp 생성

CNewClientApp::CNewClientApp()
{
	// TODO: 여기에 생성 코드를 추가합니다.
	// InitInstance에 모든 중요한 초기화 작업을 배치합니다.
}


// 유일한 CNewClientApp 개체입니다.

CNewClientApp theApp;


// CNewClientApp 초기화

BOOL CNewClientApp::InitInstance()
{
	CWinApp::InitInstance();
	WSADATA wsa;
	WSAStartup(MAKEWORD(2, 2), &wsa);

	CNewClientDlg dlg;
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();

	WSACleanup();
	return FALSE;
}

