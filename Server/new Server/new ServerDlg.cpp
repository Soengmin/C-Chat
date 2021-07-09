
// new ServerDlg.cpp : 구현 파일
//

#include "stdafx.h"
#include "new Server.h"
#include "new ServerDlg.h"
#include "afxdialogex.h"
#include <time.h>
#include <direct.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


void err_quit(char *msg)
{
	LPVOID lpMsgBuf;
	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, WSAGetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf, 0, NULL);
	MessageBox(NULL, (LPCTSTR)lpMsgBuf, (LPCTSTR)msg, MB_ICONERROR);
	LocalFree(lpMsgBuf);
	exit(1);
}
void err_display(char *msg)
{
	LPVOID lpMsgBuf;
	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, WSAGetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf, 0, NULL);
	printf("[%s] %s", msg, (char *)lpMsgBuf);
	LocalFree(lpMsgBuf);
}

void CnewServerDlg::AddEventString(CString str) {
	while (m_event_list.GetCount() > 300) {
		m_event_list.DeleteString(0);
	}
	int index = m_event_list.InsertString(-1, str);
	m_event_list.SetCurSel(index);
}
// CnewServerDlg 대화 상자

void CnewServerDlg::sendadll(Message_form msg) {
	int retval;
	for (int i = 0; i < MAX_CLIENT; i++) {
		if (client_sock_list[i] != INVALID_SOCKET && client_status[i] == 2) {
			retval = send(client_sock_list[i], (char*)&msg, sizeof(msg), 0);
		}
	}
}

