#ifndef __LOGIN_SCENE_H__
#define __LOGIN_SCENE_H__

#include <string>
#include "cocos2d.h"
#include "cocos-ext.h"
#include "ui/StringPopup.h"

using namespace std;

class LoginScene : public cocos2d::CCLayer
{
private:
	StringPopup* m_Popup;
	CCMenu* pMenu;

public:
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init(); 

    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::CCScene* scene();
    
    // a selector callback
	void menuCloseCallback(CCObject* pSender);
	void menuRegisterCallback(CCObject* pSender);
	void menuLoginCallback(CCObject* pSender);
	
	virtual bool ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);

//	void showPopup(string text);
//	void hidePopup();
    
    // implement the "static node()" method manually
    CREATE_FUNC(LoginScene);
};

#endif // __LOGIN_SCENE_H__
