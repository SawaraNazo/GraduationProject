#include "SelectRoleDuplicateLayer.h"

SelectRoleDuplicateLayer::SelectRoleDuplicateLayer()
{

}

SelectRoleDuplicateLayer::~SelectRoleDuplicateLayer()
{

}

bool SelectRoleDuplicateLayer::init()
{
	if (!Layer::init())
	{
		return false;
	}

	visibleSize = Director::getInstance()->getVisibleSize();

	Sprite* s = Sprite::create("image/PlayersRolesDuplicate.png");
	s->setPosition(visibleSize.width / 2, visibleSize.height / 2);
	this->addChild(s);

	Sprite* okn = Sprite::create("image/GreenNormal.png");
	Sprite* okp = Sprite::create("image/GreenPressed.png");
	MenuItem* mi = MenuItemSprite::create(okn, okp, CC_CALLBACK_1(SelectRoleDuplicateLayer::okCallback, this));
	mi->setPosition(0, -s->getContentSize().height / 2 + mi->getContentSize().height);

	Label* l = Label::createWithSystemFont("OK", "Arial", 30);
	l->setPosition(mi->getContentSize().width / 2, mi->getContentSize().height / 2);
	l->setTextColor(Color4B::BLACK);
	mi->addChild(l);

	Menu* m = Menu::create(mi, NULL);
	this->addChild(m);

	// ���¼�
	auto callback = [](Touch*, Event*) {return true; };
	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = callback;
	listener->setSwallowTouches(true);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

	return true;
}

void SelectRoleDuplicateLayer::okCallback(Ref* pSender)
{
	this->removeFromParentAndCleanup(true);
}