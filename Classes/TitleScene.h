#pragma once

#include "cocos2d.h"
#include "TitleLayer.h"
#include "TitleBackgroundLayer.h"
#include "SimpleAudioEngine.h"

using namespace cocos2d;
using namespace CocosDenshion;

class TitleScene : public Scene
{
public:
	TitleScene();
	~TitleScene();

	virtual bool init();

	CREATE_FUNC(TitleScene);
};

