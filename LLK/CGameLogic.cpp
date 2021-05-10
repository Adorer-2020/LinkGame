#include "pch.h"
#include "CGameControl.h"
#include "CGameException.h"

// 构造函数
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

// 初始化地图
int** CGameLogic::InitMap()
{
	// 获取地图大小和花色
	int nRows = CGameControl::s_nRows;		// 地图行数	
	int nCols = CGameControl::s_nCols;		// 地图列数
	int nPicNum = CGameControl::s_nPicNum;	// 图片数

	// 开辟内存区域
	int** pGameMap = new int* [nRows];

	if (pGameMap == NULL)
	{
		throw new CGameException(_T("内存操作异常！\n"));
	}
	else
	{
		for (int i = 0; i < nRows; i++)
		{
			pGameMap[i] = new int[nCols];
			if (pGameMap == NULL)
			{
				throw new CGameException(_T("内存操作异常！\n"));
			}
			memset(pGameMap[i], NULL, sizeof(int) * nCols);
		}
	}

	// 多少花色
	if ((nRows * nCols) % (nPicNum * 2) != 0)
	{
		ReleaseMap(pGameMap);
		throw new CGameException(_T("游戏花色与游戏地图大小不匹配！"));
	}
	int nRepeatNum = nRows * nCols / nPicNum;
	int nCount = 0;
	for (int i = 0; i < nPicNum; i++)
	{
		// 重复数
		for (int j = 0; j < nRepeatNum; j++)
		{
			pGameMap[nCount / nCols][nCount % nCols] = i;
			nCount++;
		}
	}

	return RerankMap(pGameMap);
}

// 打乱游戏地图
int** CGameLogic::RerankMap(int** pGameMap)
{
	// 设置种子
	srand((int)time(NULL));

	// 随机任意交换两个数字
	int nVertexNum = CGameControl::s_nRows * CGameControl::s_nCols;
	for (int i = 0; i < nVertexNum; i++)
	{
		// 随机得到两个坐标
		int nIndex1 = rand() % nVertexNum;
		int nIndex2 = rand() % nVertexNum;

		// 交换两个数值
		int nTmp = pGameMap[nIndex1 / CGameControl::s_nCols][nIndex1 % CGameControl::s_nCols];
		pGameMap[nIndex1 / CGameControl::s_nCols][nIndex1 % CGameControl::s_nCols] = 
			pGameMap[nIndex2 / CGameControl::s_nCols][nIndex2 % CGameControl::s_nCols];
		pGameMap[nIndex2 / CGameControl::s_nCols][nIndex2 % CGameControl::s_nCols] = nTmp;
	}

	return pGameMap;
}

// 释放地图资源
void CGameLogic::ReleaseMap(int**& pGameMap)
{
	for (int i = 0; i < CGameControl::s_nRows; i++)
	{
		delete[]pGameMap[i];
	}
	delete[]pGameMap;
}

// 判断是否连通
bool CGameLogic::IsLink(int** pGameMap, Vertex v1, Vertex v2)
{
	// 一条直线消子
	PushVertex(v1);
	// X 直连方式
	if (v1.row == v2.row)
	{
		if (LinkInRow(pGameMap, v1, v2)) 
		{
			PushVertex(v2);
			return true;
		}
	}
	//Y 直连方式
	if (v1.col == v2.col)
	{
		if (LinkInCol(pGameMap, v1, v2))
		{
			PushVertex(v2);
			return true;
		}
	}

	//两条直线消子
	if (OneCornerLink(pGameMap, v1, v2))
	{
		PushVertex(v2);
		return true;
	}

	//三条直线消子
	if (TwoCornerLink(pGameMap, v1, v2))
	{
		PushVertex(v2);
		return true;
	}
	ClearStack();
	return false;
}

