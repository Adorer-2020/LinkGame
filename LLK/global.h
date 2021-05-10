#pragma once
#include "CGameSetting.h"
#include <string>
using namespace std;

typedef struct tagVertex 
{
	int row;   // 行
	int col;   // 列
	int info;  // 信息类
} Vertex;

#define GAMEWND_WIDTH 800
#define GAMEWND_HEIGHT 600
#define BLANK -1

#define BASIC 1
#define RELAX 2 
#define LEVEL 3

#define PLAY_TIMER_ID 1

#define SKY1 1
#define SKY2 2
#define ARENA 3
#define FRUIT 4

typedef struct user
{
	string name;	// 用户名
	int mode;		// 游戏模式
	int score;		// 得分
} User;

