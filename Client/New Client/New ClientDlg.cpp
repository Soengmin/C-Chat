
// New ClientDlg.cpp : 구현 파일
//

#include "stdafx.h"
#include "New Client.h"
#include "New ClientDlg.h"
#include "afxdialogex.h"
#include "JoinDlg.h"
#include "ChatDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

void err_quit(char *msg) {
	LPVOID lpMsgBuf;
	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM, NULL, WSAGetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPTSTR)&lpMsgBuf, 0, NULL);
	MessageBox(NULL, (LPCTSTR)lpMsgBuf, (LPCTSTR)msg, MB_ICONERROR);
	LocalFree(lpMsgBuf);
	exit(1);
}

// CNewClientDlg 대화 상자



CNewClientDlg::CNewClientDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_NEWCLIENT_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CNewClientDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATUS, idc_status);
}

BEGIN_MESSAGE_MAP(CNewClientDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDOK, &CNewClientDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_LOGIN, &CNewClientDlg::OnBnClickedLogin)
	ON_BN_CLICKED(IDC_ADD_USER, &CNewClientDlg::OnBnClickedAddUser)
END_MESSAGE_MAP()


// CNewClientDlg 메시지 처리기

BOOL CNewClientDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 이 대화 상자의 아이콘을 설정합니다.  응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	int retval;
	// socket()
	server_sock = socket(AF_INET, SOCK_STREAM, 0);
	if (server_sock == INVALID_SOCKET) err_quit("socket()");

	// connect()
	SOCKADDR_IN serveraddr;
	ZeroMemory(&serveraddr, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = inet_addr("211.221.117.218");
	serveraddr.sin_port = htons(SERVERPORT);
	retval = connect(server_sock, (SOCKADDR*)&serveraddr, sizeof(serveraddr));
	if (retval == SOCKET_ERROR) err_quit("connect()");
	else idc_status.SetWindowTextA("서버에 연결되었습니다");

	recv(server_sock, (char *)&buf, sizeof(buf), 0);
	if (buf.type == -1) {
		MessageBox("최대 접속 가능 클라이언트수 초과");
		exit(0);
	}
	// TODO: 여기에 추가 초기화 작업을 추가합니다.

	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다.  문서/뷰 모델을 사용하는 MFC 응용 프로그램의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void CNewClientDlg::OnPaint()
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
HCURSOR CNewClientDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CNewClientDlg::OnBnClickedOk()
{
	// 엔터 치면?
	OnBnClickedLogin();
	//CDialogEx::OnOK();
}


void CNewClientDlg::OnEnChangeEdit1()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialogEx::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void CNewClientDlg::OnBnClickedLogin()
{
	//로그인 버튼 클릭 시
	int retval;
	buf.type = 0;
	CString tmp;
	GetDlgItemText(IDC_ID_INPUT, tmp);
	strcpy(buf.id, tmp.GetBuffer(0));
	GetDlgItemText(IDC_PWD_INPUT, tmp);
	strcpy(buf.password, tmp.GetBuffer(0));
	retval = send(server_sock, (char*)&buf, sizeof(buf), 0);
	if (retval == SOCKET_ERROR) err_quit("send()");

	retval = recv(server_sock, (char*)&buf, sizeof(buf), 0);
	if (buf.type == 1) {
		MessageBox("로그인 성공");
		PostMessage(WM_SHOWWINDOW, FALSE, SW_OTHERUNZOOM);
		CChatDlg ChatDlg;
		ChatDlg.DoModal();

		ChatDlg.DestroyWindow();

	}
	else if (buf.type == 0) {
		MessageBox("로그인 실패");
	}
	else if (buf.type == 2) {
		MessageBox("이미 접속된 계정입니다.");
	}
	else if (buf.type == 3) {
		MessageBox("입장이 금지된 ID입니다.");
	}
}


void CNewClientDlg::OnBnClickedAddUser()
{
	// 회원가입 버튼 누르면
	int retval;
	CJoinDlg joinDlg;
	joinDlg.DoModal();

	buf.type = joinDlg.join_buf.type;
	strcpy(buf.id, joinDlg.join_buf.id);
	strcpy(buf.password, joinDlg.join_buf.password);


	retval = send(server_sock, (char*)&buf, sizeof(buf), 0);
	if (retval == SOCKET_ERROR) err_quit("send()");
	
	retval = recv(server_sock, (char*)&buf, sizeof(buf), 0);

	if (buf.type == 0) {
		MessageBox("중복되는 ID가 있습니다");
	}
	else if (buf.type == 1) {
		MessageBox("회원가입 성공");
	}
	else if (buf.type == 2) {
		MessageBox("ID, password에 공백이 포함될 수 없습니다");
	}
	else if (buf.type == 3) {
		MessageBox("ID, password를 입력해주세요");
	}
	joinDlg.DestroyWindow();

}

BOOL CNewClientDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: Add your specialized code here and/or call the base class
	if (pMsg->message == WM_KEYDOWN) {
		if (pMsg->wParam == VK_RETURN)
		{
			OnBnClickedLogin();
			return TRUE;
		}
	}

	return CDialog::PreTranslateMessage(pMsg);
}