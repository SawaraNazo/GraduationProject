#pragma once

#include "cocos2d.h"
#include "TitleAniLayer.h"

using namespace std;
using namespace cocos2d;

class TitleAniScene : public Scene
{
public:
	TitleAniScene();
	~TitleAniScene();

	virtual bool init();
	CREATE_FUNC(TitleAniScene);
};

