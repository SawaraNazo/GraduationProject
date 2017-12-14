#pragma once

#include "cocos2d.h"
#include "cocos-ext.h"
#include "SimpleAudioEngine.h"

using namespace cocos2d;
using namespace extension;
using namespace CocosDenshion;

class TitleGameSetLayer : public Layer
{
private:
	Size visibleSize;
	Vec2 origin;
public:
	TitleGameSetLayer();
	~TitleGameSetLayer();

	virtual bool init();

	void createMenu();

	void closeCallback(Ref* pSender);
	void switchStateChanged(Ref* pSender, Control::EventType controlEvent);
	void sliderStateChanged(Ref* pSender, Control::EventType controlEvent);

	CREATE_FUNC(TitleGameSetLayer);
};