#pragma once

#include "cocos2d.h"
#include "extensions\cocos-ext.h"
#include "SimpleAudioEngine.h"

using namespace std;
using namespace cocos2d;
using namespace cocos2d::ui;
using namespace CocosDenshion;

// 朝向：右、下、左、上
typedef enum _faceForward
{
	right,
	down,
	left,
	up
}faceForward;

// 玩家：名称、图片、图片位置、角色位置、土地位置、朝向、是否在移动、代表颜色
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
	// 不同等级地块对应GID
	const unsigned int empty_land_GID = 110;
	const unsigned int level1_land_GID = 454;
	const unsigned int level2_land_GID = 451;
	const unsigned int level3_land_GID = 466;

	// 不同等级升级花费
	
	// 不同等级过路花费


	// 初始新游戏，条件选择

	// 游戏玩家数量
	int playersNumber;
	// 初始资金
	int startMoney;
	// 胜利条件


	// 显示尺寸
	Size visibleSize;
	// 显示原点
	Vec2 origin;

	// TMX 地图
	TMXTiledMap* tileMap;

	TMXLayer* background;
	TMXLayer* road;
	int landLevelNumber;
	vector<TMXLayer*> lands;
	TMXObjectGroup* objectGroup;

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


public:
	NewGameScene();
	~NewGameScene();

	// 初始化
	virtual bool init();

	// 创建地图
	void createMap();
	// 创造玩家
	void createPlayer();
	// 创建游戏面板
	void createPlayerPro();

	void diceEvent(Ref* pSender, Widget::TouchEventType type);

	void playerGo(float dt);

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
	void otherMenuClose();

	CREATE_FUNC(NewGameScene);
};
