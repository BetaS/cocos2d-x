#include "GameScene.h"
#include "AppDelegate.h"
#include "net/GameClient.h"
#include "json/JsonBox.h"

USING_NS_CC;

#define TYPE_CALL	0
#define TYPE_DOUBLE 1
#define TYPE_CHECK	2
#define TYPE_DIE	3

#define IDX(i) (myIdx+i)%4

GameScene::GameScene()
{
	totalBetting = 0;
	myIdx = 0;
	mStatus = 0;
	memset(key, 0, 8);
}

GameScene::~GameScene()
{
	pthread_detach(thread);
}

CCScene* GameScene::scene(const char* key, int idx, string name, int money)
{
    // 'scene' is an autorelease object
    CCScene *scene = CCScene::create();
    
    // 'layer' is an autorelease object
    GameScene *layer = GameScene::create();
	memcpy(layer->key, key, 8);
	layer->myIdx = idx;

	layer->player_info[0].setPlayerName(name);
	layer->player_info[0].setMoney(money);

	layer->initPlayerUI();
	layer->updatePlayerInfo();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool GameScene::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !CCLayer::init() )
    {
        return false;
    }

    CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
    CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();

	CCSprite *pBackground = CCSprite::create("table_bg.png");
	pBackground->setPosition(ccp(origin.x + visibleSize.width/2, origin.y + visibleSize.height/2));
	this->addChild(pBackground, 0);

    // add a "close" icon to exit the progress. it's an autorelease object
    CCMenuItemImage *pCloseItem = CCMenuItemImage::create(
                                        "CloseNormal.png",
                                        "CloseSelected.png",
                                        this,
                                        menu_selector(GameScene::menuCloseCallback));
    
	pCloseItem->setPosition(ccp(origin.x + visibleSize.width - pCloseItem->getContentSize().width/2 ,
                                origin.y + pCloseItem->getContentSize().height/2));
	
    // create menu, it's an autorelease object
	pMenu = CCMenu::create(pCloseItem, NULL);
    pMenu->setPosition(CCPointZero);
    this->addChild(pMenu, 99);
	
	pPopup = BettingPopup::create();
	pPopup->setVisible(false);
	
	CCNotificationCenter::sharedNotificationCenter()->addObserver(this,
																  callfuncO_selector(GameScene::hidePopup),
																  "betting", NULL);
	CCNotificationCenter::sharedNotificationCenter()->addObserver(this,
		callfuncO_selector(GameScene::onMessage),
		"message", NULL);
	
	this->addChild(pPopup, 99);
	
	this->setTouchEnabled(true);
	CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 0, true);
	
	common_slot = CardSlot::create();
	common_slot->setSlotSize(3);
	common_slot->setScale(0.55f);
	common_slot->setPosition(ccp(visibleSize.width/2, visibleSize.height/2+100));
	this->addChild(common_slot, 1);

	pLabelTotalBet = CCLabelTTF::create("Total Bet\n$ 0", "Helvetica", 40);
	pLabelTotalBet->setColor(ccc3(255, 255, 255));
	pLabelTotalBet->setPosition(ccp(visibleSize.width/2, 300));
	this->addChild(pLabelTotalBet, 2);

	for(int i=0; i<4; i++) 
	{
		player_slot[i] = CardSlot::create();
		player_slot[i]->setSlotSize(4);
		this->addChild(player_slot[i], 1);

		pLabelPlayerName[i] = CCLabelTTF::create("Player", "Helvetica", 38);
		pLabelPlayerName[i]->setColor(ccc3(255, 255, 255));
		this->addChild(pLabelPlayerName[i], 2);
		pLabelPlayerMoney[i] = CCLabelTTF::create("$ 0", "Helvetica", 30);
		pLabelPlayerMoney[i]->setColor(ccc3(255, 255, 255));
		this->addChild(pLabelPlayerMoney[i], 2);
		pLabelPlayerStatus[i] = CCLabelTTF::create("call ($ 0)", "Helvetica", 22);
		pLabelPlayerStatus[i]->setColor(ccc3(200, 200, 200));
		this->addChild(pLabelPlayerStatus[i], 2);
	}
	
	pthread_create(&thread, NULL, _heartbeat, this);

    return true;
}

