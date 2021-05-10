#pragma once
#include "LLKDlg.h"
#include "CGameControl.h"

// CGameDlg 对话框

class CGameDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CGameDlg)

public:
	CGameDlg(int mode,CWnd* pParent = nullptr);
	// 标准构造函数
	virtual ~CGameDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_GAME_DIALOG };
#endif

protected:
	HICON m_hIcon;          // 系统图标
	CDC m_dcMem;            // 内存DC
	CDC m_dcBG;             // 背景DC
	CDC m_dcElement;        // 元素内存DC
	CDC m_dcMask;           // 掩码内存DC
	CDC m_dcStop;			// 暂停游戏DC
	CPoint m_ptGameTop;     // 游戏区起始点
	CSize m_sizeElem;       // 元素图片的大小
	CRect m_rtGameRect;     // 游戏区域大小
	CGameControl m_GameC;   // 游戏控制类
	int m_GameMode;			// 游戏模式	1.基本模式	2.休闲模式	3.挑战模式
	CProgressCtrl m_GameProgress;	// 进度条

public:
	bool m_bFirstPoint;		// 是否是第一次点击的点

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:

	virtual BOOL OnInitDialog();			// 窗口初始化函数

	afx_msg void OnPaint();					// WM_PAINT消息响应函数
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnBnClickedBtnStart();		// 开始按钮
	afx_msg void OnBnClickedBtnStop();		// 暂停按钮
	afx_msg void OnBnClickedBtnNotice();	// 提示按钮
	afx_msg void OnBnClickedBtnRerank();	// 重排按钮
	afx_msg void OnBnClickedBtnSetting2();	// 设置按钮
	afx_msg void OnBnClickedBtnHelp2();		// 帮助按钮

	void InitBackground();					// 初始化窗口背景
	void InitElement();						// 初始化元素图片与DC
	void UpdateWindow();					// 更新界面
	void UpdateMap();						// 更新游戏地图
	void DrawTipFrame(int nRow, int nCol);
	void DrawTipLine(Vertex avPath[4], int nVexnum);

	afx_msg void OnTimer(UINT_PTR nIDEvent);
	void DrawGameTime();
	bool JudgeWin();
};
