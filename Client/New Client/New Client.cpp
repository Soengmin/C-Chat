
// New Client.cpp : ���� ���α׷��� ���� Ŭ���� ������ �����մϴ�.
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


// CNewClientApp ����

CNewClientApp::CNewClientApp()
{
	// TODO: ���⿡ ���� �ڵ带 �߰��մϴ�.
	// InitInstance�� ��� �߿��� �ʱ�ȭ �۾��� ��ġ�մϴ�.
}


// ������ CNewClientApp ��ü�Դϴ�.

CNewClientApp theApp;


// CNewClientApp �ʱ�ȭ

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

