#include "ContinueMainLayer.h"
#include <sstream>

ContinueMainLayer::ContinueMainLayer()
{

}

ContinueMainLayer::~ContinueMainLayer()
{

}

// set 函数集

//void ContinueMainLayer::setPlayersNumber(int pn)
//{
//	this->playersNumber = pn;
//}
//
//void ContinueMainLayer::setStartMoney(int sm)
//{
//	this->startMoney = sm;
//}
//
//void ContinueMainLayer::setPlayersRoles(vector<int> pr)
//{
//	for (int i : pr)
//	{
//		this->roles.push_back(i);
//	}
//}
//
//void ContinueMainLayer::setMapNumber(int mn)
//{
//	this->mapNumber = mn;
//}

// get 函数集

int ContinueMainLayer::getPlayersNumber()
{
	return this->playersNumber;
}

int ContinueMainLayer::getStartMoney()
{
	return this->startMoney;
}

int ContinueMainLayer::getMapNumber()
{
	return this->mapNumber;
}

int ContinueMainLayer::getEmEventNumber()
{
	return this->emEventNumber;
}

int ContinueMainLayer::getRounds()
{
	return this->rounds;
}

int ContinueMainLayer::getNowPlayerNumber()
{
	return this->nowPlayerNumber;
}

vector<int> ContinueMainLayer::getRoles()
{
	return this->roles;
}

vector<Player> ContinueMainLayer::getPlayers()
{
	return this->players;
}

// schedule 相关函数

void ContinueMainLayer::removeParticle(float dt)
{
	auto p = this->getChildByName("particle");
	p->removeFromParentAndCleanup(true);
}

void ContinueMainLayer::continueLoading(float dt)
{
	auto l = (Label*)(this->getChildByName("loading"));
	l->setString(l->getString() + " .");
}

void ContinueMainLayer::removeLoading(float dt)
{
	auto a = this->getChildByName("loading");
	a->removeFromParentAndCleanup(true);
}

// init

bool ContinueMainLayer::init()
{
	if (!Layer::init())
	{
		return false;
	}

	visibleSize = Director::getInstance()->getVisibleSize();
	origin = Director::getInstance()->getVisibleOrigin();

	// 初始化到第一位玩家
	//nowPlayerNumber = 1;
	nowPlayerNumber = UserDefault::getInstance()->getIntegerForKey("nowPlayerNumber");

	// 初始化突发事件数量
	//emEventNumber = 8;
	emEventNumber = UserDefault::getInstance()->getIntegerForKey("emEventNumber");

	// 预设置开始的土地等级数
	landLevelNumber = 8;

	// 回合数
	//rounds = 1;
	rounds = UserDefault::getInstance()->getIntegerForKey("rounds");

	// 删除了NewMain里的Set方法，直接取值来替代

	playersNumber = UserDefault::getInstance()->getIntegerForKey("playersNumber");
	startMoney = UserDefault::getInstance()->getIntegerForKey("startMoney");

	for (int n = 0; n < playersNumber; ++n)
	{
		ostringstream stream;
		stream<<n;
		string s = "role" + stream.str();
		roles.push_back(UserDefault::getInstance()->getIntegerForKey(s.c_str()));
	}

	//ここまでだ

	// Nowloading
	Label* loading = Label::createWithSystemFont("Now Loading", "Arial", 30);
	loading->enableBold();
	loading->setPosition(visibleSize / 2);
	this->addChild(loading, 5, "loading");

	this->schedule(schedule_selector(ContinueMainLayer::continueLoading), 0.5, 3, 0.5);

	this->scheduleOnce(schedule_selector(ContinueMainLayer::removeLoading), 2);
	// ここまでだ

	// 开始主函数
	this->scheduleOnce(schedule_selector(ContinueMainLayer::setParameter), 2);

	return true;
}

void ContinueMainLayer::setParameter(float dt)
{
	this->createMap();
	this->createPlayer();
	this->createPlayerPro();

	// 打底色 q(≧▽≦q)
	Color4B color4B;
	color4B.r = 139;
	color4B.g = 129;
	color4B.b = 76;
	color4B.a = 255;
	auto l = LayerColor::create(color4B);
	this->addChild(l, -10);
}

// 初始化相关函数

void ContinueMainLayer::createMap()
{
	//string str = "tmx/map" + to_string(mapNumber) + ".tmx";
	mapNumber = UserDefault::getInstance()->getIntegerForKey("mapNumber");
	ostringstream stream1;
	stream1 << mapNumber;
	string str = "tmx/map" + stream1.str() + ".tmx";
	tileMap = TMXTiledMap::create(str);
    tileMap->setScale((visibleSize.width*3/4)/tileMap->getContentSize().width,
                      visibleSize.height/tileMap->getContentSize().height);

	this->addChild(tileMap);

	road = tileMap->getLayer("Road");

	for (int i = 1; i <= landLevelNumber; i++)
	{
		ostringstream stream2;
		stream2 << i;
		string s = "Land" + stream2.str();
		TMXLayer* tl = tileMap->getLayer(s);
		lands.push_back(tl);
	}

	// 总回合数
	auto ngContent = __Dictionary::createWithContentsOfFile("XML/NewGame.xml");
	const char* roundsC = ((__String*)ngContent->objectForKey("rounds"))->getCString();
	Label* roundsL1 = Label::createWithSystemFont(roundsC, "Arial", 24);
	roundsL1->enableBold();
	roundsL1->setPosition(visibleSize.width * 3 / 8, visibleSize.height - roundsL1->getContentSize().height);
	this->addChild(roundsL1, 10);

	ostringstream stream3;
	stream3 << rounds;
	Label* roundsL2 = Label::createWithSystemFont(stream3.str(), "Arial", 18);
	roundsL2->enableBold();
	roundsL2->setPosition(visibleSize.width * 3 / 8, roundsL1->getPosition().y - roundsL2->getContentSize().height);
	this->addChild(roundsL2, 10, "roundsL");;
}

void ContinueMainLayer::createPlayer()
{
	objectGroup = tileMap->getObjectGroup("Object");
	auto player = objectGroup->getObject("player");
	float px = player["x"].asFloat();
	float py = player["y"].asFloat();

	for (int i = 1; i <= playersNumber; i++)
	{
		Player p;
		//p.name = "player" + to_string(i);
		ostringstream stream1;
		stream1 << i - 1;

		string s1 = "player" + stream1.str() + "name";
		p.name = UserDefault::getInstance()->getStringForKey(s1.c_str());
		//p.serialNumber = i;
		string s14 = "player" + stream1.str() + "serialNumber";
		p.serialNumber = UserDefault::getInstance()->getIntegerForKey(s14.c_str());
		//p.isGoing = false;
		string s7 = "player" + stream1.str() + "isGoing";
		p.isGoing = UserDefault::getInstance()->getBoolForKey(s7.c_str());
		//p.faceTo = faceForward::right;
		string s6 = "player" + stream1.str() + "faceTo";
		switch (UserDefault::getInstance()->getIntegerForKey(s6.c_str()))
		{
		case 0:
			p.faceTo = faceForward::right;
			break;
		case 1:
			p.faceTo = faceForward::down;
			break;
		case 2:
			p.faceTo = faceForward::left;
			break;
		case 3:
			p.faceTo = faceForward::up;
			break;
		}
		//p.rolePosition = Vec2(4, 4);
		string s4 = "player" + stream1.str() + "rolePositionX";
		p.rolePosition.x = UserDefault::getInstance()->getFloatForKey(s4.c_str());
		string s5 = "player" + stream1.str() + "rolePositionY";
		p.rolePosition.y = UserDefault::getInstance()->getFloatForKey(s5.c_str());
		//p.roleSprite = Sprite::create("image/role" + to_string(roles[p.serialNumber - 1]) + "_right.png");
		ostringstream stream2;
		stream2 << roles[p.serialNumber - 1];
		switch (p.faceTo)
		{
		case faceForward::right:
			p.roleSprite = Sprite::create("image/role" + stream2.str() + "_right.png");
			break;
		case faceForward::down:
			p.roleSprite = Sprite::create("image/role" + stream2.str() + "_down.png");
			break;
		case faceForward::left:
			p.roleSprite = Sprite::create("image/role" + stream2.str() + "_left.png");
			break;
		case faceForward::up:
			p.roleSprite = Sprite::create("image/role" + stream2.str() + "_up.png");
			break;
		}
		//p.money = startMoney;
		string s11 = "player" + stream1.str() + "money";
		p.money = UserDefault::getInstance()->getIntegerForKey(s11.c_str());
		//p.state = stateType::normal;
		string s12 = "player" + stream1.str() + "state";
		switch (UserDefault::getInstance()->getIntegerForKey(s12.c_str()))
		{
		case 0:
			p.state = stateType::normal;
			break;
		case 1:
			p.state = stateType::parking;
			break;
		case 2:
			p.state = stateType::prison;
			break;
		}
		//p.spritePosition = Vec2(px + i * 3, py);
		string s2 = "player" + stream1.str() + "spritePositionX";
		p.spritePosition.x = UserDefault::getInstance()->getFloatForKey(s2.c_str());
		string s3 = "player" + stream1.str() + "spritePositionY";
		p.spritePosition.y = UserDefault::getInstance()->getFloatForKey(s3.c_str());
		p.roleSprite->setPosition(p.spritePosition);
		//p.stayRound = 0;
		string s13 = "player" + stream1.str() + "stayRound";
		p.stayRound = UserDefault::getInstance()->getIntegerForKey(s13.c_str());

		// Color
		/*if (i == 1)
		{
		p.color = Color3B::RED;
		}
		else if (i == 2)
		{
		p.color = Color3B::BLUE;
		}
		else if (i == 3)
		{
		p.color = Color3B::BLACK;
		}
		else if (i == 4)
		{
		p.color = Color3B::MAGENTA;
		}*/

		string s8 = "player" + stream1.str() + "colorR";
		p.color.r = UserDefault::getInstance()->getIntegerForKey(s8.c_str());
		string s9 = "player" + stream1.str() + "colorG";
		p.color.g = UserDefault::getInstance()->getIntegerForKey(s9.c_str());
		string s10 = "player" + stream1.str() + "colorB";
		p.color.b = UserDefault::getInstance()->getIntegerForKey(s10.c_str());

		ostringstream stream3;
		stream3 << i;

		tileMap->addChild(p.roleSprite, tileMap->getChildrenCount(), "player" + stream3.str());

		players.push_back(p);
	}
}

