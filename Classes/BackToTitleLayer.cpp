#include "BackToTitleLayer.h"

BackToTitleLayer::BackToTitleLayer()
{

}

BackToTitleLayer::~BackToTitleLayer()
{

}

bool BackToTitleLayer::init()
{
	if (!Layer::init())
	{
		return false;
	}

	visibleSize = Director::getInstance()->getVisibleSize();

	Sprite* s = Sprite::create("image/backToTitle.png");
	s->setPosition(visibleSize / 2);
	this->addChild(s);

	Menu* m = Menu::create();
	
	// Y~es
	Sprite* okn = Sprite::create("image/GreenNormal.png");
	Sprite* okp = Sprite::create("image/GreenPressed.png");
	auto okmi = MenuItemSprite::create(okn, okp, CC_CALLBACK_1(BackToTitleLayer::okCallback, this));
	okmi->setPosition(-s->getContentSize().width / 2 + okmi->getContentSize().width * 3 / 4,
		-s->getContentSize().height / 2 + okmi->getContentSize().height);

	Label* okl = Label::createWithSystemFont("Yes", "Arial", 20);
	okl->setColor(Color3B::BLACK);
	okl->setPosition(okmi->getContentSize().width / 2, okmi->getContentSize().height / 2);
	okmi->addChild(okl);
	
	m->addChild(okmi);

	// N~o
	Sprite* cn = Sprite::create("image/RedNormal.png");
	Sprite* cp = Sprite::create("image/RedPressed.png");
	auto cmi = MenuItemSprite::create(cn, cp, CC_CALLBACK_1(BackToTitleLayer::cancelCallback, this));
	cmi->setPosition(s->getContentSize().width / 2 - cmi->getContentSize().width * 3 / 4,
		-s->getContentSize().height / 2 + cmi->getContentSize().height);

	Label* cl = Label::createWithSystemFont("No", "Arial", 20);
	cl->setColor(Color3B::BLACK);
	cl->setPosition(cmi->getContentSize().width / 2, cmi->getContentSize().height / 2);
	cmi->addChild(cl);

	m->addChild(cmi);
	
	this->addChild(m);

	// ÍÌÊÂ¼þ
	auto callback = [](Touch*, Event*) {return true; };
	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = callback;
	listener->setSwallowTouches(true);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
	
	return true;
}

void BackToTitleLayer::okCallback(Ref* pSender)
{
	Director::getInstance()->popScene();
}

void BackToTitleLayer::cancelCallback(Ref* pSender)
{
	this->removeFromParentAndCleanup(true);
}