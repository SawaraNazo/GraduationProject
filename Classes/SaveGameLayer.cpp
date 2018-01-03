#include "SaveGameLayer.h"

SaveGameLayer::SaveGameLayer()
{

}

SaveGameLayer::~SaveGameLayer()
{

}

bool SaveGameLayer::init()
{
	if (!Layer::init())
	{
		return false;
	}

	visibleSize = Director::getInstance()->getVisibleSize();

	Sprite* s = Sprite::create("image/SaveGame.png");
	s->setPosition(visibleSize / 2);
	this->addChild(s, 10, "cover");

	Menu* m = Menu::create();

	// Y~es
	Sprite* okn = Sprite::create("image/GreenNormal.png");
	Sprite* okp = Sprite::create("image/GreenPressed.png");
	auto okmi = MenuItemSprite::create(okn, okp, CC_CALLBACK_1(SaveGameLayer::saveCallback, this));
	okmi->setPosition(-s->getContentSize().width / 2 + okmi->getContentSize().width * 3 / 4,
		-s->getContentSize().height / 4 - okmi->getContentSize().height);

	Label* okl = Label::createWithSystemFont("Yes", "Arial", 30);
	okl->setColor(Color3B::BLACK);
	okl->setPosition(okmi->getContentSize().width / 2, okmi->getContentSize().height / 2);
	okmi->addChild(okl);

	m->addChild(okmi);

	// N~o
	Sprite* cn = Sprite::create("image/RedNormal.png");
	Sprite* cp = Sprite::create("image/RedPressed.png");
	auto cmi = MenuItemSprite::create(cn, cp, CC_CALLBACK_1(SaveGameLayer::cancelCallback, this));
	cmi->setPosition(s->getContentSize().width / 2 - cmi->getContentSize().width * 3 / 4,
		-s->getContentSize().height / 4 - cmi->getContentSize().height);

	Label* cl = Label::createWithSystemFont("No", "Arial", 30);
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

void SaveGameLayer::saveCallback(Ref* pSender)
{
	bool exitFlag = UserDefault::getInstance()->isXMLFileExist();

	if (exitFlag)
	{
		auto sgcl = SaveGameCoverLayer::create();
		(this->getParent())->addChild(sgcl);

		this->removeFromParentAndCleanup(true);
	}
	else
	{
		auto gm = (GameMainLayer*)(this->getParent());

		UserDefault::getInstance()->setIntegerForKey("playersNumber", gm->getPlayersNumber());
		UserDefault::getInstance()->setIntegerForKey("startMoney", gm->getStartMoney());
		UserDefault::getInstance()->setIntegerForKey("mapNumber", gm->getMapNumber());
		UserDefault::getInstance()->setIntegerForKey("emEventNumber", gm->getEmEventNumber());
		UserDefault::getInstance()->setIntegerForKey("rounds", gm->getRounds());
		UserDefault::getInstance()->setIntegerForKey("nowPlayerNumber", gm->getNowPlayerNumber());

		vector<int> vr = gm->getRoles();

		for (size_t n = 0; n < vr.size(); ++n)
		{
			ostringstream stream1;
			stream1<<n;
			string s = "role" + stream1.str();
			UserDefault::getInstance()->setIntegerForKey(s.c_str(), vr[n]);
		}

		vector<Player> vp = gm->getPlayers();

		for (size_t n = 0; n < vp.size(); ++n)
		{
			ostringstream stream2;
			stream2<<n;
			string s1 = "player" + stream2.str() + "name";
			UserDefault::getInstance()->setStringForKey(s1.c_str(), vp[n].name);
			// roleSprite
			string s14 = "player" + stream2.str() + "serialNumber";
			UserDefault::getInstance()->setIntegerForKey(s14.c_str(), vp[n].serialNumber);
			string s2 = "player" + stream2.str() + "spritePositionX";
			UserDefault::getInstance()->setFloatForKey(s2.c_str(), vp[n].spritePosition.x);
			string s3 = "player" + stream2.str() + "spritePositionY";
			UserDefault::getInstance()->setFloatForKey(s3.c_str(), vp[n].spritePosition.y);
			string s4 = "player" + stream2.str()+ "rolePositionX";
			UserDefault::getInstance()->setFloatForKey(s4.c_str(), vp[n].rolePosition.x);
			string s5 = "player" + stream2.str() + "rolePositionY";
			UserDefault::getInstance()->setFloatForKey(s5.c_str(), vp[n].rolePosition.y);
			string s6 = "player" + stream2.str() + "faceTo";
			UserDefault::getInstance()->setIntegerForKey(s6.c_str(), vp[n].faceTo);
			string s7 = "player" + stream2.str() + "isGoing";
			UserDefault::getInstance()->setBoolForKey(s7.c_str(), vp[n].isGoing);
			string s8 = "player" + stream2.str() + "colorR";
			UserDefault::getInstance()->setIntegerForKey(s8.c_str(), vp[n].color.r);
			string s9 = "player" + stream2.str() + "colorG";
			UserDefault::getInstance()->setIntegerForKey(s9.c_str(), vp[n].color.g);
			string s10 = "player" + stream2.str() + "colorB";
			UserDefault::getInstance()->setIntegerForKey(s10.c_str(), vp[n].color.b);
			string s11 = "player" + stream2.str() + "money";
			UserDefault::getInstance()->setIntegerForKey(s11.c_str(), vp[n].money);
			string s12 = "player" + stream2.str() + "state";
			UserDefault::getInstance()->setIntegerForKey(s12.c_str(), vp[n].state);
			string s13 = "player" + stream2.str() + "stayRound";
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
		auto okmi = MenuItemSprite::create(okn, okp, CC_CALLBACK_1(SaveGameLayer::cancelCallback, this));
		okmi->setPosition(0, -s->getContentSize().height / 4 - okmi->getContentSize().height);

		Label* okl = Label::createWithSystemFont("Yes", "Arial", 30);
		okl->setColor(Color3B::BLACK);
		okl->setPosition(okmi->getContentSize().width / 2, okmi->getContentSize().height / 2);
		okmi->addChild(okl);

		m->addChild(okmi);
		this->addChild(m);
	}
}

void SaveGameLayer::cancelCallback(Ref* pSender)
{
	this->removeFromParentAndCleanup(true);
}