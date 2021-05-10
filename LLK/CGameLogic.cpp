#include "pch.h"
#include "CGameControl.h"
#include "CGameException.h"

// ���캯��
CGameLogic::CGameLogic() 
{
	for (int i = 0; i < 4; i++)
	{
		m_avPath[i].col = 0;
		m_avPath[i].row = 0;
		m_avPath[i].info = 0;
	}
	m_nVexNum = 0;
}

// ��ʼ����ͼ
int** CGameLogic::InitMap()
{
	// ��ȡ��ͼ��С�ͻ�ɫ
	int nRows = CGameControl::s_nRows;		// ��ͼ����	
	int nCols = CGameControl::s_nCols;		// ��ͼ����
	int nPicNum = CGameControl::s_nPicNum;	// ͼƬ��

	// �����ڴ�����
	int** pGameMap = new int* [nRows];

	if (pGameMap == NULL)
	{
		throw new CGameException(_T("�ڴ�����쳣��\n"));
	}
	else
	{
		for (int i = 0; i < nRows; i++)
		{
			pGameMap[i] = new int[nCols];
			if (pGameMap == NULL)
			{
				throw new CGameException(_T("�ڴ�����쳣��\n"));
			}
			memset(pGameMap[i], NULL, sizeof(int) * nCols);
		}
	}

	// ���ٻ�ɫ
	if ((nRows * nCols) % (nPicNum * 2) != 0)
	{
		ReleaseMap(pGameMap);
		throw new CGameException(_T("��Ϸ��ɫ����Ϸ��ͼ��С��ƥ�䣡"));
	}
	int nRepeatNum = nRows * nCols / nPicNum;
	int nCount = 0;
	for (int i = 0; i < nPicNum; i++)
	{
		// �ظ���
		for (int j = 0; j < nRepeatNum; j++)
		{
			pGameMap[nCount / nCols][nCount % nCols] = i;
			nCount++;
		}
	}

	return RerankMap(pGameMap);
}

// ������Ϸ��ͼ
int** CGameLogic::RerankMap(int** pGameMap)
{
	// ��������
	srand((int)time(NULL));

	// ������⽻����������
	int nVertexNum = CGameControl::s_nRows * CGameControl::s_nCols;
	for (int i = 0; i < nVertexNum; i++)
	{
		// ����õ���������
		int nIndex1 = rand() % nVertexNum;
		int nIndex2 = rand() % nVertexNum;

		// ����������ֵ
		int nTmp = pGameMap[nIndex1 / CGameControl::s_nCols][nIndex1 % CGameControl::s_nCols];
		pGameMap[nIndex1 / CGameControl::s_nCols][nIndex1 % CGameControl::s_nCols] = 
			pGameMap[nIndex2 / CGameControl::s_nCols][nIndex2 % CGameControl::s_nCols];
		pGameMap[nIndex2 / CGameControl::s_nCols][nIndex2 % CGameControl::s_nCols] = nTmp;
	}

	return pGameMap;
}

// �ͷŵ�ͼ��Դ
void CGameLogic::ReleaseMap(int**& pGameMap)
{
	for (int i = 0; i < CGameControl::s_nRows; i++)
	{
		delete[]pGameMap[i];
	}
	delete[]pGameMap;
}

// �ж��Ƿ���ͨ
bool CGameLogic::IsLink(int** pGameMap, Vertex v1, Vertex v2)
{
	// һ��ֱ������
	PushVertex(v1);
	// X ֱ����ʽ
	if (v1.row == v2.row)
	{
		if (LinkInRow(pGameMap, v1, v2)) 
		{
			PushVertex(v2);
			return true;
		}
	}
	//Y ֱ����ʽ
	if (v1.col == v2.col)
	{
		if (LinkInCol(pGameMap, v1, v2))
		{
			PushVertex(v2);
			return true;
		}
	}

	//����ֱ������
	if (OneCornerLink(pGameMap, v1, v2))
	{
		PushVertex(v2);
		return true;
	}

	//����ֱ������
	if (TwoCornerLink(pGameMap, v1, v2))
	{
		PushVertex(v2);
		return true;
	}
	ClearStack();
	return false;
}

// ����ֱ������
bool CGameLogic::OneCornerLink(int** pGameMap, Vertex v1, Vertex v2)
{
	// �ж��ཻ�Ķ����Ƿ�Ϊ��
	if (pGameMap[v1.row][v2.col] == BLANK)
	{
		if (LineY(pGameMap, v1.row, v2.row, v2.col) &&
			LineX(pGameMap, v1.row, v1.col, v2.col))
		{
			Vertex vertex = { v1.row, v2.col, BLANK };
			PushVertex(vertex);
			return true;
		}
	}

	// �ж��ཻ�Ķ����Ƿ�Ϊ��
	if (pGameMap[v2.row][v1.col] == BLANK)
	{
		if (LineY(pGameMap, v1.row, v2.row, v1.col) &&
			LineX(pGameMap, v2.row, v1.col, v2.col))
		{
			Vertex vertex = { v2.row, v1.col, BLANK };
			PushVertex(vertex);
			return true;
		}
	}
	return false;
}

