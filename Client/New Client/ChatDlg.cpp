// ChatDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "New Client.h"
#include "ChatDlg.h"
#include "afxdialogex.h"
#include "New ClientDlg.h"


// CChatDlg 대화 상자입니다.

void err_quit2(char *msg) {
	LPVOID lpMsgBuf;
	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM, NULL, WSAGetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPTSTR)&lpMsgBuf, 0, NULL);
	MessageBox(NULL, (LPCTSTR)lpMsgBuf, (LPCTSTR)msg, MB_ICONERROR);
	LocalFree(lpMsgBuf);
	exit(1);
}

IMPLEMENT_DYNAMIC(CChatDlg, CDialogEx)

CChatDlg::CChatDlg(CNewClientDlg* pParent /*=NULL*/)
	: CDialogEx(IDD_CHATDLG, pParent)
	, m_edit_ch(_T(""))
{
	mParent = (CNewClientDlg *)::AfxGetMainWnd();
	chat_sock = mParent->server_sock;
	strcpy(chat_buf.id, mParent->buf.id);
	chat_buf.type = 3;  // 접속중인 id 추가
	send(chat_sock, (char*)&chat_buf, sizeof(chat_buf), 0);

	std::thread receiveT(&CChatDlg::receive_thread, this);
	receiveT.detach();
}

CChatDlg::~CChatDlg()
{
	strcpy(chat_buf.id, mParent->buf.id);
	chat_buf.type = 4; // 접속자목록에서 지워라
	send(chat_sock, (char*)&chat_buf, sizeof(chat_buf), 0);
	exit(0);
}

void CChatDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_event_list);
	DDX_Control(pDX, IDC_USER_LIST, user_list);
	DDX_Text(pDX, IDC_EDIT_JOIN, m_edit_ch);
	DDV_MaxChars(pDX, m_edit_ch, 18);
	DDX_Control(pDX, IDC_IG_LIST, ig_list);
}


BEGIN_MESSAGE_MAP(CChatDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CChatDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BTN_SEND, &CChatDlg::OnBnClickedBtnSend)
	ON_BN_CLICKED(IDC_BTN_IGNORE, &CChatDlg::OnBnClickedBtnIgnore)
	ON_BN_CLICKED(IDC_BTN_IGLIST, &CChatDlg::OnBnClickedBtnIglist)
	ON_BN_CLICKED(IDC_NOTICE, &CChatDlg::OnBnClickedNotice)
	ON_LBN_DBLCLK(IDC_USER_LIST, &CChatDlg::OnDblclkUserList)
	ON_BN_CLICKED(IDC_JOIN_CH, &CChatDlg::OnBnClickedJoinCh)
END_MESSAGE_MAP()


// CChatDlg 메시지 처리기입니다.


void CChatDlg::OnBnClickedOk()
{
	//CDialogEx::OnOK();
	OnBnClickedBtnSend();
}

void CChatDlg::OnBnClickedBtnSend()
{
	CString tmp;
	int retval;
	chat_buf.type = 2;
	strcpy(chat_buf.id, mParent->buf.id);
	GetDlgItemText(IDC_TEXT_EDIT, tmp);
	strcpy(chat_buf.password, tmp.GetBuffer(0));
	SetDlgItemText(IDC_TEXT_EDIT, "");
	strcpy(chat_buf.channel, current_ch);

	retval = send(chat_sock, (char*)&chat_buf, sizeof(chat_buf), 0);
}

BOOL CChatDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: Add your specialized code here and/or call the base class
	if (pMsg->message == WM_KEYDOWN) {
		if (pMsg->wParam == VK_RETURN)
		{
			OnBnClickedBtnSend();
			return TRUE;
		}
	}

	return CDialog::PreTranslateMessage(pMsg);
}

