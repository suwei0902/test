#include "RobotMgr.h"
#include "TraceLog.h"

CRobot::CRobot(){
	
}

void CRobot::InitRobotInfo()
{
	int i, j, k, nCount = 0;

	for (i = 0; i < 15; i++)
	{
		for (j = 0; j < 15; j++)
		{
			m_board[i][j] = 0;
		}
	}

	for (i = 0; i < 15; i++)
	{
		for (j = 0; j < 15; j++)
		{
			for (k = 0; k < 572; k++)
			{
				m_Player[i][j][k] = false;
				m_Computer[i][j][k] = false;
			}
		}
	}
	for (i = 0; i < 2; i++)
	{
		for (j = 0; j < 572; j++)
		{
			m_Win[i][j] = 0;
		}
	}
	for (i = 0; i < 15; i++)
	{
		for (j = 0; j < 11; j++)
		{
			for (k = 0; k < 5; k++)
			{
				m_Player[j + k][i][nCount] = true;
				m_Computer[j + k][i][nCount] = true;
			}
			nCount++;
		}
	}
	for (i = 0; i < 15; i++)
	{
		for (j = 0; j < 11; j++)
		{
			for (k = 0; k < 5; k++)
			{
				m_Player[i][j + k][nCount] = true;
				m_Computer[i][j + k][nCount] = true;
			}
			nCount++;
		}
	}
	for (i = 0; i < 11; i++)
	{
		for (j = 0; j < 11; j++)
		{
			for (k = 0; k < 5; k++)
			{
				m_Player[j + k][i + k][nCount] = true;
				m_Computer[j + k][i + k][nCount] = true;
			}
			nCount++;
		}
	}
	for (i = 0; i < 11; i++)
	{
		for (j = 14; j >= 4; j--)
		{
			for (k = 0; k < 5; k++)
			{
				m_Player[j - k][i + k][nCount] = true;
				m_Computer[j - k][i + k][nCount] = true;
			}
			nCount++;
		}
	}
	m_bGameStart = true;
}

//其它玩家修改棋盘值
void CRobot::MoveGoBang(UINT8 x, UINT8 y)
{
	g_traceLog->VarTrace(TRACE_INFO, "Player Think...x: %d  y:%d", x, y);

	y = GetRobotLocation(y);
	m_board[x][y] = 1;

	// 修改玩家下子后棋盘状态的变化
	for (int i = 0; i < 572; i++)
	{
		// 修改状态变化
		if (m_Player[x][y][i] && m_Win[0][i] != 7)
			m_Win[0][i]++;
		if (m_Computer[x][y][i])
		{
			m_Computer[x][y][i] = false;
			m_Win[1][i] = 7;
		}
	}

}

//机器人思考并且修改棋盘
void CRobot::OnLetComputerThink(INT32 &x, INT32 &y)
{
	UINT8 bestx, besty;
	INT32 i, j, pi, pj, ptemp, ctemp, pscore = 10, cscore = -10000;
	UINT8 ctempTable[15][15], ptempTable[15][15];
	INT32 m, n, temp1[20], temp2[20]; // 暂存第一步搜索的信息

	if (m_bGameStart)
	{
		if (m_board[7][7] != 1)
		{
			m_board[7][7] = 2;
			bestx = besty = 7;
		}
		else
		{
			m_board[8][8] = 2;
			bestx = besty = 8;
		}
		m_bGameStart = false;
		
	}
	else
	{
		//寻找最佳位置
		INT32 mx, my;
		GetTable(ctempTable, m_board);
		while (SearchBlank(i, j, ctempTable))
		{
			n = 0;
			pscore = 10;
			GetTable(ptempTable, m_board);
			ctempTable[i][j] = 3; // 标记已被查找
			mx = i;
			my = j;
			// 给这个空位打分
			ctemp = GiveScore(2, mx, my);
			for (m = 0; m < 572; m++)
			{
				// 暂时更改玩家信息
				if (m_Player[i][j][m])
				{
					temp1[n] = m;
					m_Player[i][j][m] = false;
					temp2[n] = m_Win[0][m];
					m_Win[0][m] = 0;
					n++;
				}
			}
			ptempTable[i][j] = 1;

			pi = i;
			pj = j;
			while (SearchBlank(i, j, ptempTable))
			{

				ptempTable[i][j] = 3; // 标记已被查找
				mx = i;
				my = j;
				ptemp = GiveScore(1, mx, my);
				if (pscore > ptemp) // 此时为玩家下子，运用极小极大法时应选取最小值
					pscore = ptemp;
			}
			for (m = 0; m < n; m++)
			{
				// 恢复玩家信息
				m_Player[pi][pj][temp1[m]] = true;
				m_Win[0][temp1[m]] = temp2[m];
			}
			if (ctemp + pscore > cscore) // 此时为计算机下子，运用极小极大法时应选取最大值
			{
				cscore = ctemp + pscore;
				bestx = pi;
				besty = pj;
			}
		}
	}

	for (i = 0; i < 572; i++)
	{
		// 修改计算机下子后，棋盘的变化状况
		if (m_Computer[bestx][besty][i] && m_Win[1][i] != 7)
			m_Win[1][i]++;
		if (m_Player[bestx][besty][i])
		{
			m_Player[bestx][besty][i] = false;
			m_Win[0][i] = 7;
		}
	}

	m_board[bestx][besty] = 2;
	x = (bestx);
	y = GetRobotLocation(besty);
	g_traceLog->VarTrace(TRACE_INFO, "Robot Think...x: %d  y:%d", x, y);

}

void CRobot::GetTable(UINT8 tempTable[][15], UINT8 nowTable[][15])
{
	UINT8 i, j;
	for (i = 0; i < 15; i++)
	{
		for (j = 0; j < 15; j++)
		{
			tempTable[i][j] = nowTable[i][j];
		}
	}
}

bool CRobot::SearchBlank(INT32 &i, INT32 &j, UINT8 nowTable[][15])
{
	UINT8 x, y;
	for (x = 0; x < 15; x++)
	{
		for (y = 0; y < 15; y++)
		{
			if (nowTable[x][y] == 0 && nowTable[x][y] != 3)
			{
				i = x;
				j = y;
				return true;
			}
		}
	}
	return false;
}

INT32 CRobot::GiveScore(UINT8 type, INT32 x, INT32 y)
{
	INT32 i, nScore = 0;
	for (i = 0; i < 572; i++)
	{
		if (type == 1)
		{
			// 玩家下
			if (m_Player[x][y][i])
			{
				switch (m_Win[0][i])
				{
				case 1:
					nScore -= 5;
					break;
				case 2:
					nScore -= 50;
					break;
				case 3:
					nScore -= 500;
					break;
				case 4:
					nScore -= 5000;
					break;
				default:
					break;
				}
			}
		}
		else
		{
			// 计算机下
			if (m_Computer[x][y][i])
			{
				switch (m_Win[1][i])
				{
				case 1:
					nScore += 5;
					break;
				case 2:
					nScore += 50;
					break;
				case 3:
					nScore += 100;
					break;
				case 4:
					nScore += 10000;
					break;
				default:
					break;
				}
			}
		}
	}
	return nScore;
}

