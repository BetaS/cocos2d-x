#ifndef __GAME_SCENE_H__
#define __GAME_SCENE_H__

#include "cocos2d.h"
#include "data/PlayerInfo.h"
#include "ui/CardSlot.h"
#include "ui/BettingPopup.h"

#include <queue>
#include <pthread.h>

void* _heartbeat(void* arg);

class GameScene : public cocos2d::CCLayer
{
private:
	CCMenu* pMenu;
	
	CCLabelTTF* pLabelTotalBet;
	CCLabelTTF* pLabelPlayerName[4];
	CCLabelTTF* pLabelPlayerMoney[4];
	CCLabelTTF* pLabelPlayerStatus[4];

	pthread_t thread;

	int mStatus;
	int myIdx;

public:
	GameScene();
	virtual ~GameScene();
	
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init(); 
	void initPlayerUI();

    // there's no 'id' in cpp, so we recommend returning the class instance pointer
	static cocos2d::CCScene* scene(const char* key, int idx, std::string name, int money);

	// a selector callback
	void menuCloseCallback(CCObject* pSender);
	virtual bool ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent);

	void onMessage(CCObject* obj);

	// implement the "static node()" method manually
	CREATE_FUNC(GameScene);
    
private:
	char key[8];

	int totalBetting;
	CardSlot* common_slot;

public:
	char* getRoomID() { return key; }
	void updateTableInfo();

private:
	PlayerInfo player_info[4];
	CardSlot* player_slot[4];

public:
	void updatePlayerInfo();
	void betting(int player, int type, int amount);

private:
	BettingPopup* pPopup;

public:
	void showPopup(int amount);
	void hidePopup(CCObject* pSender);
};

#endif // __GAME_SCENE_H__
