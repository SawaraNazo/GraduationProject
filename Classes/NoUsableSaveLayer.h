#pragma once

#include "cocos2d.h"

using namespace cocos2d;

class NoUsableSaveLayer : public Layer
{
private:
	Size visibleSize;
public:
	NoUsableSaveLayer();
	~NoUsableSaveLayer();

	virtual bool init();

	void closeCallback(Ref* pSender);

	CREATE_FUNC(NoUsableSaveLayer);
};