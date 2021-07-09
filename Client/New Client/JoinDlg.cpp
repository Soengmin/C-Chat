// JoinDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "New Client.h"
#include "JoinDlg.h"
#include "afxdialogex.h"


// CJoinDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CJoinDlg, CDialogEx)

CJoinDlg::CJoinDlg(CNewClientDlg* pParent /*=NULL*/)
	: CDialogEx(IDD_JOIN, pParent)
	, JoinId(_T(""))
{
	mParent = pParent;

}

CJoinDlg::~CJoinDlg()
{

}

void CJoinDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_JOIN_ID, JoinId);
	DDV_MaxChars(pDX, JoinId, 10);
}


BEGIN_MESSAGE_MAP(CJoinDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CJoinDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_ID_INPUT, &CJoinDlg::OnBnClickedIdInput)
END_MESSAGE_MAP()


// CJoinDlg 메시지 처리기입니다.


void CJoinDlg::OnBnClickedOk()
{
	//
}


void CJoinDlg::OnBnClickedIdInput()
{
	CString tmp;
	join_buf.type = 1;
	GetDlgItemText(IDC_JOIN_ID, tmp);
	strcpy(join_buf.id, tmp.GetBuffer(0));
	GetDlgItemText(IDC_JOIN_PW, tmp);
	strcpy(join_buf.password, tmp.GetBuffer(0));
	end();
}

void CJoinDlg::end(){
	CDialogEx::OnOK();
}

BOOL CJoinDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: Add your specialized code here and/or call the base class
	if (pMsg->message == WM_KEYDOWN) {
		if (pMsg->wParam == VK_RETURN)
		{
			OnBnClickedIdInput();
			return TRUE;
		}
	}

	return CDialog::PreTranslateMessage(pMsg);
}