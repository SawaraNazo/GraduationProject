#include "GameInitLayer.h"

GameInitLayer::GameInitLayer()
{

}

GameInitLayer::~GameInitLayer()
{

}

int GameInitLayer::getPlayersNumber()
{
	return playersNumber;
}

void GameInitLayer::setPlayersRoles(vector<int> v)
{
	playersRoles.clear();

	for (auto i : v)
	{
		playersRoles.push_back(i);
	}
}

vector<int> GameInitLayer::getPlayersRoles()
{
	return playersRoles;
}

bool GameInitLayer::init()
{
	if (!Layer::init())
	{
		return false;
	}

	visibleSize = Director::getInstance()->getVisibleSize();

	this->createMenu();

	return true;
}

void GameInitLayer::createMenu()
{
	dict = Dictionary::createWithContentsOfFile("XML/GameInit.xml");

	m = Menu::create();

	this->createBackground();
	this->nzPlayersNumber();
	this->nzStartMoney();
	this->nzPlayersRoles();
	this->nzMapNumber();
	
	// 开始游戏
	Sprite* nok = Sprite::create("image/GreenNormal.png");
	Sprite* pok = Sprite::create("image/GreenPressed.png");
	MenuItem* mok = MenuItemSprite::create(nok, pok, CC_CALLBACK_1(GameInitLayer::okCallback, this));
	mok->setPosition(-visibleSize.width / 4, -visibleSize.height * 3 / 8 - mok->getContentSize().height / 4);

	auto ok = ((String*)dict->objectForKey("ok"))->getCString();
	Label* lok = Label::create(ok, "arial", 18);
	lok->setTextColor(Color4B::BLACK);
	lok->setPosition(mok->getContentSize().width / 2, mok->getContentSize().height / 2);
	mok->addChild(lok);
	m->addChild(mok);

	// 返回主菜单
	Sprite* ncan = Sprite::create("image/RedNormal.png");
	Sprite* pcan = Sprite::create("image/RedPressed.png");
	MenuItem* mcan = MenuItemSprite::create(ncan, pcan, CC_CALLBACK_1(GameInitLayer::cancelCallback, this));
	mcan->setPosition(visibleSize.width / 4, -visibleSize.height * 3 / 8 - mcan->getContentSize().height / 4);

	auto cancel = ((String*)dict->objectForKey("cancel"))->getCString();
	Label* lcancel = Label::create(cancel, "arial", 18);
	lcancel->setTextColor(Color4B::BLACK);
	lcancel->setPosition(mcan->getContentSize().width / 2, mcan->getContentSize().height / 2);
	mcan->addChild(lcancel);
	m->addChild(mcan);

	this->addChild(m);
}

void GameInitLayer::createBackground()
{
	Sprite* bg = Sprite::create("image/GameInitBG.png");
	bg->setPosition(visibleSize.width / 2, visibleSize.height / 2);
	this->addChild(bg);
}

void GameInitLayer::nzPlayersNumber()
{
	auto d = ((String*)dict->objectForKey("playersNumber"))->getCString();
	Label* l = Label::create(d, "arial", 24);
	l->setTextColor(Color4B::ORANGE);
	l->setPosition(visibleSize.width / 8, visibleSize.height * 7 / 8);
	this->addChild(l);

	Label* number = Label::create(to_string(playersNumber), "arial", 20);
	number->setTextColor(Color4B::ORANGE);
	number->setPosition(visibleSize.width * 3 / 8, l->getPosition().y);
	this->addChild(number);

	Sprite* left_normal = Sprite::create("image/button-normal-left.png");
	Sprite* left_pressed = Sprite::create("image/button-pressed-left.png");
	MenuItem* left_mi = MenuItemSprite::create(left_normal, left_pressed, CC_CALLBACK_1(GameInitLayer::playersNumberLeft, this, number));
	left_mi->setPosition(-visibleSize.width * 3 / 16, visibleSize.height * 3 / 8);

	Sprite* right_normal = Sprite::create("image/button-normal-right.png");
	Sprite* right_pressed = Sprite::create("image/button-pressed-right.png");
	MenuItem* right_mi = MenuItemSprite::create(right_normal, right_pressed, CC_CALLBACK_1(GameInitLayer::playersNumberRight, this, number));
	right_mi->setPosition(-visibleSize.width / 16, visibleSize.height * 3 / 8);

	m->addChild(left_mi);
	m->addChild(right_mi);
}

void GameInitLayer::playersNumberLeft(Ref* pSender, Label* &l)
{
	--playersNumber;

	if (playersNumber == 1)
	{
		playersNumber = 4;
	}
	
	l->setString(to_string(playersNumber));
}

void GameInitLayer::playersNumberRight(Ref* pSender, Label* &l)
{
	++playersNumber;

	if (playersNumber == 5)
	{
		playersNumber = 2;
	}

	l->setString(to_string(playersNumber));
}