void CnewServerDlg::ProcessClient() {
	int retval;
	SOCKADDR_IN serveraddr;
	int addrlen;
	Message_form buf;
	CString str;
	int i;
	char tmpId[20];
	char tmpPw[500];

	for (i = 0; i < MAX_CLIENT; i++) {
		if (client_sock_list[i] == INVALID_SOCKET) {
			client_sock_list[i] = client_sock;
			client_status[i] = 1; // 1 : 로그인 안된 상태
			break;
		}
	}

	addrlen = sizeof(serveraddr);
	getpeername(client_sock_list[i], (SOCKADDR *)&serveraddr, &addrlen);
	str.Format(_T("[%d] %s 클라이언트 접속"), serveraddr.sin_port, inet_ntoa(serveraddr.sin_addr));
	AddEventString(str);

	FILE *Log = fopen(Logfilepath, "a+");
	// Log 디렉터리에 오늘 날짜로 txt파일을 만든다, 있다면 수정모드로 실행

	fseek(Log, 0, SEEK_END);
	fprintf(Log, "[클라이언트 접속] IP : %s\n", inet_ntoa(serveraddr.sin_addr));
	fclose(Log);

	while (1) {
		retval = recv(client_sock_list[i], (char*)&buf, sizeof(buf), 0);

		/*
		Message Type 0 : 로그인
		1 : 회원가입
		2 : 일반 채팅
		3 : 사용자 목록에 지금 접속한 ID 추가
		4 : 지금 접속 종료한 ID 제거
		*/

		if (retval == SOCKET_ERROR) {
			err_display("recv()");
			break;
		}
		if (buf.type == 0) {
			// 로그인
			FILE *UserData = fopen(UserDataPath, "a+");
			fseek(UserData, 0, SEEK_SET);
			for (; fscanf(UserData, "%s %s\n", tmpId, tmpPw) != EOF;) {
				if ((strcmp(tmpId, buf.id) == 0) && (strcmp(tmpPw, buf.password) == 0)) {
					buf.type = 1; // 로그인 성공
					FILE *Log = fopen(Logfilepath, "a+");
					fseek(Log, 0, SEEK_END);
					fprintf(Log, "[로그인성공] ID : %s\n", buf.id);
					fclose(Log);
					break;
				}
				else {
					buf.type = 0; // 실패
				}
			}
			if (buf.type == 1) {
				// 이미 로그인 된 ID인지?
				retval = user_list.FindString(-1, buf.id); // 못찾으면 -1
				if (retval != -1) {
					buf.type = 2;
				}

				// ban 당한 id인지
				if (is_ban(buf.id)) buf.type = 3;
			}
			if (buf.type == 1) client_status[i] = 2; // 2 : 로그인 된 상태
			retval = send(client_sock_list[i], (char*)&buf, sizeof(buf), 0);
			fclose(UserData);
		}

		else if (buf.type == 1)
		{
			//회원가입
			FILE *UserData = fopen(UserDataPath, "a+");
			fseek(UserData, 0, SEEK_SET);
			for (; fscanf(UserData, "%s %s\n", tmpId, tmpPw) != EOF;) {
				if (strcmp(tmpId, buf.id) == 0) {
					buf.type = 0; // 이미 id가 존재함
				}
			}

			if (buf.type == 1) {
				if (strlen(buf.id) == 0 || strlen(buf.password) == 0) {
					buf.type = 3; // 패스워드 혹은 아이디가 비어있다.
				}
				else {
					for (int i = 0; i < strlen(buf.id); i++) {
						if (buf.id[i] == ' ') {
							buf.type = 2; // id에 공백이 포함되어있다.
						}
					}
					for (int i = 0; i < strlen(buf.password); i++) {
						if (buf.password[i] == ' ') {
							buf.type = 2;
						}
					}
				}
			}

			if (buf.type == 1) {
				str.Format(_T("아이디 : %s 비밀번호 : %s 생성"), buf.id, buf.password);
				buf.type = 1;
				AddEventString(str);
				fseek(UserData, 0, SEEK_END);
				fprintf(UserData, "%s %s\n", buf.id, buf.password);
				
				FILE *Log = fopen(Logfilepath, "a+");
				fseek(Log, 0, SEEK_END);
				fprintf(Log, "[회원가입 성공] ID : %s, IP : %s\n", buf.id, inet_ntoa(serveraddr.sin_addr));
				fclose(Log);
			}
			fclose(UserData);
			retval = send(client_sock_list[i], (char*)&buf, sizeof(buf), 0);
			// type 1 : 회원가입 성공, type 0 : 실패 ID중복 2 : 아이디, 패스워드에 공백포함됨;;
		}

		else if (buf.type == 2) {
			//2 : 채팅 받으면
			++client_chat_count[i];

			str.Format(_T("%s : %s"), buf.id, buf.password);
			AddEventString(str);
			sendadll(buf);
			FILE *Log = fopen(Logfilepath, "a+");
			fseek(Log, 0, SEEK_END);
			fprintf(Log, "%s : %s\n", buf.id, buf.password);
			fclose(Log);

			if (client_chat_count[i] >= 10) {
				buf.type = 15;
				retval = send(client_sock_list[i], (char*)&buf, sizeof(buf), 0);

				//ID Ban 처리
				if (!is_ban(buf.id)) {
					FILE *Banfile = fopen(BanDataPath, "a+");
					fseek(Banfile, 0, SEEK_END);
					fprintf(Banfile, "%s\n", buf.id);
					fclose(Banfile);
					UpdateBanList();
				}
			}
		}
		else if (buf.type == 3) {
			//로그인 후 생성자 받음
		}
		else if (buf.type == 4) {
			//접속 종료한 유저 ID 리스트에서 지우기
			retval = user_list.FindString(-1, buf.id);
			if (retval != -1) {
				user_list.DeleteString(retval);
				sendadll(buf);
				FILE *Log = fopen(Logfilepath, "a+");
				fseek(Log, 0, SEEK_END);
				fprintf(Log, "[로그아웃] ID : %s\n", buf.id);
				fclose(Log);
			}
		}
		else if (buf.type == 5) {
			// 사용자가 채널을 바꾸면 모든 사용자에게 5를 보냄
			sendadll(buf);
			user_list.ResetContent();
		}
		else if (buf.type == 6) {
			sendadll(buf);
			user_list.AddString(buf.id);
			// 채널 사용자 업데이트
		}
		//str.Format(_T("%d"), buf.type);
		//AddEventString(str);
		ZeroMemory(&buf, sizeof(buf));
	}

	client_count--;
	closesocket(client_sock_list[i]);
	client_sock_list[i] = INVALID_SOCKET;
	client_status[i] = 0;
}

