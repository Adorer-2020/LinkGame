#pragma once
#include "CGameLogic.h"

class CGameControl
{
private:
	Vertex m_svSelFst;		// 选中的第一个点
	Vertex m_svSelSec;		// 选中的第二个点

	int** m_pGameMap;		// 游戏地图数组指针
	CGameLogic m_GameLogic; // 游戏逻辑操作对象

public:
	int m_GameLevel;					// 游戏关卡数
	int m_TipsBalance;					// 剩余提示次数
	int m_GameScore;					// 得分
	int m_ResetBalance;					// 剩余重排次数
	static int m_GameTheme;				// 游戏主题
	const static int s_nRows = 10;		// 游戏行数
	const static int s_nCols = 16;		// 游戏列数 
	const static int s_nPicNum = 16;	// 图片数

	void StartGame();								// 开始游戏
	void OverGame(int gameMode);					// 结束游戏
	void Rerank();									// 重排
	int GetElement(int nRow, int nCol);				// 得到某一个元素
	void SetFirstPoint(int nRow, int nCol);			// 设置第一个点
	void SetSecPoint(int nRow, int nCol);			// 设置第二个点
	bool Link(Vertex avPath[4], int& nVexnum);		// 消子判断
	bool IsWin();									// 判断是否获胜
	bool Notice(Vertex avPath[4], int& nVexnum);	// 提示
};

