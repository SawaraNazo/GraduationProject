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

	// ��ʼ������һλ���
	nowPlayerNumber = 1;

	// Ԥ���ÿ�ʼ��ǮΪ10000Ԫ
	startMoney = 600;

	// ��ʼ��ͻ���¼�����
	emEventNumber = 5;

	// Ԥ���ÿ�ʼ�����صȼ���
	landLevelNumber = 8;

	// Ԥ�����������
	playersNumber = 2;

	// �غ���
	rounds = 1;

	this->createMap();
	this->createPlayer();
	this->createPlayerPro();
	
	return true;
}

// ��ʼ����غ���

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

	// �ܻغ���
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
		p.roleSprite = Sprite::create("image/" + p.name + "_right.png");
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
		// �������
		const char* pc = ((String*)ng->objectForKey(p.name))->getCString();
		Label* l1 = Label::createWithSystemFont(pc, "arial", 20);
		l1->setColor(p.color);
		l1->setPosition(visibleSize.width * 6 / 7,
			visibleSize.height - visibleSize.height * 3 * times / 16 - l1->getContentSize().height / 2);
		this->addChild(l1, 2, pc);

		// ��ҽ�Ǯ
		const char* rmb = ((String*)ng->objectForKey("rmb"))->getCString();
		string m = rmb + to_string(p.money);
		string blank = " ";
		Label* l2 = Label::createWithSystemFont(m, "arial", 18);
		l2->setPosition(visibleSize.width * 6 / 7, l1->getPosition().y - 2 * l1->getContentSize().height);
		this->addChild(l2, 2, p.name + blank + "money");

		times++;
	}

	// ���Ӱ�ť
	Button* diceButton = Button::create("image/diceButtonNormal.png", "image/diceButtonPressed.png");
	diceButton->setPosition(Vec2(visibleSize.width - diceButton->getContentSize().width / 2,
		diceButton->getContentSize().height / 2));
	diceButton->addTouchEventListener(CC_CALLBACK_2(NewGameScene::diceEvent, this));
	diceButton->setPressedActionEnabled(true);
	this->addChild(diceButton, 2);
}

// ������غ���

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

						this->schedule(schedule_selector(NewGameScene::playerGo), 0.5, i - 1, 0);
						this->scheduleOnce(schedule_selector(NewGameScene::checkRoad), i*0.5);
					}
					else
					{
						// (��-��-)
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
						nowPlayer.roleSprite->setTexture("image/" + nowPlayer.name + "_up.png");
					}
					else if (road->getTileAt(Vec2(nowPlayer.rolePosition.x, nowPlayer.rolePosition.y + 1)))
					{
						nowPlayer.rolePosition.y++;
						nowPlayer.spritePosition.y -= 30;
						nowPlayer.faceTo = faceForward::down;
						nowPlayer.roleSprite->setTexture("image/" + nowPlayer.name + "_down.png");
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
						nowPlayer.roleSprite->setTexture("image/" + nowPlayer.name + "_right.png");
					}
					else if (road->getTileAt(Vec2(nowPlayer.rolePosition.x - 1, nowPlayer.rolePosition.y)))
					{
						nowPlayer.rolePosition.x--;
						nowPlayer.spritePosition.x -= 30;
						nowPlayer.faceTo = faceForward::left;
						nowPlayer.roleSprite->setTexture("image/" + nowPlayer.name + "_left.png");
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
						nowPlayer.roleSprite->setTexture("image/" + nowPlayer.name + "_down.png");
					}
					else if (road->getTileAt(Vec2(nowPlayer.rolePosition.x, nowPlayer.rolePosition.y - 1)))
					{
						nowPlayer.rolePosition.y--;
						nowPlayer.spritePosition.y += 30;
						nowPlayer.faceTo = faceForward::up;
						nowPlayer.roleSprite->setTexture("image/" + nowPlayer.name + "_up.png");
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
						nowPlayer.roleSprite->setTexture("image/" + nowPlayer.name + "_right.png");
					}
					else if (road->getTileAt(Vec2(nowPlayer.rolePosition.x - 1, nowPlayer.rolePosition.y)))
					{
						nowPlayer.rolePosition.x--;
						nowPlayer.spritePosition.x -= 30;
						nowPlayer.faceTo = faceForward::left;
						nowPlayer.roleSprite->setTexture("image/" + nowPlayer.name + "_left.png");
					}
				}
			}

			nowPlayer.roleSprite->setPosition(nowPlayer.spritePosition);

			break;
		}
		n++;
	}
}

