#include "StringPopup.h"

// on "init" you need to initialize your instance
bool StringPopup::init()
{
	//////////////////////////////
	// 1. super init first
	if ( !CCLayer::init() )
	{
		return false;
	}

	cocos2d::CCSize winSize = cocos2d::CCDirector::sharedDirector()->getWinSize();

	setTouchEnabled(true);

	return initWithColor(ccc4(0,0,0,80), winSize.width, winSize.height);
}


bool StringPopup::ccTouchBegan(cocos2d::CCTouch *touch, cocos2d::CCEvent *event) {
	//hidePopup();
	return true;
}