// ����ֱ������
bool CGameLogic::TwoCornerLink(int** pGameMap, Vertex v1, Vertex v2)
{
	int v1_nCol = v1.col;
	int v2_nCol = v2.col;
	if (v1_nCol > v2_nCol)
	{
		int nTemp = v1_nCol;
		v1_nCol = v2_nCol;
		v2_nCol = nTemp;
	}
	for (int nCol = v1_nCol; nCol < CGameControl::s_nCols; nCol++)
	{
		// �ҵ�һ���� Y ��ƽ�е���ͨ�߶�
		if (pGameMap[v1.row][nCol] == BLANK && 
			pGameMap[v2.row][nCol] == BLANK)
		{
			if (LineY(pGameMap, v1.row, v2.row, nCol))
			{
				if (LineX(pGameMap, v1.row, v1.col, nCol) && 
					LineX(pGameMap, v2.row, v2.col, nCol))
				{
					// ����ڵ�
					Vertex vertex1 = { v1.row, nCol, BLANK };
					Vertex vertex2 = { v2.row, nCol, BLANK };
					PushVertex(vertex1);
					PushVertex(vertex2);
					return true;
				}
			}
		}
	}
	for (int nCol = v1_nCol; nCol >= 0; nCol--)
	{
		// �ҵ�һ���� Y ��ƽ�е���ͨ�߶�
		if (pGameMap[v1.row][nCol] == BLANK &&
			pGameMap[v2.row][nCol] == BLANK)
		{
			if (LineY(pGameMap, v1.row, v2.row, nCol))
			{
				if (LineX(pGameMap, v1.row, v1.col, nCol) &&
					LineX(pGameMap, v2.row, v2.col, nCol))
				{
					// ����ڵ�
					Vertex vertex1 = { v1.row, nCol, BLANK };
					Vertex vertex2 = { v2.row, nCol, BLANK };
					PushVertex(vertex1);
					PushVertex(vertex2);
					return true;
				}
			}
		}
	}

	int v1_nRow = v1.row;
	int v2_nRow = v2.row;
	if (v1_nRow > v2_nRow)
	{
		int nTemp = v1_nRow;
		v1_nRow = v2_nRow;
		v2_nRow = nTemp;
	}
	for (int nRow = v1_nRow; nRow < CGameControl::s_nRows; nRow++)
	{
		// �ҵ�һ���� X ��ƽ�е���ͨ�߶�
		if (pGameMap[nRow][v1.col] == BLANK && 
			pGameMap[nRow][v2.col] == BLANK)
		{
			if (LineX(pGameMap, nRow, v1.col, v2.col))
			{
				if (LineY(pGameMap, nRow, v1.row, v1.col) && 
					LineY(pGameMap, nRow, v2.row, v2.col))
				{
					// ����ڵ�
					Vertex vertex1 = { nRow, v1.col, BLANK };
					Vertex vertex2 = { nRow, v2.col, BLANK };
					PushVertex(vertex1);
					PushVertex(vertex2);
					return true;
				}
			}
		}
	}
	for (int nRow = v1_nRow; nRow >=0; nRow--)
	{
		// �ҵ�һ���� X ��ƽ�е���ͨ�߶�
		if (pGameMap[nRow][v1.col] == BLANK &&
			pGameMap[nRow][v2.col] == BLANK)
		{
			if (LineX(pGameMap, nRow, v1.col, v2.col))
			{
				if (LineY(pGameMap, nRow, v1.row, v1.col) &&
					LineY(pGameMap, nRow, v2.row, v2.col))
				{
					// ����ڵ�
					Vertex vertex1 = { nRow, v1.col, BLANK };
					Vertex vertex2 = { nRow, v2.col, BLANK };
					PushVertex(vertex1);
					PushVertex(vertex2);
					return true;
				}
			}
		}
	}
	return false;
}

// �к���ͬʱ���жϺ����Ƿ���ͨ
bool CGameLogic::LinkInRow(int** pGameMap, Vertex v1, Vertex v2)
{
	int v1_nCol = v1.col;
	int v2_nCol = v2.col;
	int nRow = v1.row;
	// ����ڵ�ͼ��Ե
	if (nRow == 0 || nRow == CGameControl::s_nRows - 1)
		return true;
	// ��֤ v1_nCol ��ֵС�� v2_nCol
	if (v1_nCol > v2_nCol)
	{
		// ���ݽ���
		int nTemp = v1_nCol;
		v1_nCol = v2_nCol;
		v2_nCol = nTemp;
	}
	// ֱͨ
	for (int i = v1_nCol + 1; i <= v2_nCol; i++)
	{
		if (i == v2_nCol)
			return true;
		if (pGameMap[nRow][i] != BLANK)
			break;
	}
	return false;
}

