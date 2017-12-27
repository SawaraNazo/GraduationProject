#pragma once

#include "cocos2d.h"
#include "cocos-ext.h"
#include "SimpleAudioEngine.h"

#include <string>

using namespace cocos2d;
using namespace extension;
using namespace CocosDenshion;
using namespace std;

class GameSetLayer : public Layer
{
private:
	Size visibleSize;
	Vec2 origin;

	int nowMusic;

public:
	GameSetLayer();
	~GameSetLayer();

	virtual bool init();

	void createMenu();

	void closeCallback(Ref* pSender);
	void switchStateChanged(Ref* pSender, Control::EventType controlEvent);
	void sliderStateChanged(Ref* pSender, Control::EventType controlEvent);
	void changeBGMPre(Ref* pSender, Label* &bgm, ControlSwitch* cs);
	void changeBGMNext(Ref* pSender, Label* &bgm, ControlSwitch* cs);

	CREATE_FUNC(GameSetLayer);
};