void GameScene::initPlayerUI()
{
	CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();

	player_slot[IDX(0)]->setScale(0.65f);
	player_slot[IDX(0)]->setPosition(ccp(visibleSize.width/2, 0+player_slot[IDX(0)]->getContentSize().height*player_slot[IDX(0)]->getScale()/2));
	pLabelPlayerName[IDX(0)]->setAnchorPoint(ccp(1.0, 0));
	pLabelPlayerName[IDX(0)]->setPosition(ccp(visibleSize.width/2-220, 0+player_slot[IDX(0)]->getContentSize().height*player_slot[IDX(0)]->getScale()/2+20));
	pLabelPlayerMoney[IDX(0)]->setAnchorPoint(ccp(1.0, 0));
	pLabelPlayerMoney[IDX(0)]->setPosition(ccp(visibleSize.width/2-220, 0+player_slot[IDX(0)]->getContentSize().height*player_slot[IDX(0)]->getScale()/2-15));
	pLabelPlayerStatus[IDX(0)]->setAnchorPoint(ccp(1.0, 0));
	pLabelPlayerStatus[IDX(0)]->setPosition(ccp(visibleSize.width/2-220, 0+player_slot[IDX(0)]->getContentSize().height*player_slot[IDX(0)]->getScale()/2-40));

	player_slot[IDX(1)]->setScale(0.6f);
	player_slot[IDX(1)]->setPosition(ccp(0+player_slot[IDX(1)]->getContentSize().height*player_slot[IDX(1)]->getScale()/2, visibleSize.height/2));
	player_slot[IDX(1)]->setRotation(-90);
	pLabelPlayerName[IDX(1)]->setAnchorPoint(ccp(0, 0));
	pLabelPlayerName[IDX(1)]->setPosition(ccp(10, visibleSize.height/2+255));
	pLabelPlayerMoney[IDX(1)]->setAnchorPoint(ccp(0, 0));
	pLabelPlayerMoney[IDX(1)]->setPosition(ccp(10, visibleSize.height/2+225));
	pLabelPlayerStatus[IDX(1)]->setAnchorPoint(ccp(0, 0));
	pLabelPlayerStatus[IDX(1)]->setPosition(ccp(10, visibleSize.height/2+200));

	player_slot[IDX(2)]->setScale(0.6f);
	player_slot[IDX(2)]->setPosition(ccp(visibleSize.width/2, visibleSize.height-player_slot[IDX(2)]->getContentSize().height*player_slot[IDX(2)]->getScale()/2));
	pLabelPlayerName[IDX(2)]->setAnchorPoint(ccp(0, 0));
	pLabelPlayerName[IDX(2)]->setPosition(ccp(visibleSize.width/2+205, visibleSize.height-player_slot[IDX(2)]->getContentSize().height*player_slot[IDX(2)]->getScale()/2+20));
	pLabelPlayerMoney[IDX(2)]->setAnchorPoint(ccp(0, 0));
	pLabelPlayerMoney[IDX(2)]->setPosition(ccp(visibleSize.width/2+205, visibleSize.height-player_slot[IDX(2)]->getContentSize().height*player_slot[IDX(2)]->getScale()/2-15));
	pLabelPlayerStatus[IDX(2)]->setAnchorPoint(ccp(0, 0));
	pLabelPlayerStatus[IDX(2)]->setPosition(ccp(visibleSize.width/2+205, visibleSize.height-player_slot[IDX(2)]->getContentSize().height*player_slot[IDX(2)]->getScale()/2-40));

	player_slot[IDX(3)]->setScale(0.6f);
	player_slot[IDX(3)]->setPosition(ccp(visibleSize.width-player_slot[IDX(3)]->getContentSize().height*player_slot[IDX(3)]->getScale()/2, visibleSize.height/2));
	player_slot[IDX(3)]->setRotation(90);
	pLabelPlayerName[IDX(3)]->setAnchorPoint(ccp(1.0, 0));
	pLabelPlayerName[IDX(3)]->setPosition(ccp(visibleSize.width-10, visibleSize.height/2+255));
	pLabelPlayerMoney[IDX(3)]->setAnchorPoint(ccp(1.0, 0));
	pLabelPlayerMoney[IDX(3)]->setPosition(ccp(visibleSize.width-10, visibleSize.height/2+225));
	pLabelPlayerStatus[IDX(3)]->setAnchorPoint(ccp(1.0, 0));
	pLabelPlayerStatus[IDX(3)]->setPosition(ccp(visibleSize.width-10, visibleSize.height/2+200));
}

