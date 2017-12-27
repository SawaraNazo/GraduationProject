#pragma once

#include "cocos2d.h"

#include <string>
#include <vector>

#include "NewGameScene.h"
#include "SelectGameRolesLayer.h"
#include "GameMainLayer.h"

using namespace std;
using namespace cocos2d;

class GameInitLayer :public Layer
{
private:
	Size visibleSize;

	__Dictionary* dict;
	Menu* m;
	vector<int> v{ 500,1000,2000,5000,10000 };

	// 玩家数量
	int playersNumber = 2;
	// 地图选择
	int mapNumber = 1;
	// 初始金钱数
	int startMoney = 500;
	// 游戏角色				可选择Role数量：5
	vector<int> playersRoles{ 1,2,3,4 };

public :
	GameInitLayer();
	~GameInitLayer();

	int getPlayersNumber();
	vector<int> getPlayersRoles();
	void setPlayersRoles(vector<int> v);

	virtual bool init();

	void createMenu();
	void createBackground();

	void nzPlayersNumber();
	void playersNumberLeft(Ref* pSender,Label* &l);
	void playersNumberRight(Ref* pSender,Label* &l);

	void nzStartMoney();
	void startMoneyLeft(Ref* pSender, Label* &l);
	void startMoneyRight(Ref* pSender, Label* &l);

	void nzMapNumber();
	void mapNumberLeft(Ref* pSender, Sprite* &s);
	void mapNumberRight(Ref* pSender, Sprite* &s);

	void nzPlayersRoles();
	void createPRLayer(Ref* pSender);

	void okCallback(Ref* pSender);
	void cancelCallback(Ref* pSender);

	CREATE_FUNC(GameInitLayer);
};