#pragma once
#include "New ClientDlg.h"

// CJoinDlg 대화 상자입니다.

class CJoinDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CJoinDlg)

public:
	CJoinDlg(CNewClientDlg* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CJoinDlg();
	Message_form join_buf;
	
private:
	CNewClientDlg *mParent;

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_JOIN };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedIdInput();
	void end();
	BOOL PreTranslateMessage(MSG * pMsg);
	CString JoinId;
};