void ContinueMainLayer::createPlayerPro()
{
	auto ng = __Dictionary::createWithContentsOfFile("XML/NewGame.xml");

	int times = 0;

	for (auto p : players)
	{
		// 玩家姓名
		const char* pc = ((__String*)ng->objectForKey(p.name))->getCString();
		Label* l1 = Label::createWithSystemFont(pc, "Arial", 20);
		l1->setColor(p.color);
		l1->enableBold();
		l1->setPosition(visibleSize.width * 6 / 7,
			visibleSize.height - visibleSize.height * 3 * times / 16 - l1->getContentSize().height / 2);
		this->addChild(l1, 2, pc);

		// 玩家金钱
		const char* rmb = ((__String*)ng->objectForKey("rmb"))->getCString();
		ostringstream stream1;
		stream1 << p.money;
		string m = rmb + stream1.str();
		string blank = " ";
		Label* l2 = Label::createWithSystemFont(m, "Arial", 18);
		l2->enableBold();
		l2->setPosition(visibleSize.width * 6 / 7, l1->getPosition().y - 2 * l1->getContentSize().height);
		this->addChild(l2, 2, p.name + blank + "money");

		times++;
	}

	// 骰子按钮
	Button* diceButton = Button::create("image/diceButtonNormal.png", "image/diceButtonPressed.png");
	diceButton->setPosition(Vec2(visibleSize.width - diceButton->getContentSize().width / 2,
		diceButton->getContentSize().height / 2));
	diceButton->addTouchEventListener(CC_CALLBACK_2(ContinueMainLayer::diceEvent, this));
	diceButton->setPressedActionEnabled(true);
	this->addChild(diceButton, 2);

	// 设置按钮
	Button* setButton = Button::create("image/settingButtonNormal.png", "image/settingButtonPressed.png");
	setButton->setPosition(Vec2(setButton->getContentSize().width,
		visibleSize.height - setButton->getContentSize().height));
	setButton->addTouchEventListener(CC_CALLBACK_2(ContinueMainLayer::setEvent, this));
	setButton->setPressedActionEnabled(true);
	this->addChild(setButton, 2);

	// 存储按钮
	Button* saveButton = Button::create("image/saveButtonNormal.png", "image/saveButtonPressed.png");
	saveButton->setPosition(Vec2(saveButton->getContentSize().width,
		visibleSize.height - saveButton->getContentSize().height * 2.5));
	saveButton->addTouchEventListener(CC_CALLBACK_2(ContinueMainLayer::saveEvent, this));
	saveButton->setPressedActionEnabled(true);
	this->addChild(saveButton, 2);

	// 关闭按钮
	Button* exitButton = Button::create("image/exitButtonNormal.png", "image/exitButtonPressed.png");
	exitButton->setPosition(Vec2(exitButton->getContentSize().width,
		visibleSize.height - exitButton->getContentSize().height * 4));
	exitButton->addTouchEventListener(CC_CALLBACK_2(ContinueMainLayer::exitEvent, this));
	exitButton->setPressedActionEnabled(true);
	this->addChild(exitButton, 2);

	/*
	// 问题按钮
	Button* questionButton = Button::create("image/settingButtonNormal.png", "image/settingButtonPressed.png");
	questionButton->setPosition(Vec2(questionButton->getContentSize().width,
	visibleSize.height - questionButton->getContentSize().height * 3));
	questionButton->addTouchEventListener(CC_CALLBACK_2(ContinueMainLayer::setEvent, this));
	questionButton->setPressedActionEnabled(true);
	this->addChild(questionButton, 2);*/
}

// 动作相关函数
void ContinueMainLayer::diceEvent(Ref* pSender, Widget::TouchEventType type)
{
	switch (type)
	{
	case Widget::TouchEventType::BEGAN:
		break;
	case Widget::TouchEventType::MOVED:
		break;
	case Widget::TouchEventType::ENDED:
	{
		int n = 1;

		for (auto& p : players)
		{
			if (n == nowPlayerNumber)
			{
				if (p.isGoing == false)
				{
					if (this->checkState())
					{
						int i = random(1, 6);

						switch (i)
						{
						case 1:
							dicePointS = Sprite::create("image/point1.png");
							break;
						case 2:
							dicePointS = Sprite::create("image/point2.png");
							break;
						case 3:
							dicePointS = Sprite::create("image/point3.png");
							break;
						case 4:
							dicePointS = Sprite::create("image/point4.png");
							break;
						case 5:
							dicePointS = Sprite::create("image/point5.png");
							break;
						case 6:
							dicePointS = Sprite::create("image/point6.png");
							break;
						}
						dicePointS->setPosition(visibleSize.width / 2, visibleSize.height / 2);
						this->addChild(dicePointS, 5, "dicePoint");

						p.isGoing = true;

						this->schedule(schedule_selector(ContinueMainLayer::playerGo), 0.5, i - 1, 0);
						this->scheduleOnce(schedule_selector(ContinueMainLayer::checkRoad), i*0.5);
					}
					else
					{
						// (☆-ｖ-)
					}
				}

				break;
			}

			n++;
		}

		break;
	}
	case Widget::TouchEventType::CANCELED:
		break;
	default:
		break;
	}
}

void ContinueMainLayer::setEvent(Ref* pSender, Widget::TouchEventType type)
{
	switch (type)
	{
	case Widget::TouchEventType::BEGAN:
		break;
	case Widget::TouchEventType::MOVED:
		break;
	case Widget::TouchEventType::ENDED:
	{
		auto gsl = GameSetLayer::create();
		this->addChild(gsl, 100);
	}
	case Widget::TouchEventType::CANCELED:
		break;
	default:
		break;
	}
}

void ContinueMainLayer::saveEvent(Ref* pSender, Widget::TouchEventType type)
{
	switch (type)
	{
	case Widget::TouchEventType::BEGAN:
		break;
	case Widget::TouchEventType::MOVED:
		break;
	case Widget::TouchEventType::ENDED:
	{
		auto sgl = SaveGameLayer::create();
		this->addChild(sgl, 100);
	}
	case Widget::TouchEventType::CANCELED:
		break;
	default:
		break;
	}
}

void ContinueMainLayer::exitEvent(Ref* pSender, Widget::TouchEventType type)
{
	switch (type)
	{
	case Widget::TouchEventType::BEGAN:
		break;
	case Widget::TouchEventType::MOVED:
		break;
	case Widget::TouchEventType::ENDED:
	{
		auto btt = BackToTitleLayer::create();
		this->addChild(btt, 100);
	}
	case Widget::TouchEventType::CANCELED:
		break;
	default:
		break;
	}
}

void ContinueMainLayer::playerGo(float dt)
{
	int n = 1;

	for (auto& nowPlayer : players)
	{
		if (n == nowPlayerNumber)
		{
			if (road->getTileAt(nowPlayer.rolePosition))
			{
				ostringstream stream1;
				stream1 << roles[nowPlayer.serialNumber - 1];

				if (nowPlayer.faceTo == faceForward::right)
				{
					if (road->getTileAt(Vec2(nowPlayer.rolePosition.x + 1, nowPlayer.rolePosition.y)))
					{
						nowPlayer.rolePosition.x++;
						nowPlayer.spritePosition.x += 30;
					}
					else if (road->getTileAt(Vec2(nowPlayer.rolePosition.x, nowPlayer.rolePosition.y + 1)))
					{
						nowPlayer.rolePosition.y++;
						nowPlayer.spritePosition.y -= 30;
						nowPlayer.faceTo = faceForward::down;
						nowPlayer.roleSprite->setTexture("image/role" + stream1.str() + "_down.png");
					}
					else if (road->getTileAt(Vec2(nowPlayer.rolePosition.x, nowPlayer.rolePosition.y - 1)))
					{
						nowPlayer.rolePosition.y--;
						nowPlayer.spritePosition.y += 30;
						nowPlayer.faceTo = faceForward::up;
						nowPlayer.roleSprite->setTexture("image/role" + stream1.str() + "_up.png");
					}
				}
				else if (nowPlayer.faceTo == faceForward::down)
				{
					if (road->getTileAt(Vec2(nowPlayer.rolePosition.x, nowPlayer.rolePosition.y + 1)))
					{
						nowPlayer.rolePosition.y++;
						nowPlayer.spritePosition.y -= 30;
					}
					else if (road->getTileAt(Vec2(nowPlayer.rolePosition.x + 1, nowPlayer.rolePosition.y)))
					{
						nowPlayer.rolePosition.x++;
						nowPlayer.spritePosition.x += 30;
						nowPlayer.faceTo = faceForward::right;
						nowPlayer.roleSprite->setTexture("image/role" + stream1.str() + "_right.png");
					}
					else if (road->getTileAt(Vec2(nowPlayer.rolePosition.x - 1, nowPlayer.rolePosition.y)))
					{
						nowPlayer.rolePosition.x--;
						nowPlayer.spritePosition.x -= 30;
						nowPlayer.faceTo = faceForward::left;
						nowPlayer.roleSprite->setTexture("image/role" + stream1.str() + "_left.png");
					}
				}
				else if (nowPlayer.faceTo == faceForward::left)
				{
					if (road->getTileAt(Vec2(nowPlayer.rolePosition.x - 1, nowPlayer.rolePosition.y)))
					{
						nowPlayer.rolePosition.x--;
						nowPlayer.spritePosition.x -= 30;
					}
					else if (road->getTileAt(Vec2(nowPlayer.rolePosition.x, nowPlayer.rolePosition.y + 1)))
					{
						nowPlayer.rolePosition.y++;
						nowPlayer.spritePosition.y -= 30;
						nowPlayer.faceTo = faceForward::down;
						nowPlayer.roleSprite->setTexture("image/role" + stream1.str() + "_down.png");
					}
					else if (road->getTileAt(Vec2(nowPlayer.rolePosition.x, nowPlayer.rolePosition.y - 1)))
					{
						nowPlayer.rolePosition.y--;
						nowPlayer.spritePosition.y += 30;
						nowPlayer.faceTo = faceForward::up;
						nowPlayer.roleSprite->setTexture("image/role" + stream1.str() + "_up.png");
					}
				}
				else if (nowPlayer.faceTo == faceForward::up)
				{
					if (road->getTileAt(Vec2(nowPlayer.rolePosition.x, nowPlayer.rolePosition.y - 1)))
					{
						nowPlayer.rolePosition.y--;
						nowPlayer.spritePosition.y += 30;
					}
					else if (road->getTileAt(Vec2(nowPlayer.rolePosition.x + 1, nowPlayer.rolePosition.y)))
					{
						nowPlayer.rolePosition.x++;
						nowPlayer.spritePosition.x += 30;
						nowPlayer.faceTo = faceForward::right;
						nowPlayer.roleSprite->setTexture("image/role" + stream1.str() + "_right.png");
					}
					else if (road->getTileAt(Vec2(nowPlayer.rolePosition.x - 1, nowPlayer.rolePosition.y)))
					{
						nowPlayer.rolePosition.x--;
						nowPlayer.spritePosition.x -= 30;
						nowPlayer.faceTo = faceForward::left;
						nowPlayer.roleSprite->setTexture("image/role" + stream1.str() + "_left.png");
					}
				}
			}

			nowPlayer.roleSprite->setPosition(nowPlayer.spritePosition);

			break;
		}
		n++;
	}
}

