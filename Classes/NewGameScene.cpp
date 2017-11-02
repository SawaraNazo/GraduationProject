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

	nowPlayerNumber = 1;
	
	this->createMap();
	this->createPlayer();
	this->createPlayerPro();


	return true;
}

void NewGameScene::createMap()
{
	tileMap = TMXTiledMap::create("tmx/smallMap.tmx");

	this->addChild(tileMap);

	background = tileMap->getLayer("Background");
	road = tileMap->getLayer("Road");
	land = tileMap->getLayer("Land");
}

void NewGameScene::createPlayer()
{
	playersNumber = 2;

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
		const char* pc = ((String*)ng->objectForKey(p.name))->getCString();
		Label* l = Label::createWithSystemFont(pc, "arial", 20);
		l->setColor(p.color);
		l->setPosition(visibleSize.width * 6 / 7,
			visibleSize.height - visibleSize.height * 3 * times / 16 - l->getContentSize().height / 2);
		this->addChild(l);

		times++;
	}

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

					this->schedule(CC_CALLBACK_1(NewGameScene::playerGo, this, p.name), 0.5, i - 1, 0, "playerGo");
					this->scheduleOnce(CC_CALLBACK_1(NewGameScene::removeDicePointS, this, p.name), i*0.5, "removeDicePointS");
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

void NewGameScene::playerGo(float dt, string playerName)
{
	for (auto& nowPlayer : players)
	{
		if (nowPlayer.name == playerName)
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
	}
}

void NewGameScene::removeDicePointS(float dt, string playerName)
{
	this->removeChildByName("dicePoint");
	//this->checkLand();

	for (auto& nowPlayer : players)
	{
		if (nowPlayer.name == playerName)
		{
			nowPlayer.isGoing = false;
		}
	}
	
	nowPlayerNumber++;
	if (nowPlayerNumber > players.size())
	{
		nowPlayerNumber = 1;
	}
}

void NewGameScene::checkLand()
{
	// 1.先检查所在地块是否有土地
	// 2.检查土地所属

	switch (nowFace)
	{
	case faceForward::right:
		nowLand = Vec2(playerPoint.x, playerPoint.y + 1);
		break;
	case faceForward::down:
		nowLand = Vec2(playerPoint.x - 1, playerPoint.y);
		break;
	case faceForward::left:
		nowLand = Vec2(playerPoint.x, playerPoint.y - 1);
		break;
	case faceForward::up:
		nowLand = Vec2(playerPoint.x + 1, playerPoint.y);
		break;
	}

	sLand = land->getTileAt(nowLand);
	gLand = land->getTileGIDAt(nowLand);

	if (sLand)
	{
		// 菜单面板图片
		menuBoard = Sprite::create("image/Popup.png");
		menuBoard->setPosition(visibleSize.width / 2, visibleSize.height / 2);
		this->addChild(menuBoard);

		// 字儿们
		auto ngContent = Dictionary::createWithContentsOfFile("XML/NewGame.xml");

		// 菜单 ：是
		MenuItem* yesM = MenuItemImage::create("image/GreenNormal.png",
			"image/GreenPressed.png", CC_CALLBACK_0(NewGameScene::menuYes, this));
		yesM->setPosition(-visibleSize.width / 8, -visibleSize.height / 5);

		const char* yesC = ((String*)ngContent->objectForKey("yes"))->getCString();
		Label* yesL = Label::createWithSystemFont(yesC, "arial", 20);
		yesM->addChild(yesL);
		yesL->setPosition(yesM->getContentSize().width / 2, yesM->getContentSize().height / 2);
		yesL->setTextColor(Color4B::BLACK);

		// 菜单 ：否
		MenuItem* noM = MenuItemImage::create("image/RedNormal.png",
			"image/RedPressed.png", CC_CALLBACK_0(NewGameScene::menuNo, this));
		noM->setPosition(visibleSize.width / 8, -visibleSize.height / 5);

		const char* noC = ((String*)ngContent->objectForKey("no"))->getCString();
		Label* noL = Label::createWithSystemFont(noC, "arial", 20);
		noM->addChild(noL);
		noL->setPosition(noM->getContentSize().width / 2, noM->getContentSize().height / 2);
		noL->setTextColor(Color4B::BLACK);

		Label* noticeL;

		if (gLand == empty_land)
		{
			const char* upgradeLand0 = ((String*)ngContent->objectForKey("upgradeLand0"))->getCString();
			noticeL = Label::createWithSystemFont(upgradeLand0, "arial", 30);
		}
		else if (gLand == level1_land)
		{
			const char* upgradeLand1 = ((String*)ngContent->objectForKey("upgradeLand1"))->getCString();
			noticeL = Label::createWithSystemFont(upgradeLand1, "arial", 30);
		}
		else if (gLand == level2_land)
		{
			const char* upgradeLand2 = ((String*)ngContent->objectForKey("upgradeLand2"))->getCString();
			noticeL = Label::createWithSystemFont(upgradeLand2, "arial", 30);
		}

		menuBoard->addChild(noticeL);
		noticeL->setPosition(menuBoard->getContentSize().width / 2, menuBoard->getContentSize().height * 3 / 4);
		noticeL->setTextColor(Color4B::BLACK);

		noticeMenu = Menu::create(yesM, noM, NULL);
		this->addChild(noticeMenu);
	}
}

void NewGameScene::menuYes()
{
	if (sLand)
	{
		if (gLand == empty_land)
		{
			land->setTileGID(level1_land, nowLand);
			//land->getTileAt(nowLand)->setColor(player1_color);
		}
		else if (gLand == level1_land)
		{
			land->setTileGID(level2_land, nowLand);
			//land->getTileAt(nowLand)->setColor(player1_color);
		}
		else if (gLand == level2_land)
		{
			land->setTileGID(level3_land, nowLand);
			//land->getTileAt(nowLand)->setColor(player1_color);
		}
	}

	noticeMenu->removeFromParentAndCleanup(true);
	menuBoard->removeFromParentAndCleanup(true);
}

void NewGameScene::menuNo()
{
	noticeMenu->removeFromParentAndCleanup(true);
	menuBoard->removeFromParentAndCleanup(true);
}
