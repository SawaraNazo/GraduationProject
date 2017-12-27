#include "SelectGameRolesLayer.h"
#include <sstream>

SelectGameRolesLayer::SelectGameRolesLayer()
{

}

SelectGameRolesLayer::~SelectGameRolesLayer()
{

}

bool SelectGameRolesLayer::init()
{
	if (!Layer::init())
	{
		return false;
	}

	visibleSize = Director::getInstance()->getVisibleSize();

	this->scheduleOnce(schedule_selector(SelectGameRolesLayer::setParameter), 0.05f);

	// ���¼�
	auto callback = [](Touch*, Event*) {return true; };
	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = callback;
	listener->setSwallowTouches(true);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

	return true;
}

void SelectGameRolesLayer::setParameter(float dt)
{
	auto ngs = this->getParent();
	auto gil = (GameInitLayer*)(ngs->getChildByName("gameinit"));
	playersNumber = gil->getPlayersNumber();

	for (auto i : gil->getPlayersRoles())
	{
		playersRoles.push_back(i);
	}

	this->createMenu();
}

void SelectGameRolesLayer::createMenu()
{
	Sprite* bg = Sprite::create("image/PlayersRoles.png");
	bg->setPosition(visibleSize.width / 2, visibleSize.height / 2);
	this->addChild(bg);

	auto w = bg->getContentSize().width;
	auto h = bg->getContentSize().height;

	Menu* m = Menu::create();

	if (playersNumber == 2)
	{
		// 1
		Label* l1 = Label::createWithSystemFont("player1", "Arial", 18);
		l1->setPosition(w / 8, h * 3 / 4);
		bg->addChild(l1);

		ostringstream stream1;
		stream1<<playersRoles[0];
		string s1loc = "image/role" + stream1.str() + "_head.png";
		Sprite* s1 = Sprite::create(s1loc);
		s1->setPosition(w * 3 / 4, l1->getPosition().y);
		bg->addChild(s1);

		Sprite* ln1 = Sprite::create("image/role_normal_left.png");
		Sprite* lp1 = Sprite::create("image/role_pressed_left.png");
		MenuItem* mil1 = MenuItemSprite::create(ln1, lp1, CC_CALLBACK_1(SelectGameRolesLayer::leftCallback, this, 0, s1));
		mil1->setPosition(w / 8, h / 4);
		m->addChild(mil1);

		Sprite* rn1 = Sprite::create("image/role_normal_right.png");
		Sprite* rp1 = Sprite::create("image/role_pressed_right.png");
		MenuItem* mir1 = MenuItemSprite::create(rn1, rp1, CC_CALLBACK_1(SelectGameRolesLayer::rightCallback, this, 0, s1));
		mir1->setPosition(w * 3 / 8, h / 4);
		m->addChild(mir1);

		// 2
		Label* l2 = Label::createWithSystemFont("player2", "Arial", 18);
		l2->setPosition(w / 8, h / 2);
		bg->addChild(l2);

		ostringstream stream2;
		stream2<<playersRoles[1];
		string s2loc = "image/role" + stream2.str() + "_head.png";
		Sprite* s2 = Sprite::create(s2loc);
		s2->setPosition(w * 3 / 4, l2->getPosition().y);
		bg->addChild(s2);

		Sprite* ln2 = Sprite::create("image/role_normal_left.png");
		Sprite* lp2 = Sprite::create("image/role_pressed_left.png");
		MenuItem* mil2 = MenuItemSprite::create(ln2, lp2, CC_CALLBACK_1(SelectGameRolesLayer::leftCallback, this, 1, s2));
		mil2->setPosition(w / 8, 0);
		m->addChild(mil2);

		Sprite* rn2 = Sprite::create("image/role_normal_right.png");
		Sprite* rp2 = Sprite::create("image/role_pressed_right.png");
		MenuItem* mir2 = MenuItemSprite::create(rn2, rp2, CC_CALLBACK_1(SelectGameRolesLayer::rightCallback, this, 1, s2));
		mir2->setPosition(w * 3 / 8, 0);
		m->addChild(mir2);

	}
	else if (playersNumber == 3)
	{
		// 1
		Label* l1 = Label::createWithSystemFont("player1", "Arial", 18);
		l1->setPosition(w / 8, h * 3 / 4);
		bg->addChild(l1);

		ostringstream ostringstream1;
		ostringstream1<<playersRoles[0];
		string s1loc = "image/role" + ostringstream1.str() + "_head.png";
		Sprite* s1 = Sprite::create(s1loc);
		s1->setPosition(w * 3 / 4, l1->getPosition().y);
		bg->addChild(s1);

		Sprite* ln1 = Sprite::create("image/role_normal_left.png");
		Sprite* lp1 = Sprite::create("image/role_pressed_left.png");
		MenuItem* mil1 = MenuItemSprite::create(ln1, lp1, CC_CALLBACK_1(SelectGameRolesLayer::leftCallback, this, 0, s1));
		mil1->setPosition(w / 8, h / 4);
		m->addChild(mil1);

		Sprite* rn1 = Sprite::create("image/role_normal_right.png");
		Sprite* rp1 = Sprite::create("image/role_pressed_right.png");
		MenuItem* mir1 = MenuItemSprite::create(rn1, rp1, CC_CALLBACK_1(SelectGameRolesLayer::rightCallback, this, 0, s1));
		mir1->setPosition(w * 3 / 8, h / 4);
		m->addChild(mir1);

		// 2
		Label* l2 = Label::createWithSystemFont("player2", "Arial", 18);
		l2->setPosition(w / 8, h / 2);
		bg->addChild(l2);

		ostringstream ostringstream2;
		ostringstream2<<playersRoles[1];
		string s2loc = "image/role" + ostringstream2.str() + "_head.png";
		Sprite* s2 = Sprite::create(s2loc);
		s2->setPosition(w * 3 / 4, l2->getPosition().y);
		bg->addChild(s2);

		Sprite* ln2 = Sprite::create("image/role_normal_left.png");
		Sprite* lp2 = Sprite::create("image/role_pressed_left.png");
		MenuItem* mil2 = MenuItemSprite::create(ln2, lp2, CC_CALLBACK_1(SelectGameRolesLayer::leftCallback, this, 1, s2));
		mil2->setPosition(w / 8, 0);
		m->addChild(mil2);

		Sprite* rn2 = Sprite::create("image/role_normal_right.png");
		Sprite* rp2 = Sprite::create("image/role_pressed_right.png");
		MenuItem* mir2 = MenuItemSprite::create(rn2, rp2, CC_CALLBACK_1(SelectGameRolesLayer::rightCallback, this, 1, s2));
		mir2->setPosition(w * 3 / 8, 0);
		m->addChild(mir2);

		// 3
		Label* l3 = Label::createWithSystemFont("player3", "Arial", 18);
		l3->setPosition(w / 8, h / 4);
		bg->addChild(l3);

		ostringstream ostringstream3;
		ostringstream3 << playersRoles[2];
		string s3loc = "image/role" + ostringstream3.str() + "_head.png";
		Sprite* s3 = Sprite::create(s3loc);
		s3->setPosition(w * 3 / 4, l3->getPosition().y);
		bg->addChild(s3);

		Sprite* ln3 = Sprite::create("image/role_normal_left.png");
		Sprite* lp3 = Sprite::create("image/role_pressed_left.png");
		MenuItem* mil3 = MenuItemSprite::create(ln3, lp3, CC_CALLBACK_1(SelectGameRolesLayer::leftCallback, this, 2, s3));
		mil3->setPosition(w / 8, -h / 4);
		m->addChild(mil3);

		Sprite* rn3 = Sprite::create("image/role_normal_right.png");
		Sprite* rp3 = Sprite::create("image/role_pressed_right.png");
		MenuItem* mir3 = MenuItemSprite::create(rn3, rp3, CC_CALLBACK_1(SelectGameRolesLayer::rightCallback, this, 2, s3));
		mir3->setPosition(w * 3 / 8, -h / 4);
		m->addChild(mir3);
	}
	else if (playersNumber == 4)
	{
		// 1
		Label* l1 = Label::createWithSystemFont("player1", "Arial", 18);
		l1->setPosition(w / 8, h * 3 / 4);
		bg->addChild(l1);

		ostringstream ostringstream1;
		ostringstream1 <<playersRoles[0];
		string s1loc = "image/role" + ostringstream1.str() + "_head.png";
		Sprite* s1 = Sprite::create(s1loc);
		s1->setPosition(w * 3 / 4, l1->getPosition().y);
		bg->addChild(s1);

		Sprite* ln1 = Sprite::create("image/role_normal_left.png");
		Sprite* lp1 = Sprite::create("image/role_pressed_left.png");
		MenuItem* mil1 = MenuItemSprite::create(ln1, lp1, CC_CALLBACK_1(SelectGameRolesLayer::leftCallback, this, 0, s1));
		mil1->setPosition(w / 8, h / 4);
		m->addChild(mil1);

		Sprite* rn1 = Sprite::create("image/role_normal_right.png");
		Sprite* rp1 = Sprite::create("image/role_pressed_right.png");
		MenuItem* mir1 = MenuItemSprite::create(rn1, rp1, CC_CALLBACK_1(SelectGameRolesLayer::rightCallback, this, 0, s1));
		mir1->setPosition(w * 3 / 8, h / 4);
		m->addChild(mir1);

		// 2
		Label* l2 = Label::createWithSystemFont("player2", "Arial", 18);
		l2->setPosition(w / 8, h * 7 / 12);
		bg->addChild(l2);

		ostringstream ostringstream2;
		ostringstream2 << playersRoles[1];
		string s2loc = "image/role" + ostringstream2.str() + "_head.png";
		Sprite* s2 = Sprite::create(s2loc);
		s2->setPosition(w * 3 / 4, l2->getPosition().y);
		bg->addChild(s2);

		Sprite* ln2 = Sprite::create("image/role_normal_left.png");
		Sprite* lp2 = Sprite::create("image/role_pressed_left.png");
		MenuItem* mil2 = MenuItemSprite::create(ln2, lp2, CC_CALLBACK_1(SelectGameRolesLayer::leftCallback, this, 1, s2));
		mil2->setPosition(w / 8, h / 12);
		m->addChild(mil2);

		Sprite* rn2 = Sprite::create("image/role_normal_right.png");
		Sprite* rp2 = Sprite::create("image/role_pressed_right.png");
		MenuItem* mir2 = MenuItemSprite::create(rn2, rp2, CC_CALLBACK_1(SelectGameRolesLayer::rightCallback, this, 1, s2));
		mir2->setPosition(w * 3 / 8, h / 12);
		m->addChild(mir2);

		// 3
		Label* l3 = Label::createWithSystemFont("player3", "Arial", 18);
		l3->setPosition(w / 8, h * 5 / 12);
		bg->addChild(l3);

		ostringstream ostringstream3;
		ostringstream3 << playersRoles[2];
		string s3loc = "image/role" + ostringstream3.str() + "_head.png";
		Sprite* s3 = Sprite::create(s3loc);
		s3->setPosition(w * 3 / 4, l3->getPosition().y);
		bg->addChild(s3);

		Sprite* ln3 = Sprite::create("image/role_normal_left.png");
		Sprite* lp3 = Sprite::create("image/role_pressed_left.png");
		MenuItem* mil3 = MenuItemSprite::create(ln3, lp3, CC_CALLBACK_1(SelectGameRolesLayer::leftCallback, this, 2, s3));
		mil3->setPosition(w / 8, -h / 12);
		m->addChild(mil3);

		Sprite* rn3 = Sprite::create("image/role_normal_right.png");
		Sprite* rp3 = Sprite::create("image/role_pressed_right.png");
		MenuItem* mir3 = MenuItemSprite::create(rn3, rp3, CC_CALLBACK_1(SelectGameRolesLayer::rightCallback, this, 2, s3));
		mir3->setPosition(w * 3 / 8, -h / 12);
		m->addChild(mir3);

		// 4
		Label* l4 = Label::createWithSystemFont("player4", "Arial", 18);
		l4->setPosition(w / 8, h / 4);
		bg->addChild(l4);

		ostringstream ostringstream4;
		ostringstream4 << playersRoles[3];
		string s4loc = "image/role" + ostringstream4.str() + "_head.png";
		Sprite* s4 = Sprite::create(s4loc);
		s4->setPosition(w * 3 / 4, l4->getPosition().y);
		bg->addChild(s4);

		Sprite* ln4 = Sprite::create("image/role_normal_left.png");
		Sprite* lp4 = Sprite::create("image/role_pressed_left.png");
		MenuItem* mil4 = MenuItemSprite::create(ln4, lp4, CC_CALLBACK_1(SelectGameRolesLayer::leftCallback, this, 3, s4));
		mil4->setPosition(w / 8, -h / 4);
		m->addChild(mil4);

		Sprite* rn4 = Sprite::create("image/role_normal_right.png");
		Sprite* rp4 = Sprite::create("image/role_pressed_right.png");
		MenuItem* mir4 = MenuItemSprite::create(rn4, rp4, CC_CALLBACK_1(SelectGameRolesLayer::rightCallback, this, 3, s4));
		mir4->setPosition(w * 3 / 8, -h / 4);
		m->addChild(mir4);
	}

	Sprite* normal = Sprite::create("image/GreenNormal.png");
	Sprite* pressed = Sprite::create("image/GreenPressed.png");
	MenuItem* mi = MenuItemSprite::create(normal, pressed, CC_CALLBACK_1(SelectGameRolesLayer::okCallback, this));
	mi->setPosition(0, -h / 2 + mi->getContentSize().height);
	Label* lm = Label::createWithSystemFont("OK", "Arial", 18);
	lm->setTextColor(Color4B::BLACK);
	lm->setPosition(mi->getContentSize().width / 2, mi->getContentSize().height / 2);
	mi->addChild(lm);
	m->addChild(mi);

	this->addChild(m);
}

