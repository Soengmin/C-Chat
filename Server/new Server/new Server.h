
// new Server.h : PROJECT_NAME ���� ���α׷��� ���� �� ��� �����Դϴ�.
//

#pragma once

#ifndef __AFXWIN_H__
	#error "PCH�� ���� �� ������ �����ϱ� ���� 'stdafx.h'�� �����մϴ�."
#endif

#include "resource.h"		// �� ��ȣ�Դϴ�.


// CnewServerApp:
// �� Ŭ������ ������ ���ؼ��� new Server.cpp�� �����Ͻʽÿ�.
//

class CnewServerApp : public CWinApp
{
public:
	CnewServerApp();

// �������Դϴ�.
public:
	virtual BOOL InitInstance();

// �����Դϴ�.

	DECLARE_MESSAGE_MAP()
};

extern CnewServerApp theApp;