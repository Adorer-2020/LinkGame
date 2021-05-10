#pragma once
#include "global.h"

class CGameLogic
{
public:
	bool IsLink(int** pGameMap, Vertex v1, Vertex v2);     // �ж��Ƿ���ͨ
	void Clear(int** pGameMap, Vertex v1, Vertex v2);      // ����
	int GetVexPath(Vertex avPath[4]);                      // �õ�·�������ص��Ƕ�����
public:
	int** InitMap();                        // ��ʼ����Ϸ��ͼ
	int** RerankMap(int** pGameMap);		// ������Ϸ��ͼ
	void ReleaseMap(int**& pGameMap);       // �ͷŵ�ͼ

	Vertex m_avPath[4];     // �����ڽ��������ж�ʱ�������Ķ���
	int m_nVexNum;          // ������
	CGameLogic();
	bool LinkInRow(int** pGameMap, Vertex v1, Vertex v2);			// �жϺ����Ƿ���ͨ
	bool LinkInCol(int** pGameMap, Vertex v1, Vertex v2);			// �ж������Ƿ���ͨ
	bool OneCornerLink(int** pGameMap, Vertex v1, Vertex v2);		// һ���յ���ͨ�ж�
	bool LineY(int** pGameMap, int nRow1, int nRow2, int nCol);		// ֱ����ͨ Y ��
	bool LineX(int** pGameMap, int nRow, int nCol1, int nCol2);		// ֱ����ͨ X ��
	void PushVertex(Vertex vertex);									// ���һ��·������
	void PopVertex();												// ȡ��һ������
	void ClearStack();												// ���ջ
	bool TwoCornerLink(int** pGameMap, Vertex v1, Vertex v2);		// ����ֱ�������ж�
	bool IsBlank(int** pGameMap);									// �жϵ�ͼ���Ƿ���Ԫ��
	bool SearchValiPath(int** pGameMap, Vertex avPath[4], int& nVertex);	// 
	void ClearMap(int** pGameMap);									// 
};

