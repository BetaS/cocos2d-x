#include "GameScene.h"
#include "AppDelegate.h"
#include "net/GameClient.h"
#include "json/JsonBox.h"

USING_NS_CC;

GameScene::~GameScene()
{
	if ( thread )
    {
        pthread_detach(thread);
		
        thread = NULL;
    }
}

CCScene* GameScene::scene(const char* key)
{
    // 'scene' is an autorelease object
    CCScene *scene = CCScene::create();
    
    // 'layer' is an autorelease object
    GameScene *layer = GameScene::create();
	memcpy(layer->key, key, 8);

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
	
	this->addChild(pPopup, 99);
	
	this->setTouchEnabled(true);
	CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 0, true);
	
	common_slot = CardSlot::create();
	common_slot->setSlotSize(3);
	common_slot->setScale(0.55f);
	common_slot->setPosition(ccp(visibleSize.width/2, visibleSize.height/2+100));
	this->addChild(common_slot, 1);

	for(int i=0; i<4; i++) 
	{
		player_slot[i] = CardSlot::create();
		player_slot[i]->setSlotSize(4);
	}

	player_slot[0]->setScale(0.65f);
	player_slot[0]->setPosition(ccp(visibleSize.width/2, 0+player_slot[0]->getContentSize().height*player_slot[0]->getScale()/2));
	this->addChild(player_slot[0], 1);

	player_slot[1]->setScale(0.6f);
	player_slot[1]->setPosition(ccp(0+player_slot[1]->getContentSize().height*player_slot[1]->getScale()/2, visibleSize.height/2));
	player_slot[1]->setRotation(-90);
	this->addChild(player_slot[1], 1);

	player_slot[2]->setScale(0.6f);
	player_slot[2]->setPosition(ccp(visibleSize.width/2, visibleSize.height-player_slot[2]->getContentSize().height*player_slot[2]->getScale()/2));
	this->addChild(player_slot[2], 1);

	player_slot[3]->setScale(0.6f);
	player_slot[3]->setPosition(ccp(visibleSize.width-player_slot[3]->getContentSize().height*player_slot[3]->getScale()/2, visibleSize.height/2));
	player_slot[3]->setRotation(90);
	this->addChild(player_slot[3], 1);
	
	pTotalBet = CCLabelTTF::create("", "Helvetica", 12);
	pTotalBet->setPosition(ccp(visibleSize.width/2, 300));
	this->addChild(pTotalBet, 2);
	
	pthread_create(&thread, NULL, _heartbeat, this);

    return true;
}

void* _heartbeat(void* args)
{
	GameScene* scene = (GameScene*)args;
	
	unsigned int past = CCTime::getTickCount();
	while(1)
	{
		unsigned int now = CCTime::getTickCount();
		
		if((now - past) > 200) // 5fps로 게임정보 갱신
		{
			string result;
			g_Client->request(result, scene->getRoomID(), TYPE_PING, "");
			
			past = CCTime::getTickCount();
		}
	}
}

bool GameScene::ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
	showPopup();
	
	return true;
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

void GameScene::showPopup() {
	// Remove the scene from the touch dispatcher to prevent anything behind the
	// popup from receiving touches.
	CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
	CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(pMenu);
 
	// Show the popup.
	pPopup->setVisible(true);
	CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(pPopup, 0, true);
}
 
void GameScene::hidePopup(CCObject* pSender) {
	// Hide the popup again.
	pPopup->setVisible(false);
	CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(pPopup);
 
	// Add this scene to the touch dispatcher again so that we enable all our touch input.
	CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 0, true);
	CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(pMenu, kCCMenuHandlerPriority, true);
}
 