bool ContinueMainLayer::checkState()
{
	// 字儿们
	auto ngContent = __Dictionary::createWithContentsOfFile("XML/NewGame.xml");

	int n = 1;

	for (auto& p : players)
	{
		if (n == nowPlayerNumber)
		{
			if (p.state == stateType::normal)
			{
				// 正常行走
				return true;
			}
			else if (p.state == stateType::parking && p.stayRound > 0)
			{
				// 继续待在停车场
				p.isGoing = true;

				p.stayRound--;

				// 菜单面板图片
				menuBoard = Sprite::create("image/Popup.png");
				menuBoard->setPosition(visibleSize.width / 2, visibleSize.height / 2);
				this->addChild(menuBoard);

				// 菜单：确定
				MenuItem* okM = MenuItemImage::create("image/OrangeNormal.png",
					"image/OrangePressed.png", CC_CALLBACK_0(ContinueMainLayer::cleanAndChange, this));
				okM->setPosition(0, -visibleSize.height / 5);

				const char* okC = ((__String*)ngContent->objectForKey("ok"))->getCString();
				Label* okL = Label::createWithSystemFont(okC, "Arial", 20);
				okM->addChild(okL);
				okL->setPosition(okM->getContentSize().width / 2, okM->getContentSize().height / 2);
				okL->setTextColor(Color4B::BLACK);

				// 菜单主要内容
				const char* roundsToStayInParkinglot1 = ((__String*)ngContent->objectForKey("roundsToStayInParkinglot1"))->getCString();
				const char* roundsToStayInParkinglot2 = ((__String*)ngContent->objectForKey("roundsToStayInParkinglot2"))->getCString();
				string blank = " ";
				ostringstream stream1;
				stream1 << p.stayRound;
				string s = roundsToStayInParkinglot1 + blank + "\n" + roundsToStayInParkinglot2 + stream1.str();
				Label* noticeL = Label::createWithSystemFont(s, "Arial", 25);

				menuBoard->addChild(noticeL);
				noticeL->setPosition(menuBoard->getContentSize().width / 2, menuBoard->getContentSize().height * 3 / 4);
				noticeL->setTextColor(Color4B::BLACK);
				noticeL->setMaxLineWidth(520);

				noticeMenu = Menu::create(okM, NULL);
				this->addChild(noticeMenu);

				return false;
			}
			else if (p.state == stateType::prison && p.stayRound > 0)
			{
				// 继续待在监狱
				p.isGoing = true;

				p.stayRound--;

				// 菜单面板图片
				menuBoard = Sprite::create("image/Popup.png");
				menuBoard->setPosition(visibleSize.width / 2, visibleSize.height / 2);
				this->addChild(menuBoard);

				// 菜单：确定
				MenuItem* okM = MenuItemImage::create("image/OrangeNormal.png",
					"image/OrangePressed.png", CC_CALLBACK_0(ContinueMainLayer::cleanAndChange, this));
				okM->setPosition(0, -visibleSize.height / 5);

				const char* okC = ((__String*)ngContent->objectForKey("ok"))->getCString();
				Label* okL = Label::createWithSystemFont(okC, "Arial", 20);
				okM->addChild(okL);
				okL->setPosition(okM->getContentSize().width / 2, okM->getContentSize().height / 2);
				okL->setTextColor(Color4B::BLACK);

				// 菜单主要内容
				const char* roundsToStayInPrison1 = ((__String*)ngContent->objectForKey("roundsToStayInPrison1"))->getCString();
				const char* roundsToStayInPrison2 = ((__String*)ngContent->objectForKey("roundsToStayInPrison2"))->getCString();
				string blank = " ";
				ostringstream stream1;
				stream1 << p.stayRound;
				string s = roundsToStayInPrison1 + blank + "\n" + roundsToStayInPrison2 + stream1.str();
				Label* noticeL = Label::createWithSystemFont(s, "Arial", 25);

				menuBoard->addChild(noticeL);
				noticeL->setPosition(menuBoard->getContentSize().width / 2, menuBoard->getContentSize().height * 3 / 4);
				noticeL->setTextColor(Color4B::BLACK);
				noticeL->setMaxLineWidth(520);

				noticeMenu = Menu::create(okM, NULL);
				this->addChild(noticeMenu);

				return false;
			}
			else if (p.state == stateType::parking && p.stayRound == 0)
			{
				// 停车场时间到，出发

				p.state = stateType::normal;

				// 菜单面板图片
				menuBoard = Sprite::create("image/Popup.png");
				menuBoard->setPosition(visibleSize.width / 2, visibleSize.height / 2);
				this->addChild(menuBoard);

				// 菜单：确定
				MenuItem* okM = MenuItemImage::create("image/OrangeNormal.png",
					"image/OrangePressed.png", CC_CALLBACK_0(ContinueMainLayer::cleanMenu, this));
				okM->setPosition(0, -visibleSize.height / 5);

				const char* okC = ((__String*)ngContent->objectForKey("ok"))->getCString();
				Label* okL = Label::createWithSystemFont(okC, "Arial", 20);
				okM->addChild(okL);
				okL->setPosition(okM->getContentSize().width / 2, okM->getContentSize().height / 2);
				okL->setTextColor(Color4B::BLACK);

				// 菜单主要内容
				const char* outOfParkinglot = ((__String*)ngContent->objectForKey("outOfParkinglot"))->getCString();
				Label* noticeL = Label::createWithSystemFont(outOfParkinglot, "Arial", 25);

				menuBoard->addChild(noticeL);
				noticeL->setPosition(menuBoard->getContentSize().width / 2, menuBoard->getContentSize().height * 3 / 4);
				noticeL->setTextColor(Color4B::BLACK);

				noticeMenu = Menu::create(okM, NULL);
				this->addChild(noticeMenu);

				return true;
			}
			else if (p.state == stateType::prison && p.stayRound == 0)
			{
				// 囹圄时间到，出狱

				p.state = stateType::normal;

				// 菜单面板图片
				menuBoard = Sprite::create("image/Popup.png");
				menuBoard->setPosition(visibleSize.width / 2, visibleSize.height / 2);
				this->addChild(menuBoard);

				// 菜单：确定
				MenuItem* okM = MenuItemImage::create("image/OrangeNormal.png",
					"image/OrangePressed.png", CC_CALLBACK_0(ContinueMainLayer::cleanMenu, this));
				okM->setPosition(0, -visibleSize.height / 5);

				const char* okC = ((__String*)ngContent->objectForKey("ok"))->getCString();
				Label* okL = Label::createWithSystemFont(okC, "Arial", 20);
				okM->addChild(okL);
				okL->setPosition(okM->getContentSize().width / 2, okM->getContentSize().height / 2);
				okL->setTextColor(Color4B::BLACK);

				// 菜单主要内容
				const char* outOfPrison = ((__String*)ngContent->objectForKey("outOfPrison"))->getCString();
				Label* noticeL = Label::createWithSystemFont(outOfPrison, "Arial", 25);

				menuBoard->addChild(noticeL);
				noticeL->setPosition(menuBoard->getContentSize().width / 2, menuBoard->getContentSize().height * 3 / 4);
				noticeL->setTextColor(Color4B::BLACK);

				noticeMenu = Menu::create(okM, NULL);
				this->addChild(noticeMenu);

				return true;
			}
		}

		n++;
	}

	return true;
}

