#pragma once

#include "cocos2d.h"
#include <vector>

#include "NewGameScene.h"
#include "GameInitLayer.h"
#include "SelectRoleDuplicateLayer.h"

using namespace std;
using namespace cocos2d;

class SelectGameRolesLayer :public Layer
{
private:
	Size visibleSize;

	int playersNumber;
	vector<int> playersRoles;

public:
	SelectGameRolesLayer();
	~SelectGameRolesLayer();

	void setParameter(float dt);
	virtual bool init();

	void createMenu();

	void leftCallback(Ref* pSender, int n, Sprite* &s);
	void rightCallback(Ref* pSender, int n, Sprite* &s);
	void okCallback(Ref* pSender);

	CREATE_FUNC(SelectGameRolesLayer);
};