#include "SaveGameCoverLayer.h"

SaveGameCoverLayer::SaveGameCoverLayer()
{

}

SaveGameCoverLayer::~SaveGameCoverLayer()
{

}

bool SaveGameCoverLayer::init()
{
	if (!Layer::init())
	{
		return false;
	}

	visibleSize = Director::getInstance()->getVisibleSize();

	Sprite* s = Sprite::create("image/SaveCover.png");
	s->setPosition(visibleSize / 2);
	this->addChild(s, 10, "cover");

	Menu* m = Menu::create();

	// Y~es
	Sprite* okn = Sprite::create("image/GreenNormal.png");
	Sprite* okp = Sprite::create("image/GreenPressed.png");
	auto okmi = MenuItemSprite::create(okn, okp, CC_CALLBACK_1(SaveGameCoverLayer::okCallback, this));
	okmi->setPosition(-s->getContentSize().width / 2 + okmi->getContentSize().width * 3 / 4,
		-s->getContentSize().height / 4 - okmi->getContentSize().height);

	Label* okl = Label::create("Yes", "arial", 20);
	okl->setColor(Color3B::BLACK);
	okl->setPosition(okmi->getContentSize().width / 2, okmi->getContentSize().height / 2);
	okmi->addChild(okl);

	m->addChild(okmi);

	// N~o
	Sprite* cn = Sprite::create("image/RedNormal.png");
	Sprite* cp = Sprite::create("image/RedPressed.png");
	auto cmi = MenuItemSprite::create(cn, cp, CC_CALLBACK_1(SaveGameCoverLayer::cancelCallback, this));
	cmi->setPosition(s->getContentSize().width / 2 - cmi->getContentSize().width * 3 / 4,
		-s->getContentSize().height / 4 - cmi->getContentSize().height);

	Label* cl = Label::create("No", "arial", 20);
	cl->setColor(Color3B::BLACK);
	cl->setPosition(cmi->getContentSize().width / 2, cmi->getContentSize().height / 2);
	cmi->addChild(cl);

	m->addChild(cmi);

	this->addChild(m, 10, "menu");

	// ÍÌÊÂ¼þ
	auto callback = [](Touch*, Event*) {return true; };
	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = callback;
	listener->setSwallowTouches(true);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

	return true;
}

void SaveGameCoverLayer::okCallback(Ref* pSender)
{
	auto gm = (GameMainLayer*)(this->getParent());

	UserDefault::getInstance()->setIntegerForKey("playersNumber", gm->getPlayersNumber());
	UserDefault::getInstance()->setIntegerForKey("startMoney", gm->getStartMoney());
	UserDefault::getInstance()->setIntegerForKey("mapNumber", gm->getMapNumber());
	UserDefault::getInstance()->setIntegerForKey("emEventNumber", gm->getEmEventNumber());
	UserDefault::getInstance()->setIntegerForKey("rounds", gm->getRounds());
	UserDefault::getInstance()->setIntegerForKey("nowPlayerNumber", gm->getNowPlayerNumber());

	vector<int> vr = gm->getRoles();

	for (int n = 0; n < vr.size(); ++n)
	{
		string s = "role" + to_string(n);
		UserDefault::getInstance()->setIntegerForKey(s.c_str(), vr[n]);
	}

	vector<Player> vp = gm->getPlayers();

	for (int n = 0; n < vp.size(); ++n)
	{
		string s1 = "player" + to_string(n) + "name";
		UserDefault::getInstance()->setStringForKey(s1.c_str(), vp[n].name);
		// roleSprite
		string s2 = "player" + to_string(n) + "spritePositionX";
		UserDefault::getInstance()->setFloatForKey(s2.c_str(), vp[n].spritePosition.x);
		string s3 = "player" + to_string(n) + "spritePositionY";
		UserDefault::getInstance()->setFloatForKey(s3.c_str(), vp[n].spritePosition.y);
		string s4 = "player" + to_string(n) + "rolePositionX";
		UserDefault::getInstance()->setFloatForKey(s4.c_str(), vp[n].rolePosition.x);
		string s5 = "player" + to_string(n) + "rolePositionY";
		UserDefault::getInstance()->setFloatForKey(s5.c_str(), vp[n].rolePosition.y);
		string s6 = "player" + to_string(n) + "faceTo";
		UserDefault::getInstance()->setIntegerForKey(s6.c_str(), vp[n].faceTo);
		string s7 = "player" + to_string(n) + "isGoing";
		UserDefault::getInstance()->setBoolForKey(s7.c_str(), vp[n].isGoing);
		string s8 = "player" + to_string(n) + "colorR";
		UserDefault::getInstance()->setIntegerForKey(s8.c_str(), vp[n].color.r);
		string s9 = "player" + to_string(n) + "colorG";
		UserDefault::getInstance()->setIntegerForKey(s9.c_str(), vp[n].color.g);
		string s10 = "player" + to_string(n) + "colorB";
		UserDefault::getInstance()->setIntegerForKey(s10.c_str(), vp[n].color.b);
		string s11 = "player" + to_string(n) + "money";
		UserDefault::getInstance()->setIntegerForKey(s11.c_str(), vp[n].money);
		string s12 = "player" + to_string(n) + "state";
		UserDefault::getInstance()->setIntegerForKey(s12.c_str(), vp[n].state);
		string s13 = "player" + to_string(n) + "stayRound";
		UserDefault::getInstance()->setIntegerForKey(s13.c_str(), vp[n].stayRound);
	}

	UserDefault::getInstance()->flush();

	this->removeChildByName("cover", true);
	this->removeChildByName("menu", true);

	Sprite* s = Sprite::create("image/SaveSuccess.png");
	s->setPosition(visibleSize / 2);
	this->addChild(s);

	Menu* m = Menu::create();

	// Y~es
	Sprite* okn = Sprite::create("image/GreenNormal.png");
	Sprite* okp = Sprite::create("image/GreenPressed.png");
	auto okmi = MenuItemSprite::create(okn, okp, CC_CALLBACK_1(SaveGameCoverLayer::cancelCallback, this));
	okmi->setPosition(0, -s->getContentSize().height / 4 - okmi->getContentSize().height);

	Label* okl = Label::create("Yes", "arial", 20);
	okl->setColor(Color3B::BLACK);
	okl->setPosition(okmi->getContentSize().width / 2, okmi->getContentSize().height / 2);
	okmi->addChild(okl);

	m->addChild(okmi);
	this->addChild(m);
}

void SaveGameCoverLayer::cancelCallback(Ref* pSender)
{
	this->removeFromParentAndCleanup(true);
}