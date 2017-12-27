#include "TitleAniLayer.h"

TitleAniLayer::TitleAniLayer()
{
}

TitleAniLayer::~TitleAniLayer()
{
}

bool TitleAniLayer::init()
{
	if (!Layer::init())
	{
		return false;
	}

	visibleSize = Director::getInstance()->getVisibleSize();
	origin = Director::getInstance()->getVisibleOrigin();

	printf("time flies very fast\n");

	this->createLabels();

	this->scheduleOnce(schedule_selector(TitleAniLayer::update), 1);

	return true;
}

void TitleAniLayer::createLabels()
{
	Label* labelT1, *labelT2, *labelT3, *labelT4;

	auto titleAni = __Dictionary::createWithContentsOfFile("XML/TitleAni.xml");

	const char* laT1 = ((cocos2d::__String*)titleAni->objectForKey("labelT1"))->getCString();
	labelT1 = Label::createWithSystemFont(laT1, "arial", 40);
	labelT1->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height * 2 / 3));
	this->addChild(labelT1);

	const char* laT2 = ((cocos2d::__String*)titleAni->objectForKey("labelT2"))->getCString();
	labelT2 = Label::createWithSystemFont(laT2, "arial", 30);
	labelT2->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2));
	this->addChild(labelT2);

	const char* laT3 = ((cocos2d::__String*)titleAni->objectForKey("labelT3"))->getCString();
	labelT3 = Label::createWithSystemFont(laT3, "arial", 25);
	labelT3->setPosition(Vec2(origin.x + visibleSize.width * 4 / 5, origin.y + visibleSize.height / 5));
	this->addChild(labelT3);

	const char* laT4 = ((cocos2d::__String*)titleAni->objectForKey("labelT4"))->getCString();
	labelT4 = Label::createWithSystemFont(laT4, "arial", 20);
	labelT4->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + labelT4->getContentSize().height));
	this->addChild(labelT4);
}

void TitleAniLayer::goNextScene()
{
	auto title = TitleScene::create();
	TransitionScene* transition = TransitionFade::create(3, title);

	Director::getInstance()->replaceScene(transition);
}

void TitleAniLayer::update(float dt)
{
	this->goNextScene();
}