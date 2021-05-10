#include "pch.h" 
#include "CGameControl.h"
#include <fstream>
using namespace std;

int CGameControl::m_GameTheme = SKY1;

// 开始游戏
void CGameControl::StartGame()
{
	m_pGameMap = m_GameLogic.InitMap();
}

// 重排
void CGameControl::Rerank()
{
	m_GameLogic.RerankMap(m_pGameMap);
}

// 结束游戏
void CGameControl::OverGame(int gameMode)
{
	m_GameLogic.ClearStack();
	m_GameLogic.ClearMap(m_pGameMap);

	ofstream save("score.txt", ios::app);

	save << m_GameScore << endl;

	char tips[20] = "最终得分：";
	char score[10];
	_itoa(m_GameScore, score, 10);
	strcat(tips, score);
	MessageBox(NULL, tips, NULL, MB_OK);

	save.close();
}

// 获取地图上各个坐标对应的图片编号
int CGameControl::GetElement(int nRow, int nCol)
{
	return m_pGameMap[nRow][nCol];
}

// 设置第一个点
void CGameControl::SetFirstPoint(int nRow, int nCol)
{
	m_svSelFst.row = nRow;
	m_svSelFst.col = nCol;
}

// 设置第二个点
void CGameControl::SetSecPoint(int nRow, int nCol)
{
	m_svSelSec.row = nRow;
	m_svSelSec.col = nCol;
}

// 判断选择的两张图片是否是同一种图片
bool CGameControl::Link(Vertex avPath[4], int& nVexnum)
{
	// 判断是否同一张图片
	if (m_svSelFst.row == m_svSelSec.row && 
		m_svSelFst.col == m_svSelSec.col)
	{
		m_GameLogic.ClearStack();
		return false;
	}

	// 判断图片是否相同
	if (m_pGameMap[m_svSelFst.row][m_svSelFst.col] != 
		m_pGameMap[m_svSelSec.row][m_svSelSec.col])
	{
		m_GameLogic.ClearStack();
		return false;
	}

	// 判断是否连通
	if (m_GameLogic.IsLink(m_pGameMap, m_svSelFst, m_svSelSec))
	{
		// 返回路径顶点
		nVexnum = m_GameLogic.GetVexPath(avPath);

		// 消子
		m_GameLogic.Clear(m_pGameMap, m_svSelFst, m_svSelSec);

		m_GameLogic.ClearStack();
		return true;
	}

	m_GameLogic.ClearStack();
	return false;
}

// 判断游戏输赢
bool CGameControl::IsWin() 
{
	return m_GameLogic.IsBlank(m_pGameMap);
}

// 提示
bool CGameControl::Notice(Vertex avPath[4], int& nVexnum)
{
	if (m_GameLogic.SearchValiPath(m_pGameMap, avPath, nVexnum))
	{
		nVexnum = m_GameLogic.GetVexPath(avPath);
		m_GameLogic.ClearStack();
		return true;
	}
	return false;
}