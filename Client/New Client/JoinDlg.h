#pragma once
#include "New ClientDlg.h"

// CJoinDlg ��ȭ �����Դϴ�.

class CJoinDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CJoinDlg)

public:
	CJoinDlg(CNewClientDlg* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CJoinDlg();
	Message_form join_buf;
	
private:
	CNewClientDlg *mParent;

// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_JOIN };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedIdInput();
	void end();
	BOOL PreTranslateMessage(MSG * pMsg);
	CString JoinId;
};