void ContinueMainLayer::checkRoad(float dt)
{
	// 清除骰子图片
	this->removeChildByName("dicePoint");

	auto ngContent = __Dictionary::createWithContentsOfFile("XML/NewGame.xml");

	int n = 1;

	for (auto& p : players)
	{
		if (n == nowPlayerNumber)
		{
			if (road->getTileGIDAt(p.rolePosition) == normal_road_GID)
			{
				// 正常通过
				this->scheduleOnce(schedule_selector(ContinueMainLayer::checkLand), 0.1f);
			}
			else if (road->getTileGIDAt(p.rolePosition) == prisonEntrance_road_GID)
			{
				// 蹲监狱

				// 修改当前玩家的状态、位置和图片位置
				p.state = stateType::prison;
				p.rolePosition = Vec2(4, 17);
				p.spritePosition.x -= 17 * 30;
				p.spritePosition.y -= 14 * 30;
				p.roleSprite->setPosition(p.spritePosition);
				ostringstream stream1;
				stream1 << roles[p.serialNumber - 1];
				p.roleSprite->setTexture("image/role" + stream1.str() + "_up.png");
				p.faceTo = faceForward::up;
				p.stayRound = 3;

				// 菜单面板图片
				menuBoard = Sprite::create("image/Popup.png");
				menuBoard->setPosition(visibleSize.width / 2, visibleSize.height / 2);
				this->addChild(menuBoard);

				// 菜单：确定
				MenuItem* okM = MenuItemImage::create("image/OrangeNormal.png",
					"image/OrangePressed.png", CC_CALLBACK_0(ContinueMainLayer::cleanAndChange, this));
				okM->setPosition(0, -visibleSize.height / 5);

				const char* okC = ((__String*)ngContent->objectForKey("ok"))->getCString();
				Label* okL = Label::createWithSystemFont(okC, "Arial", 20);
				okM->addChild(okL);
				okL->setPosition(okM->getContentSize().width / 2, okM->getContentSize().height / 2);
				okL->setTextColor(Color4B::BLACK);

				// 菜单主要内容
				const char* prisonEnterance1 = ((__String*)ngContent->objectForKey("prisonEnterance1"))->getCString();
				const char* prisonEnterance2 = ((__String*)ngContent->objectForKey("prisonEnterance2"))->getCString();
				string blank = " ";
				string s = prisonEnterance1 + blank + "\n" + prisonEnterance2;
				Label* noticeL = Label::createWithSystemFont(s, "Arial", 25);

				menuBoard->addChild(noticeL);
				noticeL->setPosition(menuBoard->getContentSize().width / 2, menuBoard->getContentSize().height * 3 / 4);
				noticeL->setTextColor(Color4B::BLACK);
				noticeL->setMaxLineWidth(520);

				noticeMenu = Menu::create(okM, NULL);
				this->addChild(noticeMenu);
			}
			else if (road->getTileGIDAt(p.rolePosition) == prison_road_GID)
			{
				// 一般通过监狱
				this->changePlayer();
			}
			else if (road->getTileGIDAt(p.rolePosition) == parkinglot_road_GID)
			{
				// 困在停车场
				p.state = stateType::parking;
				p.stayRound = 2;

				// 菜单面板图片
				menuBoard = Sprite::create("image/Popup.png");
				menuBoard->setPosition(visibleSize.width / 2, visibleSize.height / 2);
				this->addChild(menuBoard);

				// 菜单：确定
				MenuItem* okM = MenuItemImage::create("image/OrangeNormal.png",
					"image/OrangePressed.png", CC_CALLBACK_0(ContinueMainLayer::cleanAndChange, this));
				okM->setPosition(0, -visibleSize.height / 5);

				const char* okC = ((__String*)ngContent->objectForKey("ok"))->getCString();
				Label* okL = Label::createWithSystemFont(okC, "Arial", 20);
				okM->addChild(okL);
				okL->setPosition(okM->getContentSize().width / 2, okM->getContentSize().height / 2);
				okL->setTextColor(Color4B::BLACK);

				// 菜单主要内容
				const char* parkinglot = ((__String*)ngContent->objectForKey("parkinglot"))->getCString();
				Label* noticeL = Label::createWithSystemFont(parkinglot, "Arial", 25);

				menuBoard->addChild(noticeL);
				noticeL->setPosition(menuBoard->getContentSize().width / 2, menuBoard->getContentSize().height * 3 / 4);
				noticeL->setTextColor(Color4B::BLACK);
				noticeL->setMaxLineWidth(520);

				noticeMenu = Menu::create(okM, NULL);
				this->addChild(noticeMenu);
			}
			else if (road->getTileGIDAt(p.rolePosition) == emergency_road_GID)
			{
				// 突发事件

				// 事件类型
				int i = random(1, emEventNumber);
				int loss;

				switch (i)
				{
				case 1:
					loss = 100;
					break;
				case 2:
					loss = 300;
					break;
				case 3:
					loss = 500;
					break;
				case 4:
					loss = 1000;
					break;
				case 5:
					loss = 2000;
					break;
				case 6:
					loss = -400;
					break;
				case 7:
					loss = -800;
					break;
				case 8:
					loss = -1200;
					break;
				}

				// 菜单面板图片
				menuBoard = Sprite::create("image/Popup.png");
				menuBoard->setPosition(visibleSize.width / 2, visibleSize.height / 2);
				this->addChild(menuBoard);

				// 菜单：确定
				MenuItem* okM = MenuItemImage::create("image/OrangeNormal.png",
					"image/OrangePressed.png", CC_CALLBACK_0(ContinueMainLayer::checkPayPlayerMoneyLo, this, p.name, loss));
				okM->setPosition(0, -visibleSize.height / 5);

				const char* okC = ((__String*)ngContent->objectForKey("ok"))->getCString();
				Label* okL = Label::createWithSystemFont(okC, "Arial", 20);
				okM->addChild(okL);
				okL->setPosition(okM->getContentSize().width / 2, okM->getContentSize().height / 2);
				okL->setTextColor(Color4B::BLACK);

				// 菜单主要内容
				ostringstream stream1;
				stream1 << i;
				const char* emergencyEvent = ((__String*)ngContent->objectForKey("emergencyEvent" + stream1.str()))->getCString();
				Label* noticeL = Label::createWithSystemFont(emergencyEvent, "Arial", 25);

				menuBoard->addChild(noticeL);
				noticeL->setPosition(menuBoard->getContentSize().width / 2, menuBoard->getContentSize().height * 3 / 4);
				noticeL->setTextColor(Color4B::BLACK);

				noticeMenu = Menu::create(okM, NULL);
				this->addChild(noticeMenu);
			}
			else if (road->getTileGIDAt(p.rolePosition) == tax_road_GID)
			{
				// 交小费

				// 菜单面板图片
				menuBoard = Sprite::create("image/Popup.png");
				menuBoard->setPosition(visibleSize.width / 2, visibleSize.height / 2);
				this->addChild(menuBoard);

				// 菜单：确定
				MenuItem* okM = MenuItemImage::create("image/OrangeNormal.png",
					"image/OrangePressed.png", CC_CALLBACK_0(ContinueMainLayer::checkPayPlayerMoneyLo, this, p.name, 700));
				okM->setPosition(0, -visibleSize.height / 5);

				const char* okC = ((__String*)ngContent->objectForKey("ok"))->getCString();
				Label* okL = Label::createWithSystemFont(okC, "Arial", 20);
				okM->addChild(okL);
				okL->setPosition(okM->getContentSize().width / 2, okM->getContentSize().height / 2);
				okL->setTextColor(Color4B::BLACK);

				// 菜单主要内容
				const char* tax1 = ((__String*)ngContent->objectForKey("tax1"))->getCString();
				const char* tax2 = ((__String*)ngContent->objectForKey("tax2"))->getCString();
				string blank = " ";
				string s = tax1 + blank + "\n" + tax2;
				Label* noticeL = Label::createWithSystemFont(s, "Arial", 25);


				menuBoard->addChild(noticeL);
				noticeL->setPosition(menuBoard->getContentSize().width / 2, menuBoard->getContentSize().height * 3 / 4);
				noticeL->setTextColor(Color4B::BLACK);

				noticeMenu = Menu::create(okM, NULL);
				this->addChild(noticeMenu);
			}
			else if (road->getTileGIDAt(p.rolePosition) == entrance_road_GID)
			{
				// 一般通过起始位置
				this->changePlayer();
			}

			break;
		}

		n++;
	}
}

void ContinueMainLayer::checkLand(float dt)
{
	int n = 1;

	for (auto& p : players)
	{
		if (n == nowPlayerNumber)
		{
			// 判断人物朝向，确定土地位置
			switch (p.faceTo)
			{
			case faceForward::right:
				nowLand = Vec2(p.rolePosition.x, p.rolePosition.y + 1);
				break;
			case faceForward::down:
				nowLand = Vec2(p.rolePosition.x - 1, p.rolePosition.y);
				break;
			case faceForward::left:
				nowLand = Vec2(p.rolePosition.x, p.rolePosition.y - 1);
				break;
			case faceForward::up:
				nowLand = Vec2(p.rolePosition.x + 1, p.rolePosition.y);
				break;
			}

			// 从所有土地层判断是否在对应坐标存在土地
			for (auto l : lands)
			{
				sLand = l->getTileAt(nowLand);

				if (sLand)
				{
					gLand = l->getTileGIDAt(nowLand);

					break;
				}
			}

			if (sLand)
			{
				if (gLand == empty_land_GID && sLand->getColor() == Color3B::WHITE)
				{
					this->emptyLand();
				}
				else if (sLand->getColor() == p.color)
				{
					this->myLand();
				}
				else
				{
					this->otherLand();
				}
			}
			else
			{
				this->changePlayer();
			}

			break;
		}

		n++;
	}
}

// Land相关函数

