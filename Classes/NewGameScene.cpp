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

	visibleSize = Director::getInstance()->getVisibleSize();
	origin = Director::getInstance()->getVisibleOrigin();

	// 初始化到第一位玩家
	nowPlayerNumber = 1;

	// 预设置开始金钱为10000元
	startMoney = 10000;

	// 预设置开始的土地等级数
	landLevelNumber = 8;

	// 预设置玩家数量
	playersNumber = 2;

	// 回合数
	rounds = 1;

	this->createMap();
	this->createPlayer();
	this->createPlayerPro();
	
	return true;
}

void NewGameScene::createMap()
{
	tileMap = TMXTiledMap::create("tmx/smallMap.tmx");

	this->addChild(tileMap);

	road = tileMap->getLayer("Road");
	
	for (int i = 1; i <= landLevelNumber; i++)
	{
		string s = "Land" + to_string(i);
		TMXLayer* tl = tileMap->getLayer(s);
		lands.push_back(tl);
	}

	// 总回合数
	auto ngContent = Dictionary::createWithContentsOfFile("XML/NewGame.xml");
	const char* roundsC = ((String*)ngContent->objectForKey("rounds"))->getCString();
	Label* roundsL1 = Label::createWithSystemFont(roundsC, "arial", 24);
	roundsL1->setPosition(visibleSize.width * 3 / 8, visibleSize.height - roundsL1->getContentSize().height);
	this->addChild(roundsL1,10);
	
	Label* roundsL2 = Label::createWithSystemFont(to_string(rounds), "arial", 18);
	roundsL2->setPosition(visibleSize.width * 3 / 8, roundsL1->getPosition().y - roundsL2->getContentSize().height);
	this->addChild(roundsL2, 10, "roundsL");;
}

void NewGameScene::createPlayer()
{
	objectGroup = tileMap->getObjectGroup("Object");
	auto player = objectGroup->getObject("player");
	float px = player["x"].asFloat();
	float py = player["y"].asFloat();

	for (int i = 1; i <= playersNumber; i++)
	{
		Player p;
		p.name = "player" + to_string(i);
		p.isGoing = false;
		p.faceTo = faceForward::right;
		p.rolePosition = Vec2(4, 4);
		p.roleSprite = Sprite::create("image/" + p.name + ".png");
		p.money = startMoney;
		p.state = stateType::normal;
		p.spritePosition = Vec2(px + i * 3, py);
		p.roleSprite->setPosition(p.spritePosition);

		if (i == 1)
		{
			p.color = Color3B::RED;
		}
		else if (i == 2)
		{
			p.color = Color3B::BLUE;
		}
		else if (i == 3)
		{
			p.color = Color3B::YELLOW;
		}
		else if (i == 4)
		{
			p.color = Color3B::GREEN;
		}

		tileMap->addChild(p.roleSprite, tileMap->getChildrenCount(), "player" + to_string(i));

		players.push_back(p);
	}
}

void NewGameScene::createPlayerPro()
{
	auto ng = Dictionary::createWithContentsOfFile("XML/NewGame.xml");

	int times = 0;

	for (auto p : players)
	{
		// 玩家姓名
		const char* pc = ((String*)ng->objectForKey(p.name))->getCString();
		Label* l1 = Label::createWithSystemFont(pc, "arial", 20);
		l1->setColor(p.color);
		l1->setPosition(visibleSize.width * 6 / 7,
			visibleSize.height - visibleSize.height * 3 * times / 16 - l1->getContentSize().height / 2);
		this->addChild(l1, 2, pc);

		// 玩家金钱
		const char* rmb = ((String*)ng->objectForKey("rmb"))->getCString();
		string m = rmb + to_string(p.money);
		string blank = " ";
		Label* l2 = Label::createWithSystemFont(m, "arial", 18);
		l2->setPosition(visibleSize.width * 6 / 7, l1->getPosition().y - 2 * l1->getContentSize().height);
		this->addChild(l2, 2, p.name + blank + "money");

		times++;
	}

	// 骰子按钮
	Button* diceButton = Button::create("image/diceButtonNormal.png", "image/diceButtonPressed.png");
	diceButton->setPosition(Vec2(visibleSize.width - diceButton->getContentSize().width / 2,
		diceButton->getContentSize().height / 2));
	diceButton->addTouchEventListener(CC_CALLBACK_2(NewGameScene::diceEvent, this));
	diceButton->setPressedActionEnabled(true);
	this->addChild(diceButton, 2);
}

