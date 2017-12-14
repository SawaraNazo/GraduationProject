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

	this->createMenu();

	// 吞
	auto callback = [](Touch*, Event*) {return true; };
	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = callback;
	listener->setSwallowTouches(true);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

	return true;
}

void TitleGameSetLayer::createMenu()
{
	Sprite* s = Sprite::create("image/Popup.png");
	s->setPosition(visibleSize.width / 2, visibleSize.height / 2);
	this->addChild(s);

	auto d = Dictionary::createWithContentsOfFile("XML/GameSet.xml");

	// 标题
	auto title = ((String*)d->objectForKey("title"))->getCString();
	Label* l1 = Label::createWithSystemFont(title, "arial", 40);
	l1->setTextColor(Color4B::BLACK);
	l1->setPosition(Vec2(s->getContentSize().width / 2, s->getContentSize().height * 3 / 4));
	s->addChild(l1);

	// 音乐开关
	auto onOff = ((String*)d->objectForKey("onOff"))->getCString();
	Label* l2 = Label::createWithSystemFont(onOff, "arial", 23);
	l2->setTextColor(Color4B::BLACK);
	l2->setPosition(Vec2(s->getContentSize().width / 4, s->getContentSize().height / 2 + l2->getContentSize().height));
	s->addChild(l2);

	Sprite* maskSprite = Sprite::create("image/switch-mask.png");
	Sprite* onSprite = Sprite::create("image/switch-on.png");
	Sprite* offSprite = Sprite::create("image/switch-off.png");
	Sprite* thumbSpriteW = Sprite::create("image/switch-thumb.png");
	Label* onLabel = Label::createWithSystemFont("ON", "arial", 16);
	Label* offLabel = Label::createWithSystemFont("OFF", "arial", 16);
	ControlSwitch* csw = ControlSwitch::create(maskSprite, onSprite, offSprite, thumbSpriteW,
		onLabel, offLabel);
	csw->addTargetWithActionForControlEvents(this, cccontrol_selector(TitleGameSetLayer::switchStateChanged), Control::EventType::VALUE_CHANGED);
	csw->setScale(1.2);
	csw->setPosition(s->getContentSize().width / 4, l2->getPosition().y - l2->getContentSize().height * 2);
	s->addChild(csw);

	// 音量调节
	auto volume = ((String*)d->objectForKey("volume"))->getCString();
	Label* l3 = Label::createWithSystemFont(volume, "arial", 23);
	l3->setTextColor(Color4B::BLACK);
	l3->setPosition(Vec2(s->getContentSize().width / 4, s->getContentSize().height * 3 / 16 + l3->getContentSize().height));
	s->addChild(l3);

	Sprite* backgroundSprite = Sprite::create("image/sliderTracker.png");
	Sprite* pogressSprite = Sprite::create("image/sliderProgress.png");
	Sprite* thumbSpriteL = Sprite::create("image/sliderThumb.png");
	ControlSlider* csl = ControlSlider::create(backgroundSprite, pogressSprite, thumbSpriteL);
	csl->setMinimumValue(0);
	csl->setMaximumValue(100);
	csl->setValue(100);
	csl->addTargetWithActionForControlEvents(this, cccontrol_selector(TitleGameSetLayer::sliderStateChanged), Control::EventType::VALUE_CHANGED);
	csl->setPosition(s->getContentSize().width / 4, l3->getPosition().y + l3->getContentSize().height * 2);
	s->addChild(csl);

	// 音乐选择
	auto chooseMusic = ((String*)d->objectForKey("chooseMusic"))->getCString();
	Label* l4 = Label::createWithSystemFont(chooseMusic, "arial", 23);
	l4->setTextColor(Color4B::BLACK);
	l4->setPosition(Vec2(s->getContentSize().width * 3 / 4, s->getContentSize().height / 2));
	s->addChild(l4);

	// 关闭
	MenuItem* closeButton = MenuItemImage::create("image/OrangeNormal.png",
		"image/OrangePressed.png",
		CC_CALLBACK_1(TitleGameSetLayer::closeCallback, this));
	auto close = ((String*)d->objectForKey("close"))->getCString();
	Label* l5 = Label::createWithSystemFont(close, "arial", 20);
	l5->setTextColor(Color4B::BLACK);
	l5->setPosition(Vec2(closeButton->getContentSize().width / 2, closeButton->getContentSize().height / 2));
	closeButton->addChild(l5);
	closeButton->setScale(1.2);
	closeButton->setPosition(0, -(s->getContentSize().height / 2) + closeButton->getContentSize().height * 3);

	Menu* m = Menu::create(closeButton, NULL);
	this->addChild(m);
}

void TitleGameSetLayer::closeCallback(Ref* pSender)
{
	this->removeFromParentAndCleanup(true);
}

void TitleGameSetLayer::switchStateChanged(Ref* pSender, Control::EventType controlEvent)
{
	ControlSwitch* controlSwitch = (ControlSwitch*)pSender;
	auto sm = SimpleAudioEngine::getInstance();
	if (controlSwitch->isOn())
	{
		SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
	}
	else
	{
		SimpleAudioEngine::getInstance()->stopBackgroundMusic(false);
	}
}

void TitleGameSetLayer::sliderStateChanged(Ref* pSender, Control::EventType controlEvent)
{
	ControlSlider* controlSlider = (ControlSlider*)pSender;

	float volume = controlSlider->getValue();
	volume /= 100;
	SimpleAudioEngine::getInstance()->setBackgroundMusicVolume(volume);
}