bool NewGameScene::checkState()
{
	// �ֶ���
	auto ngContent = Dictionary::createWithContentsOfFile("XML/NewGame.xml");

	int n = 1;

	for (auto& p : players)
	{
		if (n == nowPlayerNumber)
		{
			if (p.state == stateType::normal)
			{
				// ��������
				return true;
			}
			else if (p.state == stateType::parking && p.stayRound > 0)
			{
				// ��������ͣ����
				p.isGoing = true;

				p.stayRound--;

				// �˵����ͼƬ
				menuBoard = Sprite::create("image/Popup.png");
				menuBoard->setPosition(visibleSize.width / 2, visibleSize.height / 2);
				this->addChild(menuBoard);
				
				// �˵���ȷ��
				MenuItem* okM = MenuItemImage::create("image/OrangeNormal.png",
					"image/OrangePressed.png", CC_CALLBACK_0(NewGameScene::cleanAndChange, this));
				okM->setPosition(0, -visibleSize.height / 5);

				const char* okC = ((String*)ngContent->objectForKey("ok"))->getCString();
				Label* okL = Label::createWithSystemFont(okC, "arial", 20);
				okM->addChild(okL);
				okL->setPosition(okM->getContentSize().width / 2, okM->getContentSize().height / 2);
				okL->setTextColor(Color4B::BLACK);

				// �˵���Ҫ����
				const char* roundsToStayInParkinglot1 = ((String*)ngContent->objectForKey("roundsToStayInParkinglot1"))->getCString();
				const char* roundsToStayInParkinglot2 = ((String*)ngContent->objectForKey("roundsToStayInParkinglot2"))->getCString();
				string blank = " ";
				string s = roundsToStayInParkinglot1 + blank + "\n" + roundsToStayInParkinglot2 + to_string(p.stayRound);
				Label* noticeL = Label::createWithSystemFont(s, "arial", 30);
				
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
				// �������ڼ���
				p.isGoing = true;
				
				p.stayRound--;
				
				// �˵����ͼƬ
				menuBoard = Sprite::create("image/Popup.png");
				menuBoard->setPosition(visibleSize.width / 2, visibleSize.height / 2);
				this->addChild(menuBoard);

				// �˵���ȷ��
				MenuItem* okM = MenuItemImage::create("image/OrangeNormal.png",
					"image/OrangePressed.png", CC_CALLBACK_0(NewGameScene::cleanAndChange, this));
				okM->setPosition(0, -visibleSize.height / 5);

				const char* okC = ((String*)ngContent->objectForKey("ok"))->getCString();
				Label* okL = Label::createWithSystemFont(okC, "arial", 20);
				okM->addChild(okL);
				okL->setPosition(okM->getContentSize().width / 2, okM->getContentSize().height / 2);
				okL->setTextColor(Color4B::BLACK);

				// �˵���Ҫ����
				const char* roundsToStayInPrison1 = ((String*)ngContent->objectForKey("roundsToStayInPrison1"))->getCString();
				const char* roundsToStayInPrison2 = ((String*)ngContent->objectForKey("roundsToStayInPrison2"))->getCString();
				string blank = " ";
				string s = roundsToStayInPrison1 + blank + "\n" + roundsToStayInPrison2 + to_string(p.stayRound);
				Label* noticeL = Label::createWithSystemFont(s, "arial", 30);

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
				// ͣ����ʱ�䵽������

				p.state = stateType::normal;

				// �˵����ͼƬ
				menuBoard = Sprite::create("image/Popup.png");
				menuBoard->setPosition(visibleSize.width / 2, visibleSize.height / 2);
				this->addChild(menuBoard);

				// �˵���ȷ��
				MenuItem* okM = MenuItemImage::create("image/OrangeNormal.png",
					"image/OrangePressed.png", CC_CALLBACK_0(NewGameScene::cleanMenu, this));
				okM->setPosition(0, -visibleSize.height / 5);

				const char* okC = ((String*)ngContent->objectForKey("ok"))->getCString();
				Label* okL = Label::createWithSystemFont(okC, "arial", 20);
				okM->addChild(okL);
				okL->setPosition(okM->getContentSize().width / 2, okM->getContentSize().height / 2);
				okL->setTextColor(Color4B::BLACK);

				// �˵���Ҫ����
				const char* outOfParkinglot = ((String*)ngContent->objectForKey("outOfParkinglot"))->getCString();
				Label* noticeL = Label::createWithSystemFont(outOfParkinglot, "arial", 30);

				menuBoard->addChild(noticeL);
				noticeL->setPosition(menuBoard->getContentSize().width / 2, menuBoard->getContentSize().height * 3 / 4);
				noticeL->setTextColor(Color4B::BLACK);

				noticeMenu = Menu::create(okM, NULL);
				this->addChild(noticeMenu);

				return true;
			}
			else if (p.state == stateType::prison && p.stayRound == 0)
			{
				// ����ʱ�䵽������

				p.state = stateType::normal;

				// �˵����ͼƬ
				menuBoard = Sprite::create("image/Popup.png");
				menuBoard->setPosition(visibleSize.width / 2, visibleSize.height / 2);
				this->addChild(menuBoard);

				// �˵���ȷ��
				MenuItem* okM = MenuItemImage::create("image/OrangeNormal.png",
					"image/OrangePressed.png", CC_CALLBACK_0(NewGameScene::cleanMenu, this));
				okM->setPosition(0, -visibleSize.height / 5);

				const char* okC = ((String*)ngContent->objectForKey("ok"))->getCString();
				Label* okL = Label::createWithSystemFont(okC, "arial", 20);
				okM->addChild(okL);
				okL->setPosition(okM->getContentSize().width / 2, okM->getContentSize().height / 2);
				okL->setTextColor(Color4B::BLACK);

				// �˵���Ҫ����
				const char* outOfPrison = ((String*)ngContent->objectForKey("outOfPrison"))->getCString();
				Label* noticeL = Label::createWithSystemFont(outOfPrison, "arial", 30);

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
}

void NewGameScene::checkRoad(float dt)
{
	// �������ͼƬ
	this->removeChildByName("dicePoint");

	auto ngContent = Dictionary::createWithContentsOfFile("XML/NewGame.xml");
	
	int n = 1;

	for (auto& p : players)
	{
		if (n == nowPlayerNumber)
		{
			if (road->getTileGIDAt(p.rolePosition) == normal_road_GID)
			{
				// ����ͨ��
				this->scheduleOnce(schedule_selector(NewGameScene::checkLand), 0.1);
			}
			else if (road->getTileGIDAt(p.rolePosition) == prisonEnterance_road_GID)
			{
				// �׼���

				// �޸ĵ�ǰ��ҵ�״̬��λ�ú�ͼƬλ��
				p.state = stateType::prison;
				p.rolePosition = Vec2(4, 17);
				p.spritePosition.x -= 17 * 30;
				p.spritePosition.y -= 14 * 30;
				p.roleSprite->setPosition(p.spritePosition);
				p.roleSprite->setTexture("image/" + p.name + "_up.png");
				p.faceTo = faceForward::up;
				p.stayRound = 3;

				// �˵����ͼƬ
				menuBoard = Sprite::create("image/Popup.png");
				menuBoard->setPosition(visibleSize.width / 2, visibleSize.height / 2);
				this->addChild(menuBoard);

				// �˵���ȷ��
				MenuItem* okM = MenuItemImage::create("image/OrangeNormal.png",
					"image/OrangePressed.png", CC_CALLBACK_0(NewGameScene::cleanAndChange, this));
				okM->setPosition(0, -visibleSize.height / 5);

				const char* okC = ((String*)ngContent->objectForKey("ok"))->getCString();
				Label* okL = Label::createWithSystemFont(okC, "arial", 20);
				okM->addChild(okL);
				okL->setPosition(okM->getContentSize().width / 2, okM->getContentSize().height / 2);
				okL->setTextColor(Color4B::BLACK);

				// �˵���Ҫ����
				const char* prisonEnterance1 = ((String*)ngContent->objectForKey("prisonEnterance1"))->getCString();
				const char* prisonEnterance2 = ((String*)ngContent->objectForKey("prisonEnterance2"))->getCString();
				string blank = " ";
				string s = prisonEnterance1 + blank + "\n" + prisonEnterance2;
				Label* noticeL = Label::createWithSystemFont(s, "arial", 30);

				menuBoard->addChild(noticeL);
				noticeL->setPosition(menuBoard->getContentSize().width / 2, menuBoard->getContentSize().height * 3 / 4);
				noticeL->setTextColor(Color4B::BLACK);
				noticeL->setMaxLineWidth(520);

				noticeMenu = Menu::create(okM, NULL);
				this->addChild(noticeMenu);
			}
			else if (road->getTileGIDAt(p.rolePosition) == prison_road_GID)
			{
				// һ��ͨ������
				this->changePlayer();
			}
			else if (road->getTileGIDAt(p.rolePosition) == parkinglot_road_GID)
			{
				// ����ͣ����
				p.state = stateType::parking;
				p.stayRound = 2;

				// �˵����ͼƬ
				menuBoard = Sprite::create("image/Popup.png");
				menuBoard->setPosition(visibleSize.width / 2, visibleSize.height / 2);
				this->addChild(menuBoard);

				// �˵���ȷ��
				MenuItem* okM = MenuItemImage::create("image/OrangeNormal.png",
					"image/OrangePressed.png", CC_CALLBACK_0(NewGameScene::cleanAndChange, this));
				okM->setPosition(0, -visibleSize.height / 5);

				const char* okC = ((String*)ngContent->objectForKey("ok"))->getCString();
				Label* okL = Label::createWithSystemFont(okC, "arial", 20);
				okM->addChild(okL);
				okL->setPosition(okM->getContentSize().width / 2, okM->getContentSize().height / 2);
				okL->setTextColor(Color4B::BLACK);

				// �˵���Ҫ����
				const char* parkinglot = ((String*)ngContent->objectForKey("parkinglot"))->getCString();
				Label* noticeL = Label::createWithSystemFont(parkinglot, "arial", 30);

				menuBoard->addChild(noticeL);
				noticeL->setPosition(menuBoard->getContentSize().width / 2, menuBoard->getContentSize().height * 3 / 4);
				noticeL->setTextColor(Color4B::BLACK);
				noticeL->setMaxLineWidth(520);

				noticeMenu = Menu::create(okM, NULL);
				this->addChild(noticeMenu);
			}
			else if (road->getTileGIDAt(p.rolePosition) == emergency_road_GID)
			{
				// ͻ���¼�
				
				// �¼�����
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
				}

				// �˵����ͼƬ
				menuBoard = Sprite::create("image/Popup.png");
				menuBoard->setPosition(visibleSize.width / 2, visibleSize.height / 2);
				this->addChild(menuBoard);

				// �˵���ȷ��
				MenuItem* okM = MenuItemImage::create("image/OrangeNormal.png",
					"image/OrangePressed.png", CC_CALLBACK_0(NewGameScene::payLoss, this, p.name, loss));
				okM->setPosition(0, -visibleSize.height / 5);

				const char* okC = ((String*)ngContent->objectForKey("ok"))->getCString();
				Label* okL = Label::createWithSystemFont(okC, "arial", 20);
				okM->addChild(okL);
				okL->setPosition(okM->getContentSize().width / 2, okM->getContentSize().height / 2);
				okL->setTextColor(Color4B::BLACK);

				// �˵���Ҫ����
				const char* emergencyEvent = ((String*)ngContent->objectForKey("emergencyEvent" + to_string(i)))->getCString();
				Label* noticeL = Label::createWithSystemFont(emergencyEvent, "arial", 20);
				
				menuBoard->addChild(noticeL);
				noticeL->setPosition(menuBoard->getContentSize().width / 2, menuBoard->getContentSize().height * 3 / 4);
				noticeL->setTextColor(Color4B::BLACK);

				noticeMenu = Menu::create(okM, NULL);
				this->addChild(noticeMenu);
			}
			else if (road->getTileGIDAt(p.rolePosition) == tax_road_GID)
			{
				// ��С��

				// �˵����ͼƬ
				menuBoard = Sprite::create("image/Popup.png");
				menuBoard->setPosition(visibleSize.width / 2, visibleSize.height / 2);
				this->addChild(menuBoard);

				// �˵���ȷ��
				MenuItem* okM = MenuItemImage::create("image/OrangeNormal.png",
					"image/OrangePressed.png", CC_CALLBACK_0(NewGameScene::payLoss, this, p.name, 700));
				okM->setPosition(0, -visibleSize.height / 5);

				const char* okC = ((String*)ngContent->objectForKey("ok"))->getCString();
				Label* okL = Label::createWithSystemFont(okC, "arial", 20);
				okM->addChild(okL);
				okL->setPosition(okM->getContentSize().width / 2, okM->getContentSize().height / 2);
				okL->setTextColor(Color4B::BLACK);

				// �˵���Ҫ����
				const char* tax1 = ((String*)ngContent->objectForKey("tax1"))->getCString();
				const char* tax2 = ((String*)ngContent->objectForKey("tax2"))->getCString();
				string blank = " ";
				string s = tax1 + blank + "\n" + tax2;
				Label* noticeL = Label::createWithSystemFont(s, "arial", 30);


				menuBoard->addChild(noticeL);
				noticeL->setPosition(menuBoard->getContentSize().width / 2, menuBoard->getContentSize().height * 3 / 4);
				noticeL->setTextColor(Color4B::BLACK);

				noticeMenu = Menu::create(okM, NULL);
				this->addChild(noticeMenu);
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
			// �ж����ﳯ��ȷ������λ��
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

			// ���������ز��ж��Ƿ��ڶ�Ӧ�����������
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

// Land��غ���

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

	// �˵����ͼƬ
	menuBoard = Sprite::create("image/Popup.png");
	menuBoard->setPosition(visibleSize.width / 2, visibleSize.height / 2);
	this->addChild(menuBoard);

	// �ֶ���
	auto ngContent = Dictionary::createWithContentsOfFile("XML/NewGame.xml");

	// �˵� ����
	MenuItem* yesM = MenuItemImage::create("image/GreenNormal.png",
		"image/GreenPressed.png", CC_CALLBACK_0(NewGameScene::emptyMenuYes, this));
	yesM->setPosition(-visibleSize.width / 8, -visibleSize.height / 5);

	const char* yesC = ((String*)ngContent->objectForKey("yes"))->getCString();
	Label* yesL = Label::createWithSystemFont(yesC, "arial", 20);
	yesM->addChild(yesL);
	yesL->setPosition(yesM->getContentSize().width / 2, yesM->getContentSize().height / 2);
	yesL->setTextColor(Color4B::BLACK);

	// �˵� ����
	MenuItem* noM = MenuItemImage::create("image/RedNormal.png",
		"image/RedPressed.png", CC_CALLBACK_0(NewGameScene::emptyMenuNo, this));
	noM->setPosition(visibleSize.width / 8, -visibleSize.height / 5);

	const char* noC = ((String*)ngContent->objectForKey("no"))->getCString();
	Label* noL = Label::createWithSystemFont(noC, "arial", 20);
	noM->addChild(noL);
	noL->setPosition(noM->getContentSize().width / 2, noM->getContentSize().height / 2);
	noL->setTextColor(Color4B::BLACK);
	

	// �˵���Ҫ����
	Label* noticeL;

	const char* nc = ((String*)ngContent->objectForKey(name))->getCString();
	string blank = " ";
	const char* comma = ((String*)ngContent->objectForKey("comma"))->getCString();
	const char* upgradeLand0 = ((String*)ngContent->objectForKey("upgradeLand0"))->getCString();
	const char* payToUpgrade = ((String*)ngContent->objectForKey("payToUpgrade"))->getCString();

	// ���ڿհ׵ؿ�ļ۸�
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
					auto ngContent = Dictionary::createWithContentsOfFile("XML/NewGame.xml");

					if (p.money >= emptyBuildCost)
					{
						l->getTileAt(nowLand)->setColor(p.color);
						p.money -= emptyBuildCost;

						const char* rmb = ((String*)ngContent->objectForKey("rmb"))->getCString();
						string blank = " ";
						string s = rmb + blank + to_string(p.money);

						((Label*)this->getChildByName(p.name + blank + "money"))->setString(s);

						this->cleanAndChange();
					}
					else
					{
						this->cleanMenu();
						
						// �˵����ͼƬ
						menuBoard = Sprite::create("image/Popup.png");
						menuBoard->setPosition(visibleSize.width / 2, visibleSize.height / 2);
						this->addChild(menuBoard);

						// �˵���ȷ��
						MenuItem* okM = MenuItemImage::create("image/OrangeNormal.png",
							"image/OrangePressed.png", CC_CALLBACK_0(NewGameScene::cleanAndChange, this));
						okM->setPosition(0, -visibleSize.height / 5);

						const char* okC = ((String*)ngContent->objectForKey("ok"))->getCString();
						Label* okL = Label::createWithSystemFont(okC, "arial", 20);
						okM->addChild(okL);
						okL->setPosition(okM->getContentSize().width / 2, okM->getContentSize().height / 2);
						okL->setTextColor(Color4B::BLACK);

						// �˵���Ҫ����
						const char* lackOfMoney = ((String*)ngContent->objectForKey("lackOfMoney"))->getCString();
						Label* noticeL = Label::createWithSystemFont(lackOfMoney, "arial", 30);

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

void NewGameScene::emptyMenuNo()
{
	this->cleanAndChange();
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

	// �˵����ͼƬ
	menuBoard = Sprite::create("image/Popup.png");
	menuBoard->setPosition(visibleSize.width / 2, visibleSize.height / 2);
	this->addChild(menuBoard);

	// �ֶ���
	auto ngContent = Dictionary::createWithContentsOfFile("XML/NewGame.xml");

	// �˵� ����
	MenuItem* yesM = MenuItemImage::create("image/GreenNormal.png",
		"image/GreenPressed.png", CC_CALLBACK_0(NewGameScene::myMenuYes, this));
	yesM->setPosition(-visibleSize.width / 8, -visibleSize.height / 5);

	const char* yesC = ((String*)ngContent->objectForKey("yes"))->getCString();
	Label* yesL = Label::createWithSystemFont(yesC, "arial", 20);
	yesM->addChild(yesL);
	yesL->setPosition(yesM->getContentSize().width / 2, yesM->getContentSize().height / 2);
	yesL->setTextColor(Color4B::BLACK);

	// �˵� ����
	MenuItem* noM = MenuItemImage::create("image/RedNormal.png",
		"image/RedPressed.png", CC_CALLBACK_0(NewGameScene::myMenuNo, this));
	noM->setPosition(visibleSize.width / 8, -visibleSize.height / 5);

	const char* noC = ((String*)ngContent->objectForKey("no"))->getCString();
	Label* noL = Label::createWithSystemFont(noC, "arial", 20);
	noM->addChild(noL);
	noL->setPosition(noM->getContentSize().width / 2, noM->getContentSize().height / 2);
	noL->setTextColor(Color4B::BLACK);

	// ��ʾ������
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
		// ������Ϊ��߼������赯���Ի���
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
			int buildCost = 0;
			auto ngContent = Dictionary::createWithContentsOfFile("XML/NewGame.xml");

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

						auto ngContent = Dictionary::createWithContentsOfFile("XML/NewGame.xml");
						const char* rmb = ((String*)ngContent->objectForKey("rmb"))->getCString();
						string blank = " ";
						string s = rmb + blank + to_string(p.money);

						((Label*)this->getChildByName(p.name + blank + "money"))->setString(s);

						this->cleanAndChange();
					}
					else
					{
						this->cleanMenu();

						// �˵����ͼƬ
						menuBoard = Sprite::create("image/Popup.png");
						menuBoard->setPosition(visibleSize.width / 2, visibleSize.height / 2);
						this->addChild(menuBoard);

						// �˵���ȷ��
						MenuItem* okM = MenuItemImage::create("image/OrangeNormal.png",
							"image/OrangePressed.png", CC_CALLBACK_0(NewGameScene::cleanAndChange, this));
						okM->setPosition(0, -visibleSize.height / 5);

						const char* okC = ((String*)ngContent->objectForKey("ok"))->getCString();
						Label* okL = Label::createWithSystemFont(okC, "arial", 20);
						okM->addChild(okL);
						okL->setPosition(okM->getContentSize().width / 2, okM->getContentSize().height / 2);
						okL->setTextColor(Color4B::BLACK);

						// �˵���Ҫ����
						const char* lackOfMoney = ((String*)ngContent->objectForKey("lackOfMoney"))->getCString();
						Label* noticeL = Label::createWithSystemFont(lackOfMoney, "arial", 30);

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

void NewGameScene::myMenuNo()
{
	this->cleanAndChange();
}

void NewGameScene::otherLand()
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

	// �˵����ͼƬ
	menuBoard = Sprite::create("image/Popup.png");
	menuBoard->setPosition(visibleSize.width / 2, visibleSize.height / 2);
	this->addChild(menuBoard);

	// �ֶ���
	auto ngContent = Dictionary::createWithContentsOfFile("XML/NewGame.xml");
	
	// ��ʾ������
	Label* noticeL;
	const char* belongLand = ((String*)ngContent->objectForKey("belongLand"))->getCString();
	string blank = " ";
	const char* earnC = ((String*)ngContent->objectForKey(earnName))->getCString();
	const char* comma = ((String*)ngContent->objectForKey("comma"))->getCString();
	const char* payC = ((String*)ngContent->objectForKey(payName))->getCString();
	const char* payLand = ((String*)ngContent->objectForKey("payLand"))->getCString();

	int price, rateTimes;
	int rateTimes1 = 1;
	int rateTimes2 = 1;
	int rate = 0;

	for (auto l : lands)
	{
		if (l->getTileAt(nowLand))
		{
			// ��������ͬ��ҵĵأ�2��+5%��3��+10%��4��+20%��5��+50%,6��+100%

			if (nowF == faceForward::down || nowF == faceForward::up)
			{
				// ����
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

				// ����
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
				// ����
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

				// ����
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

			// ���ݲ�ͬ��ռ����������ȡ��ͬ�ķ���
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

	const char* yuan = ((String*)ngContent->objectForKey("yuan"))->getCString();

	string s;
	if (rate == 0)
	{
		s = belongLand + blank + earnC + comma + payC + "\n" +
			payLand + blank + to_string(price) + yuan;
	}
	else
	{
		s = belongLand + blank + earnC + comma + payC + "\n" +
			payLand + blank + to_string(price) + yuan + "(+" + to_string(rate) + "%)";
	}

	noticeL = Label::createWithSystemFont(s, "arial", 30);

	menuBoard->addChild(noticeL);
	noticeL->setPosition(menuBoard->getContentSize().width / 2, menuBoard->getContentSize().height * 3 / 4);
	noticeL->setTextColor(Color4B::BLACK);

	// �˵���ȷ��
	MenuItem* okM = MenuItemImage::create("image/OrangeNormal.png",
		"image/OrangePressed.png", CC_CALLBACK_0(NewGameScene::otherMenuClose, this, payName, earnName,price));
	okM->setPosition(0, -visibleSize.height / 5);

	const char* okC = ((String*)ngContent->objectForKey("ok"))->getCString();
	Label* okL = Label::createWithSystemFont(okC, "arial", 20);
	okM->addChild(okL);
	okL->setPosition(okM->getContentSize().width / 2, okM->getContentSize().height / 2);
	okL->setTextColor(Color4B::BLACK);

	noticeMenu = Menu::create(okM, NULL);
	this->addChild(noticeMenu);
}

void NewGameScene::otherMenuClose(string payName, string earnName,int price)
{	
	// �ҳ���Ӧ��������ң����н�Ǯ����
	for (auto& p : players)
	{
		for (auto& e : players)
		{
			if (p.name == payName && e.name == earnName)
			{
				p.money -= price;
				e.money += price;

				auto ngContent = Dictionary::createWithContentsOfFile("XML/NewGame.xml");
				const char* rmb = ((String*)ngContent->objectForKey("rmb"))->getCString();
				string blank = " ";
				string sp = rmb + blank + to_string(p.money);
				string se = rmb + blank + to_string(e.money);

				((Label*)this->getChildByName(payName + blank + "money"))->setString(sp);
				((Label*)this->getChildByName(earnName + blank + "money"))->setString(se);

				break;
			}
		}
	}

	this->cleanAndChange();
}

void NewGameScene::payLoss(string payName,int loss)
{
	for (auto& p : players)
	{
		if (p.name == payName)
		{
			auto ngContent = Dictionary::createWithContentsOfFile("XML/NewGame.xml");
			const char* rmb = ((String*)ngContent->objectForKey("rmb"))->getCString();
			string blank = " ";
			p.money -= loss;

			string sp = rmb + blank + to_string(p.money);

			((Label*)this->getChildByName(payName + blank + "money"))->setString(sp);

			break;
		}
	}

	this->cleanAndChange();
}

// �����غ���

void NewGameScene::cleanMenu()
{
	noticeMenu->removeFromParentAndCleanup(true);
	menuBoard->removeFromParentAndCleanup(true);
}

void NewGameScene::changePlayer()
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

	// �����ҽ�ǮС��0�����߳���Ϸ
	n = 0;
	bool flag = false;

	for (auto& p : players)
	{
		if (p.money < 0)
		{
			// �˵����ͼƬ
			menuBoard = Sprite::create("image/Popup.png");
			menuBoard->setPosition(visibleSize.width / 2, visibleSize.height / 2);
			this->addChild(menuBoard);

			// �ֶ���
			auto ngContent = Dictionary::createWithContentsOfFile("XML/NewGame.xml");

			// ��ʾ������
			Label* noticeL;
			const char* belongLand = ((String*)ngContent->objectForKey("belongLand"))->getCString();
			const char* eliminate = ((String*)ngContent->objectForKey("eliminate"))->getCString();
			string str = "";
			string s = "";

			if (n == 0)
			{
				const char* player = ((String*)ngContent->objectForKey("player1"))->getCString();
				s = player + str + eliminate;
			}
			else if (n == 1)
			{
				const char* player = ((String*)ngContent->objectForKey("player2"))->getCString();
				s = player + str + eliminate;
			}
			else if (n == 2)
			{
				const char* player = ((String*)ngContent->objectForKey("player3"))->getCString();
				s = player + str + eliminate;
			}
			else if (n == 3)
			{
				const char* player = ((String*)ngContent->objectForKey("player4"))->getCString();
				s = player + str + eliminate;
			}
			
			noticeL = Label::createWithSystemFont(s, "arial", 30);

			menuBoard->addChild(noticeL);
			noticeL->setPosition(menuBoard->getContentSize().width / 2, menuBoard->getContentSize().height * 3 / 4);
			noticeL->setTextColor(Color4B::BLACK);

			// �˵���ȷ��
			MenuItem* okM = MenuItemImage::create("image/OrangeNormal.png",
				"image/OrangePressed.png", CC_CALLBACK_0(NewGameScene::removePlayer, this, n));
			okM->setPosition(0, -visibleSize.height / 5);

			const char* okC = ((String*)ngContent->objectForKey("ok"))->getCString();
			Label* okL = Label::createWithSystemFont(okC, "arial", 20);
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

	// �����������̭��ң������ֻ�
	if (flag == false)
	{
		nowPlayerNumber++;
		if (nowPlayerNumber > players.size())
		{
			nowPlayerNumber = 1;

			// �л��غ���
			rounds++;
			((Label*)this->getChildByName("roundsL"))->setString(to_string(rounds));
		}
	}
}

void NewGameScene::removePlayer(int number)
{
	int n = 0;
	for (auto& p : players)
	{
		if (n == number)
		{
			this->removeChildByName("player" + to_string(n + 1), true);

			players.erase(players.begin() + number);

			break;
		}
		n++;
	}

	this->cleanMenu();

	// �ж��Ƿ�ֻʣһ����ң����ǣ�����Ϸ����
	bool flag = false;

	if (players.size() == 1)
	{
		// �˵����ͼƬ
		menuBoard = Sprite::create("image/Popup.png");
		menuBoard->setPosition(visibleSize.width / 2, visibleSize.height / 2);
		this->addChild(menuBoard);

		// �ֶ���
		auto ngContent = Dictionary::createWithContentsOfFile("XML/NewGame.xml");

		// ��ʾ������
		Label* noticeL;
		const char* belongLand = ((String*)ngContent->objectForKey("belongLand"))->getCString();
		const char* win = ((String*)ngContent->objectForKey("win"))->getCString();
		string str = "";
		string s = "";

		if (players[0].name == "player1")
		{
			const char* player = ((String*)ngContent->objectForKey("player1"))->getCString();
			s = player + str + win;
		}
		else if (players[0].name == "player2")
		{
			const char* player = ((String*)ngContent->objectForKey("player2"))->getCString();
			s = player + str + win;
		}
		else if (players[0].name == "player3")
		{
			const char* player = ((String*)ngContent->objectForKey("player3"))->getCString();
			s = player + str + win;
		}
		else if (players[0].name == "player4")
		{
			const char* player = ((String*)ngContent->objectForKey("player4"))->getCString();
			s = player + str + win;
		}

		noticeL = Label::createWithSystemFont(s, "arial", 30);

		menuBoard->addChild(noticeL);
		noticeL->setPosition(menuBoard->getContentSize().width / 2, menuBoard->getContentSize().height * 3 / 4);
		noticeL->setTextColor(Color4B::BLACK);

		// �˵���ȷ��
		MenuItem* okM = MenuItemImage::create("image/OrangeNormal.png",
			"image/OrangePressed.png", CC_CALLBACK_0(NewGameScene::endGame, this));
		okM->setPosition(0, -visibleSize.height / 5);

		const char* okC = ((String*)ngContent->objectForKey("ok"))->getCString();
		Label* okL = Label::createWithSystemFont(okC, "arial", 20);
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
		if (nowPlayerNumber > players.size())
		{
			nowPlayerNumber = 1;

			// �л��غ���
			rounds++;
			((Label*)this->getChildByName("roundsL"))->setString(to_string(rounds));
		}
	}
}

void NewGameScene::cleanAndChange()
{
	this->cleanMenu();
	this->changePlayer();
}

void NewGameScene::endGame()
{
	TitleScene* ts = TitleScene::create();
	Director::getInstance()->pushScene(ts);
}