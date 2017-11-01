#include "TitleScene.h"

TitleScene::TitleScene()
{
}

TitleScene::~TitleScene()
{
}

bool TitleScene::init()
{
	if (!Scene::init())
	{
		return false;
	}

	auto layerB = TitleBackgroundLayer::create();
	this->addChild(layerB, 0);

	auto layerM = TitleLayer::create();
	this->addChild(layerM, 1);

	SimpleAudioEngine::getInstance()->preloadBackgroundMusic("music/rich01.mp3");

	return true;
}