#pragma once


// CGameSetting 对话框

class CGameSetting : public CDialogEx
{
	DECLARE_DYNAMIC(CGameSetting)

public:
	CGameSetting(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CGameSetting();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SETTINGBOX };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CStatic m_PicSky;
	CStatic m_PicSummer;
	afx_msg void OnBnClickedOk();
	CButton m_themeSky1;
	CButton m_themeSky2;
	CButton m_themeArena;
	CButton m_themeFruit;
//	CStatic m_PicSky1;
	CStatic m_PicArena;
	CStatic m_PicFruit;
	CStatic m_PicSky1;
	CStatic m_PicSky2;
	CEdit m_TextTheme;
};
