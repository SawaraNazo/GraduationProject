#include "NoUsableSaveLayer.h"

NoUsableSaveLayer::NoUsableSaveLayer()
{

}

NoUsableSaveLayer::~NoUsableSaveLayer()
{

}

bool NoUsableSaveLayer::init()
{
	if (!Layer::init())
	{
		return false;
	}

	visibleSize = Director::getInstance()->getVisibleSize();

	Sprite* s = Sprite::create("image/NoUsableSave.png");
	s->setPosition(visibleSize / 2);
	this->addChild(s);

	Menu* m = Menu::create();

	auto s1 = Sprite::create("image/OrangeNormal.png");
	auto s2 = Sprite::create("image/OrangePressed.png");
	MenuItem* mi = MenuItemSprite::create(s1, s2, CC_CALLBACK_1(NoUsableSaveLayer::closeCallback, this));
	mi->setPosition(0, -s->getContentSize().height / 4 - mi->getContentSize().height / 2);

	Label* l = Label::create("OK", "arial", 20);
	l->setColor(Color3B::BLACK);
	l->setPosition(mi->getContentSize().width / 2, mi->getContentSize().height / 2);
	mi->addChild(l);
	m->addChild(mi);

	this->addChild(m);

	// ÍÌÊÉÊÂ¼þ
	auto callback = [](Touch*, Event*) {return true; };
	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = callback;
	listener->setSwallowTouches(true);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

	return true;
}

void NoUsableSaveLayer::closeCallback(Ref* pSender)
{
	this->removeFromParentAndCleanup(true);
}