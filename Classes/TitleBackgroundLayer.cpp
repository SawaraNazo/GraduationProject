#include "TitleBackgroundLayer.h"

TitleBackgroundLayer::TitleBackgroundLayer()
{
}

TitleBackgroundLayer::~TitleBackgroundLayer()
{
}

bool TitleBackgroundLayer::init()
{
	if (!Layer::create())
	{
		return false;
	}

	visibleSize = Director::getInstance()->getVisibleSize();
	origin = Director::getInstance()->getVisibleOrigin();

	this->createBackground();

	return true;
}

void TitleBackgroundLayer::createBackground()
{
	Sprite* b = Sprite::create("image/titleBG.png");
	b->setPosition(visibleSize / 2);
	this->addChild(b);
}