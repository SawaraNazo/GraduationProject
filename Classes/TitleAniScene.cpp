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

	// ��Scene��ΨһLayer
	auto layer = TitleAniLayer::create();
	this->addChild(layer);

	return true;
}