#include "GameScene.h"
#include "AppDelegate.h"
#include "net/RPCClient.h"
#include "json/JsonBox.h"

USING_NS_CC;

CCScene* GameScene::scene()
{
    // 'scene' is an autorelease object
    CCScene *scene = CCScene::create();
    
    // 'layer' is an autorelease object
    GameScene *layer = GameScene::create();

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

	JsonBox::Value result;
	JsonBox::Object params, device;
	((AppDelegate*)CCApplication::sharedApplication())->getDeviceInfo()->getJSONString(device);

	params["device"] = JsonBox::Value(device);
	g_Server.request(result, "userLogin", params);

	CCLog("json: %d", (result["result"]["items"][size_t(0)]["ret"]).getInt());
    
    CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
    CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();

	CCSprite *pBackground = CCSprite::create("table_bg.png");
	pBackground->setPosition(ccp(origin.x + visibleSize.width/2, origin.y + visibleSize.height/2));
	this->addChild(pBackground, 0);

    // add a "close" icon to exit the progress. it's an autorelease object
//	CCMenuItemImage *pEnterItem = CCMenuItemImage::create(
//										"GameEnter.png" 
//										"GameEnter.png",
//										this,
//										menu_selector(HelloWorld::menuEnterCallback));

    CCMenuItemImage *pCloseItem = CCMenuItemImage::create(
                                        "CloseNormal.png",
                                        "CloseSelected.png",
                                        this,
                                        menu_selector(GameScene::menuCloseCallback));
    
	pCloseItem->setPosition(ccp(origin.x + visibleSize.width - pCloseItem->getContentSize().width/2 ,
                                origin.y + pCloseItem->getContentSize().height/2));

    // create menu, it's an autorelease object
    CCMenu* pMenu = CCMenu::create(pCloseItem, NULL);
    pMenu->setPosition(CCPointZero);
    this->addChild(pMenu, 2);
	
//	pMenu = CCMenu::create(pEnterItem, NULL);
//	pMenu->setPosition(ccp(20, 20)) ;
//	this->addChild(pMenu, 1);
//
	this->setTouchEnabled(true);
	
	common_slot = CardSlot::create();
	common_slot->setSlotSize(3);
	common_slot->setScale(0.45f);
	common_slot->setPosition(ccp(visibleSize.width/2, visibleSize.height/2+100));
	this->addChild(common_slot, 1);

	for(int i=0; i<4; i++) 
	{
		player_slot[i] = CardSlot::create();
		player_slot[i]->setSlotSize(4);

		player_slot[i]->setCard(0, "As");
		player_slot[i]->setCard(1, "2d");
		player_slot[i]->setCard(2, "3h");
		player_slot[i]->setCard(3, "Kc");
	}

	player_slot[0]->setScale(0.55f);
	player_slot[0]->setPosition(ccp(visibleSize.width/2, 0+player_slot[0]->getContentSize().height*player_slot[0]->getScale()/2));
	this->addChild(player_slot[0], 1);

	player_slot[1]->setScale(0.5f);
	player_slot[1]->setPosition(ccp(0+player_slot[1]->getContentSize().height*player_slot[1]->getScale()/2, visibleSize.height/2));
	player_slot[1]->setRotation(-90);
	this->addChild(player_slot[1], 1);

	player_slot[2]->setScale(0.5f);
	player_slot[2]->setPosition(ccp(visibleSize.width/2, visibleSize.height-player_slot[2]->getContentSize().height*player_slot[2]->getScale()/2));
	this->addChild(player_slot[2], 1);

	player_slot[3]->setScale(0.5f);
	player_slot[3]->setPosition(ccp(visibleSize.width-player_slot[3]->getContentSize().height*player_slot[3]->getScale()/2, visibleSize.height/2));
	player_slot[3]->setRotation(90);
	this->addChild(player_slot[3], 1);

    return true;
}

void GameScene::ccTouchesBegan(cocos2d::CCSet *pTouches, cocos2d::CCEvent *pEvent)
{
//	CCSprite* pSprite = (CCSprite*)this->getChildByTag(0);
//	pSprite->setTextureRect(CCRect(0, 230, 165, 230));
//	printf("test");
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
