#pragma once

#include "cocos2d.h"
#include "TitleScene.h"

#include "GameInitLayer.h"

using namespace std;
using namespace cocos2d;

class NewGameScene : public Scene
{
private:
public:
	NewGameScene();
	~NewGameScene();

	virtual bool init();
	
	CREATE_FUNC(NewGameScene);
};
