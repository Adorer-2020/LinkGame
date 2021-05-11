#include "pch.h" 
#include "CGameControl.h"
#include <fstream>
using namespace std;

int CGameControl::m_GameTheme = SKY1;

// ��ʼ��Ϸ
void CGameControl::StartGame()
{
	m_pGameMap = m_GameLogic.InitMap();
}

// ����
void CGameControl::Rerank()
{
	m_GameLogic.RerankMap(m_pGameMap);
}

// ������Ϸ
void CGameControl::OverGame(int gameMode)
{
	m_GameLogic.ClearStack();
	m_GameLogic.ClearMap(m_pGameMap);

	ofstream save("score.txt", ios::app);

	save << m_GameScore << endl;

	char tips[20] = "���յ÷֣�";
	char score[10];
	_itoa(m_GameScore, score, 10);
	strcat(tips, score);
	MessageBox(NULL, tips, NULL, MB_OK);

	save.close();
}

// ��ȡ��ͼ�ϸ��������Ӧ��ͼƬ���
int CGameControl::GetElement(int nRow, int nCol)
{
	return m_pGameMap[nRow][nCol];
}

// ���õ�һ����
void CGameControl::SetFirstPoint(int nRow, int nCol)
{
	m_svSelFst.row = nRow;
	m_svSelFst.col = nCol;
}

// ���õڶ�����
void CGameControl::SetSecPoint(int nRow, int nCol)
{
	m_svSelSec.row = nRow;
	m_svSelSec.col = nCol;
}

// �ж�ѡ�������ͼƬ�Ƿ���ͬһ��ͼƬ
bool CGameControl::Link(Vertex avPath[4], int& nVexnum)
{
	// �ж��Ƿ�ͬһ��ͼƬ
	if (m_svSelFst.row == m_svSelSec.row && 
		m_svSelFst.col == m_svSelSec.col)
	{
		m_GameLogic.ClearStack();
		return false;
	}

	// �ж�ͼƬ�Ƿ���ͬ
	if (m_pGameMap[m_svSelFst.row][m_svSelFst.col] != 
		m_pGameMap[m_svSelSec.row][m_svSelSec.col])
	{
		m_GameLogic.ClearStack();
		return false;
	}

	// �ж��Ƿ���ͨ
	if (m_GameLogic.IsLink(m_pGameMap, m_svSelFst, m_svSelSec))
	{
		// ����·������
		nVexnum = m_GameLogic.GetVexPath(avPath);

		// ����
		m_GameLogic.Clear(m_pGameMap, m_svSelFst, m_svSelSec);

		m_GameLogic.ClearStack();
		return true;
	}

	m_GameLogic.ClearStack();
	return false;
}

// �ж���Ϸ��Ӯ
bool CGameControl::IsWin() 
{
	return m_GameLogic.IsBlank(m_pGameMap);
}

// ��ʾ
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