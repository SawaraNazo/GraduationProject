#pragma once

#include "cocos2d.h"

#include <string>
#include <vector>

#include "GameMainLayer.h"

using namespace std;
using namespace cocos2d;

class SaveGameCoverLayer : public Layer
{
private:
	Size visibleSize;

public:
	SaveGameCoverLayer();
	~SaveGameCoverLayer();

	virtual bool init();

	void okCallback(Ref* pSender);
	void cancelCallback(Ref* pSender);

	CREATE_FUNC(SaveGameCoverLayer);
};