void CnewServerDlg::sock_accept() {
	SOCKADDR_IN clientaddr;
	int addrlen;
	// 로그파일 파일 경로 만들기
	time_t t = time(NULL);
	struct tm tm = *localtime(&t);
	sprintf(Logfilepath, "%s%d-%d-%d.txt", LogPath, tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday);
	UpdateBanList();
	Message_form buf;

	while (1) {
		// accept()
		addrlen = sizeof(clientaddr);
		client_sock = accept(listen_sock, (SOCKADDR *)&clientaddr, &addrlen);
		if (client_sock == INVALID_SOCKET) {
			err_quit("accept()");
			break;
		}
		if (client_count == MAX_CLIENT) {
			//접속자수 초과
			buf.type = -1;
			send(client_sock, (char *)&buf, sizeof(buf), 0);
			continue;
		}
		else {
			buf.type = -2;
			send(client_sock, (char *)&buf, sizeof(buf), 0);
		}

		// 스레드 생성
		client_count++;
		VT.push_back(std::thread(&CnewServerDlg::ProcessClient, this));
		VT.back().detach();
	}
	closesocket(listen_sock);
}


CnewServerDlg::CnewServerDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_NEWSERVER_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CnewServerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_event_list);
	DDX_Control(pDX, IDC_USER_LIST, user_list);
	//DDX_Control(pDX, IDC_BAN_LIST, m_ban_list);
	DDX_Control(pDX, IDC_BAN_LIST, m_banned_list);
}

BEGIN_MESSAGE_MAP(CnewServerDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDOK, &CnewServerDlg::OnBnClickedOk)
//	ON_LBN_DBLCLK(IDC_BAN_LIST, &CnewServerDlg::OnDblclkBanList)
ON_LBN_DBLCLK(IDC_USER_LIST, &CnewServerDlg::OnDblclkUserList)
ON_LBN_DBLCLK(IDC_BAN_LIST, &CnewServerDlg::OnDblclkBanList)
ON_BN_CLICKED(IDC_BTN_KICK, &CnewServerDlg::OnBnClickedBtnKick)
ON_BN_CLICKED(IDC_BTN_BAN, &CnewServerDlg::OnBnClickedBtnBan)
ON_BN_CLICKED(IDC_BTN_UNBAN, &CnewServerDlg::OnBnClickedBtnUnban)
END_MESSAGE_MAP()


// CnewServerDlg 메시지 처리기

BOOL CnewServerDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();


	// 이 대화 상자의 아이콘을 설정합니다.  응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	int retval;
	client_count = 0;
	mkdir("C:\\Temp");
	mkdir("C:\\Temp\\Log");
	

	for (int i = 0; i < MAX_CLIENT; i++) {
		client_sock_list[i] = INVALID_SOCKET;
		client_status[i] = 0;
	}
	//socket()
	listen_sock = socket(AF_INET, SOCK_STREAM, 0);
	if (listen_sock == INVALID_SOCKET) err_quit("socket()");
	//bind()
	SOCKADDR_IN serveraddr;
	ZeroMemory(&serveraddr, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = htons(INADDR_ANY);
	serveraddr.sin_port = htons(SERVERPORT);
	retval = bind(listen_sock, (SOCKADDR *)&serveraddr, sizeof(serveraddr));
	if (retval == SOCKET_ERROR) err_quit("bind()");
	//listen()
	retval = listen(listen_sock, SOMAXCONN);
	if (retval == SOCKET_ERROR) err_quit("listen()");
	else AddEventString("서버를 시작합니다.");

	std::thread Thread_accept(&CnewServerDlg::sock_accept, this);
	Thread_accept.detach();
	std::thread Reset_chat(&CnewServerDlg::Reset_ChatCount, this);
	Reset_chat.detach();

	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다.  문서/뷰 모델을 사용하는 MFC 응용 프로그램의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void CnewServerDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트입니다.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 클라이언트 사각형에서 아이콘을 가운데에 맞춥니다.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 아이콘을 그립니다.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
HCURSOR CnewServerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CnewServerDlg::OnBnClickedOk()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	//CDialogEx::OnOK();
}

