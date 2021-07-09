
// New ClientDlg.cpp : ���� ����
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

// CNewClientDlg ��ȭ ����



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


// CNewClientDlg �޽��� ó����

BOOL CNewClientDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// �� ��ȭ ������ �������� �����մϴ�.  ���� ���α׷��� �� â�� ��ȭ ���ڰ� �ƴ� ��쿡��
	//  �����ӿ�ũ�� �� �۾��� �ڵ����� �����մϴ�.
	SetIcon(m_hIcon, TRUE);			// ū �������� �����մϴ�.
	SetIcon(m_hIcon, FALSE);		// ���� �������� �����մϴ�.

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
	else idc_status.SetWindowTextA("������ ����Ǿ����ϴ�");

	recv(server_sock, (char *)&buf, sizeof(buf), 0);
	if (buf.type == -1) {
		MessageBox("�ִ� ���� ���� Ŭ���̾�Ʈ�� �ʰ�");
		exit(0);
	}
	// TODO: ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.

	return TRUE;  // ��Ŀ���� ��Ʈ�ѿ� �������� ������ TRUE�� ��ȯ�մϴ�.
}

// ��ȭ ���ڿ� �ּ�ȭ ���߸� �߰��� ��� �������� �׸�����
//  �Ʒ� �ڵ尡 �ʿ��մϴ�.  ����/�� ���� ����ϴ� MFC ���� ���α׷��� ��쿡��
//  �����ӿ�ũ���� �� �۾��� �ڵ����� �����մϴ�.

void CNewClientDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // �׸��⸦ ���� ����̽� ���ؽ�Ʈ�Դϴ�.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Ŭ���̾�Ʈ �簢������ �������� ����� ����ϴ�.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// �������� �׸��ϴ�.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// ����ڰ� �ּ�ȭ�� â�� ���� ���ȿ� Ŀ���� ǥ�õǵ��� �ý��ۿ���
//  �� �Լ��� ȣ���մϴ�.
HCURSOR CNewClientDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CNewClientDlg::OnBnClickedOk()
{
	// ���� ġ��?
	OnBnClickedLogin();
	//CDialogEx::OnOK();
}


void CNewClientDlg::OnEnChangeEdit1()
{
	// TODO:  RICHEDIT ��Ʈ���� ���, �� ��Ʈ����
	// CDialogEx::OnInitDialog() �Լ��� ������ 
	//�ϰ� ����ũ�� OR �����Ͽ� ������ ENM_CHANGE �÷��׸� �����Ͽ� CRichEditCtrl().SetEventMask()�� ȣ������ ������
	// �� �˸� �޽����� ������ �ʽ��ϴ�.

	// TODO:  ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
}


void CNewClientDlg::OnBnClickedLogin()
{
	//�α��� ��ư Ŭ�� ��
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
		MessageBox("�α��� ����");
		PostMessage(WM_SHOWWINDOW, FALSE, SW_OTHERUNZOOM);
		CChatDlg ChatDlg;
		ChatDlg.DoModal();

		ChatDlg.DestroyWindow();

	}
	else if (buf.type == 0) {
		MessageBox("�α��� ����");
	}
	else if (buf.type == 2) {
		MessageBox("�̹� ���ӵ� �����Դϴ�.");
	}
	else if (buf.type == 3) {
		MessageBox("������ ������ ID�Դϴ�.");
	}
}


void CNewClientDlg::OnBnClickedAddUser()
{
	// ȸ������ ��ư ������
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
		MessageBox("�ߺ��Ǵ� ID�� �ֽ��ϴ�");
	}
	else if (buf.type == 1) {
		MessageBox("ȸ������ ����");
	}
	else if (buf.type == 2) {
		MessageBox("ID, password�� ������ ���Ե� �� �����ϴ�");
	}
	else if (buf.type == 3) {
		MessageBox("ID, password�� �Է����ּ���");
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