#pragma once

#include "cocos2d.h"

using namespace cocos2d;

class TitleBackgroundLayer : public Layer
{
private:
	Size visibleSize;
	Vec2 origin;
public:
	TitleBackgroundLayer();
	~TitleBackgroundLayer();

	virtual bool init();
	void createBackground();

	CREATE_FUNC(TitleBackgroundLayer);
};