// 空地
void ContinueMainLayer::emptyLand()
{
	string name;
	int n = 1;

	for (auto p : players)
	{
		if (n == nowPlayerNumber)
		{
			name = p.name;
			break;
		}

		n++;
	}

	// 菜单面板图片
	menuBoard = Sprite::create("image/Popup.png");
	menuBoard->setPosition(visibleSize.width / 2, visibleSize.height / 2);
	this->addChild(menuBoard);

	// 字儿们
	auto ngContent = __Dictionary::createWithContentsOfFile("XML/NewGame.xml");

	// 菜单 ：是
	MenuItem* yesM = MenuItemImage::create("image/GreenNormal.png",
		"image/GreenPressed.png", CC_CALLBACK_0(ContinueMainLayer::emptyMenuYes, this));
	yesM->setPosition(-visibleSize.width / 8, -visibleSize.height / 5);

	const char* yesC = ((__String*)ngContent->objectForKey("yes"))->getCString();
	Label* yesL = Label::createWithSystemFont(yesC, "Arial", 20);
	yesM->addChild(yesL);
	yesL->setPosition(yesM->getContentSize().width / 2, yesM->getContentSize().height / 2);
	yesL->setTextColor(Color4B::BLACK);

	// 菜单 ：否
	MenuItem* noM = MenuItemImage::create("image/RedNormal.png",
		"image/RedPressed.png", CC_CALLBACK_0(ContinueMainLayer::emptyMenuNo, this));
	noM->setPosition(visibleSize.width / 8, -visibleSize.height / 5);

	const char* noC = ((__String*)ngContent->objectForKey("no"))->getCString();
	Label* noL = Label::createWithSystemFont(noC, "Arial", 20);
	noM->addChild(noL);
	noL->setPosition(noM->getContentSize().width / 2, noM->getContentSize().height / 2);
	noL->setTextColor(Color4B::BLACK);


	// 菜单主要内容
	Label* noticeL;

	const char* nc = ((__String*)ngContent->objectForKey(name))->getCString();
	string blank = " ";
	const char* comma = ((__String*)ngContent->objectForKey("comma"))->getCString();
	const char* upgradeLand0 = ((__String*)ngContent->objectForKey("upgradeLand0"))->getCString();
	const char* payToUpgrade = ((__String*)ngContent->objectForKey("payToUpgrade"))->getCString();

	// 所在空白地块的价格
	int emptyBuildCost;

	for (auto l : lands)
	{
		if (l->getTileAt(nowLand))
		{
			emptyBuildCost = l->getProperty("emptyBuildCost").asInt();
		}
	}

	const char* yuan = ((__String*)ngContent->objectForKey("yuan"))->getCString();
	ostringstream stream1;
	stream1 << emptyBuildCost;
	string s = nc + blank + comma + upgradeLand0 + "\n" +
		payToUpgrade + blank + stream1.str() + yuan;
	noticeL = Label::createWithSystemFont(s, "Arial", 25);

	menuBoard->addChild(noticeL);
	noticeL->setPosition(menuBoard->getContentSize().width / 2, menuBoard->getContentSize().height * 3 / 4);
	noticeL->setTextColor(Color4B::BLACK);

	noticeMenu = Menu::create(yesM, noM, NULL);
	this->addChild(noticeMenu);
}

void ContinueMainLayer::emptyMenuYes()
{
	int n = 1;

	for (auto& p : players)
	{
		if (n == nowPlayerNumber)
		{
			int emptyBuildCost;

			for (auto l : lands)
			{
				if (l->getTileAt(nowLand))
				{
					emptyBuildCost = l->getProperty("emptyBuildCost").asInt();
					auto ngContent = __Dictionary::createWithContentsOfFile("XML/NewGame.xml");

					if (p.money >= emptyBuildCost)
					{
						l->getTileAt(nowLand)->setColor(p.color);

						// 粒子显示
						ParticleGalaxy* pg = ParticleGalaxy::create();

                        pg->setPosition(visibleSize/2);
						/*switch (p.faceTo)
						{
						case faceForward::left:
							pg->setPosition(p.spritePosition.x, p.spritePosition.y + 30);
							break;
						case faceForward::down:
							pg->setPosition(p.spritePosition.x - 30, p.spritePosition.y);
							break;
						case faceForward::right:
							pg->setPosition(p.spritePosition.x, p.spritePosition.y - 30);
							break;
						case faceForward::up:
							pg->setPosition(p.spritePosition.x + 30, p.spritePosition.y);
							break;
						}*/
						Color4F color1;
						color1.r = p.color.r / 255;
						color1.g = p.color.g / 255;
						color1.b = p.color.b / 255;
						color1.a = 255;
						pg->setStartColor(color1);
						Color4F color2;
						color2.r = 0;
						color2.g = 0;
						color2.b = 0;
						color2.a = 0;
						pg->setEndColor(color2);
						this->addChild(pg, 0, "particle");

						this->scheduleOnce(schedule_selector(ContinueMainLayer::removeParticle), 1.0f);

						// ここまでだ

						p.money -= emptyBuildCost;

						const char* rmb = ((__String*)ngContent->objectForKey("rmb"))->getCString();
						string blank = " ";
						ostringstream stream1;
						stream1 << p.money;
						string s = rmb + blank + stream1.str();

						((Label*)this->getChildByName(p.name + blank + "money"))->setString(s);

						this->cleanAndChange();
					}
					else
					{
						this->cleanMenu();

						// 菜单面板图片
						menuBoard = Sprite::create("image/Popup.png");
						menuBoard->setPosition(visibleSize.width / 2, visibleSize.height / 2);
						this->addChild(menuBoard);

						// 菜单：确定
						MenuItem* okM = MenuItemImage::create("image/OrangeNormal.png",
							"image/OrangePressed.png", CC_CALLBACK_0(ContinueMainLayer::cleanAndChange, this));
						okM->setPosition(0, -visibleSize.height / 5);

						const char* okC = ((__String*)ngContent->objectForKey("ok"))->getCString();
						Label* okL = Label::createWithSystemFont(okC, "Arial", 20);
						okM->addChild(okL);
						okL->setPosition(okM->getContentSize().width / 2, okM->getContentSize().height / 2);
						okL->setTextColor(Color4B::BLACK);

						// 菜单主要内容
						const char* lackOfMoney = ((__String*)ngContent->objectForKey("lackOfMoney"))->getCString();
						Label* noticeL = Label::createWithSystemFont(lackOfMoney, "Arial", 25);

						menuBoard->addChild(noticeL);
						noticeL->setPosition(menuBoard->getContentSize().width / 2, menuBoard->getContentSize().height * 3 / 4);
						noticeL->setTextColor(Color4B::BLACK);

						noticeMenu = Menu::create(okM, NULL);
						this->addChild(noticeMenu);
					}

					break;
				}
			}

			break;
		}

		n++;
	}
}

void ContinueMainLayer::emptyMenuNo()
{
	this->cleanAndChange();
}

// 我的
void ContinueMainLayer::myLand()
{
	string name;
	int n = 1;

	for (auto p : players)
	{
		if (n == nowPlayerNumber)
		{
			name = p.name;
			break;
		}

		n++;
	}

	// 菜单面板图片
	menuBoard = Sprite::create("image/Popup.png");
	menuBoard->setPosition(visibleSize.width / 2, visibleSize.height / 2);
	this->addChild(menuBoard);

	// 字儿们
	auto ngContent = __Dictionary::createWithContentsOfFile("XML/NewGame.xml");

	// 菜单 ：是
	MenuItem* yesM = MenuItemImage::create("image/GreenNormal.png",
		"image/GreenPressed.png", CC_CALLBACK_0(ContinueMainLayer::myMenuYes, this));
	yesM->setPosition(-visibleSize.width / 8, -visibleSize.height / 5);

	const char* yesC = ((__String*)ngContent->objectForKey("yes"))->getCString();
	Label* yesL = Label::createWithSystemFont(yesC, "Arial", 20);
	yesM->addChild(yesL);
	yesL->setPosition(yesM->getContentSize().width / 2, yesM->getContentSize().height / 2);
	yesL->setTextColor(Color4B::BLACK);

	// 菜单 ：否
	MenuItem* noM = MenuItemImage::create("image/RedNormal.png",
		"image/RedPressed.png", CC_CALLBACK_0(ContinueMainLayer::myMenuNo, this));
	noM->setPosition(visibleSize.width / 8, -visibleSize.height / 5);

	const char* noC = ((__String*)ngContent->objectForKey("no"))->getCString();
	Label* noL = Label::createWithSystemFont(noC, "Arial", 20);
	noM->addChild(noL);
	noL->setPosition(noM->getContentSize().width / 2, noM->getContentSize().height / 2);
	noL->setTextColor(Color4B::BLACK);

	// 提示栏内容
	Label* noticeL;
	const char* nc = ((__String*)ngContent->objectForKey(name))->getCString();
	string blank = " ";
	const char* comma = ((__String*)ngContent->objectForKey("comma"))->getCString();
	const char* upgradeLand;
	int price;

	if (gLand == empty_land_GID)
	{
		upgradeLand = ((__String*)ngContent->objectForKey("upgradeLand1"))->getCString();

		for (auto l : lands)
		{
			if (l->getTileAt(nowLand))
			{
				price = l->getProperty("level1BuildCost").asInt();
			}
		}
	}
	else if (gLand == level1_land_GID)
	{
		upgradeLand = ((__String*)ngContent->objectForKey("upgradeLand2"))->getCString();

		for (auto l : lands)
		{
			if (l->getTileAt(nowLand))
			{
				price = l->getProperty("level2BuildCost").asInt();
			}
		}
	}
	else if (gLand == level2_land_GID)
	{
		upgradeLand = ((__String*)ngContent->objectForKey("upgradeLand3"))->getCString();

		for (auto l : lands)
		{
			if (l->getTileAt(nowLand))
			{
				price = l->getProperty("level3BuildCost").asInt();
			}
		}
	}
	else if (gLand == level3_land_GID)
	{
		// 土地已为最高级，无需弹出对话框
	}

	const char* payToUpgrade = ((__String*)ngContent->objectForKey("payToUpgrade"))->getCString();
	const char* yuan = ((__String*)ngContent->objectForKey("yuan"))->getCString();

	ostringstream stream1;
	stream1 << price;
	string s = nc + blank + comma + upgradeLand + "\n" +
		payToUpgrade + blank + stream1.str() + yuan;
	noticeL = Label::createWithSystemFont(s, "Arial", 25);

	menuBoard->addChild(noticeL);
	noticeL->setPosition(menuBoard->getContentSize().width / 2, menuBoard->getContentSize().height * 3 / 4);
	noticeL->setTextColor(Color4B::BLACK);

	noticeMenu = Menu::create(yesM, noM, NULL);
	this->addChild(noticeMenu);
}