void NewGameScene::diceEvent(Ref* pSender, Widget::TouchEventType type)
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

					this->schedule(schedule_selector(NewGameScene::playerGo), 0.5, i - 1, 0);
					this->scheduleOnce(schedule_selector(NewGameScene::checkLand), i*0.5);
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

void NewGameScene::playerGo(float dt)
{
	int n = 1;

	for (auto& nowPlayer : players)
	{
		if (n == nowPlayerNumber)
		{
			if (road->getTileAt(nowPlayer.rolePosition))
			{
				if (nowPlayer.faceTo == faceForward::right)
				{
					if (road->getTileAt(Vec2(nowPlayer.rolePosition.x + 1, nowPlayer.rolePosition.y)))
					{
						nowPlayer.rolePosition.x++;
						nowPlayer.spritePosition.x += 30;
					}
					else if (road->getTileAt(Vec2(nowPlayer.rolePosition.x, nowPlayer.rolePosition.y - 1)))
					{
						nowPlayer.rolePosition.y--;
						nowPlayer.spritePosition.y += 30;
						nowPlayer.faceTo = faceForward::up;
					}
					else if (road->getTileAt(Vec2(nowPlayer.rolePosition.x, nowPlayer.rolePosition.y + 1)))
					{
						nowPlayer.rolePosition.y++;
						nowPlayer.spritePosition.y -= 30;
						nowPlayer.faceTo = faceForward::down;
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
					}
					else if (road->getTileAt(Vec2(nowPlayer.rolePosition.x - 1, nowPlayer.rolePosition.y)))
					{
						nowPlayer.rolePosition.x--;
						nowPlayer.spritePosition.x -= 30;
						nowPlayer.faceTo = faceForward::left;
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
					}
					else if (road->getTileAt(Vec2(nowPlayer.rolePosition.x, nowPlayer.rolePosition.y - 1)))
					{
						nowPlayer.rolePosition.y--;
						nowPlayer.spritePosition.y += 30;
						nowPlayer.faceTo = faceForward::up;
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
					}
					else if (road->getTileAt(Vec2(nowPlayer.rolePosition.x - 1, nowPlayer.rolePosition.y)))
					{
						nowPlayer.rolePosition.x--;
						nowPlayer.spritePosition.x -= 30;
						nowPlayer.faceTo = faceForward::left;
					}
				}
			}

			nowPlayer.roleSprite->setPosition(nowPlayer.spritePosition);

			break;
		}
		n++;
	}
}

void NewGameScene::checkRoad(float dt)
{
	// 移除骰子图片
	this->removeChildByName("dicePoint");

	int n = 1;

	for (auto& p : players)
	{
		if (n == nowPlayerNumber)
		{
			if (road->getTileGIDAt(p.rolePosition) == normal_road_GID)
			{

			}
			else if (road->getTileGIDAt(p.rolePosition) == prisonEnterance_road_GID)
			{

			}
			else if (road->getTileGIDAt(p.rolePosition) == prison_road_GID)
			{

			}
			else if (road->getTileGIDAt(p.rolePosition) == parkinglot_road_GID)
			{

			}
			else if (road->getTileGIDAt(p.rolePosition) == emergency_road_GID)
			{

			}
			else if (road->getTileGIDAt(p.rolePosition) == tax_road_GID)
			{

			}

			break;
		}

		n++;
	}
}

void NewGameScene::checkLand(float dt)
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
				// 设置角色可以移动
				p.isGoing = false;

				// 切换操作玩家
				nowPlayerNumber++;
				if (nowPlayerNumber > players.size())
				{
					nowPlayerNumber = 1;

					rounds++;
					((Label*)this->getChildByName("roundsL"))->setString(to_string(rounds));
				}
			}

			break;
		}

		n++;
	}
}

