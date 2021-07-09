#pragma once
#include "New ClientDlg.h"
#include "afxwin.h"
#include "Resource.h"
#include <thread>
#include <list>

// CChatDlg 대화 상자입니다.

class CChatDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CChatDlg)


// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CHATDLG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	CStatic idc_my_id;
	CNewClientDlg *mParent;
	std::list<CString> igList;
	CString current_ch = "main";

public:
	CChatDlg(CNewClientDlg* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CChatDlg();
	SOCKET chat_sock;
	Message_form chat_buf;
	afx_msg void OnBnClickedBtnSend();
	BOOL PreTranslateMessage(MSG * pMsg);
	void receive_thread();
	void AddEventString(CString str);
private:
	CListBox m_event_list;
public:
	afx_msg void OnStnClickedMyId();
private:
	CListBox user_list;
public:
	afx_msg void OnBnClickedBtnIgnore();
	int is_ignore(char id[]);
	afx_msg void OnBnClickedBtnIglist();
	afx_msg void OnBnClickedNotice();
	afx_msg void OnDblclkUserList();
	afx_msg void OnBnClickedJoinCh();
	CString m_edit_ch;
	CListBox ig_list;
};
