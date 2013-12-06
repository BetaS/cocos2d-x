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

//	string result;
//	GameClient client = GameClient("localhost", 15400);
//	client.request(result, "HEllO");
//	CCMessageBox(result.c_str(), "TEST");
//
//
//	m_Popup = StringPopup::create();
//	this->addChild(m_Popup, 99);
//	m_Popup->setHideCallback(std::bind(&LoginScene::hidePopup, this));
//	m_Popup->setVisible(false);

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

	CCMenuItemImage *pSignupbtn = CCMenuItemImage::create("btn_signup.png", "btn_signup.png", this, menu_selector(LoginScene::menuRegisterCallback));
	pSignupbtn->setPosition(ccp(visibleSize.width/2-95, visibleSize.height/2-105));

	CCMenuItemImage *pLoginbtn = CCMenuItemImage::create("btn_login.png", "btn_login.png", this, menu_selector(LoginScene::menuLoginCallback));
	pLoginbtn->setPosition(ccp(visibleSize.width/2+95, visibleSize.height/2-105));
	
	CCEditBox *idForm = CCEditBox::create(CCSize(250, 30), CCScale9Sprite::create("form_input.png"));
	idForm->setPosition(ccp(visibleSize.width/2+30, visibleSize.height/2-5));
	idForm->setFont("맑은고딕", 15);
	idForm->setFontColor(ccc3(0, 0, 0));
	idForm->setPlaceHolder(_AtoU8("ID를 입력하세요"));
	idForm->setPlaceholderFontColor(ccc3(0, 0, 0));
	this->addChild(idForm, 2, vFormID);

	CCEditBox *pwForm = CCEditBox::create(CCSize(250, 30), CCScale9Sprite::create("form_input.png"));
	pwForm->setPosition(ccp(visibleSize.width/2+30, visibleSize.height/2-50));
	pwForm->setFont("맑은고딕", 15);
	pwForm->setFontColor(ccc3(0, 0, 0));
	pwForm->setPlaceHolder(_AtoU8("비밀번호를 입력하세요"));
	pwForm->setPlaceholderFontColor(ccc3(0, 0, 0));
	pwForm->setInputFlag(kEditBoxInputFlagPassword);
	this->addChild(pwForm, 2, vFormPW);

	// create menu, it's an autorelease object
	pMenu = CCMenu::create(pCloseItem, pSignupbtn, pLoginbtn, NULL);
	pMenu->setPosition(CCPointZero);

	this->addChild(pMenu, 2);

    return true;
}

bool LoginScene::ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
//	CCSprite* pSprite = (CCSprite*)this->getChildByTag(0);
//	pSprite->setTextureRect(CCRect(0, 230, 165, 230));
//	printf("test");
	return true;
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

void LoginScene::menuRegisterCallback(CCObject* pSender)
{
	CCEditBox* form_id = (CCEditBox*)getChildByTag(vFormID);
	CCEditBox* form_pw = (CCEditBox*)getChildByTag(vFormPW);

	JsonBox::Value result;
	JsonBox::Object params, device;
	((AppDelegate*)CCApplication::sharedApplication())->getDeviceInfo()->getJSONString(device);

	params["email"] = JsonBox::Value(form_id->getText());
	params["pw"] = JsonBox::Value(form_pw->getText());
	g_Server.request(result, "authUserRegister", params);

	if((result["result"]["items"][size_t(0)]["result"]).getBoolean())
	{
		// 가입 성공
		CCMessageBox("가입에 성공하였습니다.", "OK");
	}
	else
	{
		// 가입 실패
		//showPopup("");
		CCMessageBox("가입에 실패하였습니다.", "ERROR");
	}
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

	string authkey = (result["result"]["items"][size_t(0)]["authkey"]).getString();

	if(&authkey != NULL && authkey.length() == 64)
	{
		params.clear();
		params["authkey"] = JsonBox::Value(authkey);
		params["device"] = JsonBox::Value(device);
		g_Server.request(result, "authAuthenticate", params);

		if((result["result"]["items"][size_t(0)]["result"]).getBoolean()) 
		{
			CCDirector::sharedDirector()->replaceScene(CCTransitionFade::create(0.5f, RobbyScene::scene()));
		}
		else
		{
			CCMessageBox("서버 세션 에러.", "ERROR");
		}
	}
	else
	{
		CCMessageBox("비밀번호가 일치하지 않습니다.", "ERROR");
	}
}
/*
void LoginScene::showPopup(string text) {
	// Remove the scene from the touch dispatcher to prevent anything behind the
	// popup from receiving touches.
	CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
	CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(pMenu);

	((CCEditBox*)this->getChildByTag(vFormID))->setTouchEnabled(false);
	((CCEditBox*)this->getChildByTag(vFormPW))->setTouchEnabled(false);

	// Show the popup.
	m_Popup->setVisible(true);
	CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(m_Popup, 0, true);
}

void LoginScene::hidePopup() {
	// Hide the popup again.
	m_Popup->setVisible(false);
	CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(m_Popup);

	// Add this scene to the touch dispatcher again so that we enable all our touch input.
	CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, 0, true);
	CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(pMenu, kCCMenuHandlerPriority, true);

	((CCEditBox*)this->getChildByTag(vFormID))->setTouchEnabled(true);
	((CCEditBox*)this->getChildByTag(vFormPW))->setTouchEnabled(true);
}
*/