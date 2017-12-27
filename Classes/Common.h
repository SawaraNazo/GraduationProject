#pragma once

#include "cocos2d.h"

using namespace std;
using namespace cocos2d;

// �����ҡ��¡�����
typedef enum _faceForward
{
	right,
	down,
	left,
	up
} faceForward;

// ���״̬������ͣ����2����������3��
typedef enum _stateType
{
	normal,
	parking,
	prison
} stateType;

// ��ң����ơ���ɫ��š�ͼƬ��ͼƬλ�á���ɫλ�á������Ƿ����ƶ���������ɫ���ʽ����״̬�������غ���
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