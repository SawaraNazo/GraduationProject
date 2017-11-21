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
} faceForward;

// ���״̬������ͣ����2����������3��
typedef enum _stateType
{
	normal,
	parking,
	prison
} stateType;

// ��ң����ơ�ͼƬ��ͼƬλ�á���ɫλ�á�����λ�á������Ƿ����ƶ���������ɫ�����״̬
struct Player
{
	string name;
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

class NewGameScene : public Scene
{
private:
	// ��ͬ�ȼ��ؿ��ӦGID

	// �յ�
	const unsigned int empty_land_GID = 110;
	// һ������
	const unsigned int level1_land_GID = 454;
	// ��������
	const unsigned int level2_land_GID = 451;
	// ��������
	const unsigned int level3_land_GID = 466;
	

	// ��ͬ��·��Ӧ��GID

	// ��ʼ��
	const unsigned int enterance_road_GID = 390;
	// ��ͨ��·
	const unsigned int normal_road_GID = 568;
	// �������
	const unsigned int prisonEnterance_road_GID = 398;
	// ����
	const unsigned int prison_road_GID = 536;
	// ͻ���¼�
	const unsigned int emergency_road_GID = 1038;
	// ͣ����
	const unsigned int parkinglot_road_GID = 1046;
	// ��˰
	const unsigned int tax_road_GID = 1460;

	
	// ��ʼ����Ϸ������ѡ��

	// ��Ϸ�������
	int playersNumber;
	// ��ʼ�ʽ�
	int startMoney;
	// ʤ������

	// ͻ���¼�����
	int emEventNumber;

	// ��ʾ�ߴ�
	Size visibleSize;
	// ��ʾԭ��
	Vec2 origin;

	// TMX ��ͼ
	TMXTiledMap* tileMap;

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

	// �غ���
	int rounds;


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
	// ������
	void diceEvent(Ref* pSender, Widget::TouchEventType type);
	// ������
	void playerGo(float dt);

	// ������״̬
	bool checkState();
	// ����·
	void checkRoad(float dt);
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
	void otherMenuClose(string payName, string earnName,int price);
	// ��˰
	void payLoss(string payName, int loss);
	// ������
	void cleanMenu();
	// �ֻ���һλ���
	void changePlayer();
	// �����岢��
	void cleanAndChange();

	CREATE_FUNC(NewGameScene);
};
