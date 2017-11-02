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
};

class NewGameScene : public Scene
{
private:
	const unsigned int empty_land = 110;
	const unsigned int level1_land = 454;
	const unsigned int level2_land = 451;
	const unsigned int level3_land = 466;


	// 初始新游戏，条件选择

	// 游戏玩家数量
	int playersNumber;
	// 初始资金
	float startMoney;
	// 胜利条件

	
	// 显示尺寸
	Size visibleSize;
	// 显示原点
	Vec2 origin;

	// TMX 地图
	TMXTiledMap* tileMap;
	TMXLayer* background;
	TMXLayer* road;
	TMXLayer* land;
	TMXObjectGroup* objectGroup;
	
	// 角色集合
	vector<Player> players;

	// 正在操作的玩家序号
	int nowPlayerNumber;
	
	Vec2 playerPoint;
	faceForward nowFace = faceForward::right;
	bool isGoing = false;

	Sprite* dicePointS;

	// Belong to checkLand()
	// 地块图片、地块GID、现在的地块坐标、Menu面板
	Sprite* sLand;
	uint32_t gLand;
	Vec2 nowLand;
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

	void diceEvent(Ref* pSender,Widget::TouchEventType type);

	void playerGo(float dt, string playerName);
	void removeDicePointS(float dt, string playerName);
	void checkLand();
	void menuYes();
	void menuNo();

	CREATE_FUNC(NewGameScene);
};
