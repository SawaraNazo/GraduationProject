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

	playerPoint = Vec2(4, 4);

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
	objectGroup = tileMap->getObjectGroup("Players");

	auto player1 = objectGroup->getObject("Player1");
	Sprite* p1 = Sprite::create("image/Player.png");
	float p1x = player1["x"].asFloat();
	float p1y = player1["y"].asFloat();
	p1->setPosition(p1x, p1y);
	tileMap->addChild(p1, tileMap->getChildrenCount(), "player1");

}

void NewGameScene::createPlayerPro()
{
	auto ng = Dictionary::createWithContentsOfFile("XML/NewGame.xml");

	const char* player1 = ((String*)ng->objectForKey("player1"))->getCString();
	Label* l1 = Label::createWithSystemFont(player1, "arial", 20);
	l1->setPosition(visibleSize.width * 6 / 7, visibleSize.height-l1->getContentSize().height);
	this->addChild(l1);

	const char* player2 = ((String*)ng->objectForKey("player2"))->getCString();
	Label* l2 = Label::createWithSystemFont(player2, "arial", 20);
	l2->setPosition(visibleSize.width * 6 / 7, l1->getPosition().y - visibleSize.height / 5);
	this->addChild(l2);

	Button* diceButton = Button::create("image/diceButtonNormal.png", "image/diceButtonPressed.png");
	diceButton->setPosition(Vec2(visibleSize.width - diceButton->getContentSize().width / 2, diceButton->getContentSize().height / 2));
	diceButton->addTouchEventListener(CC_CALLBACK_2(NewGameScene::diceEvent, this));
	diceButton->setPressedActionEnabled(true);
	this->addChild(diceButton, 2);
}

void NewGameScene::playerGo(float dt)
{
	auto player1 = tileMap->getChildByName("player1");
	auto player1Position = player1->getPosition();
	
	if (road->getTileAt(playerPoint))
	{
		if (nowFace == faceForward::right)
		{
			if (road->getTileAt(Vec2(playerPoint.x + 1, playerPoint.y)))
			{
				playerPoint.x++;
				player1Position.x += 30;
			}
			else if (road->getTileAt(Vec2(playerPoint.x, playerPoint.y - 1)))
			{
				playerPoint.y--;
				player1Position.y += 30;
				nowFace = faceForward::up;
			}
			else if (road->getTileAt(Vec2(playerPoint.x, playerPoint.y + 1)))
			{
				playerPoint.y++;
				player1Position.y -= 30;
				nowFace = faceForward::down;
			}
		}
		else if (nowFace == faceForward::down)
		{
			if (road->getTileAt(Vec2(playerPoint.x, playerPoint.y + 1)))
			{
				playerPoint.y++;
				player1Position.y -= 30;
			}
			else if (road->getTileAt(Vec2(playerPoint.x + 1, playerPoint.y)))
			{
				playerPoint.x++;
				player1Position.x += 30;
				nowFace = faceForward::right;
			}
			else if (road->getTileAt(Vec2(playerPoint.x - 1, playerPoint.y)))
			{
				playerPoint.x--;
				player1Position.x -= 30;
				nowFace = faceForward::left;
			}
		}
		else if (nowFace == faceForward::left)
		{
			if (road->getTileAt(Vec2(playerPoint.x - 1, playerPoint.y)))
			{
				playerPoint.x--;
				player1Position.x -= 30;
			}
			else if (road->getTileAt(Vec2(playerPoint.x, playerPoint.y + 1)))
			{
				playerPoint.y++;
				player1Position.y -= 30;
				nowFace = faceForward::down;
			}
			else if (road->getTileAt(Vec2(playerPoint.x, playerPoint.y - 1)))
			{
				playerPoint.y--;
				player1Position.y += 30;
				nowFace = faceForward::up;
			}
		}
		else if (nowFace == faceForward::up)
		{
			if (road->getTileAt(Vec2(playerPoint.x, playerPoint.y - 1)))
			{
				playerPoint.y--;
				player1Position.y += 30;
			}
			else if (road->getTileAt(Vec2(playerPoint.x + 1, playerPoint.y)))
			{
				playerPoint.x++;
				player1Position.x += 30;
				nowFace = faceForward::right;
			}
			else if (road->getTileAt(Vec2(playerPoint.x - 1, playerPoint.y)))
			{
				playerPoint.x--;
				player1Position.x -= 30;
				nowFace = faceForward::left;
			}
		}
		player1->setPosition(player1Position);
	}
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
		if (isGoing == false)
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

			isGoing = true;

			this->schedule(schedule_selector(NewGameScene::playerGo), 0.5, i - 1, 0);
			this->scheduleOnce(schedule_selector(NewGameScene::removeDicePointS), i*0.5);
		}
		break;
	case Widget::TouchEventType::CANCELED:
		break;
	default:
		break;
	}
}

void NewGameScene::removeDicePointS(float dt)
{
	this->removeChildByName("dicePoint");
	this->checkLand();
	isGoing = false;
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
			land->getTileAt(nowLand)->setColor(player1_color);
		}
		else if (gLand == level1_land)
		{
			land->setTileGID(level2_land, nowLand);
			land->getTileAt(nowLand)->setColor(player1_color);
		}
		else if (gLand == level2_land)
		{
			land->setTileGID(level3_land, nowLand);
			land->getTileAt(nowLand)->setColor(player1_color);
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