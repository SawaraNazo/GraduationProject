#pragma once

#include "cocos2d.h"

#include <string>
#include <vector>

#include "SaveGameCoverLayer.h"
#include "GameMainLayer.h"

using namespace std;
using namespace cocos2d;

class SaveGameLayer : public Layer
{
private:
	Size visibleSize;

public:
	SaveGameLayer();
	~SaveGameLayer();

	virtual bool init();

	void saveCallback(Ref* pSender);
	void cancelCallback(Ref* pSender);

	CREATE_FUNC(SaveGameLayer);
};