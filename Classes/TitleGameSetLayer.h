#pragma once

#include "cocos2d.h"

using namespace cocos2d;

class TitleGameSetLayer : public Layer
{
private:
	Size visibleSize;
	Vec2 origin;
public:
	TitleGameSetLayer();
	~TitleGameSetLayer();

	virtual bool init();


	CREATE_FUNC(TitleGameSetLayer);
};