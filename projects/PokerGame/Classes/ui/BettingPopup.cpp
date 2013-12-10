#include "BettingPopup.h"

// on "init" you need to initialize your instance
bool BettingPopup::init()
{
	//////////////////////////////
	// 1. super init first
	if ( !CCLayer::init() )
	{
		return false;
	}

	cocos2d::CCSize winSize = cocos2d::CCDirector::sharedDirector()->getWinSize();

	setTouchEnabled(true);
	
    label1 = CCMenuItemLabel::create(CCLabelTTF::create(_AtoU8("Call\n($0)"), "Helvetica", 32), this, menu_selector(BettingPopup::menuBtnCall));
	label1->setPosition(ccp(-90, 75));
	
    label2 = CCMenuItemLabel::create(CCLabelTTF::create(_AtoU8("Double\n($0)"), "Helvetica", 32), this, menu_selector(BettingPopup::menuBtnDouble));
	label2->setPosition(ccp(90, 75));
	
    label3 = CCMenuItemLabel::create(CCLabelTTF::create(_AtoU8("Check\n($0)"), "Helvetica", 32), this, menu_selector(BettingPopup::menuBtnCheck));
	label3->setPosition(ccp(-90, -75));
	
    label4 = CCMenuItemLabel::create(CCLabelTTF::create(_AtoU8("Die\n"), "Helvetica", 32), this, menu_selector(BettingPopup::menuBtnDie));
	label4->setPosition(ccp(90, -75));
	
    // create menu, it's an autorelease object
    CCMenu* pMenu = CCMenu::create(label1, label2, label3, label4, NULL);
    pMenu->setPosition(ccp(winSize.width/2, winSize.height/2));
    this->addChild(pMenu, 99);


	return initWithColor(ccc4(0,0,0,150), winSize.width, winSize.height);
}

void BettingPopup::setAmount(int amount)
{
	char str[128];
	sprintf(str, "Call\n($%d)", amount);
	label1->setString(str);

	sprintf(str, "Double\n($%d)", amount*2);
	label2->setString(str);
}

bool BettingPopup::ccTouchBegan(CCTouch *touch, CCEvent *event)
{
	return true;
}

void BettingPopup::menuBtnCall(CCObject* pSender)
{
	CCNotificationCenter::sharedNotificationCenter()->postNotification("betting", CCInteger::create(0));
}

void BettingPopup::menuBtnDouble(CCObject* pSender)
{
	CCNotificationCenter::sharedNotificationCenter()->postNotification("betting", CCInteger::create(1));
}

void BettingPopup::menuBtnCheck(CCObject* pSender)
{
	CCNotificationCenter::sharedNotificationCenter()->postNotification("betting", CCInteger::create(2));
}

void BettingPopup::menuBtnDie(CCObject* pSender)
{
	CCNotificationCenter::sharedNotificationCenter()->postNotification("betting", CCInteger::create(3));
}