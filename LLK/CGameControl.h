#pragma once
#include "CGameLogic.h"

class CGameControl
{
private:
	Vertex m_svSelFst;		// ѡ�еĵ�һ����
	Vertex m_svSelSec;		// ѡ�еĵڶ�����

	int** m_pGameMap;		// ��Ϸ��ͼ����ָ��
	CGameLogic m_GameLogic; // ��Ϸ�߼���������

public:
	int m_GameLevel;					// ��Ϸ�ؿ���
	int m_TipsBalance;					// ʣ����ʾ����
	int m_GameScore;					// �÷�
	int m_ResetBalance;					// ʣ�����Ŵ���
	static int m_GameTheme;				// ��Ϸ����
	const static int s_nRows = 10;		// ��Ϸ����
	const static int s_nCols = 16;		// ��Ϸ���� 
	const static int s_nPicNum = 16;	// ͼƬ��

	void StartGame();								// ��ʼ��Ϸ
	void OverGame(int gameMode);					// ������Ϸ
	void Rerank();									// ����
	int GetElement(int nRow, int nCol);				// �õ�ĳһ��Ԫ��
	void SetFirstPoint(int nRow, int nCol);			// ���õ�һ����
	void SetSecPoint(int nRow, int nCol);			// ���õڶ�����
	bool Link(Vertex avPath[4], int& nVexnum);		// �����ж�
	bool IsWin();									// �ж��Ƿ��ʤ
	bool Notice(Vertex avPath[4], int& nVexnum);	// ��ʾ
};