void GameInitLayer::nzStartMoney()
{
	auto d = ((String*)dict->objectForKey("startMoney"))->getCString();
	Label* l = Label::create(d, "arial", 24);
	l->setTextColor(Color4B::ORANGE);
	l->setPosition(visibleSize.width / 8, visibleSize.height / 2);
	this->addChild(l);

	Label* number = Label::create(to_string(startMoney), "arial", 20);
	number->setTextColor(Color4B::ORANGE);
	number->setPosition(visibleSize.width * 3 / 8, l->getPosition().y);
	this->addChild(number);

	Sprite* left_normal = Sprite::create("image/button-normal-left.png");
	Sprite* left_pressed = Sprite::create("image/button-pressed-left.png");
	MenuItem* left_mi = MenuItemSprite::create(left_normal, left_pressed, CC_CALLBACK_1(GameInitLayer::startMoneyLeft, this, number));
	left_mi->setPosition(-visibleSize.width * 3 / 16, 0);

	Sprite* right_normal = Sprite::create("image/button-normal-right.png");
	Sprite* right_pressed = Sprite::create("image/button-pressed-right.png");
	MenuItem* right_mi = MenuItemSprite::create(right_normal, right_pressed, CC_CALLBACK_1(GameInitLayer::startMoneyRight, this, number));
	right_mi->setPosition(-visibleSize.width / 16, 0);

	m->addChild(left_mi);
	m->addChild(right_mi);
}

void GameInitLayer::startMoneyLeft(Ref* pSender, Label* &l)
{
	for (int i = 0; i < v.size(); ++i)
	{
		if (startMoney == v[i])
		{
			if (i == 0)
			{
				startMoney = v[v.size() - 1];
			}
			else
			{
				startMoney = v[i - 1];
			}
			break;
		}
	}

	l->setString(to_string(startMoney));
}

void GameInitLayer::startMoneyRight(Ref* pSender, Label* &l)
{
	for (int i = 0; i < v.size(); ++i)
	{
		if (startMoney == v[i])
		{
			if (i == v.size() - 1)
			{
				startMoney = v[0];
			}
			else
			{
				startMoney = v[i + 1];
			}
			break;
		}
	}

	l->setString(to_string(startMoney));
}

void GameInitLayer::nzPlayersRoles()
{
	Sprite* bn = Sprite::create("image/OrangeNormal.png");
	Sprite* bp = Sprite::create("image/OrangePressed.png");

	MenuItem* mi = MenuItemSprite::create(bn, bp, CC_CALLBACK_1(GameInitLayer::createPRLayer, this));
	mi->setPosition(-visibleSize.width / 4, -visibleSize.height / 4);
	mi->setScale(1.5);
	auto mC = mi->getContentSize();
	
	auto d = ((String*)dict->objectForKey("playersRoles"))->getCString();
	Label* l = Label::create(d, "arial", 16);
	l->setTextColor(Color4B::BLACK);
	l->setPosition(mC.width / 2, mC.height / 2);

	mi->addChild(l);
	m->addChild(mi);
}

void GameInitLayer::createPRLayer(Ref* pSender)
{
	auto s = this->getParent();
	auto rl = SelectGameRolesLayer::create();
	s->addChild(rl, 10);
}

void GameInitLayer::nzMapNumber()
{
	auto d = ((String*)dict->objectForKey("mapNumber"))->getCString();
	Label* l = Label::create(d, "arial", 24);
	l->setTextColor(Color4B::ORANGE);
	l->setPosition(visibleSize.width * 3 / 4, visibleSize.height * 7 / 8);
	this->addChild(l);

	Sprite* s = Sprite::create("image/map" + to_string(mapNumber) + ".png");
	s->setPosition(visibleSize.width * 3 / 4, visibleSize.height / 2);
	this->addChild(s);

	Sprite* left_normal = Sprite::create("image/button-normal-left.png");
	Sprite* left_pressed = Sprite::create("image/button-pressed-left.png");
	MenuItem* left_mi = MenuItemSprite::create(left_normal, left_pressed, CC_CALLBACK_1(GameInitLayer::mapNumberLeft, this, s));
	left_mi->setPosition(left_mi->getContentSize().width, 0);

	Sprite* right_normal = Sprite::create("image/button-normal-right.png");
	Sprite* right_pressed = Sprite::create("image/button-pressed-right.png");
	MenuItem* right_mi = MenuItemSprite::create(right_normal, right_pressed, CC_CALLBACK_1(GameInitLayer::mapNumberRight, this, s));
	right_mi->setPosition(visibleSize.width / 2 - right_mi->getContentSize().width, 0);

	m->addChild(left_mi);
	m->addChild(right_mi);
}

void GameInitLayer::mapNumberLeft(Ref* pSender,Sprite* &s)
{
	--mapNumber;
	if (mapNumber == 0)
	{
		mapNumber = 3;
	}

	string str = "image/map" + to_string(mapNumber) + ".png";
	Sprite* spr = Sprite::create(str);
	s->setTexture(spr->getTexture());
}

void GameInitLayer::mapNumberRight(Ref* pSender, Sprite* &s)
{
	++mapNumber;
	if (mapNumber == 4)
	{
		mapNumber = 1;
	}

	string str = "image/map" + to_string(mapNumber) + ".png";
	Sprite* spr = Sprite::create(str);
	s->setTexture(spr->getTexture());
}

void GameInitLayer::okCallback(Ref* pSender)
{
	auto s = this->getParent();
	auto main = GameMainLayer::create();

	main->setPlayersNumber(this->playersNumber);
	main->setStartMoney(this->startMoney);
	main->setPlayersRoles(this->playersRoles);
	main->setMapNumber(this->mapNumber);

	s->addChild(main);
	this->removeFromParentAndCleanup(true);
}

void GameInitLayer::cancelCallback(Ref* pSender)
{
	Director::getInstance()->popScene();
}