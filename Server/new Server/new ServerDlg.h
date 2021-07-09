
// new ServerDlg.h : 헤더 파일
//

#pragma once
#include "afxwin.h"
#define SERVERPORT 9009
#define MAX_CLIENT 25
#define UserDataPath "c:\\Temp\\UserData.txt"
#define BanDataPath "c:\\Temp\\BanData.txt"
#define LogPath "c:\\Temp\\Log\\"


struct Message_form {
	int type;
	char id[20];
	char password[500];
	char channel[20];
};



// CnewServerDlg 대화 상자
class CnewServerDlg : public CDialogEx
{
// 생성입니다.
public:
	void AddEventString(CString str);
	void sendadll(Message_form msg);
	void ProcessClient();
	void sock_accept();
	CnewServerDlg(CWnd* pParent = NULL);	// 표준 생성자입니다.
	

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_NEWSERVER_DIALOG };
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
private:
	SOCKET client_sock_list[MAX_CLIENT];
	int client_status[MAX_CLIENT];
	std::vector<std::thread> VT;
	CListBox m_event_list;
	SOCKET listen_sock;
	SOCKET client_sock;
	int client_count;
	CListBox user_list;
	char Logfilepath[50];
public:
	CListBox m_ban_list;
	afx_msg void OnDblclkUserList();
	afx_msg void OnDblclkBanList();
	afx_msg void OnBnClickedBtnKick();
	afx_msg void OnBnClickedBtnBan();
	int is_ban(char id[]);
	void UpdateBanList();
	CListBox m_banned_list;
	afx_msg void OnBnClickedBtnUnban();
	void Reset_ChatCount();
	int client_chat_count[MAX_CLIENT];
};