void NewGameScene::emptyLand()
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
	auto ngContent = Dictionary::createWithContentsOfFile("XML/NewGame.xml");

	// 菜单 ：是
	MenuItem* yesM = MenuItemImage::create("image/GreenNormal.png",
		"image/GreenPressed.png", CC_CALLBACK_0(NewGameScene::emptyMenuYes, this));
	yesM->setPosition(-visibleSize.width / 8, -visibleSize.height / 5);

	const char* yesC = ((String*)ngContent->objectForKey("yes"))->getCString();
	Label* yesL = Label::createWithSystemFont(yesC, "arial", 20);
	yesM->addChild(yesL);
	yesL->setPosition(yesM->getContentSize().width / 2, yesM->getContentSize().height / 2);
	yesL->setTextColor(Color4B::BLACK);

	// 菜单 ：否
	MenuItem* noM = MenuItemImage::create("image/RedNormal.png",
		"image/RedPressed.png", CC_CALLBACK_0(NewGameScene::emptyMenuNo, this));
	noM->setPosition(visibleSize.width / 8, -visibleSize.height / 5);

	const char* noC = ((String*)ngContent->objectForKey("no"))->getCString();
	Label* noL = Label::createWithSystemFont(noC, "arial", 20);
	noM->addChild(noL);
	noL->setPosition(noM->getContentSize().width / 2, noM->getContentSize().height / 2);
	noL->setTextColor(Color4B::BLACK);
	

	// 菜单主要内容
	Label* noticeL;

	const char* nc = ((String*)ngContent->objectForKey(name))->getCString();
	string blank = " ";
	const char* comma = ((String*)ngContent->objectForKey("comma"))->getCString();
	const char* upgradeLand0 = ((String*)ngContent->objectForKey("upgradeLand0"))->getCString();
	const char* payToUpgrade = ((String*)ngContent->objectForKey("payToUpgrade"))->getCString();

	// 所在空白地块的价格
	int emptyBuildCost;

	for (auto l : lands)
	{
		if (l->getTileAt(nowLand))
		{
			emptyBuildCost = l->getProperty("emptyBuildCost").asInt();
		}
	}

	const char* yuan = ((String*)ngContent->objectForKey("yuan"))->getCString();
	string s = nc + blank + comma + upgradeLand0 + "\n" +
		payToUpgrade + blank + to_string(emptyBuildCost) + yuan;
	noticeL = Label::createWithSystemFont(s, "arial", 30);

	menuBoard->addChild(noticeL);
	noticeL->setPosition(menuBoard->getContentSize().width / 2, menuBoard->getContentSize().height * 3 / 4);
	noticeL->setTextColor(Color4B::BLACK);

	noticeMenu = Menu::create(yesM, noM, NULL);
	this->addChild(noticeMenu);
}

void NewGameScene::emptyMenuYes()
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

					l->getTileAt(nowLand)->setColor(p.color);
					p.money -= emptyBuildCost;

					break;
				}
			}
			
			auto ngContent = Dictionary::createWithContentsOfFile("XML/NewGame.xml");
			const char* rmb = ((String*)ngContent->objectForKey("rmb"))->getCString();
			string blank = " ";
			string s = rmb + blank + to_string(p.money);

			((Label*)this->getChildByName(p.name + blank + "money"))->setString(s);

			// 设置角色可以移动
			p.isGoing = false;

			break;
		}

		n++;
	}

	noticeMenu->removeFromParentAndCleanup(true);
	menuBoard->removeFromParentAndCleanup(true);

	this->changePlayer();
}

void NewGameScene::emptyMenuNo()
{
	noticeMenu->removeFromParentAndCleanup(true);
	menuBoard->removeFromParentAndCleanup(true);

	int n = 1;

	for (auto& p : players)
	{
		if (n == nowPlayerNumber)
		{
			// 设置角色可以移动
			p.isGoing = false;
			break;
		}

		n++;
	}

	this->changePlayer();
}