// �к���ͬʱ���ж��ܷ�������ͨ
bool CGameLogic::LinkInCol(int** pGameMap, Vertex v1, Vertex v2)
{
	int v1_nRow = v1.row;
	int v2_nRow = v2.row;
	int nCol = v1.col;
	// ����ڵ�ͼ��Ե
	if (nCol == 0 || nCol == CGameControl::s_nCols - 1)
		return true;
	// ��֤ v1_nRow ��ֵС�� v2_nRow
	if (v1_nRow > v2_nRow)
	{
		int nTemp = v1_nRow;
		v1_nRow = v2_nRow;
		v2_nRow = nTemp;
	}
	//ֱͨ
	for (int i = v1_nRow + 1; i <= v2_nRow; i++)
	{
		if (i == v2_nRow)
			return true;
		if (pGameMap[i][nCol] != BLANK)
			break;
	}
	return false;
}

// �ж�(nRow1, nCol1)��(nRow1, nCol2)�ܷ���ͨ��
bool CGameLogic::LineX(int** pGameMap, int nRow, int v1_nCol, int v2_nCol)
{
	// ��֤ nCol1 С�� nCol2
	if (v1_nCol > v2_nCol)
	{
		//��������
		int nTemp = v1_nCol;
		v1_nCol = v2_nCol;
		v2_nCol = nTemp;
	}

	for (int i = v1_nCol + 1; i <= v2_nCol; i++)
	{
		if (i == v2_nCol)
			return true;
		if (pGameMap[nRow][i] != BLANK)
			break;
	}
	return false;
}

// �ж�(nRow1, nCol2)��(nRow2, nCol2)�ܷ���ͨ
bool CGameLogic::LineY(int** pGameMap, int v1_nRow, int v2_nRow, int nCol)
{
	if (v1_nRow > v2_nRow)
	{
		//��������
		int nTemp = v1_nRow;
		v1_nRow = v2_nRow;
		v2_nRow = nTemp;
	}

	for (int i = v1_nRow + 1; i <= v2_nRow; i++)
	{
		if (i == v2_nRow)
			return true;
		if (pGameMap[i][nCol] != BLANK)
			break;
	}
	return false;
}

// ��ȡ����·��
int CGameLogic::GetVexPath(Vertex avPath[4])
{
	for (int i = 0; i < m_nVexNum; i++)
	{
		avPath[i] = m_avPath[i];
	}
	return m_nVexNum;
}

// ����
void CGameLogic::Clear(int** pGameMap, Vertex v1, Vertex v2)
{
	pGameMap[v1.row][v1.col] = BLANK;
	pGameMap[v2.row][v2.col] = BLANK;
}

// ���һ��·������
void CGameLogic::PushVertex(Vertex vertex)
{
	m_avPath[m_nVexNum++] = vertex;
}

// ȡ��һ������
void CGameLogic::PopVertex() 
{
	m_nVexNum--;
}

// ���ջ
void CGameLogic::ClearStack() 
{
	m_nVexNum = 0;
}

// �жϵ�ͼԪ���Ƿ�Ϊ��
bool CGameLogic::IsBlank(int** pGameMap)
{
	for (int nRow = 0; nRow < CGameControl::s_nRows; nRow++)
		for (int nCol = 0; nCol < CGameControl::s_nCols; nCol++)
			if (pGameMap[nRow][nCol] != BLANK)
				return false;

	return true;
}

bool CGameLogic::SearchValiPath(int** pGameMap, Vertex avPath[4], int& nVertex)
{
	Vertex vertex1, vertex2;

	if (pGameMap == NULL) return false;

	for (int nRow = 0; nRow < CGameControl::s_nRows; nRow++)
	{
		for (int nCol = 0; nCol < CGameControl::s_nCols; nCol++)
		{
			if (pGameMap[nRow][nCol] == BLANK) continue;
			for (int row = 0; row < CGameControl::s_nRows; row++)
			{
				for (int col = 0; col < CGameControl::s_nCols; col++)
				{
					if (nRow == row && nCol == col) continue;
					// ���ҵ�һ����ͼƬ��ͬ�ĵ�
					if (pGameMap[nRow][nCol] == pGameMap[row][col])
					{
						vertex1 = { nRow,nCol,pGameMap[nRow][nCol] };
						vertex2 = { row,col,pGameMap[row][col] };
						//�ж��Ƿ���ͨ
						if (IsLink(pGameMap, vertex1, vertex2))
						{
							avPath[0] = vertex1;
							avPath[1] = vertex2;
							nVertex = 2;
							return true;
						}
					}
				}
			}
		}
	}
	return false;
}

void CGameLogic::ClearMap(int** pGameMap)
{
	for (int nRow = 0; nRow < CGameControl::s_nRows; nRow++)
		for (int nCol = 0; nCol < CGameControl::s_nCols; nCol++)
			pGameMap[nRow][nCol] = BLANK;
}
