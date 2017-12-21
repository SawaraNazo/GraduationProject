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

	Label* okl = Label::create("Yes", "arial", 20);
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

	Label* cl = Label::create("No", "arial", 20);
	cl->setColor(Color3B::BLACK);
	cl->setPosition(cmi->getContentSize().width / 2, cmi->getContentSize().height / 2);
	cmi->addChild(cl);

	m->addChild(cmi);

	this->addChild(m, 10, "menu");

	// 吞事件
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
		// 不会走到这里来
	}
}

void SaveGameLayer::cancelCallback(Ref* pSender)
{
	this->removeFromParentAndCleanup(true);
}