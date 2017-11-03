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
	
	// ����ͼƬ
	Sprite* dicePointS;

	
	// Belong to checkLand()

	// �ؿ�ͼƬ
	Sprite* sLand;
	// �ؿ�GID��+1��
	uint32_t gLand;
	// ���ڵĵؿ�����
	Vec2 nowLand;
	// Menu��弰ͼƬ
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

	void playerGo(float dt);

	// ������ع���
	void checkLand(float dt);
	// �յ�
	void emptyLand();
	void emptyMenuYes();
	void emptyMenuNo();
	// �Լ�������
	void myLand();
	void myMenuYes();
	void myMenuNo();
	// ���˵�����
	void otherLand();
	void otherMenuClose();
	
	CREATE_FUNC(NewGameScene);
};
