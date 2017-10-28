#include "TitleAniScene.h"

TitleAniScene::TitleAniScene()
{
}

TitleAniScene::~TitleAniScene()
{
}

bool TitleAniScene::init()
{
	if (!Scene::init())
	{
		return false;
	}

	// ¸ÃSceneµÄÎ¨Ò»Layer
	auto layer = TitleAniLayer::create();
	this->addChild(layer);

	return true;
}