#pragma once

#include "cocos2d.h"
#include <vector>

using namespace std;
using namespace cocos2d;

class SelectGameRolesLayer :public Layer
{
private:
	Size visibleSize;
	Vec2 origin;

public:
	SelectGameRolesLayer();
	~SelectGameRolesLayer();

	virtual bool init();

	void createMenu();

	CREATE_FUNC(SelectGameRolesLayer);
};