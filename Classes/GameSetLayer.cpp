#include "GameSetLayer.h"
#include <sstream>

GameSetLayer::GameSetLayer()
{

}

GameSetLayer::~GameSetLayer()
{

}

bool GameSetLayer::init()
{
	if (!Layer::init())
	{
		return false;
	}

	visibleSize = Director::getInstance()->getVisibleSize();
	origin = Director::getInstance()->getVisibleOrigin();

	this->createMenu();

	// 吞
	auto callback = [](Touch*, Event*) {return true; };
	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = callback;
	listener->setSwallowTouches(true);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

	return true;
}

void GameSetLayer::createMenu()
{
	Sprite* s = Sprite::create("image/Popup.png");
	s->setPosition(visibleSize.width / 2, visibleSize.height / 2);
	this->addChild(s);

	auto d = __Dictionary::createWithContentsOfFile("XML/GameSet.xml");

	// 标题
	auto title = ((__String*)d->objectForKey("title"))->getCString();
	Label* l1 = Label::createWithSystemFont(title, "Arial", 40);
	l1->setTextColor(Color4B::BLACK);
	l1->setPosition(Vec2(s->getContentSize().width / 2, s->getContentSize().height * 3 / 4));
	s->addChild(l1);

	// 音乐开关
	auto onOff = ((__String*)d->objectForKey("onOff"))->getCString();
	Label* l2 = Label::createWithSystemFont(onOff, "Arial", 23);
	l2->setTextColor(Color4B::BLACK);
	l2->setPosition(Vec2(s->getContentSize().width / 4, s->getContentSize().height / 2 + l2->getContentSize().height));
	s->addChild(l2);

	Sprite* maskSprite = Sprite::create("image/switch-mask.png");
	Sprite* onSprite = Sprite::create("image/switch-on.png");
	Sprite* offSprite = Sprite::create("image/switch-off.png");
	Sprite* thumbSpriteW = Sprite::create("image/switch-thumb.png");
	Label* onLabel = Label::createWithSystemFont("ON", "Arial", 16);
	Label* offLabel = Label::createWithSystemFont("OFF", "Arial", 16);
	ControlSwitch* csw = ControlSwitch::create(maskSprite, onSprite, offSprite, thumbSpriteW, onLabel, offLabel);
	csw->addTargetWithActionForControlEvents(this, cccontrol_selector(GameSetLayer::switchStateChanged), Control::EventType::VALUE_CHANGED);
	csw->setScale(1.2f);
	csw->setPosition(s->getContentSize().width / 4, l2->getPosition().y - l2->getContentSize().height * 2);
	s->addChild(csw);

	// 音量调节
	auto volume = ((__String*)d->objectForKey("volume"))->getCString();
	Label* l3 = Label::createWithSystemFont(volume, "Arial", 23);
	l3->setTextColor(Color4B::BLACK);
	l3->setPosition(Vec2(s->getContentSize().width * 3 / 4, s->getContentSize().height / 2 + l3->getContentSize().height));
	s->addChild(l3);

	Sprite* backgroundSprite = Sprite::create("image/sliderTracker.png");
	Sprite* pogressSprite = Sprite::create("image/sliderProgress.png");
	Sprite* thumbSpriteL = Sprite::create("image/sliderThumb.png");
	ControlSlider* csl = ControlSlider::create(backgroundSprite, pogressSprite, thumbSpriteL);
	csl->setMinimumValue(0);
	csl->setMaximumValue(100);
	csl->setValue(100);
	csl->addTargetWithActionForControlEvents(this, cccontrol_selector(GameSetLayer::sliderStateChanged), Control::EventType::VALUE_CHANGED);
	csl->setPosition(s->getContentSize().width * 3 / 4, l3->getPosition().y - l3->getContentSize().height * 2);
	s->addChild(csl);

	// BGM选择
	auto chooseMusic = ((__String*)d->objectForKey("chooseMusic"))->getCString();
	Label* l4 = Label::createWithSystemFont(chooseMusic, "Arial", 23);
	l4->setTextColor(Color4B::BLACK);
	l4->setPosition(s->getContentSize().width / 4, s->getContentSize().height / 4 + l4->getContentSize().height);
	s->addChild(l4);

	Label* bgm = Label::createWithSystemFont("BGM01", "Arial", 21);
	bgm->setTextColor(Color4B::BLACK);
	bgm->setPosition(s->getContentSize().width * 3 / 4, l4->getPosition().y);
	s->addChild(bgm);

	Sprite* bnl = Sprite::create("image/button-normal-left.png");
	Sprite* bpl = Sprite::create("image/button-pressed-left.png");
	MenuItem* button_left = MenuItemSprite::create(bnl, bpl, CC_CALLBACK_1(GameSetLayer::changeBGMPre, this, bgm, csw));
	button_left->setPosition(s->getContentSize().width / 4 - bgm->getContentSize().width / 2 - button_left->getContentSize().width, -s->getContentSize().height / 4 + l4->getContentSize().height);

	Sprite* bnr = Sprite::create("image/button-normal-right.png");
	Sprite* bpr = Sprite::create("image/button-pressed-right.png");
	MenuItem* button_right = MenuItemSprite::create(bnr, bpr, CC_CALLBACK_1(GameSetLayer::changeBGMNext, this, bgm, csw));
	button_right->setPosition(s->getContentSize().width / 4 + bgm->getContentSize().width / 2 + button_left->getContentSize().width, -s->getContentSize().height / 4 + l4->getContentSize().height);


	// 关闭
	MenuItem* closeButton = MenuItemImage::create("image/OrangeNormal.png",
		"image/OrangePressed.png",
		CC_CALLBACK_1(GameSetLayer::closeCallback, this));
	auto close = ((__String*)d->objectForKey("close"))->getCString();
	Label* l5 = Label::createWithSystemFont(close, "Arial", 20);
	l5->setTextColor(Color4B::BLACK);
	l5->setPosition(Vec2(closeButton->getContentSize().width / 2, closeButton->getContentSize().height / 2));
	closeButton->addChild(l5);
	closeButton->setScale(1.2f);
	closeButton->setPosition(0, -s->getContentSize().height / 4 - closeButton->getContentSize().height * 1.5);

	Menu* m = Menu::create(closeButton, button_left, button_right, NULL);
	this->addChild(m);
}

