#include "TitleGameSetLayer.h"

TitleGameSetLayer::TitleGameSetLayer()
{

}

TitleGameSetLayer::~TitleGameSetLayer()
{

}

bool TitleGameSetLayer::init()
{
	if (!Layer::init())
	{
		return false;
	}

	visibleSize = Director::getInstance()->getVisibleSize();
	origin = Director::getInstance()->getVisibleOrigin();


}