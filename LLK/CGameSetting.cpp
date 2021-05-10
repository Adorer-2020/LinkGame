// CGameSetting.cpp: 实现文件
//

#include "pch.h"
#include "LLK.h"
#include "CGameSetting.h"
#include "afxdialogex.h"
#include "CGameControl.h"


// CGameSetting 对话框

IMPLEMENT_DYNAMIC(CGameSetting, CDialogEx)

CGameSetting::CGameSetting(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_SETTINGBOX, pParent)
{

}

CGameSetting::~CGameSetting()
{
}

void CGameSetting::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_THEME_SKY1, m_themeSky1);
	DDX_Control(pDX, IDC_THEME_SKY2, m_themeSky2);
	DDX_Control(pDX, IDC_THEME_ARENA, m_themeArena);
	DDX_Control(pDX, IDC_THEME_FRUIT, m_themeFruit);
	DDX_Control(pDX, IDC_PIC_ARENA, m_PicArena);
	DDX_Control(pDX, IDC_PIC_FRUIT, m_PicFruit);
	DDX_Control(pDX, IDC_PIC_SKY1, m_PicSky1);
	DDX_Control(pDX, IDC_PIC_SKY2, m_PicSky2);
	DDX_Control(pDX, IDC_EDIT_THEME, m_TextTheme);
}


BEGIN_MESSAGE_MAP(CGameSetting, CDialogEx)
	ON_BN_CLICKED(IDOK, &CGameSetting::OnBnClickedOk)
END_MESSAGE_MAP()


// CGameSetting 消息处理程序


BOOL CGameSetting::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	m_TextTheme.SetWindowTextA("主题设置");

	switch (CGameControl::m_GameTheme)
	{
	case SKY1:
		m_themeSky1.SetCheck(true);
		break;
	case SKY2:
		m_themeSky2.SetCheck(true);
		break;
	case ARENA:
		m_themeArena.SetCheck(true);
		break;
	case FRUIT:
		m_themeFruit.SetCheck(true);
		break;
	};

	HBITMAP sky1 = (HBITMAP)LoadImage(AfxGetInstanceHandle(), _T("theme\\picture\\sky1_theme.bmp"),
		IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);  // 将位图IDB_BITMAP1加载到bitmap  
	HBITMAP sky2 = (HBITMAP)LoadImage(AfxGetInstanceHandle(), _T("theme\\picture\\sky2_theme.bmp"),
		IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);  // 将位图IDB_BITMAP1加载到bitmap  	
	HBITMAP arena = (HBITMAP)LoadImage(AfxGetInstanceHandle(), _T("theme\\picture\\arena_theme.bmp"),
		IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);  // 将位图IDB_BITMAP1加载到bitmap  	
	HBITMAP fruit = (HBITMAP)LoadImage(AfxGetInstanceHandle(), _T("theme\\picture\\fruit_theme.bmp"),
		IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);  // 将位图IDB_BITMAP1加载到bitmap 

	m_PicSky1.SetBitmap(sky1);
	m_PicSky2.SetBitmap(sky2);
	m_PicArena.SetBitmap(arena);
	m_PicFruit.SetBitmap(fruit);

	return TRUE;
}

void CGameSetting::OnBnClickedOk()
{
	CGameControl::m_GameTheme = m_themeSky1.GetCheck() ? SKY1 :
		m_themeSky2.GetCheck() ? SKY2 :
		m_themeArena.GetCheck() ? ARENA : FRUIT;

	CDialogEx::OnOK();
}
