
// new Server.cpp : 응용 프로그램에 대한 클래스 동작을 정의합니다.
//

#include "stdafx.h"
#include "new Server.h"
#include "new ServerDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CnewServerApp

BEGIN_MESSAGE_MAP(CnewServerApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CnewServerApp 생성

CnewServerApp::CnewServerApp()
{
	// TODO: 여기에 생성 코드를 추가합니다.
	// InitInstance에 모든 중요한 초기화 작업을 배치합니다.
}


// 유일한 CnewServerApp 개체입니다.

CnewServerApp theApp;


// CnewServerApp 초기화

BOOL CnewServerApp::InitInstance()
{
	CWinApp::InitInstance();

	WSADATA wsa;
	WSAStartup(MAKEWORD(2, 2), &wsa);

	CnewServerDlg dlg;
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();

	WSACleanup();
	return FALSE;
}

