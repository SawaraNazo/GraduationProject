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
	Sprite* b = Sprite::create("image/background.png", Rect(origin.x, origin.y, visibleSize.width * 2, visibleSize.height * 2));
	this->addChild(b);
}