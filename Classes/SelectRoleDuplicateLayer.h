#pragma once

#include "cocos2d.h"

using namespace cocos2d;

class SelectRoleDuplicateLayer : public Layer
{
private:
	Size visibleSize;
public:
	SelectRoleDuplicateLayer();
	~SelectRoleDuplicateLayer();

	virtual bool init();

	void okCallback(Ref* pSender);

	CREATE_FUNC(SelectRoleDuplicateLayer);
};