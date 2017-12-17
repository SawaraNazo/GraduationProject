#pragma once

#include "cocos2d.h"
#include <vector>

using namespace std;
using namespace cocos2d;

class GameInitLayer :public Layer
{
private:
	Size visibleSize;
	Vec2 origin;

	__Dictionary* dict;
	Menu* m;
	vector<int> v{ 500,1000,2000,5000,10000 };

	// �������
	int playersNumber = 2;
	// ��ͼѡ��
	int mapNumber = 1;
	//��ʼ��Ǯ��
	int startMoney = 500;
	//��Ϸ��ɫ
	vector<int> playersRoles;

public :
	GameInitLayer();
	~GameInitLayer();

	virtual bool init();

	void createMenu();
	void createBackground();

	void setPlayersNumber();
	void playersNumberLeft(Ref* pSender,Label* &l);
	void playersNumberRight(Ref* pSender,Label* &l);

	void setStartMoney();
	void startMoneyLeft(Ref* pSender, Label* &l);
	void startMoneyRight(Ref* pSender, Label* &l);

	void setMapNumber();
	void mapNumberLeft(Ref* pSender);
	void mapNumberRight(Ref* pSender);

	void setPlayersRoles();
	void createPRLayer(Ref* pSender);

	void okCallback(Ref* pSender);
	void cancelCallback(Ref* pSender);

	CREATE_FUNC(GameInitLayer);
};