void GameScene::onMessage(CCObject* obj)
{
	CCArray* arr = (CCArray*)obj;
	int typ =((CCInteger*)arr->data->arr[0])->getValue();
	string data = string(((CCString*)arr->data->arr[1])->getCString());

	if(typ == TYPE_JOIN)
	{
		vector<string> elems;
		stringstream ss(data);
		string item;
		while (std::getline(ss, item, '|')) {
			elems.push_back(item);
		}
		ss.flush();

		int idx = atoi(elems[0].c_str());

		player_info[idx].setPlayerName(elems[1]);
		player_info[idx].setMoney(atoi(elems[2].c_str()));

		updatePlayerInfo();
	} 
	else if(typ == TYPE_STATUS)
	{
		mStatus = atoi(data.c_str());
	}
	else if(typ == TYPE_DRAW_CARD)
	{
		vector<string> elems;
		stringstream ss(data);
		string item;
		while (std::getline(ss, item, '|')) {
			elems.push_back(item);
		}
		ss.flush();

		if(elems[0] == "t")
		{
			for(int i=1; i<elems.size(); i++)
				common_slot->setCard(i-1, elems[i].c_str());
		}
		else
		{
			int idx = atoi(elems[0].c_str());
			for(int i=1; i<elems.size(); i++)
				player_slot[idx]->setCard(i-1, elems[i].c_str());
		}
	} 
	else if(typ == TYPE_MYTURN)
	{
		showPopup(atoi(data.c_str()));
	}
	else if(typ == TYPE_BETTING)
	{
		vector<string> elems;
		stringstream ss(data);
		string item;
		while (std::getline(ss, item, '|')) {
			elems.push_back(item);
		}
		ss.flush();

		betting(atoi(elems[0].c_str()), atoi(elems[1].c_str()), atoi(elems[2].c_str()));
	}
}

bool GameScene::ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
	updatePlayerInfo();

	return true;
}

void GameScene::updateTableInfo()
{
	char szAmount[128];
	sprintf(szAmount, "Total Bet\n$ %d", totalBetting);

	pLabelTotalBet->setString(szAmount);
}

void GameScene::updatePlayerInfo()
{
	for(int i=0; i<4; i++)
	{
		char szAmount[128];
		sprintf(szAmount, "$ %d", player_info[i].getMoney());
		pLabelPlayerMoney[i]->setString(szAmount);

		char szName[128];
		if(player_info[i].getPlayerName().length() > 0)
		{
			sprintf(szName, "Player%02d", i);
			pLabelPlayerName[i]->setString(szName);
			pLabelPlayerName[i]->setColor(ccc3(255, 255, 255));
		}
		else
		{
			pLabelPlayerName[i]->setString("WAITING");
			pLabelPlayerName[i]->setColor(ccc3(80, 80, 80));
		}

		char szOption[128];
		if(player_info[i].getLastOption() == TYPE_CALL)
		{
			sprintf(szOption, "CALL (%d$)", player_info[i].getMoney());
		}
		else if(player_info[i].getLastOption() == TYPE_DOUBLE)
		{
			sprintf(szOption, "DOUBLE (%d$)", player_info[i].getMoney());
		}
		else if(player_info[i].getLastOption() == TYPE_CHECK)
		{
			sprintf(szOption, "CHECK (%d$)", player_info[i].getMoney());
		}
		else if(player_info[i].getLastOption() == TYPE_DIE)
		{
			pLabelPlayerName[i]->setString("(DIE)");
			pLabelPlayerName[i]->setColor(ccc3(80, 80, 80));

			sprintf(szOption, "DIE (0$)");
		}
		else if(player_info[i].getLastOption() == -1)
		{
			sprintf(szOption, "waiting...", player_info[i].getMoney());
		}
		pLabelPlayerStatus[i]->setString(szOption);
	}

	pLabelPlayerName[IDX(0)]->setString(player_info[IDX(0)].getPlayerName().c_str());
}