void CChatDlg::receive_thread() {
	int retval;
	CString str;
	Sleep(500);
	OnBnClickedJoinCh();
	while (1) {
		retval = recv(chat_sock, (char*)&chat_buf, sizeof(chat_buf), 0);
		if (retval == SOCKET_ERROR) err_quit2("recv()");
		if (chat_buf.type == 2 && !is_ignore(chat_buf.id) && strcmp(chat_buf.channel, current_ch) == 0) {
			str.Format(_T("%s : %s"), chat_buf.id, chat_buf.password);
			AddEventString(str);
		}
		else if (chat_buf.type == 3 && strcmp(current_ch, "main") == 0) {
			retval = user_list.FindString(-1, chat_buf.id);
			// 접속자 유저 리스트박스에 찾는 id가없으면 추가
			if (retval == -1) user_list.AddString(chat_buf.id);
		}
		else if (chat_buf.type == 4) {
			// 접속해제한 유저 리스트에서 지우기
			retval = user_list.FindString(-1, chat_buf.id);
			if (retval != -1) user_list.DeleteString(retval);
		}
		else if (chat_buf.type == 10) {
			//서버가 퇴장시키려는 클라이언트를 찾습니다.
			if (strcmp(chat_buf.id, mParent->buf.id) == 0) {
				MessageBox("서버에 의해 퇴장당했습니다.");
				this->~CChatDlg();
			}
		}
		else if (chat_buf.type == 15) {
			MessageBox("도배로 인해 ID Ban처리 됩니다.");
			this->~CChatDlg();
		}
		else if (chat_buf.type == 5) {
			//5를 받을 경우 자신의 id와 채널을 다시 서버로 보냄
			user_list.ResetContent();
			chat_buf.type = 6;
			strcpy(chat_buf.id, mParent->buf.id);
			strcpy(chat_buf.channel, current_ch);

			retval = send(chat_sock, (char*)&chat_buf, sizeof(chat_buf), 0);
		}
		else if (chat_buf.type == 6) {
			if (strcmp(chat_buf.channel, current_ch) == 0) {
				retval = user_list.FindString(-1, chat_buf.id);
				if (retval == -1) user_list.AddString(chat_buf.id);
			}
		}
	}
}

void CChatDlg::AddEventString(CString str) {
	while (m_event_list.GetCount() > 300) {
		m_event_list.DeleteString(0);
	}
	int index = m_event_list.InsertString(-1, str);
	m_event_list.SetCurSel(index);
}


void CChatDlg::OnBnClickedBtnIgnore()
{
	// 무시 버튼 눌렀을 경우
	CString str;
	char tmp[20];
	int n = user_list.GetCurSel(); // 0부터 시작, 선택된게 없으면 -1
	if (n != -1) {
		user_list.GetText(n, str);
		strcpy(tmp, str);
	}
	if (n == -1) MessageBox("대상을 선택해주세요");
	else if (is_ignore(tmp)) {
		MessageBox(str + "님은 이미 무시중입니다");
	}
	else {
		igList.push_back(str);
		ig_list.InsertString(-1, str);
		MessageBox(str + "님을 무시합니다");
	}
}

int CChatDlg::is_ignore(char id[]) {
	std::list<CString>::iterator it;
	CString Stid;
	Stid.Format(_T("%s"), id);

	for (it = igList.begin(); it != igList.end(); it++) {
		if (Stid == *it) return 1;
	}

	return 0;
}

void CChatDlg::OnBnClickedBtnIglist()
{
	// 차단해제
	std::list<CString>::iterator it;
	CString str;
	int n = ig_list.GetCurSel(); // 0부터 시작, 선택된게 없으면 -1

	if (n == -1) MessageBox("차단 목록에서 대상을 선택해주세요");
	else {
		ig_list.GetText(n, str);
		it = igList.begin();
		while (it != igList.end()) {
			if (*it == str) {
				igList.erase(it++);
				ig_list.DeleteString(n);
				MessageBox(str + "님을 더 이상 무시하지 않습니다");
			}
			else it++;
		}
	}
}


void CChatDlg::OnBnClickedNotice()
{
	MessageBox("                                      NOTICE                                        \n1. 도배 시 ID 차단 됩니다.\n * 도배기준 : 3초에 10회 채팅\n2. 사용자 ID클릭 후 차단/해제로 해당 유저의 채팅 차단\n3. 차단관련문의 Email : aeternussm@gmail.com\n4. 초기 채팅 채널명은 main입니다.","팀 얼리버드");
}



void CChatDlg::OnDblclkUserList()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void CChatDlg::OnBnClickedJoinCh()
{
	CString str;
	int retval;

	GetDlgItemText(IDC_EDIT_JOIN, current_ch);
	if (strlen(current_ch) == 0) current_ch = "main";

	str = current_ch + " 채널에 접속하셨습니다.";
	AddEventString(str);

	chat_buf.type = 5;
	strcpy(chat_buf.id, mParent->buf.id);
	strcpy(chat_buf.channel, current_ch);
	retval = send(chat_sock, (char*)&chat_buf, sizeof(chat_buf), 0);
}
