#pragma once

#include "cocos2d.h"
#include "extensions\cocos-ext.h"
#include "SimpleAudioEngine.h"

using namespace std;
using namespace cocos2d;
using namespace cocos2d::ui;
using namespace CocosDenshion;

// �����ҡ��¡�����
typedef enum _faceForward
{
	right,
	down,
	left,
	up
}faceForward;

// ��ң����ơ�ͼƬ��ͼƬλ�á���ɫλ�á�����λ�á������Ƿ����ƶ���������ɫ
struct Player
{
	string name;
	Sprite* roleSprite;
	Vec2 spritePosition;
	Vec2 rolePosition;
	Vec2 landPosition;
	faceForward faceTo;
	bool isGoing;
	Color3B color;
};

class NewGameScene : public Scene
{
private:
	const unsigned int empty_land = 110;
	const unsigned int level1_land = 454;
	const unsigned int level2_land = 451;
	const unsigned int level3_land = 466;


	// ��ʼ����Ϸ������ѡ��

	// ��Ϸ�������
	int playersNumber;
	// ��ʼ�ʽ�
	float startMoney;
	// ʤ������

	
	// ��ʾ�ߴ�
	Size visibleSize;
	// ��ʾԭ��
	Vec2 origin;

	// TMX ��ͼ
	TMXTiledMap* tileMap;
	TMXLayer* background;
	TMXLayer* road;
	TMXLayer* land;
	TMXObjectGroup* objectGroup;
	
	// ��ɫ����
	vector<Player> players;

	// ���ڲ�����������
	int nowPlayerNumber;
	
	Vec2 playerPoint;
	faceForward nowFace = faceForward::right;
	bool isGoing = false;

	Sprite* dicePointS;

	// Belong to checkLand()
	// �ؿ�ͼƬ���ؿ�GID�����ڵĵؿ����ꡢMenu���
	Sprite* sLand;
	uint32_t gLand;
	Vec2 nowLand;
	Menu* noticeMenu;
	Sprite* menuBoard;


public:
	NewGameScene();
	~NewGameScene();

	// ��ʼ��
	virtual bool init();

	// ������ͼ
	void createMap();
	// �������
	void createPlayer();
	// ������Ϸ���
	void createPlayerPro();

	void diceEvent(Ref* pSender,Widget::TouchEventType type);

	void playerGo(float dt, string playerName);
	void removeDicePointS(float dt, string playerName);
	void checkLand();
	void menuYes();
	void menuNo();

	CREATE_FUNC(NewGameScene);
};
