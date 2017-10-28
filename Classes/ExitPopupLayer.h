#pragma once

#include "cocos2d.h"

using namespace cocos2d;

class ExitPopupLayer : public Layer
{
private:
	Size visibleSize;
	Vec2 origin;
public:
	ExitPopupLayer();
	~ExitPopupLayer();

	virtual bool init();

	void createMenu();

	void okCallBack(Ref* pSender);
	void cancelCallBack(Ref* pSender);

	CREATE_FUNC(ExitPopupLayer);
};