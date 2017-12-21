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
	// 不同等级地块对应GID

	// 空地
	const unsigned int empty_land_GID = 110;
	// 一级建筑
	const unsigned int level1_land_GID = 454;
	// 二级建筑
	const unsigned int level2_land_GID = 451;
	// 三级建筑
	const unsigned int level3_land_GID = 466;


	// 不同道路对应的GID

	// 起始点
	const unsigned int entrance_road_GID = 390;
	// 普通道路
	const unsigned int normal_road_GID = 568;
	// 监狱入口
	const unsigned int prisonEntrance_road_GID = 398;
	// 监狱
	const unsigned int prison_road_GID = 436;
	// 突发事件
	const unsigned int emergency_road_GID = 1038;
	// 停车场
	const unsigned int parkinglot_road_GID = 1046;
	// 交税
	const unsigned int tax_road_GID = 1460;


	// 初始新游戏，条件选择

	// 游戏玩家数量
	int playersNumber;
	// 初始资金
	int startMoney;
	// 胜利条件  XD
	// 地图名字
	int mapNumber;

	// 突发事件数量
	int emEventNumber;

	// 显示尺寸
	Size visibleSize;
	// 显示原点
	Vec2 origin;

	// TMX 地图
	TMXTiledMap* tileMap;

	TMXLayer* road;
	int landLevelNumber;
	vector<TMXLayer*> lands;
	TMXObjectGroup* objectGroup;

	// 玩家角色集合
	vector<int> roles;
	// 角色集合
	vector<Player> players;

	// 正在操作的玩家序号
	int nowPlayerNumber;

	// 骰子图片
	Sprite* dicePointS;


	// Belong to checkLand()

	// 地块图片
	Sprite* sLand;
	// 地块GID（+1）
	uint32_t gLand;
	// 现在的地块坐标
	Vec2 nowLand;
	// Menu面板及图片
	Menu* noticeMenu;
	Sprite* menuBoard;

	// 回合数
	int rounds;

public:
	ContinueMainLayer();
	~ContinueMainLayer();

	// private变量相关
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

	// 定时删除粒子
	void removeParticle(float dt);
	// 加载中 Loading
	void continueLoading(float dt);
	// 定时删除 NowLoading
	void removeLoading(float dt);

	// 初始化
	virtual bool init();
	void setParameter(float dt);

	// 创建地图
	void createMap();
	// 创造玩家
	void createPlayer();
	// 创建游戏面板
	void createPlayerPro();
	// 掷骰子
	void diceEvent(Ref* pSender, Widget::TouchEventType type);
	// 设置
	void setEvent(Ref* pSender, Widget::TouchEventType type);
	// 保存
	void saveEvent(Ref* pSender, Widget::TouchEventType type);
	// 关闭
	void exitEvent(Ref* pSender, Widget::TouchEventType type);
	// 走两步
	void playerGo(float dt);

	// 检查玩家状态
	bool checkState();
	// 检查道路
	void checkRoad(float dt);
	// 检查土地归属
	void checkLand(float dt);
	// 空地
	void emptyLand();
	void emptyMenuYes();
	void emptyMenuNo();
	// 自己的土地
	void myLand();
	void myMenuYes();
	void myMenuNo();
	// 别人的土地
	void otherLand();
	void checkPayPlayerMoney(string payName, string earnName, int price);
	void checkToClose(Ref* pSender, Widget::TouchEventType type, string payName, string earnName, int price);
	void otherMenuClose(string payName, string earnName, int price);
	// 交税
	void checkPayPlayerMoneyLo(string payName, int loss);
	void checkToCloseLo(Ref* pSender, Widget::TouchEventType type, string payName, int loss);
	void payLoss(string payName, int loss);

	// 清除面板
	void cleanMenu();
	// 轮换下一位玩家
	void changePlayer();
	// 清除无资金玩家
	void removePlayer(int number);
	// 清楚面板轮换玩家
	void cleanAndChange();
	// 结束游戏
	void endGame();

	CREATE_FUNC(GameMainLayer);
};