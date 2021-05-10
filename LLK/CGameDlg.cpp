// CGameDlg.cpp: 实现文件
//

#include <string>
#include "pch.h"
#include "LLK.h"
#include "LLKDlg.h"
#include "global.h"
#include "CGameDlg.h"
#include "afxdialogex.h"
using namespace std;


// CGameDlg 对话框

IMPLEMENT_DYNAMIC(CGameDlg, CDialogEx)

CGameDlg::CGameDlg(int gameMode, CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_GAME_DIALOG, pParent)
{	
	// 图标
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	m_GameMode = gameMode;

	// 初始化起始点坐标
	m_ptGameTop.x = 20;
	m_ptGameTop.y = 20;

	// 初始化图片元素大小
	m_sizeElem.cx = 40;
	m_sizeElem.cy = 40;

	// 初始化游戏更新区域
	m_rtGameRect.left = m_ptGameTop.x;
	m_rtGameRect.top = m_ptGameTop.y;
	m_rtGameRect.right = m_rtGameRect.left + m_sizeElem.cx * CGameControl::s_nCols;
	m_rtGameRect.bottom = m_rtGameRect.top + m_sizeElem.cy * CGameControl::s_nRows;

	// 初始化图标选中状态
	m_bFirstPoint = true;

}

CGameDlg::~CGameDlg()
{
}

void CGameDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_GAME_TIME, m_GameProgress);
}


BEGIN_MESSAGE_MAP(CGameDlg, CDialogEx)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_BTN_START, &CGameDlg::OnBnClickedBtnStart)
	ON_BN_CLICKED(IDC_BTN_STOP, &CGameDlg::OnBnClickedBtnStop)
	ON_BN_CLICKED(IDC_BTN_NOTICE, &CGameDlg::OnBnClickedBtnNotice)
	ON_BN_CLICKED(IDC_BTN_RERANK, &CGameDlg::OnBnClickedBtnRerank)
	ON_BN_CLICKED(IDC_BTN_SETTING2, &CGameDlg::OnBnClickedBtnSetting2)
	ON_BN_CLICKED(IDC_BTN_HELP2, &CGameDlg::OnBnClickedBtnHelp2)
	ON_WM_LBUTTONUP()
	ON_WM_TIMER()
END_MESSAGE_MAP()

// CGameDlg 消息处理程序

// 窗体初始化
BOOL CGameDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 窗口标题
	m_GameMode == BASIC ?
		this->SetWindowText("欢乐连连看——基本模式") :
		m_GameMode == RELAX ?
		this->SetWindowText("欢乐连连看——休闲模式") :
		this->SetWindowText("欢乐连连看——关卡模式");
	
	this->GetDlgItem(IDC_BTN_STOP)->EnableWindow(FALSE);
	this->GetDlgItem(IDC_BTN_NOTICE)->EnableWindow(FALSE);
	this->GetDlgItem(IDC_BTN_RERANK)->EnableWindow(FALSE);

	if (CGameControl::m_GameTheme == NULL)
		CGameControl::m_GameTheme = 1;

	// 设置图标
	SetIcon(m_hIcon, true);

	// 初始化背景
	InitBackground();

	//初始化元素
	InitElement();

	// 初始化窗口大小和位置
	UpdateWindow();

	return TRUE;
}

