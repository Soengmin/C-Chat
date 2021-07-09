
// New ClientDlg.h : ��� ����
//

#pragma once
#include "afxwin.h"
#define SERVERPORT 9009

struct Message_form {
	int type;
	char id[20];
	char password[500];
	char channel[20] = "main";
};
// CNewClientDlg ��ȭ ����
class CNewClientDlg : public CDialogEx
{
// �����Դϴ�.
	
public:
	Message_form buf;
	SOCKET server_sock;
	CNewClientDlg(CWnd* pParent = NULL);	// ǥ�� �������Դϴ�.


// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_NEWCLIENT_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV �����Դϴ�.


// �����Դϴ�.
protected:
	HICON m_hIcon;

	// ������ �޽��� �� �Լ�
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnEnChangeEdit1();
	CStatic idc_status;
	afx_msg void OnBnClickedLogin();
	afx_msg void OnBnClickedAddUser();
	BOOL PreTranslateMessage(MSG * pMsg);
};