void ContinueMainLayer::myMenuYes()
{
	int n = 1;

	for (auto& p : players)
	{
		if (n == nowPlayerNumber)
		{
			int buildCost = 0;
			auto ngContent = __Dictionary::createWithContentsOfFile("XML/NewGame.xml");

			for (auto l : lands)
			{
				if (l->getTileAt(nowLand))
				{
					if (gLand == empty_land_GID)
					{
						buildCost = l->getProperty("level1BuildCost").asInt();
					}
					else if (gLand == level1_land_GID)
					{
						buildCost = l->getProperty("level2BuildCost").asInt();
					}
					else if (gLand == level2_land_GID)
					{
						buildCost = l->getProperty("level3BuildCost").asInt();
					}

					if (p.money >= buildCost)
					{
						if (gLand == empty_land_GID)
						{
							l->setTileGID(level1_land_GID, nowLand);
						}
						else if (gLand == level1_land_GID)
						{
							l->setTileGID(level2_land_GID, nowLand);
						}
						else if (gLand == level2_land_GID)
						{
							l->setTileGID(level3_land_GID, nowLand);
						}

						l->getTileAt(nowLand)->setColor(p.color);
						p.money -= buildCost;

						// 粒子显示
						ParticleGalaxy* pg = ParticleGalaxy::create();

						pg->setPosition(visibleSize/2);
						/*switch (p.faceTo)
						{
						case faceForward::left:
							pg->setPosition(p.spritePosition.x, p.spritePosition.y + 30);
							break;
						case faceForward::down:
							pg->setPosition(p.spritePosition.x - 30, p.spritePosition.y);
							break;
						case faceForward::right:
							pg->setPosition(p.spritePosition.x, p.spritePosition.y - 30);
							break;
						case faceForward::up:
							pg->setPosition(p.spritePosition.x + 30, p.spritePosition.y);
							break;
						}
						  */
						Color4F color4F1;
						color4F1.r = p.color.r / 255;
						color4F1.g = p.color.g / 255;
						color4F1.b =  p.color.b / 255;
						color4F1.a = 255;
						pg->setStartColor(color4F1);
						Color4F color4F2;
						color4F2.r = 0;
						color4F2.g = 0;
						color4F2.b = 0;
						color4F2.a = 0;
						pg->setEndColor(color4F2);
						this->addChild(pg, 0, "particle");

						this->scheduleOnce(schedule_selector(ContinueMainLayer::removeParticle), 1.0f);

						// ここまでだ

						auto ngContent = __Dictionary::createWithContentsOfFile("XML/NewGame.xml");
						const char* rmb = ((__String*)ngContent->objectForKey("rmb"))->getCString();
						string blank = " ";
						ostringstream stream1;
						stream1 << p.money;
						string s = rmb + blank + stream1.str();

						((Label*)this->getChildByName(p.name + blank + "money"))->setString(s);

						this->cleanAndChange();
					}
					else
					{
						this->cleanMenu();

						// 菜单面板图片
						menuBoard = Sprite::create("image/Popup.png");
						menuBoard->setPosition(visibleSize.width / 2, visibleSize.height / 2);
						this->addChild(menuBoard);

						// 菜单：确定
						MenuItem* okM = MenuItemImage::create("image/OrangeNormal.png",
							"image/OrangePressed.png", CC_CALLBACK_0(ContinueMainLayer::cleanAndChange, this));
						okM->setPosition(0, -visibleSize.height / 5);

						const char* okC = ((__String*)ngContent->objectForKey("ok"))->getCString();
						Label* okL = Label::createWithSystemFont(okC, "Arial", 20);
						okM->addChild(okL);
						okL->setPosition(okM->getContentSize().width / 2, okM->getContentSize().height / 2);
						okL->setTextColor(Color4B::BLACK);

						// 菜单主要内容
						const char* lackOfMoney = ((__String*)ngContent->objectForKey("lackOfMoney"))->getCString();
						Label* noticeL = Label::createWithSystemFont(lackOfMoney, "Arial", 25);

						menuBoard->addChild(noticeL);
						noticeL->setPosition(menuBoard->getContentSize().width / 2, menuBoard->getContentSize().height * 3 / 4);
						noticeL->setTextColor(Color4B::BLACK);

						noticeMenu = Menu::create(okM, NULL);
						this->addChild(noticeMenu);
					}

					break;
				}
			}

			break;
		}

		n++;
	}
}

void ContinueMainLayer::myMenuNo()
{
	this->cleanAndChange();
}

// 他们的

void ContinueMainLayer::otherLand()
{
	string payName, earnName;
	faceForward nowF;
	int n = 1;

	for (auto p : players)
	{
		if (n == nowPlayerNumber)
		{
			payName = p.name;
			nowF = p.faceTo;
		}
		if (p.color == sLand->getColor())
		{
			earnName = p.name;
		}

		n++;
	}

	// 菜单面板图片
	menuBoard = Sprite::create("image/Popup.png");
	menuBoard->setPosition(visibleSize.width / 2, visibleSize.height / 2);
	this->addChild(menuBoard);

	// 字儿们
	auto ngContent = __Dictionary::createWithContentsOfFile("XML/NewGame.xml");

	// 提示框内容
	Label* noticeL;
	const char* belongLand = ((__String*)ngContent->objectForKey("belongLand"))->getCString();
	string blank = " ";
	const char* earnC = ((__String*)ngContent->objectForKey(earnName))->getCString();
	const char* comma = ((__String*)ngContent->objectForKey("comma"))->getCString();
	const char* payC = ((__String*)ngContent->objectForKey(payName))->getCString();
	const char* payLand = ((__String*)ngContent->objectForKey("payLand"))->getCString();

	int price, rateTimes;
	int rateTimes1 = 1;
	int rateTimes2 = 1;
	int rate = 0;

	for (auto l : lands)
	{
		if (l->getTileAt(nowLand))
		{
			// 相邻有相同玩家的地，2块+5%，3块+10%，4块+20%，5块+50%,6块+100%

			if (nowF == faceForward::down || nowF == faceForward::up)
			{
				// 向上
				for (;;)
				{
					if (l->getTileAt(Vec2(nowLand.x, nowLand.y - rateTimes1)))
					{
						if (l->getTileAt(Vec2(nowLand.x, nowLand.y - rateTimes1))->getColor() ==
							l->getTileAt(nowLand)->getColor())
						{
							rateTimes1++;
						}
						else
						{
							break;
						}
					}
					else
					{
						break;
					}
				}

				// 向下
				for (;;)
				{
					if (l->getTileAt(Vec2(nowLand.x, nowLand.y + rateTimes2)))
					{
						if (l->getTileAt(Vec2(nowLand.x, nowLand.y + rateTimes2))->getColor() ==
							l->getTileAt(nowLand)->getColor())
						{
							rateTimes2++;
						}
						else
						{
							break;
						}
					}
					else
					{
						break;
					}
				}
			}
			else if (nowF == faceForward::right || nowF == faceForward::left)
			{
				// 向左
				for (;;)
				{
					if (l->getTileAt(Vec2(nowLand.x - rateTimes1, nowLand.y)))
					{
						if (l->getTileAt(Vec2(nowLand.x - rateTimes1, nowLand.y))->getColor() ==
							l->getTileAt(nowLand)->getColor())
						{
							rateTimes1++;
						}
						else
						{
							break;
						}
					}
					else
					{
						break;
					}
				}

				// 向右
				for (;;)
				{
					if (l->getTileAt(Vec2(nowLand.x + rateTimes2, nowLand.y)))
					{
						if (l->getTileAt(Vec2(nowLand.x + rateTimes2, nowLand.y))->getColor() ==
							l->getTileAt(nowLand)->getColor())
						{
							rateTimes2++;
						}
						else
						{
							break;
						}
					}
					else
					{
						break;
					}
				}
			}

			// 根据不同的占地数量，收取不同的费用
			rateTimes = rateTimes1 + rateTimes2;
			switch (rateTimes)
			{
			case 2:
				rate = 0;
				break;
			case 3:
				rate = 5;
				break;
			case 4:
				rate = 10;
				break;
			case 5:
				rate = 20;
				break;
			case 6:
				rate = 50;
				break;
			case 7:
				rate = 100;
				break;
			}

			if (gLand == empty_land_GID)
			{
				price = l->getProperty("emptyValue").asInt();
			}
			else if (gLand == level1_land_GID)
			{
				price = l->getProperty("level1Value").asInt();
			}
			else if (gLand == level2_land_GID)
			{
				price = l->getProperty("level2Value").asInt();
			}
			else if (gLand == level3_land_GID)
			{
				price = l->getProperty("level3Value").asInt();
			}

			price += price*(rate / 100);
		}
	}

	const char* yuan = ((__String*)ngContent->objectForKey("yuan"))->getCString();

	string s;
	if (rate == 0)
	{
		ostringstream stream1;
		stream1 << price;
		s = belongLand + blank + earnC + comma + payC + "\n" +
			payLand + blank + stream1.str() + yuan;
	}
	else
	{
		ostringstream stream1, stream2;
		stream1 << price;
		stream2 << rate;
		s = belongLand + blank + earnC + comma + payC + "\n" +
			payLand + blank + stream1.str() + yuan + "(+" + stream2.str() + "%)";
	}

	noticeL = Label::createWithSystemFont(s, "Arial", 25);

	menuBoard->addChild(noticeL);
	noticeL->setPosition(menuBoard->getContentSize().width / 2, menuBoard->getContentSize().height * 3 / 4);
	noticeL->setTextColor(Color4B::BLACK);

	// 菜单：确定
	MenuItem* okM = MenuItemImage::create("image/OrangeNormal.png",
		"image/OrangePressed.png", CC_CALLBACK_0(ContinueMainLayer::checkPayPlayerMoney, this, payName, earnName, price));
	okM->setPosition(0, -visibleSize.height / 5);

	const char* okC = ((__String*)ngContent->objectForKey("ok"))->getCString();
	Label* okL = Label::createWithSystemFont(okC, "Arial", 20);
	okM->addChild(okL);
	okL->setPosition(okM->getContentSize().width / 2, okM->getContentSize().height / 2);
	okL->setTextColor(Color4B::BLACK);

	noticeMenu = Menu::create(okM, NULL);
	this->addChild(noticeMenu);
}

