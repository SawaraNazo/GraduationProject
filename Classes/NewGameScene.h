#pragma once

#include "cocos2d.h"
#include "extensions\cocos-ext.h"
#include "SimpleAudioEngine.h"

using namespace cocos2d;
using namespace cocos2d::ui;
using namespace CocosDenshion;

typedef enum _faceForward
{
	right,
	down,
	left,
	up
}faceForward;

class NewGameScene : public Scene
{
private:
	const unsigned int empty_land = 110;
	const unsigned int level1_land = 454;
	const unsigned int level2_land = 451;
	const unsigned int level3_land = 466;

	const Color3B player1_color = Color3B::RED;
	const Color3B player2_color = Color3B::BLUE;
	const Color3B player3_color = Color3B::YELLOW;
	const Color3B player4_color = Color3B::GREEN;

	Size visibleSize;
	Vec2 origin;
	TMXTiledMap* tileMap;
	TMXLayer* background;
	TMXLayer* road;
	TMXLayer* land;

	TMXObjectGroup* objectGroup;
	Vec2 playerPoint;
	faceForward nowFace = faceForward::right;

	Sprite* dicePointS;
	bool isGoing = false;

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

	virtual bool init();

	void createMap();
	void createPlayer();
	void createPlayerPro();

	void diceEvent(Ref* pSender,Widget::TouchEventType type);

	void playerGo(float dt);
	void removeDicePointS(float dt);
	void checkLand();
	void menuYes();
	void menuNo();

	CREATE_FUNC(NewGameScene);
};

