#pragma once

#include "cocos2d.h"
#include "ContinueMainLayer.h"

using namespace std;
using namespace cocos2d;

class ContinueGameScene : public Scene
{
public:
	ContinueGameScene();
	~ContinueGameScene();

	virtual bool init();

	CREATE_FUNC(ContinueGameScene);
};