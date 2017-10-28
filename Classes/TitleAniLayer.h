#pragma once

#include "cocos2d.h"
#include "TitleScene.h"

using namespace std;
using namespace cocos2d;

class TitleAniLayer : public Layer
{
private:
	Size visibleSize;
	Vec2 origin;
	
	void goNextScene();
public:
	TitleAniLayer();
	~TitleAniLayer();

	void createLabels();

	virtual bool init();
	void update(float dt);

	CREATE_FUNC(TitleAniLayer);
};