void CnewServerDlg::OnDblclkUserList()
{
	// 유저리스트박스에서 더블클릭 했을 때
	CString str;
	int n = user_list.GetCurSel();
	user_list.GetText(n, str);
	SetDlgItemText(IDC_INPUT_BAN, str);
}

void CnewServerDlg::OnDblclkBanList()
{
	// Ban 리스트박스에서 더블클릭 했을 때
	CString str;
	int n = m_banned_list.GetCurSel();
	m_banned_list.GetText(n, str);
	SetDlgItemText(IDC_INPUT_BAN, str);
}


void CnewServerDlg::OnBnClickedBtnKick()
{
	// KICK button
	CString tmp;
	Message_form banMessage;
	banMessage.type = 10;
	GetDlgItemText(IDC_INPUT_BAN, tmp);
	strcpy(banMessage.id, tmp);
	strcpy(banMessage.password, "");

	if (IDOK == MessageBox(tmp + "님을 퇴장시키시겠습니까?", "알림", MB_OKCANCEL)) {
		sendadll(banMessage);
		FILE *Log = fopen(Logfilepath, "a+");
		fseek(Log, 0, SEEK_END);
		fprintf(Log, "[KICK] ID : %s\n", banMessage.id);
		fclose(Log);
	}
}

void CnewServerDlg::OnBnClickedBtnBan()
{
	// BAN button
	OnBnClickedBtnKick();
	CString tmp;
	char id[20];
	GetDlgItemText(IDC_INPUT_BAN, tmp);
	strcpy(id, tmp);
	if (!is_ban(id)) {
		FILE *Banfile = fopen(BanDataPath, "a+");
		fseek(Banfile, 0, SEEK_END);
		fprintf(Banfile, "%s\n", id);
		fclose(Banfile);
		UpdateBanList();
	}
}

int CnewServerDlg::is_ban(char id[]) {
	FILE *Banfile = fopen(BanDataPath, "a+");
	fseek(Banfile, 0, SEEK_SET);
	char banid[20];

	for (; fscanf(Banfile, "%s\n", banid) != EOF;) {
		if (strcmp(id, banid) == 0) {
			fclose(Banfile);
			return 1;
		}
	}
	fclose(Banfile);
	return 0;
}

void CnewServerDlg::UpdateBanList() {
	m_banned_list.ResetContent();
	FILE *Banfile = fopen(BanDataPath, "a+");
	fseek(Banfile, 0, SEEK_SET);
	char banid[20];
	CString tmp;
	for (; fscanf(Banfile, "%s\n", banid) != EOF;) {
		tmp.Format(_T("%s"), banid);
		m_banned_list.InsertString(-1, tmp);
	}
	fclose(Banfile);
}

void CnewServerDlg::OnBnClickedBtnUnban()
{
	// UNBAN btn
	CString tmp;
	GetDlgItemText(IDC_INPUT_BAN, tmp);
	char unbanid[20];
	strcpy(unbanid, tmp);
	char banid[20];

	FILE *Banfile = fopen(BanDataPath, "r");
	fseek(Banfile, 0, SEEK_SET);
	FILE *tempFile = fopen("c:\\Temp\\tmp.txt", "w+");

	for (; fscanf(Banfile, "%s\n", banid) != EOF;) {
		if (strcmp(banid, unbanid) != 0) fprintf(tempFile, "%s\n", banid);
	}

	fclose(Banfile);
	Banfile = fopen(BanDataPath, "w+");
	fseek(tempFile, 0, SEEK_SET);

	for (; fscanf(tempFile, "%s\n", banid) != EOF;) {
		fprintf(Banfile, "%s\n", banid);
	}
	fclose(Banfile);
	fclose(tempFile);
	remove("c:\\Temp\\tmp.txt");
	UpdateBanList();
}

void CnewServerDlg::Reset_ChatCount() {
	int Update_user = 0;
	Message_form UpdateUser;
	UpdateUser.type = 5;
	while (true) {
		for (int i = 0; i < MAX_CLIENT; i++) client_chat_count[i] = 0;
		Sleep(3000);
		Update_user++;
		if (Update_user >= 10) {
			sendadll(UpdateUser);
			Update_user = 0;
			user_list.ResetContent();
			AddEventString("유저 목록 업데이트");
		}
	}
}
