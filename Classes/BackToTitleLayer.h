#pragma once

#include "cocos2d.h"

using namespace cocos2d;

class BackToTitleLayer : public Layer
{
private :
	Size visibleSize;

public:
	BackToTitleLayer();
	~BackToTitleLayer();

	virtual bool init();

	void okCallback(Ref* pSender);
	void cancelCallback(Ref* pSender);

	CREATE_FUNC(BackToTitleLayer);
};