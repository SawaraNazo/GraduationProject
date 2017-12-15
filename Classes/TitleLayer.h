#pragma once

#include "cocos2d.h"
#include "ExitPopupLayer.h"
#include "NewGameScene.h"
#include "SimpleAudioEngine.h"
#include "GameSetLayer.h"

using namespace std;
using namespace cocos2d;
using namespace CocosDenshion;

class TitleLayer : public Layer
{
private:
	Size visibleSize;
	Vec2 origin;

public:
	TitleLayer();
	~TitleLayer();

	virtual bool init();

	void createLogo();
	void createMenu();

	void newGameCallback(Ref* pSender);
	void loadGameCallback(Ref* pSender);
	void settingCallback(Ref* pSender);
	void exitCallback(Ref* pSender);

	CREATE_FUNC(TitleLayer);
};

