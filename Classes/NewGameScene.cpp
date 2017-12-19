#include "NewGameScene.h"

NewGameScene::NewGameScene()
{
}

NewGameScene::~NewGameScene()
{
}

bool NewGameScene::init()
{
	if (!Scene::init())
	{
		return false;
	}

	auto gameInitLayer = GameInitLayer::create();
	this->addChild(gameInitLayer, 2, "gameinit");

	return true;
}