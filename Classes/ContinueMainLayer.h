#pragma once

#include "cocos2d.h"
#include "cocos-ext.h"
#include "SimpleAudioEngine.h"

#include "GameSetLayer.h"
#include "TitleScene.h"
#include "BackToTitleLayer.h"
#include "SaveGameLayer.h"

#include <string>
#include <vector>

using namespace std;
using namespace cocos2d;
using namespace cocos2d::ui;
using namespace CocosDenshion;

class ContinueMainLayer : public Layer
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
	const unsigned int entrance_road_GID = 390;
	// ��ͨ��·
	const unsigned int normal_road_GID = 568;
	// �������
	const unsigned int prisonEntrance_road_GID = 398;
	// ����
	const unsigned int prison_road_GID = 436;
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
	// ʤ������  XD
	// ��ͼ����
	int mapNumber;

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

	// ��ҽ�ɫ����
	vector<int> roles;
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
	ContinueMainLayer();
	~ContinueMainLayer();

	// private�������
	void setStartMoney(int sm);
	void setPlayersNumber(int pn);
	void setPlayersRoles(vector<int> pr);
	void setMapNumber(int mn);

	int getPlayersNumber();
	int getStartMoney();
	int getMapNumber();
	int getEmEventNumber();
	int getRounds();
	int getNowPlayerNumber();
	vector<int> getRoles();
	vector<Player> getPlayers();

	// ��ʱɾ������
	void removeParticle(float dt);
	// ������ Loading
	void continueLoading(float dt);
	// ��ʱɾ�� NowLoading
	void removeLoading(float dt);

	// ��ʼ��
	virtual bool init();
	void setParameter(float dt);

	// ������ͼ
	void createMap();
	// �������
	void createPlayer();
	// ������Ϸ���
	void createPlayerPro();
	// ������
	void diceEvent(Ref* pSender, Widget::TouchEventType type);
	// ����
	void setEvent(Ref* pSender, Widget::TouchEventType type);
	// ����
	void saveEvent(Ref* pSender, Widget::TouchEventType type);
	// �ر�
	void exitEvent(Ref* pSender, Widget::TouchEventType type);
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
	void checkPayPlayerMoney(string payName, string earnName, int price);
	void checkToClose(Ref* pSender, Widget::TouchEventType type, string payName, string earnName, int price);
	void otherMenuClose(string payName, string earnName, int price);
	// ��˰
	void checkPayPlayerMoneyLo(string payName, int loss);
	void checkToCloseLo(Ref* pSender, Widget::TouchEventType type, string payName, int loss);
	void payLoss(string payName, int loss);

	// ������
	void cleanMenu();
	// �ֻ���һλ���
	void changePlayer();
	// ������ʽ����
	void removePlayer(int number);
	// �������ֻ����
	void cleanAndChange();
	// ������Ϸ
	void endGame();

	CREATE_FUNC(GameMainLayer);
};