void GameScene::betting(int player, int type, int amount)
{
	if(type == TYPE_CALL || type == TYPE_DOUBLE)
	{
		totalBetting += amount;
		player_info[player].betting(type, amount);

		char szAmount[128];
		sprintf(szAmount, "%d$", amount);

		CCLabelTTF* ttf = CCLabelTTF::create(szAmount, "Helvetica", 38);
		ttf->setColor(ccc3(200, 200, 0));
		ttf->setPosition(ccp(player_slot[player]->getPositionX(), player_slot[player]->getPositionY()));
		addChild(ttf, 3);

		CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
		CCAction* pAction = CCSequence::create(CCSpawn::create(CCMoveTo::create(0.5f, ccp(visibleSize.width/2, visibleSize.height/2)), CCFadeOut::create(0.5f), NULL), 
			CCRemoveSelf::create(), 
			NULL);
		ttf->runAction(pAction);
	}
	else
	{
		player_info[player].betting(type, 0);
	}

	updatePlayerInfo();

	CCLabelTTF* ttf = CCLabelTTF::create("", "Helvetica", 50);
	if(type == TYPE_CALL) {
		ttf->setColor(ccc3(30, 30, 255));
		ttf->setString("Call");
	} else if(type == TYPE_DOUBLE) {
		ttf->setColor(ccc3(255, 255, 255));
		ttf->setString("Double");
	} else if(type == TYPE_CHECK) {
		ttf->setColor(ccc3(0, 255, 0));
		ttf->setString("Check");
	} else if(type == TYPE_DIE) {
		ttf->setColor(ccc3(255, 0, 0));
		ttf->setString("DIE");
	}

	if(player == 1)
		ttf->setAnchorPoint(ccp(0, 0));
	else if(player == 3)
		ttf->setAnchorPoint(ccp(1.0, 0));
	
	ttf->setPosition(ccp(pLabelPlayerMoney[player]->getPositionX(), pLabelPlayerMoney[player]->getPositionY()));


	addChild(ttf, 3);

	CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
	CCAction* pAction = CCSequence::create(CCSpawn::create(CCMoveBy::create(0.5f, ccp(0, 30)), CCFadeOut::create(0.5f), NULL), 
		CCSpawn::create(CCRemoveSelf::create(), CCCallFunc::create(this, callfunc_selector(GameScene::updateTableInfo)), NULL), 
		NULL);
	ttf->runAction(pAction);
}

void GameScene::menuCloseCallback(CCObject* pSender)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT) || (CC_TARGET_PLATFORM == CC_PLATFORM_WP8)
	CCMessageBox("You pressed the close button. Windows Store Apps do not implement a close button.","Alert");
#else
    CCDirector::sharedDirector()->end();
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
#endif
}

void GameScene::showPopup(int amount) {
	// Remove the scene from the touch dispatcher to prevent anything behind the
	// popup from receiving touches.
	if(!pPopup->isVisible())
	{
		CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
		CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(pMenu);
 
		// Show the popup.
		pPopup->setAmount(amount);
		pPopup->setVisible(true);
		CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(pPopup, 0, true);
	}
}
 
void GameScene::hidePopup(CCObject* pSender) {
	// Hide the popup again.
	if(pPopup->isVisible())
	{
		pPopup->setVisible(false);
		CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(pPopup);
 
		// Add this scene to the touch dispatcher again so that we enable all our touch input.
		CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 0, true);
		CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(pMenu, kCCMenuHandlerPriority, true);

		int val = ((CCInteger*)pSender)->getValue();
		char data[128];
		sprintf(data, "%d", val);

		string result;
		g_Client->request(result, getRoomID(), 101, string(data));
	}
}
 
// 서버로 일정 시간마다 패킷을 보냄
void* _heartbeat(void* args)
{
	int timeout = 0;
	GameScene* scene = (GameScene*)args;

	unsigned int past = CCTime::getTickCount();
	while(1)
	{
		unsigned int now = CCTime::getTickCount();

		if((now - past) > 200) // 2fps로 게임정보 갱신
		{
			string result;
			g_Client->request(result, scene->getRoomID(), TYPE_PING, "");

			if(result.length() > 0)
			{
				timeout = 0;

				vector<string> elems;
				stringstream ss(result);
				string item;
				while (std::getline(ss, item, '\n')) {
					elems.push_back(item);
				}
				ss.flush();

				if(elems[0] == "ok")
				{
					for(int i=1; i<elems.size(); i++)
					{
						char typ = elems[i].at(0);
						string data = elems[i].substr(1);

						CCArray* arr = CCArray::create(CCInteger::create(typ), CCString::create(data), NULL);
						CCNotificationHandler::postNotificationOnMainThread("message", arr);
					}
				}
				else
				{
					CCLog("Server : Error");
				}
			}
			else
			{
				timeout++;
				if(timeout > 10)
					CCMessageBox("Server: Timeout!", "Error");
			}

			past = CCTime::getTickCount();
		}
	}
}