void SelectGameRolesLayer::leftCallback(Ref* pSender, int n, Sprite* &s)
{
	--playersRoles[n];
	if (playersRoles[n] == 0)
	{
		playersRoles[n] = 5;
	}

	ostringstream ostringstream1;
	ostringstream1 << playersRoles[n];
	string sloc = "image/role" + ostringstream1.str() + "_head.png";
	Sprite* ss = Sprite::create(sloc);
	s->setTexture(ss->getTexture());
}

void SelectGameRolesLayer::rightCallback(Ref* pSender, int n, Sprite* &s)
{
	++playersRoles[n];
	if (playersRoles[n] == 6)
	{
		playersRoles[n] = 1;
	}

	ostringstream ostringstream1;
	ostringstream1 << playersRoles[n];
	string sloc = "image/role" + ostringstream1.str() + "_head.png";
	Sprite* ss = Sprite::create(sloc);
	s->setTexture(ss->getTexture());
}

void SelectGameRolesLayer::okCallback(Ref* pSender)
{
	bool flag = true;

	for (int i = 0; i < playersNumber; ++i)
	{
		for (int j = i + 1; j < playersNumber; ++j)
		{
			if (playersRoles[i] == playersRoles[j])
			{
				flag = false;
				break;
			}
		}
	}

	if (flag == true)
	{
		auto s = this->getParent();
		auto gi = (GameInitLayer*)(s->getChildByName("gameinit"));
		gi->setPlayersRoles(playersRoles);
		this->removeFromParentAndCleanup(true);
	}
	else
	{
		auto s = this->getParent();
		auto srd = SelectRoleDuplicateLayer::create();
		s->addChild(srd, 500);
	}
}