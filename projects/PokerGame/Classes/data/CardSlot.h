//
//  CardSlot.h
//  PokerGame
//
//  Created by BetaS on 13. 11. 10..
//
//

#ifndef __PokerGame__CardSlot__
#define __PokerGame__CardSlot__

#include "cocos2d.h"
#include "Card.h"

class CardSlot: public cocos2d::CCLayer
{
public:
	CardSlot();
	virtual ~CardSlot();

public:
	virtual bool init();

	static cocos2d::CCScene* scene();
	
private:
	int m_nSize;
	
	Card* m_Cards;
	cocos2d::CCSprite** m_CardSprites;
	
private:
	void update();
	
public:
	void setSlotSize(int size);
	void setCard(int idx, const char* name);

	CREATE_FUNC(CardSlot);
};

#endif /* defined(__PokerGame__CardSlot__) */