void NewGameScene::myLand()
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
	auto ngContent = Dictionary::createWithContentsOfFile("XML/NewGame.xml");

	// 菜单 ：是
	MenuItem* yesM = MenuItemImage::create("image/GreenNormal.png",
		"image/GreenPressed.png", CC_CALLBACK_0(NewGameScene::myMenuYes, this));
	yesM->setPosition(-visibleSize.width / 8, -visibleSize.height / 5);

	const char* yesC = ((String*)ngContent->objectForKey("yes"))->getCString();
	Label* yesL = Label::createWithSystemFont(yesC, "arial", 20);
	yesM->addChild(yesL);
	yesL->setPosition(yesM->getContentSize().width / 2, yesM->getContentSize().height / 2);
	yesL->setTextColor(Color4B::BLACK);

	// 菜单 ：否
	MenuItem* noM = MenuItemImage::create("image/RedNormal.png",
		"image/RedPressed.png", CC_CALLBACK_0(NewGameScene::myMenuNo, this));
	noM->setPosition(visibleSize.width / 8, -visibleSize.height / 5);

	const char* noC = ((String*)ngContent->objectForKey("no"))->getCString();
	Label* noL = Label::createWithSystemFont(noC, "arial", 20);
	noM->addChild(noL);
	noL->setPosition(noM->getContentSize().width / 2, noM->getContentSize().height / 2);
	noL->setTextColor(Color4B::BLACK);

	// 提示栏内容
	Label* noticeL;
	const char* nc = ((String*)ngContent->objectForKey(name))->getCString();
	string blank = " ";
	const char* comma = ((String*)ngContent->objectForKey("comma"))->getCString();
	const char* upgradeLand;
	int price;

	if (gLand == empty_land_GID)
	{
		upgradeLand = ((String*)ngContent->objectForKey("upgradeLand1"))->getCString();

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
		upgradeLand = ((String*)ngContent->objectForKey("upgradeLand2"))->getCString();

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
		upgradeLand = ((String*)ngContent->objectForKey("upgradeLand3"))->getCString();

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

	const char* payToUpgrade = ((String*)ngContent->objectForKey("payToUpgrade"))->getCString();
	const char* yuan = ((String*)ngContent->objectForKey("yuan"))->getCString();

	string s = nc + blank + comma + upgradeLand + "\n" +
		payToUpgrade + blank + to_string(price) + yuan;
	noticeL = Label::createWithSystemFont(s, "arial", 30);

	menuBoard->addChild(noticeL);
	noticeL->setPosition(menuBoard->getContentSize().width / 2, menuBoard->getContentSize().height * 3 / 4);
	noticeL->setTextColor(Color4B::BLACK);

	noticeMenu = Menu::create(yesM, noM, NULL);
	this->addChild(noticeMenu);
}

void NewGameScene::myMenuYes()
{
	int n = 1;

	for (auto& p : players)
	{
		if (n == nowPlayerNumber)
		{
			int buildCost;

			for (auto l : lands)
			{
				if (l->getTileAt(nowLand))
				{
					if (gLand == empty_land_GID)
					{
						buildCost = l->getProperty("level1BuildCost").asInt();
						l->setTileGID(level1_land_GID, nowLand);
					}
					else if (gLand == level1_land_GID)
					{
						buildCost = l->getProperty("level2BuildCost").asInt();
						l->setTileGID(level2_land_GID, nowLand);
					}
					else if (gLand == level2_land_GID)
					{
						buildCost = l->getProperty("level3BuildCost").asInt();
						l->setTileGID(level3_land_GID, nowLand);
					}
					else if (gLand == level3_land_GID)
					{

					}

					l->getTileAt(nowLand)->setColor(p.color);
					p.money -= buildCost;

					break;
				}
			}

			auto ngContent = Dictionary::createWithContentsOfFile("XML/NewGame.xml");
			const char* rmb = ((String*)ngContent->objectForKey("rmb"))->getCString();
			string blank = " ";
			string s = rmb + blank + to_string(p.money);

			((Label*)this->getChildByName(p.name + blank + "money"))->setString(s);

			// 设置角色可以移动
			p.isGoing = false;

			break;
		}

		n++;
	}

	noticeMenu->removeFromParentAndCleanup(true);
	menuBoard->removeFromParentAndCleanup(true);

	this->changePlayer();
}