void GameSetLayer::closeCallback(Ref* pSender)
{
	this->removeFromParentAndCleanup(true);
}

void GameSetLayer::switchStateChanged(Ref* pSender, Control::EventType controlEvent)
{
	ControlSwitch* controlSwitch = (ControlSwitch*)pSender;
	auto sm = SimpleAudioEngine::getInstance();
	if (controlSwitch->isOn())
	{
        string sp;
        if (nowMusic < 10)
        {
            ostringstream s1;
            s1 << nowMusic;
            sp = "0" + s1.str();
        }
        else
        {
            ostringstream s1;
            s1 << nowMusic;
            sp = s1.str();
        }

        string m = "music/rich" + sp + ".mp3";
        SimpleAudioEngine::getInstance()->playBackgroundMusic(m.c_str(), true);
		//SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
	}
	else
	{
		SimpleAudioEngine::getInstance()->stopBackgroundMusic(true);
	}
}

void GameSetLayer::sliderStateChanged(Ref* pSender, Control::EventType controlEvent)
{
	ControlSlider* controlSlider = (ControlSlider*)pSender;

	float volume = controlSlider->getValue();
	volume /= 100;
	SimpleAudioEngine::getInstance()->setBackgroundMusicVolume(volume);
}

void GameSetLayer::changeBGMPre(Ref* pSender, Label* &bgm, ControlSwitch* cs)
{
	string s = bgm->getString();
	int nowNum = atoi(s.substr(3, 2).c_str());
	--nowNum;
	if (nowNum == 0)
	{
		nowNum = 14;
	}
    nowMusic = nowNum;

	string sp;
	if (nowNum < 10)
	{
		ostringstream s1;
		s1 << nowNum;
		sp = "0" + s1.str();
	}
	else
	{
		ostringstream s1;
		s1 << nowNum;
		sp = s1.str();
	}

	bgm->setString(s.substr(0, 3) + sp);
	SimpleAudioEngine::getInstance()->stopBackgroundMusic(true);
	string m = "music/rich" + sp + ".mp3";
	SimpleAudioEngine::getInstance()->playBackgroundMusic(m.c_str(), true);
	if (!cs->isOn())
	{
		SimpleAudioEngine::getInstance()->stopBackgroundMusic(false);
	}
}

void GameSetLayer::changeBGMNext(Ref* pSender, Label* &bgm, ControlSwitch* cs)
{
	string s = bgm->getString();
	int nowNum = atoi(s.substr(3, 2).c_str());
	++nowNum;
	if (nowNum == 15)
	{
		nowNum = 1;
	}
    nowMusic = nowNum;

	string sp;
	if (nowNum < 10)
	{
		ostringstream s1;
		s1 << nowNum;
		sp = "0" + s1.str();
	}
	else
	{
		ostringstream s1;
		s1 << nowNum;
		sp = s1.str();
	}

	bgm->setString(s.substr(0, 3) + sp);
	SimpleAudioEngine::getInstance()->stopBackgroundMusic(true);
	string m = "music/rich" + sp + ".mp3";
	SimpleAudioEngine::getInstance()->playBackgroundMusic(m.c_str(), true);
	if (!cs->isOn())
	{
		SimpleAudioEngine::getInstance()->stopBackgroundMusic(false);
	}
}