// 两条直线消子
bool CGameLogic::OneCornerLink(int** pGameMap, Vertex v1, Vertex v2)
{
	// 判断相交的顶点是否为空
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

	// 判断相交的顶点是否为空
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

// 三条直线消子
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
		// 找到一条与 Y 轴平行的连通线段
		if (pGameMap[v1.row][nCol] == BLANK && 
			pGameMap[v2.row][nCol] == BLANK)
		{
			if (LineY(pGameMap, v1.row, v2.row, nCol))
			{
				if (LineX(pGameMap, v1.row, v1.col, nCol) && 
					LineX(pGameMap, v2.row, v2.col, nCol))
				{
					// 保存节点
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
		// 找到一条与 Y 轴平行的连通线段
		if (pGameMap[v1.row][nCol] == BLANK &&
			pGameMap[v2.row][nCol] == BLANK)
		{
			if (LineY(pGameMap, v1.row, v2.row, nCol))
			{
				if (LineX(pGameMap, v1.row, v1.col, nCol) &&
					LineX(pGameMap, v2.row, v2.col, nCol))
				{
					// 保存节点
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
		// 找到一条与 X 轴平行的连通线段
		if (pGameMap[nRow][v1.col] == BLANK && 
			pGameMap[nRow][v2.col] == BLANK)
		{
			if (LineX(pGameMap, nRow, v1.col, v2.col))
			{
				if (LineY(pGameMap, nRow, v1.row, v1.col) && 
					LineY(pGameMap, nRow, v2.row, v2.col))
				{
					// 保存节点
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
		// 找到一条与 X 轴平行的连通线段
		if (pGameMap[nRow][v1.col] == BLANK &&
			pGameMap[nRow][v2.col] == BLANK)
		{
			if (LineX(pGameMap, nRow, v1.col, v2.col))
			{
				if (LineY(pGameMap, nRow, v1.row, v1.col) &&
					LineY(pGameMap, nRow, v2.row, v2.col))
				{
					// 保存节点
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

// 行号相同时，判断横向是否连通
bool CGameLogic::LinkInRow(int** pGameMap, Vertex v1, Vertex v2)
{
	int v1_nCol = v1.col;
	int v2_nCol = v2.col;
	int nRow = v1.row;
	// 如果在地图边缘
	if (nRow == 0 || nRow == CGameControl::s_nRows - 1)
		return true;
	// 保证 v1_nCol 的值小于 v2_nCol
	if (v1_nCol > v2_nCol)
	{
		// 数据交换
		int nTemp = v1_nCol;
		v1_nCol = v2_nCol;
		v2_nCol = nTemp;
	}
	// 直通
	for (int i = v1_nCol + 1; i <= v2_nCol; i++)
	{
		if (i == v2_nCol)
			return true;
		if (pGameMap[nRow][i] != BLANK)
			break;
	}
	return false;
}

// 列号相同时，判断能否纵向连通
bool CGameLogic::LinkInCol(int** pGameMap, Vertex v1, Vertex v2)
{
	int v1_nRow = v1.row;
	int v2_nRow = v2.row;
	int nCol = v1.col;
	// 如果在地图边缘
	if (nCol == 0 || nCol == CGameControl::s_nCols - 1)
		return true;
	// 保证 v1_nRow 的值小于 v2_nRow
	if (v1_nRow > v2_nRow)
	{
		int nTemp = v1_nRow;
		v1_nRow = v2_nRow;
		v2_nRow = nTemp;
	}
	//直通
	for (int i = v1_nRow + 1; i <= v2_nRow; i++)
	{
		if (i == v2_nRow)
			return true;
		if (pGameMap[i][nCol] != BLANK)
			break;
	}
	return false;
}

// 判断(nRow1, nCol1)到(nRow1, nCol2)能否连通。
bool CGameLogic::LineX(int** pGameMap, int nRow, int v1_nCol, int v2_nCol)
{
	// 保证 nCol1 小于 nCol2
	if (v1_nCol > v2_nCol)
	{
		//交换数据
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

// 判断(nRow1, nCol2)到(nRow2, nCol2)能否连通
bool CGameLogic::LineY(int** pGameMap, int v1_nRow, int v2_nRow, int nCol)
{
	if (v1_nRow > v2_nRow)
	{
		//交换数据
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

// 获取连接路径
int CGameLogic::GetVexPath(Vertex avPath[4])
{
	for (int i = 0; i < m_nVexNum; i++)
	{
		avPath[i] = m_avPath[i];
	}
	return m_nVexNum;
}

// 消子
void CGameLogic::Clear(int** pGameMap, Vertex v1, Vertex v2)
{
	pGameMap[v1.row][v1.col] = BLANK;
	pGameMap[v2.row][v2.col] = BLANK;
}

// 添加一个路径顶点
void CGameLogic::PushVertex(Vertex vertex)
{
	m_avPath[m_nVexNum++] = vertex;
}

// 取出一个顶点
void CGameLogic::PopVertex() 
{
	m_nVexNum--;
}

// 清除栈
void CGameLogic::ClearStack() 
{
	m_nVexNum = 0;
}

// 判断地图元素是否为空
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
					// 查找第一个与图片相同的点
					if (pGameMap[nRow][nCol] == pGameMap[row][col])
					{
						vertex1 = { nRow,nCol,pGameMap[nRow][nCol] };
						vertex2 = { row,col,pGameMap[row][col] };
						//判断是否连通
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
