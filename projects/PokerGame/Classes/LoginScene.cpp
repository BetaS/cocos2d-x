#include "LoginScene.h"
#include "RobbyScene.h"
#include "AppDelegate.h"
#include "net/RPCClient.h"
#include "json/JsonBox.h"

USING_NS_CC;
USING_NS_CC_EXT;

enum VIEW
{
	vFormID,
	vFormPW
};

CCScene* LoginScene::scene()
{
    // 'scene' is an autorelease object
    CCScene *scene = CCScene::create();
    
    // 'layer' is an autorelease object
    LoginScene *layer = LoginScene::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool LoginScene::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !CCLayer::init() )
    {
        return false;
    }

    CCSize visibleSize = CCDirector::sharedDirector()->getVisibleSize();
    CCPoint origin = CCDirector::sharedDirector()->getVisibleOrigin();

	CCSprite *pBackground = CCSprite::create("intro_bg.png");
	pBackground->setPosition(ccp(origin.x + visibleSize.width/2, origin.y + visibleSize.height/2));
	this->addChild(pBackground, 0);

	CCSprite *pLoginBox = CCSprite::create("login_box_bg.png");
	pLoginBox->setPosition(ccp(visibleSize.width/2, visibleSize.height/2-50));
	this->addChild(pLoginBox, 1);

    CCMenuItemImage *pCloseItem = CCMenuItemImage::create("CloseNormal.png", "CloseSelected.png", this, menu_selector(LoginScene::menuCloseCallback));
	pCloseItem->setPosition(ccp(origin.x + visibleSize.width - pCloseItem->getContentSize().width/2, origin.y + pCloseItem->getContentSize().height/2));

	CCMenuItemImage *pSignupbtn = CCMenuItemImage::create("btn_signup.png", "btn_signup.png", this, menu_selector(LoginScene::menuCloseCallback));
	pSignupbtn->setPosition(ccp(visibleSize.width/2-95, visibleSize.height/2-105));

	CCMenuItemImage *pLoginbtn = CCMenuItemImage::create("btn_login.png", "btn_login.png", this, menu_selector(LoginScene::menuLoginCallback));
	pLoginbtn->setPosition(ccp(visibleSize.width/2+95, visibleSize.height/2-105));

    // create menu, it's an autorelease object
    CCMenu* pMenu = CCMenu::create(pCloseItem, pSignupbtn, pLoginbtn, NULL);
    pMenu->setPosition(CCPointZero);
    this->addChild(pMenu, 2);
	
	CCEditBox *text = CCEditBox::create(CCSize(250, 30), CCScale9Sprite::create("form_input.png"));
	text->setPosition(ccp(visibleSize.width/2+30, visibleSize.height/2-5));
	text->setFont("맑은고딕", 15);
	text->setFontColor(ccc3(0, 0, 0));
	text->setPlaceHolder(_AtoU8("ID를 입력하세요"));
	text->setPlaceholderFontColor(ccc3(0, 0, 0));
	this->addChild(text, 2, vFormID);

	text = CCEditBox::create(CCSize(250, 30), CCScale9Sprite::create("form_input.png"));
	text->setPosition(ccp(visibleSize.width/2+30, visibleSize.height/2-50));
	text->setFont("맑은고딕", 15);
	text->setFontColor(ccc3(0, 0, 0));
	text->setPlaceHolder(_AtoU8("비밀번호를 입력하세요"));
	text->setPlaceholderFontColor(ccc3(0, 0, 0));
	text->setInputFlag(kEditBoxInputFlagPassword);
	this->addChild(text, 2, vFormPW);

    return true;
}

void LoginScene::ccTouchesBegan(cocos2d::CCSet *pTouches, cocos2d::CCEvent *pEvent)
{
//	CCSprite* pSprite = (CCSprite*)this->getChildByTag(0);
//	pSprite->setTextureRect(CCRect(0, 230, 165, 230));
//	printf("test");
}


void LoginScene::menuCloseCallback(CCObject* pSender)
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

void LoginScene::menuLoginCallback(CCObject* pSender)
{
	CCEditBox* form_id = (CCEditBox*)getChildByTag(vFormID);
	CCEditBox* form_pw = (CCEditBox*)getChildByTag(vFormPW);

	JsonBox::Value result;
	JsonBox::Object params, device;
	((AppDelegate*)CCApplication::sharedApplication())->getDeviceInfo()->getJSONString(device);

	params["email"] = JsonBox::Value(form_id->getText());
	params["pw"] = JsonBox::Value(form_pw->getText());
	params["device"] = JsonBox::Value(device);
	g_Server.request(result, "authUserLogin", params);

	CCLog("authkey: %s", (result["result"]["items"][size_t(0)]["authkey"]).getString().c_str());
	//CCDirector::sharedDirector()->replaceScene(CCTransitionFade::create(0.5f, RobbyScene::scene()));
}