// 初始化背景
void CGameDlg::InitBackground()
{
	// 获得当前对话框的视频内容
	CClientDC dc(this);
	// 加载位图
	HANDLE bmp = CGameControl::m_GameTheme == SKY1 ?
		::LoadImage(NULL, "theme\\picture\\sky1_bg.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE) :
		CGameControl::m_GameTheme == SKY2 ?
		::LoadImage(NULL, "theme\\picture\\sky2_bg.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE) :
		CGameControl::m_GameTheme == ARENA ?
		::LoadImage(NULL, "theme\\picture\\Arena_bg.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE) :
		::LoadImage(NULL, "theme\\picture\\fruit_bg.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);

	// 创建与视频内容兼容的内存DC
	m_dcBG.CreateCompatibleDC(&dc);

	// 将位图资源选入DC
	m_dcBG.SelectObject(bmp);

	// 初始化内存DC
	m_dcMem.CreateCompatibleDC(&dc);
	CBitmap bitmap;
	bitmap.CreateCompatibleBitmap(&dc, 800, 600);
	m_dcMem.SelectObject(&bitmap);

	// 绘制背景图片
	m_dcMem.BitBlt(0, 0, 800, 600, &m_dcBG, 0, 0, SRCCOPY);

	// 设置窗口居中
	CenterWindow();
}

// 加载元素图片
void CGameDlg::InitElement()
{
	// 获得当前对话框的视频内存
	CClientDC dc(this);
	// 加载元素图片
	HANDLE hBmp = CGameControl::m_GameTheme == SKY1 ?
		::LoadImage(NULL, "theme\\picture\\sky1_element.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE) :
		CGameControl::m_GameTheme == SKY2 ?
		::LoadImage(NULL, "theme\\picture\\sky2_element.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE) :
		CGameControl::m_GameTheme == ARENA ?
		::LoadImage(NULL, "theme\\picture\\Arena_element.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE) :
		::LoadImage(NULL, "theme\\picture\\fruit_element.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	// 创建与视频内存兼容的内存 DC
	m_dcElement.CreateCompatibleDC(&dc);
	// 将位图资源选入 DC
	m_dcElement.SelectObject(hBmp);
	// 加载掩码图片
	HANDLE hMask = CGameControl::m_GameTheme == SKY1 ?
		::LoadImage(NULL, "theme\\picture\\sky1_mask.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE) :
		CGameControl::m_GameTheme == SKY2 ?
		::LoadImage(NULL, "theme\\picture\\Sky2_mask.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE) :
		CGameControl::m_GameTheme == ARENA ?
		::LoadImage(NULL, "theme\\picture\\Arena_mask.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE) :
		::LoadImage(NULL, "theme\\picture\\fruit_mask.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	// 创建与视频内存兼容的内存 DC
	m_dcMask.CreateCompatibleDC(&dc);
	// 将位图资源选入 DC
	m_dcMask.SelectObject(hMask);
	// 加载暂停图片
	HANDLE hStop = CGameControl::m_GameTheme == SKY1 ?
		::LoadImage(NULL, "theme\\picture\\sky1_stop.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE) :
		CGameControl::m_GameTheme == SKY2 ?
		::LoadImage(NULL, "theme\\picture\\sky2_stop.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE) :
		CGameControl::m_GameTheme == ARENA ?
		::LoadImage(NULL, "theme\\picture\\Arena_stop.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE) :
		::LoadImage(NULL, "theme\\picture\\fruit_stop.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	// 创建与视频内存兼容的内存 DC
	m_dcStop.CreateCompatibleDC(&dc);
	// 将位图资源选入 DC
	m_dcStop.SelectObject(hStop);
}

// 绘制背景图片
void CGameDlg::OnPaint()
{
	CPaintDC dc(this); 
	dc.BitBlt(0, 0, 800, 600, &m_dcMem, 0, 0, SRCCOPY);
}

// 更新客户区界面
void CGameDlg::UpdateWindow() 
{
	// 调整窗口大小
	CRect rtWin;
	CRect rtClient;
	this->GetWindowRect(rtWin);
	// 获得窗口大小
	this->GetClientRect(rtClient);
	// 获得客户区大小
	// 标题栏和外边框的大小
	int nSpanWidth = rtWin.Width() - rtClient.Width();
	int nSpanHeight = rtWin.Height() - rtClient.Height();
	// 设置窗口大小
	MoveWindow(0, 0, GAMEWND_WIDTH + nSpanWidth, GAMEWND_HEIGHT + nSpanHeight);
	// 设置对话框显示是，在 windows 窗口正中央。
	CenterWindow();
}

// 更新游戏地图
void CGameDlg::UpdateMap()
{
	// 计算图片的顶点坐标与图片大小
	int nTop = m_ptGameTop.x;
	int nLeft = m_ptGameTop.y;
	int nElemW = m_sizeElem.cx;
	int nElemH = m_sizeElem.cy;

	//获取行数和列数
	int nRows = CGameControl::s_nRows;			// 行数
	int nCols = CGameControl::s_nCols;			// 列数

	m_dcMem.BitBlt(m_rtGameRect.left, m_rtGameRect.top, m_rtGameRect.Width(), 
		m_rtGameRect.Height(), &m_dcBG, m_rtGameRect.left, m_rtGameRect.top, SRCCOPY);

	for (int i = 0; i < nRows; i++)
	{
		for (int j = 0; j < nCols; j++)
		{
			// 得到图片编号的值
			int nElemVal =  m_GameC.GetElement(i, j);
			if (nElemVal == BLANK)	continue;
			// 将背景与掩码相或，边保留，图像区域为 1
			m_dcMem.BitBlt(nLeft + j * nElemW, nTop + i * nElemH, nElemW, nElemH,
				&m_dcMask, 0, nElemVal * nElemH, SRCPAINT);
			// 与元素图片相与，边保留，图像区域为元素图片
			m_dcMem.BitBlt(nLeft + j * nElemW, nTop + i * nElemH, nElemW, nElemH,
				&m_dcElement, 0, nElemVal * nElemH, SRCAND);
		}
	}
	InvalidateRect(FALSE);
}

// 放开鼠标左键
void CGameDlg::OnLButtonUp(UINT nFlags, CPoint point)
{
	// 如果游戏处于暂停状态，则客户区不可用
	CString str;
	this->GetDlgItem(IDC_BTN_STOP)->GetWindowText(str);
	char* name = str.GetBuffer();
	if (strcmp(name, "暂停游戏"))
		return CDialogEx::OnLButtonUp(nFlags, point);

	// 判断点击位置是否在游戏地图中
	if (point.y < m_rtGameRect.top || point.y > m_rtGameRect.bottom
		|| point.x < m_rtGameRect.left || point.x > m_rtGameRect.right)
	{
		return CDialogEx::OnLButtonUp(nFlags, point);
	}
	// 行号 = (点击位置 y 坐标 - 游戏地图起始 y 坐标) / 元素的高度
	int nRow = (point.y - m_ptGameTop.y) / m_sizeElem.cy;
	// 列号 = (点击位置 x 坐标 - 游戏地图起始 x 坐标) / 元素的长度
	int nCol = (point.x - m_ptGameTop.x) / m_sizeElem.cx;

	if (m_GameC.GetElement(nRow, nCol) == BLANK)
		return CDialogEx::OnLButtonUp(nFlags, point);

	if (m_bFirstPoint) 		// 第一个点
	{
		//绘制提示框
		DrawTipFrame(nRow, nCol);
		m_GameC.SetFirstPoint(nRow, nCol);
	}
	else	// 第二个点
	{
		//绘制提示框
		DrawTipFrame(nRow, nCol);
		m_GameC.SetSecPoint(nRow, nCol);

		Vertex avPath[4];     // 获得路径
		int nVexnum = 0;      // 顶点个数

		// 连子判断
		if (m_GameC.Link(avPath, nVexnum))
		{
			m_GameC.m_GameScore++;
			// 画提示线
			DrawTipLine(avPath, nVexnum);
			// 更新地图
			UpdateMap();
		}
		Sleep(200);    // 延迟
		InvalidateRect(m_rtGameRect, FALSE); // 局部矩形更新

		JudgeWin();
	}
	// 赋反值
	m_bFirstPoint = !m_bFirstPoint;

	CDialogEx::OnLButtonUp(nFlags, point);
}

// 绘制提示框
void CGameDlg::DrawTipFrame(int nRow, int nCol)
{
	// 获取DC
	CClientDC dc(this);
	CBrush brush(RGB(0, 255, 0));// 矩形框的颜色
	CRect rtTipFrame;

	// 根据矩形的左上角坐标和右上角坐标进行绘制
	rtTipFrame.left = m_ptGameTop.x + nCol * m_sizeElem.cx;
	rtTipFrame.top = m_ptGameTop.y + nRow * m_sizeElem.cy;
	rtTipFrame.right = rtTipFrame.left + m_sizeElem.cx;
	rtTipFrame.bottom = rtTipFrame.top + m_sizeElem.cy;
	dc.FrameRect(rtTipFrame, &brush);
}

// 绘制连线
void CGameDlg::DrawTipLine(Vertex avPath[4], int nVexnum)
{
	// 获取DC
	CClientDC dc(this);

	// 设置画笔
	CPen penLine(PS_SOLID, 2, RGB(0, 255, 0));

	// 将画笔选入DC
	CPen* pOldPen = dc.SelectObject(&penLine);

	dc.MoveTo(m_ptGameTop.x + avPath[0].col * m_sizeElem.cx + m_sizeElem.cx / 2,
		m_ptGameTop.y + avPath[0].row * m_sizeElem.cy + m_sizeElem.cy / 2);
	//绘制连接线
	for (int i = 0; i < nVexnum - 1; i++)
	{
		dc.LineTo(m_ptGameTop.x + avPath[i + 1].col * m_sizeElem.cx + m_sizeElem.cx / 2,
			m_ptGameTop.y + avPath[i + 1].row * m_sizeElem.cy + m_sizeElem.cy / 2);
	}

	dc.SelectObject(pOldPen);
}

// 绘制倒计时
void CGameDlg::DrawGameTime()
{
	/*
	// 获取DC
	CClientDC dc(this);

	// 创建字体
	CFont font;
	CString str;
	str.Format(_T("%d"), m_GameProgress.GetPos());
	font.CreatePointFont(160, "宋体");

	// 将字体选入DC
	CFont* pOldFont = dc.SelectObject(&font);

	// 设置字体颜色
	dc.SetTextColor(RGB(0, 0, 0));

	dc.TextOut(320, 240, str);

	dc.SelectObject(pOldFont);
	*/

	CString str;

	int time = m_GameProgress.GetPos();

	str.Format(_T("%d"), time);

	// SetDlgItemText(IDC_EDIT_TIME, str);
}

// 开始游戏
void CGameDlg::OnBnClickedBtnStart()
{
	m_GameC.m_GameScore = 0;	// 游戏得分
	m_GameC.StartGame();
	UpdateMap();
	InvalidateRect(m_rtGameRect, FALSE);

	CString str;
	this->GetDlgItem(IDC_BTN_START)->GetWindowText(str);
	char* name = str.GetBuffer();
	if (strcmp("str", "重新开始"))
	{
		this->GetDlgItem(IDC_BTN_START)->SetWindowText("开始游戏");
		this->GetDlgItem(IDC_BTN_STOP)->SetWindowText("暂停游戏");
	}
	this->GetDlgItem(IDC_BTN_START)->EnableWindow(FALSE);
	this->GetDlgItem(IDC_BTN_STOP)->EnableWindow(TRUE);
	this->GetDlgItem(IDC_BTN_NOTICE)->EnableWindow(TRUE);
	this->GetDlgItem(IDC_BTN_RERANK)->EnableWindow(TRUE);

	if (m_GameMode == BASIC)
	{
		// 提示次数设置为10次
		m_GameC.m_TipsBalance = 20;
		// 重排次数设置为2次
		m_GameC.m_ResetBalance = 5;
		// 初始化进度条
		m_GameProgress.SetRange32(0, 60 * 8);
		m_GameProgress.SetStep(-1);
		m_GameProgress.SetPos(60 * 8);

		// 启动计时器
		this->SetTimer(PLAY_TIMER_ID, 1000, NULL);
		// 绘制当前秒数
		DrawGameTime();
	}
	else if (m_GameMode == RELAX)
	{
		// 提示次数设置为无限
		m_GameC.m_TipsBalance = 100;
		// 重排次数设置为无限
		m_GameC.m_ResetBalance = 1000;
		// 禁用计时器
		m_GameProgress.EnableWindow(FALSE);
		m_GameProgress.ShowWindow(SW_HIDE);
	}
	else if (m_GameMode == LEVEL)
	{
		// 提示次数设置为20次
		m_GameC.m_TipsBalance = 500;
		// 重排次数设置为5次
		m_GameC.m_ResetBalance = 10;
		// 游戏关卡数为第一关
		m_GameC.m_GameLevel = 1;

		char tips1[20] = "当前关卡数：";
		char tips2[20] = "剩余提示次数：";
		char tips3[20] = "剩余重排次数：";
		char level[5], tipsBalance[5], resetBalance[5];
		_itoa(m_GameC.m_GameLevel, level, 10);
		_itoa(m_GameC.m_TipsBalance, tipsBalance, 10);
		_itoa(m_GameC.m_ResetBalance, resetBalance, 10);
		strcat(tips1, level);
		strcat(tips2, tipsBalance);
		strcat(tips3, resetBalance);
		strcat(tips2, "\n");
		strcat(tips2, tips3);
		strcat(tips1, "\n");
		strcat(tips1, tips2);
		MessageBox(tips1, "提示");

		// 初始化进度条
		m_GameProgress.SetRange32(0, 60 * 6);
		m_GameProgress.SetStep(-1);
		m_GameProgress.SetPos(60 * 6);
		// 启动计时器
		this->SetTimer(PLAY_TIMER_ID, 1000, NULL);
		// 绘制当前秒数
		DrawGameTime();
	}
}

// 暂停游戏
void CGameDlg::OnBnClickedBtnStop()
{
	CString str;
	this->GetDlgItem(IDC_BTN_STOP)->GetWindowText(str);
	char *name = str.GetBuffer();

	if (strcmp(name, "继续游戏") != 0)
	{
		m_dcMem.BitBlt(20, 20, 640, 400, &m_dcStop, 0, 0, SRCCOPY);
		InvalidateRect(m_rtGameRect, FALSE);
		KillTimer(PLAY_TIMER_ID);
		this->GetDlgItem(IDC_BTN_START)->SetWindowText("重新开始");
		this->GetDlgItem(IDC_BTN_STOP)->SetWindowText("继续游戏");
		this->GetDlgItem(IDC_BTN_START)->EnableWindow(TRUE);
		this->GetDlgItem(IDC_BTN_NOTICE)->EnableWindow(FALSE);
		this->GetDlgItem(IDC_BTN_RERANK)->EnableWindow(FALSE);
	}
	else
	{
		this->SetTimer(PLAY_TIMER_ID, 1000, NULL);
		this->GetDlgItem(IDC_BTN_STOP)->SetWindowText("暂停游戏");
		this->GetDlgItem(IDC_BTN_START)->EnableWindow(FALSE);
		if (m_GameC.m_TipsBalance > 0)
			this->GetDlgItem(IDC_BTN_NOTICE)->EnableWindow(TRUE);
		if (m_GameC.m_ResetBalance > 0)
			this->GetDlgItem(IDC_BTN_RERANK)->EnableWindow(TRUE);
	}
}

// 提示
void CGameDlg::OnBnClickedBtnNotice()
{
	m_GameC.m_GameScore -= 2;
	if (m_GameC.m_TipsBalance-- <= 6)
	{
		char tips[50] = "当前剩余提示次数：";
		char balance[5];
		_itoa(m_GameC.m_TipsBalance, balance, 10);
		strcat(tips, balance);
		MessageBox(tips, "提示");

		if (m_GameC.m_TipsBalance == 0)
		{
			this->GetDlgItem(IDC_BTN_NOTICE)->EnableWindow(FALSE);
		}
	}

	Vertex avPath[4];
	int nVexnum;
	if (m_GameC.Notice(avPath, nVexnum))
	{
		DrawTipFrame(avPath[0].row, avPath[0].col);
		DrawTipFrame(avPath[nVexnum - 1].row, avPath[nVexnum - 1].col);
		DrawTipLine(avPath, nVexnum);
		UpdateMap();
		Sleep(600);
		InvalidateRect(m_rtGameRect, false);
	}
}

// 重排
void CGameDlg::OnBnClickedBtnRerank()
{
	m_GameC.m_GameScore -= 10;
	if (m_GameC.m_ResetBalance-- < 11)
	{
		char tips[50] = "当前剩余重排次数：";
		char balance[5];
		_itoa(m_GameC.m_ResetBalance, balance, 10);
		strcat(tips, balance);
		MessageBox(tips, "提示");

		if (m_GameC.m_ResetBalance == 0)
			this->GetDlgItem(IDC_BTN_RERANK)->EnableWindow(FALSE);
	}

	m_GameC.Rerank();
	UpdateMap();
}

// 设置
void CGameDlg::OnBnClickedBtnSetting2()
{
	MessageBox("在规定时间内消除地图上的所有图片，获得游戏胜利，加油吧！", "帮助");
}

// 帮助
void CGameDlg::OnBnClickedBtnHelp2()
{
	CLLKDlg llk;
	llk.OnBnClickedBtnHelp();
}

// 计时器
void CGameDlg::OnTimer(UINT_PTR nIDEvent)
{
	if (nIDEvent == PLAY_TIMER_ID)
	{
		// 游戏时间减少一秒
		m_GameProgress.StepIt();
	}
	if (m_GameProgress.GetPos() <= 0)
	{
		// 关闭计时器
		KillTimer(PLAY_TIMER_ID);
		MessageBox("很遗憾，时间到了", "游戏失败");

		this->GetDlgItem(IDC_BTN_START)->SetWindowText("重新开始");
		this->GetDlgItem(IDC_BTN_START)->EnableWindow(TRUE);
		this->GetDlgItem(IDC_BTN_STOP)->EnableWindow(FALSE);
		this->GetDlgItem(IDC_BTN_NOTICE)->EnableWindow(FALSE);
		this->GetDlgItem(IDC_BTN_RERANK)->EnableWindow(FALSE);

		m_GameC.OverGame(m_GameMode);
		UpdateMap();
	}
	DrawGameTime();
	CDialogEx::OnTimer(nIDEvent);
}

// 判断胜负
bool CGameDlg::JudgeWin()
{
	// 判断游戏胜利
	if (m_GameC.IsWin())
	{
		MessageBox("恭喜您获胜了！");
		m_GameC.OverGame(m_GameMode);
		if (m_GameMode == LEVEL)
		{
			if (MessageBox("是否进行下一关？", "提示", MB_YESNO) == 6)
			{

				m_GameC.StartGame();
				UpdateMap();
				InvalidateRect(m_rtGameRect, FALSE);

				m_GameC.m_GameLevel++;
				char tips1[20] = "当前关卡数：";
				char tips2[20] = "剩余提示次数：";
				char tips3[20] = "剩余重排次数：";
				char level[5], tipsBalance[5], resetBalance[5];
				_itoa(m_GameC.m_GameLevel, level, 10);
				_itoa(m_GameC.m_TipsBalance, tipsBalance, 10);
				_itoa(m_GameC.m_ResetBalance, resetBalance, 10);
				strcat(tips1, level);
				strcat(tips2, tipsBalance);
				strcat(tips3, resetBalance);
				strcat(tips2, "\n");
				strcat(tips2, tips3);
				strcat(tips1, "\n");
				strcat(tips1, tips2);
				MessageBox(tips1, "提示");

				// 重新初始化进度条
				m_GameProgress.SetRange32(0, 60 * 6);
				m_GameProgress.SetStep(-1);
				m_GameProgress.SetPos(60 * 6);
				// 启动计时器
				this->SetTimer(PLAY_TIMER_ID, 1000, NULL);
				return true;
			}
		}

		// 关闭计时器
		KillTimer(PLAY_TIMER_ID);

		this->GetDlgItem(IDC_BTN_START)->EnableWindow(TRUE);
		this->GetDlgItem(IDC_BTN_STOP)->EnableWindow(FALSE);
		this->GetDlgItem(IDC_BTN_NOTICE)->EnableWindow(FALSE);
		this->GetDlgItem(IDC_BTN_RERANK)->EnableWindow(FALSE);

		return true;
	}
	return false;
}
