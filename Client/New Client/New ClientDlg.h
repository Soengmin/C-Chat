
// New ClientDlg.h : 헤더 파일
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
// CNewClientDlg 대화 상자
class CNewClientDlg : public CDialogEx
{
// 생성입니다.
	
public:
	Message_form buf;
	SOCKET server_sock;
	CNewClientDlg(CWnd* pParent = NULL);	// 표준 생성자입니다.


// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_NEWCLIENT_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.


// 구현입니다.
protected:
	HICON m_hIcon;

	// 생성된 메시지 맵 함수
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
