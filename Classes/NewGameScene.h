#pragma once

#include "cocos2d.h"

#include "GameInitLayer.h"

using namespace std;
using namespace cocos2d;

class NewGameScene : public Scene
{
public:
	NewGameScene();
	~NewGameScene();

	virtual bool init();
	
	CREATE_FUNC(NewGameScene);
};
