
// new Server.cpp : ���� ���α׷��� ���� Ŭ���� ������ �����մϴ�.
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


// CnewServerApp ����

CnewServerApp::CnewServerApp()
{
	// TODO: ���⿡ ���� �ڵ带 �߰��մϴ�.
	// InitInstance�� ��� �߿��� �ʱ�ȭ �۾��� ��ġ�մϴ�.
}


// ������ CnewServerApp ��ü�Դϴ�.

CnewServerApp theApp;


// CnewServerApp �ʱ�ȭ

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

