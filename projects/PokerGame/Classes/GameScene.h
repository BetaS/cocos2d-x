#ifndef __GAME_SCENE_H__
#define __GAME_SCENE_H__

#include "cocos2d.h"
#include "ui/CardSlot.h"
#include "ui/BettingPopup.h"
#include <pthread.h>

void* _heartbeat(void* arg);

class GameScene : public cocos2d::CCLayer
{
private:
	CardSlot* common_slot;
	CardSlot* player_slot[4];
	
	CCMenu* pMenu;
	
	CCLabelTTF* pTotalBet;
	CCLabelTTF* pPlayerInfo[4];
	
	BettingPopup* pPopup;
	
	pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
	pthread_t thread;
	
	char key[8] = "";
	
public:
	virtual ~GameScene();
	
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init(); 

    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::CCScene* scene(const char* key);
    
	char* getRoomID() { return key; }
	
    // a selector callback
    void menuCloseCallback(CCObject* pSender);
	
	void showPopup();
	void hidePopup(CCObject* pSender);
	
	virtual bool ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);
    
    // implement the "static node()" method manually
    CREATE_FUNC(GameScene);
};

#endif // __GAME_SCENE_H__
