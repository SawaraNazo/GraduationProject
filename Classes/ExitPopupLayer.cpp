#include "ExitPopupLayer.h"

ExitPopupLayer::ExitPopupLayer()
{
}

ExitPopupLayer::~ExitPopupLayer()
{
}

bool ExitPopupLayer::init()
{
	if (!Layer::init())
	{
		return false;
	}

	visibleSize = Director::getInstance()->getVisibleSize();
	origin = Director::getInstance()->getVisibleOrigin();

	this->createMenu();

	// ��
	auto callback = [](Touch*, Event*) {return true; };
	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = callback;
	listener->setSwallowTouches(true);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
	
	return true;
}

void ExitPopupLayer::createMenu()
{
	Sprite* s = Sprite::create("image/Popup.png");
	s->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
	this->addChild(s);

	auto d = __Dictionary::createWithContentsOfFile("XML/Title.xml");

	// ��ʾ��
	auto epT1 = ((__String*)d->objectForKey("exitPopupT1"))->getCString();
	Label* T1 = Label::createWithSystemFont(epT1, "arial", 48);
	T1->enableBold();
	T1->setTextColor(Color4B::BLACK);
	T1->setPosition(Vec2(s->getContentSize().width / 2, s->getContentSize().height * 3 / 4));
	s->addChild(T1);

	// OK ��
	MenuItem* ok = MenuItemImage::create("image/RedNormal.png",
		"image/RedPressed.png",
		CC_CALLBACK_1(ExitPopupLayer::okCallBack, this));
	auto epT2 = ((__String*)d->objectForKey("exitPopupT2"))->getCString();
	Label* T2 = Label::createWithSystemFont(epT2, "arial", 30);
	T2->setPosition(Vec2(ok->getContentSize().width / 2, ok->getContentSize().height / 2));
	ok->addChild(T2);
	ok->setScale(1.2);
	ok->setPosition(Vec2(-s->getContentSize().width / 4, -s->getContentSize().height / 4));

	// Cancel ��
	MenuItem* cancel = MenuItemImage::create("image/GreenNormal.png",
		"image/GreenPressed.png",
		CC_CALLBACK_1(ExitPopupLayer::cancelCallBack, this));
	auto epT3 = ((__String*)d->objectForKey("exitPopupT3"))->getCString();
	Label* T3 = Label::createWithSystemFont(epT3, "arial", 30);
	T3->setPosition(Vec2(cancel->getContentSize().width / 2, cancel->getContentSize().height / 2));
	cancel->addChild(T3);
	cancel->setScale(1.2);
	cancel->setPosition(Vec2(s->getContentSize().width / 4, -s->getContentSize().height / 4));

	Menu* m = Menu::create(ok, cancel, NULL);
	this->addChild(m);
}

void ExitPopupLayer::okCallBack(Ref* pSender)
{
	Director::getInstance()->end();
}

void ExitPopupLayer::cancelCallBack(Ref* pSender)
{
	this->removeFromParentAndCleanup(true);
}