void NewGameScene::myMenuNo()
{
	noticeMenu->removeFromParentAndCleanup(true);
	menuBoard->removeFromParentAndCleanup(true);

	int n = 1;

	for (auto& p : players)
	{
		if (n == nowPlayerNumber)
		{
			// 设置角色可以移动
			p.isGoing = false;
			break;
		}

		n++;
	}

	this->changePlayer();
}

void NewGameScene::otherLand()
{
	string payName, earnName;
	int n = 1;

	for (auto p : players)
	{
		if (n == nowPlayerNumber)
		{
			payName = p.name;
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
	auto ngContent = Dictionary::createWithContentsOfFile("XML/NewGame.xml");

	// 菜单：确定
	MenuItem* okM = MenuItemImage::create("image/OrangeNormal.png",
		"image/OrangePressed.png", CC_CALLBACK_0(NewGameScene::otherMenuClose, this, payName, earnName));
	okM->setPosition(0, -visibleSize.height / 5);

	const char* okC = ((String*)ngContent->objectForKey("ok"))->getCString();
	Label* okL = Label::createWithSystemFont(okC, "arial", 20);
	okM->addChild(okL);
	okL->setPosition(okM->getContentSize().width / 2, okM->getContentSize().height / 2);
	okL->setTextColor(Color4B::BLACK);

	// 提示框内容
	Label* noticeL;
	const char* belongLand = ((String*)ngContent->objectForKey("belongLand"))->getCString();
	string blank = " ";
	const char* earnC = ((String*)ngContent->objectForKey(earnName))->getCString();
	const char* comma = ((String*)ngContent->objectForKey("comma"))->getCString();
	const char* payC = ((String*)ngContent->objectForKey(payName))->getCString();
	const char* payLand = ((String*)ngContent->objectForKey("payLand"))->getCString();

	int price;

	for (auto l : lands)
	{
		if (l->getTileAt(nowLand))
		{
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
		}
	}

	const char* yuan = ((String*)ngContent->objectForKey("yuan"))->getCString();
	string s = belongLand + blank + earnC + comma + payC + "\n" +
		payLand + blank + to_string(price) + yuan;
	noticeL = Label::createWithSystemFont(s, "arial", 30);

	menuBoard->addChild(noticeL);
	noticeL->setPosition(menuBoard->getContentSize().width / 2, menuBoard->getContentSize().height * 3 / 4);
	noticeL->setTextColor(Color4B::BLACK);

	noticeMenu = Menu::create(okM, NULL);
	this->addChild(noticeMenu);
}

void NewGameScene::otherMenuClose(string payName, string earnName)
{
	noticeMenu->removeFromParentAndCleanup(true);
	menuBoard->removeFromParentAndCleanup(true);
	
	// 找出对应的两个玩家，进行金（P）钱（Y）交易
	for (auto& p : players)
	{
		for (auto& e : players)
		{
			if (p.name == payName && e.name == earnName)
			{
				int price;

				for (auto l : lands)
				{
					if (l->getTileAt(nowLand))
					{
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

						break;
					}
				}

				p.money -= price;
				e.money += price;

				auto ngContent = Dictionary::createWithContentsOfFile("XML/NewGame.xml");
				const char* rmb = ((String*)ngContent->objectForKey("rmb"))->getCString();
				string blank = " ";
				string sp = rmb + blank + to_string(p.money);
				string se = rmb + blank + to_string(e.money);

				((Label*)this->getChildByName(payName + blank + "money"))->setString(sp);
				((Label*)this->getChildByName(earnName + blank + "money"))->setString(se);

				// 设置角色可以移动
				p.isGoing = false;
				break;
			}
		}
	}

	this->changePlayer();
}

void NewGameScene::changePlayer()
{
	nowPlayerNumber++;
	if (nowPlayerNumber > players.size())
	{
		nowPlayerNumber = 1;

		// 切换回合数
		rounds++;
		((Label*)this->getChildByName("roundsL"))->setString(to_string(rounds));
	}
}