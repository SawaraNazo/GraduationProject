#include "TitleLayer.h"

TitleLayer::TitleLayer()
{
}

TitleLayer::~TitleLayer()
{
}

bool TitleLayer::init()
{
	if (!Layer::init())
	{
		return false;
	}
	
	visibleSize = Director::getInstance()->getVisibleSize();
	origin = Director::getInstance()->getVisibleOrigin();
	
	this->createLogo();
	this->createMenu();

	SimpleAudioEngine::getInstance()->playBackgroundMusic("music/rich01.mp3", true);

	return true;
}

void TitleLayer::createLogo()
{
	Sprite* logo = Sprite::create("image/logo.png");
	logo->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height * 4 / 5));
	this->addChild(logo);
}

void TitleLayer::createMenu()
{
	auto title = Dictionary::createWithContentsOfFile("XML/Title.XML");

	MenuItem* newGame = MenuItemImage::create("image/OrangeNormal.png",
		"image/OrangePressed.png",
		CC_CALLBACK_1(TitleLayer::newGameCallback, this));
	newGame->setPosition(Vec2(-visibleSize.width / 4, 0));
	const char* bgT1 = ((String*)title->objectForKey("backgroundT1"))->getCString();
	Label* newGameLabel = Label::createWithSystemFont(bgT1, "arial", 18);
	newGameLabel->setPosition(newGame->getContentSize() / 2);
	newGame->addChild(newGameLabel);


	MenuItem* loadGame = MenuItemImage::create("image/OrangeNormal.png",
		"image/OrangePressed.png",
		CC_CALLBACK_1(TitleLayer::loadGameCallback, this));
	loadGame->setPosition(Vec2(-visibleSize.width / 5, -visibleSize.height / 4));
	const char* bgT2 = ((String*)title->objectForKey("backgroundT2"))->getCString();
	Label* loadGameLabel = Label::createWithSystemFont(bgT2, "arial", 18);
	loadGameLabel->setPosition(newGame->getContentSize() / 2);
	loadGame->addChild(loadGameLabel);

	MenuItem* setting = MenuItemImage::create("image/OrangeNormal.png",
		"image/OrangePressed.png",
		CC_CALLBACK_1(TitleLayer::settingCallback, this));
	setting->setPosition(Vec2(visibleSize.width / 4, 0));
	const char* bgT3 = ((String*)title->objectForKey("backgroundT3"))->getCString();
	Label* settingLabel = Label::createWithSystemFont(bgT3, "arial", 18);
	settingLabel->setPosition(newGame->getContentSize() / 2);
	setting->addChild(settingLabel);

	MenuItem* exit = MenuItemImage::create("image/OrangeNormal.png",
		"image/OrangePressed.png",
		CC_CALLBACK_1(TitleLayer::exitCallback, this));
	exit->setPosition(Vec2(visibleSize.width / 5, -visibleSize.height / 4));
	const char* bgT4 = ((String*)title->objectForKey("backgroundT4"))->getCString();
	Label* exitLabel = Label::createWithSystemFont(bgT4, "arial", 18);
	exitLabel->setPosition(newGame->getContentSize() / 2);
	exit->addChild(exitLabel);

	Menu* menu = Menu::create(newGame, loadGame, setting, exit, NULL);
	this->addChild(menu);
}

void TitleLayer::newGameCallback(Ref* pSender)
{
	SimpleAudioEngine::getInstance()->stopBackgroundMusic(false);
	NewGameScene* ngs = NewGameScene::create();
	Director::getInstance()->pushScene(ngs);
}

void TitleLayer::loadGameCallback(Ref* pSender)
{

}

void TitleLayer::settingCallback(Ref* pSender)
{
	auto sp = TitleGameSetLayer::create();
	this->addChild(sp, 100);
}

void TitleLayer::exitCallback(Ref* pSender)
{
	auto ep = ExitPopupLayer::create();
	this->addChild(ep, 100);
}