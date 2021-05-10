// CHelpDlg.cpp: 实现文件
//

#include "pch.h"
#include "LLK.h"
#include "CHelpDlg.h"
#include "afxdialogex.h"


// CHelpDlg 对话框

IMPLEMENT_DYNAMIC(CHelpDlg, CDialogEx)

CHelpDlg::CHelpDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_HELPBOX, pParent)
{

}

CHelpDlg::~CHelpDlg()
{
}

void CHelpDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CHelpDlg, CDialogEx)
	ON_WM_HSCROLL()
END_MESSAGE_MAP()


// CHelpDlg 消息处理程序


void CHelpDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// nSBCode,滚动条消息  npos 滚动条位置
	SCROLLINFO si = { 0 };
	si.cbSize = sizeof(si);
	si.fMask = SIF_ALL;
	pScrollBar->GetScrollInfo(&si);//获取滚动条信息
	int nNewPos = si.nPos;               //拉动或点击时滚动条滑块的位置

	switch (nSBCode)
	{
	case SB_THUMBTRACK://拉动滚动条
		nNewPos = nPos;
		break;
	case SB_LINELEFT://点一下向左箭头
		nNewPos -= 1;
		break;
	case SB_LINERIGHT: //点一下向 箭头
		nNewPos += 1;
		break;
	case SB_PAGELEFT: //向左翻页
		nNewPos -= si.nPage;
		break;
	case SB_PAGERIGHT://向右翻页
		nNewPos += si.nPage;
		break;
	default:
		break;
	}
	if (nNewPos > si.nMax)
	{
		nNewPos = si.nMax;
	}
	if (nNewPos < si.nMin)
	{
		nNewPos = si.nMin;
	}
	//m_scroll.SetScrollInfo();
	pScrollBar->SetScrollPos(nNewPos);
	CString s;
	s.Format(_T("%d"), nNewPos);
	//m_pos = s;
	UpdateData(false);
	//m_pos = nNewPos;
	CDialogEx::OnHScroll(nSBCode, nPos, pScrollBar);
	CDialogEx::OnHScroll(nSBCode, nPos, pScrollBar);
}