void ContinueMainLayer::checkPayPlayerMoney(string payName, string earnName, int price)
{
	// 暴力检查

	for (auto& p : players)
	{
		if (p.name == payName)
		{
			if (p.money >= price)
			{
				this->otherMenuClose(payName, earnName, price);
			}
			else
			{
				bool sellFlag = false;
				bool thisTimeFlag = false;
				int sellNumber = 0;

				int sellMoney = 0;
				int wholeSellMoney = 0;

				int landNumbers = 0;

				while (p.money < price)
				{
					int endX = 24;
					int endY = 21;

					sellMoney = 0;
					landNumbers = 0;
					thisTimeFlag = false;

					for (auto l : lands)
					{
						++landNumbers;

						for (int y = 0; y < endY; ++y)
						{
							for (int x = 0; x < endX; ++x)
							{
								if (l->getTileAt(Vec2(x, y)))
								{
									if (l->getTileAt(Vec2(x, y))->getColor() == p.color)
									{
										sellFlag = true;
										thisTimeFlag = true;
										++sellNumber;

										if (l->getTileGIDAt(Vec2(x, y)) == empty_land_GID)
										{
											sellMoney = l->getProperty("emptyBuildCost").asInt();
										}
										else if (l->getTileGIDAt(Vec2(x, y)) == level1_land_GID)
										{
											sellMoney = l->getProperty("emptyBuildCost").asInt()
												+ l->getProperty("level1BuildCost").asInt();
										}
										else if (l->getTileGIDAt(Vec2(x, y)) == level2_land_GID)
										{
											sellMoney = l->getProperty("emptyBuildCost").asInt()
												+ l->getProperty("level1BuildCost").asInt()
												+ l->getProperty("level2BuildCost").asInt();
										}
										else if (l->getTileGIDAt(Vec2(x, y)) == level3_land_GID)
										{
											sellMoney = l->getProperty("emptyBuildCost").asInt()
												+ l->getProperty("level1BuildCost").asInt()
												+ l->getProperty("level2BuildCost").asInt()
												+ l->getProperty("level3BuildCost").asInt();
										}

										// 把土地回归本真
										l->setTileGID(empty_land_GID, Vec2(x, y));
										l->getTileAt(Vec2(x, y))->setColor(Color3B::WHITE);

										break;
									}
								}
							}

							if (thisTimeFlag == true)
							{
								break;
							}
						}

						if (thisTimeFlag == true)
						{
							break;
						}
					}

					wholeSellMoney += sellMoney;
					p.money += sellMoney;

					if (thisTimeFlag == false && landNumbers == lands.size() && sellMoney == 0)
					{
						break;
					}
				}

				if (sellFlag == true)
				{
					auto ngContent = __Dictionary::createWithContentsOfFile("XML/NewGame.xml");
					const char* selll1 = ((__String*)ngContent->objectForKey("sellLand1"))->getCString();
					const char* selll2 = ((__String*)ngContent->objectForKey("sellLand2"))->getCString();
					const char* selll3 = ((__String*)ngContent->objectForKey("sellLand3"))->getCString();
					const char* selll4 = ((__String*)ngContent->objectForKey("sellLand4"))->getCString();
					const char* yuan = ((__String*)ngContent->objectForKey("yuan"))->getCString();
					const char* comma = ((__String*)ngContent->objectForKey("comma"))->getCString();
					string blank = "";
					Sprite* s = Sprite::create("image/Popup.png");
					s->setPosition(visibleSize / 2);

					ostringstream stream1, stream2, stream3;
					stream1 << sellNumber;
					stream2 << wholeSellMoney;
					stream3 << p.money;
					string str = blank + selll1 + "\n"
						+ selll2 + stream1.str() + selll3 + stream2.str() + yuan + comma + "\n"
						+ selll4 + stream3.str() + yuan;
					Label* la = Label::createWithSystemFont(str, "Arial", 25);
					la->setTextColor(Color4B::BLACK);
					la->setPosition(s->getContentSize().width / 2,
						s->getContentSize().height * 3 / 4);
					s->addChild(la);

					Button* btn = Button::create("image/GreenNormal.png", "image/GreenPressed.png");
					btn->setPosition(Vec2(s->getContentSize().width / 2,
						s->getContentSize().height / 4 - btn->getContentSize().height));
					btn->addTouchEventListener(CC_CALLBACK_2(ContinueMainLayer::checkToClose, this, payName, earnName, price));
					btn->setPressedActionEnabled(true);

					Label* ok = Label::createWithSystemFont("OK", "Arial", 20);
					ok->setColor(Color3B::BLACK);
					ok->setPosition(btn->getContentSize().width / 2, btn->getContentSize().height / 2);
					btn->addChild(ok);

					s->addChild(btn);

					this->addChild(s, 50, "RUA");
				}
				else
				{
					this->otherMenuClose(payName, earnName, price);
				}
			}

			break;
		}
	}
}

void ContinueMainLayer::checkToClose(Ref* pSender, Widget::TouchEventType type, string payName, string earnName, int price)
{
	switch (type)
	{
	case Widget::TouchEventType::BEGAN:
		break;
	case Widget::TouchEventType::MOVED:
		break;
	case Widget::TouchEventType::ENDED:
	{
		auto a = this->getChildByName("RUA");
		a->removeFromParentAndCleanup(true);

		this->otherMenuClose(payName, earnName, price);
	}
	case Widget::TouchEventType::CANCELED:
		break;
	default:
		break;
	}
}

void ContinueMainLayer::otherMenuClose(string payName, string earnName, int price)
{
	// 找出对应的两个玩家，进行金钱交易
	for (auto& p : players)
	{
		for (auto& e : players)
		{
			if (p.name == payName && e.name == earnName)
			{
				p.money -= price;
				e.money += price;

				auto ngContent = __Dictionary::createWithContentsOfFile("XML/NewGame.xml");
				const char* rmb = ((__String*)ngContent->objectForKey("rmb"))->getCString();
				string blank = " ";
				ostringstream stream1;
				stream1 << p.money;
				string sp = rmb + blank + stream1.str();
				ostringstream stream2;
				stream2 << e.money;
				string se = rmb + blank + stream2.str();

				((Label*)this->getChildByName(payName + blank + "money"))->setString(sp);
				((Label*)this->getChildByName(earnName + blank + "money"))->setString(se);

				break;
			}
		}
	}

	this->cleanAndChange();
}

// 丢钱了

void ContinueMainLayer::checkPayPlayerMoneyLo(string payName, int loss)
{
	// 暴力检查

	for (auto& p : players)
	{
		if (p.name == payName)
		{
			if (p.money >= loss)
			{
				this->payLoss(payName, loss);
			}
			else
			{
				bool sellFlag = false;
				bool thisTimeFlag = false;
				int sellNumber = 0;

				int sellMoney = 0;
				int wholeSellMoney = 0;

				int landNumbers = 0;

				while (p.money < loss)
				{
					int endX = 24;
					int endY = 21;

					sellMoney = 0;
					landNumbers = 0;
					thisTimeFlag = false;

					for (auto l : lands)
					{
						++landNumbers;

						for (int y = 0; y < endY; ++y)
						{
							for (int x = 0; x < endX; ++x)
							{
								if (l->getTileAt(Vec2(x, y)))
								{
									if (l->getTileAt(Vec2(x, y))->getColor() == p.color)
									{
										sellFlag = true;
										++sellNumber;
										thisTimeFlag = true;

										if (l->getTileGIDAt(Vec2(x, y)) == empty_land_GID)
										{
											sellMoney = l->getProperty("emptyBuildCost").asInt();
										}
										else if (l->getTileGIDAt(Vec2(x, y)) == level1_land_GID)
										{
											sellMoney = l->getProperty("emptyBuildCost").asInt()
												+ l->getProperty("level1BuildCost").asInt();
										}
										else if (l->getTileGIDAt(Vec2(x, y)) == level2_land_GID)
										{
											sellMoney = l->getProperty("emptyBuildCost").asInt()
												+ l->getProperty("level1BuildCost").asInt()
												+ l->getProperty("level2BuildCost").asInt();
										}
										else if (l->getTileGIDAt(Vec2(x, y)) == level3_land_GID)
										{
											sellMoney = l->getProperty("emptyBuildCost").asInt()
												+ l->getProperty("level1BuildCost").asInt()
												+ l->getProperty("level2BuildCost").asInt()
												+ l->getProperty("level3BuildCost").asInt();
										}

										// 把土地回归本真
										l->setTileGID(empty_land_GID, Vec2(x, y));
										l->getTileAt(Vec2(x, y))->setColor(Color3B::WHITE);

										break;
									}
								}
							}

							if (thisTimeFlag == true)
							{
								break;
							}
						}

						if (thisTimeFlag == true)
						{
							break;
						}
					}

					wholeSellMoney += sellMoney;
					p.money += sellMoney;

					if (thisTimeFlag == false && landNumbers == lands.size() && sellMoney == 0)
					{
						break;
					}
				}

				if (sellFlag == true)
				{
					auto ngContent = __Dictionary::createWithContentsOfFile("XML/NewGame.xml");
					const char* selll1 = ((__String*)ngContent->objectForKey("sellLand1"))->getCString();
					const char* selll2 = ((__String*)ngContent->objectForKey("sellLand2"))->getCString();
					const char* selll3 = ((__String*)ngContent->objectForKey("sellLand3"))->getCString();
					const char* selll4 = ((__String*)ngContent->objectForKey("sellLand4"))->getCString();
					const char* yuan = ((__String*)ngContent->objectForKey("yuan"))->getCString();
					const char* comma = ((__String*)ngContent->objectForKey("comma"))->getCString();
					string blank = "";
					Sprite* s = Sprite::create("image/Popup.png");
					s->setPosition(visibleSize / 2);

					ostringstream stream1, stream2, stream3;
					stream1 << sellNumber;
					stream2 << wholeSellMoney;
					stream3 << p.money;
					string str = blank + selll1 + "\n"
						+ selll2 + stream1.str() + selll3 + stream2.str() + yuan + comma + "\n"
						+ selll4 + stream3.str() + yuan;
					Label* la = Label::createWithSystemFont(str, "Arial", 25);
					la->setTextColor(Color4B::BLACK);
					la->setPosition(s->getContentSize().width / 2,
						s->getContentSize().height * 3 / 4);
					s->addChild(la);

					Button* btn = Button::create("image/GreenNormal.png", "image/GreenPressed.png");
					btn->setPosition(Vec2(s->getContentSize().width / 2,
						s->getContentSize().height / 4 - btn->getContentSize().height));
					btn->addTouchEventListener(CC_CALLBACK_2(ContinueMainLayer::checkToCloseLo, this, payName, loss));
					btn->setPressedActionEnabled(true);

					Label* ok = Label::createWithSystemFont("OK", "Arial", 20);
					ok->setColor(Color3B::BLACK);
					ok->setPosition(btn->getContentSize().width / 2, btn->getContentSize().height / 2);
					btn->addChild(ok);

					s->addChild(btn);

					this->addChild(s, 50, "RUA");
				}
				else
				{
					this->payLoss(payName, loss);
				}
			}

			break;
		}
	}
}

