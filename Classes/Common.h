#pragma once

#include "cocos2d.h"

using namespace std;
using namespace cocos2d;

// 朝向：右、下、左、上
typedef enum _faceForward
{
	right,
	down,
	left,
	up
} faceForward;

// 玩家状态正常、停车（2）、监狱（3）
typedef enum _stateType
{
	normal,
	parking,
	prison
} stateType;

// 玩家：名称、角色序号、图片、图片位置、角色位置、朝向、是否在移动、代表颜色、资金、玩家状态、滞留回合数
struct Player
{
	string name;
	int serialNumber;
	Sprite* roleSprite;
	Vec2 spritePosition;
	Vec2 rolePosition;
	faceForward faceTo;
	bool isGoing;
	Color3B color;
	int money;
	stateType state;
	int stayRound;
};