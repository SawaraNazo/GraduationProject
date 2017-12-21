#include "ContinueGameScene.h"

ContinueGameScene::ContinueGameScene()
{

}

ContinueGameScene::~ContinueGameScene()
{

}

bool ContinueGameScene::init()
{
	if (!Scene::init())
	{
		return false;
	}

	auto cml = ContinueMainLayer::create();
	this->addChild(cml);

	return true;
};