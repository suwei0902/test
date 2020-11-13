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
	void InitRobotInfo();						 //�������в���
	void OnLetComputerThink(INT32 &x, INT32 &y); //�����˽���˼��
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
	bool m_bGameStart; // ��Ϸ�Ƿ�ոտ�ʼ
	bool m_Computer[15][15][572]; // ���Ի�ʤ���
	bool m_Player[15][15][572]; // ��һ�ʤ���
	UINT32 m_Win[2][572]; // ������ʤ����������������
};

#pragma pack()
#endif
