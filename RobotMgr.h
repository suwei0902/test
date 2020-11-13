#ifndef __RobotMgr_H__
#define __RobotMgr_H__

#include <iostream>
#include "TypesDef.h"

typedef unsigned char byte;
using namespace std;

class CRobot
{
public:
	CRobot();
	~CRobot(){}
	void InitRobotInfo();						 //重置所有参数
	void OnLetComputerThink(INT32 &x, INT32 &y); //机器人进行思考
	void MoveGoBang(UINT8 x, UINT8 y);
	
private:
	void GetTable(UINT8 tempTable[][15], UINT8 nowTable[][15]);
	bool SearchBlank(INT32 &i, INT32 &j, UINT8 nowTable[][15]);
	int GiveScore(UINT8 type, INT32 x, INT32 y);
	inline UINT8 GetRobotLocation(UINT8 x)
	{
		return 14 - x;
	}

private:

	UINT8 	m_board[15][15];
	bool m_bGameStart; // 游戏是否刚刚开始
	bool m_Computer[15][15][572]; // 电脑获胜组合
	bool m_Player[15][15][572]; // 玩家获胜组合
	UINT32 m_Win[2][572]; // 各个获胜组合中填入的棋子数
};

#pragma pack()
#endif