void ContinueMainLayer::checkToCloseLo(Ref* pSender, Widget::TouchEventType type, string payName, int loss)
{
	switch (type)
	{
	case Widget::TouchEventType::BEGAN:
		break;
	case Widget::TouchEventType::MOVED:
		break;
	case Widget::TouchEventType::ENDED:
	{
		auto a = this->getChildByName("RUA");
		a->removeFromParentAndCleanup(true);

		this->payLoss(payName, loss);
	}
	case Widget::TouchEventType::CANCELED:
		break;
	default:
		break;
	}
}

void ContinueMainLayer::payLoss(string payName, int loss)
{
	for (auto& p : players)
	{
		if (p.name == payName)
		{
			auto ngContent = __Dictionary::createWithContentsOfFile("XML/NewGame.xml");
			const char* rmb = ((__String*)ngContent->objectForKey("rmb"))->getCString();
			string blank = " ";
			p.money -= loss;

			ostringstream stream;
			stream << p.money;
			string sp = rmb + blank + stream.str();

			((Label*)this->getChildByName(payName + blank + "money"))->setString(sp);

			break;
		}
	}

	this->cleanAndChange();
}

// 清除相关函数

void ContinueMainLayer::cleanMenu()
{
	noticeMenu->removeFromParentAndCleanup(true);
	menuBoard->removeFromParentAndCleanup(true);
}

void ContinueMainLayer::changePlayer()
{
	int n = 1;

	for (auto& p : players)
	{
		if (n == nowPlayerNumber)
		{
			p.isGoing = false;

			break;
		}

		n++;
	}

	// 如果玩家金钱小于0，则踢出游戏
	n = 0;
	bool flag = false;

	for (auto& p : players)
	{
		if (p.money < 0)
		{
			// 菜单面板图片
			menuBoard = Sprite::create("image/Popup.png");
			menuBoard->setPosition(visibleSize.width / 2, visibleSize.height / 2);
			this->addChild(menuBoard);

			// 字儿们
			auto ngContent = __Dictionary::createWithContentsOfFile("XML/NewGame.xml");

			// 提示框内容
			Label* noticeL;
			const char* belongLand = ((__String*)ngContent->objectForKey("belongLand"))->getCString();
			const char* eliminate = ((__String*)ngContent->objectForKey("eliminate"))->getCString();
			string str = "";
			string s = "";

			// Change player get way
			ostringstream stream1;
			stream1 << p.serialNumber;
			string strPlayer = "player" + stream1.str();
			const char* player = ((__String*)ngContent->objectForKey(strPlayer))->getCString();
			s = player + str + eliminate;

			noticeL = Label::createWithSystemFont(s, "Arial", 25);

			menuBoard->addChild(noticeL);
			noticeL->setPosition(menuBoard->getContentSize().width / 2, menuBoard->getContentSize().height * 3 / 4);
			noticeL->setTextColor(Color4B::BLACK);

			// 菜单：确定
			MenuItem* okM = MenuItemImage::create("image/OrangeNormal.png",
				"image/OrangePressed.png", CC_CALLBACK_0(ContinueMainLayer::removePlayer, this, n));
			okM->setPosition(0, -visibleSize.height / 5);

			const char* okC = ((__String*)ngContent->objectForKey("ok"))->getCString();
			Label* okL = Label::createWithSystemFont(okC, "Arial", 20);
			okM->addChild(okL);
			okL->setPosition(okM->getContentSize().width / 2, okM->getContentSize().height / 2);
			okL->setTextColor(Color4B::BLACK);

			noticeMenu = Menu::create(okM, NULL);
			this->addChild(noticeMenu);

			flag = true;
			break;
		}
		n++;
	}

	// 如果不存在淘汰玩家，正常轮换
	if (flag == false)
	{
		nowPlayerNumber++;
		if ((size_t)nowPlayerNumber > players.size())
		{
			nowPlayerNumber = 1;

			// 切换回合数
			rounds++;
			ostringstream stream2;
			stream2 << rounds;
			((Label*)this->getChildByName("roundsL"))->setString(stream2.str());
		}
	}
}

void ContinueMainLayer::removePlayer(int number)
{
	int n = 0;
	for (auto& p : players)
	{
		if (n == number)
		{
			p.roleSprite->removeFromParentAndCleanup(true);
			ostringstream stream1;
			stream1 << n + 1;
			this->removeChildByName("player" + stream1.str(), true);

			players.erase(players.begin() + number);

			break;
		}
		n++;
	}

	this->cleanMenu();

	// 判断是否只剩一名玩家，若是，则游戏结束
	bool flag = false;

	if (players.size() == 1)
	{
		// 放音乐
		SimpleAudioEngine::getInstance()->playEffect("music/win.mp3", false);
		SimpleAudioEngine::getInstance()->playBackgroundMusic("music/end.mp3", true);

		// 菜单面板图片
		menuBoard = Sprite::create("image/Popup.png");
		menuBoard->setPosition(visibleSize.width / 2, visibleSize.height / 2);
		this->addChild(menuBoard);

		// 字儿们
		auto ngContent = __Dictionary::createWithContentsOfFile("XML/NewGame.xml");

		// 提示框内容
		Label* noticeL;
		const char* belongLand = ((__String*)ngContent->objectForKey("belongLand"))->getCString();
		const char* win = ((__String*)ngContent->objectForKey("win"))->getCString();
		string str = "";
		string s = "";

		if (players[0].name == "player1")
		{
			const char* player = ((__String*)ngContent->objectForKey("player1"))->getCString();
			s = player + str + win;
		}
		else if (players[0].name == "player2")
		{
			const char* player = ((__String*)ngContent->objectForKey("player2"))->getCString();
			s = player + str + win;
		}
		else if (players[0].name == "player3")
		{
			const char* player = ((__String*)ngContent->objectForKey("player3"))->getCString();
			s = player + str + win;
		}
		else if (players[0].name == "player4")
		{
			const char* player = ((__String*)ngContent->objectForKey("player4"))->getCString();
			s = player + str + win;
		}

		noticeL = Label::createWithSystemFont(s, "Arial", 25);

		menuBoard->addChild(noticeL);
		noticeL->setPosition(menuBoard->getContentSize().width / 2, menuBoard->getContentSize().height * 3 / 4);
		noticeL->setTextColor(Color4B::BLACK);

		// 菜单：确定
		MenuItem* okM = MenuItemImage::create("image/OrangeNormal.png",
			"image/OrangePressed.png", CC_CALLBACK_0(ContinueMainLayer::endGame, this));
		okM->setPosition(0, -visibleSize.height / 5);

		const char* okC = ((__String*)ngContent->objectForKey("ok"))->getCString();
		Label* okL = Label::createWithSystemFont(okC, "Arial", 20);
		okM->addChild(okL);
		okL->setPosition(okM->getContentSize().width / 2, okM->getContentSize().height / 2);
		okL->setTextColor(Color4B::BLACK);

		noticeMenu = Menu::create(okM, NULL);
		this->addChild(noticeMenu);

		flag = true;
	}

	if (flag == false)
	{
		nowPlayerNumber++;
		if ((size_t)nowPlayerNumber > players.size())
		{
			nowPlayerNumber = 1;

			// 切换回合数
			rounds++;
			ostringstream stream2;
			stream2 << rounds;
			((Label*)this->getChildByName("roundsL"))->setString(stream2.str());
		}
	}
}

void ContinueMainLayer::cleanAndChange()
{
	this->cleanMenu();
	this->changePlayer();
}

void ContinueMainLayer::endGame()
{
	TitleScene* ts = TitleScene::create();
	Director::getInstance()->pushScene(ts);
}