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
	int money;
};

class NewGameScene : public Scene
{
private:
	// ��ͬ�ȼ��ؿ��ӦGID
	const unsigned int empty_land_GID = 110;
	const unsigned int level1_land_GID = 454;
	const unsigned int level2_land_GID = 451;
	const unsigned int level3_land_GID = 466;

	// ��ͬ�ȼ���������
	
	// ��ͬ�ȼ���·����


	// ��ʼ����Ϸ������ѡ��

	// ��Ϸ�������
	int playersNumber;
	// ��ʼ�ʽ�
	int startMoney;
	// ʤ������


	// ��ʾ�ߴ�
	Size visibleSize;
	// ��ʾԭ��
	Vec2 origin;

	// TMX ��ͼ
	TMXTiledMap* tileMap;

	TMXLayer* background;
	TMXLayer* road;
	int landLevelNumber;
	vector<TMXLayer*> lands;
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

	void diceEvent(Ref* pSender, Widget::TouchEventType type);

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
