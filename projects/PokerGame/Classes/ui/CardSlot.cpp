//
//  CardSlot.cpp
//  PokerGame
//
//  Created by BetaS on 13. 11. 10..
//
//

#include "CardSlot.h"

CardSlot::CardSlot()
{
}

CardSlot::~CardSlot()
{
	delete [] m_Cards;
	delete [] m_CardSprites;
}

cocos2d::CCScene* CardSlot::scene()
{
	// 'scene' is an autorelease object
	cocos2d::CCScene *scene = cocos2d::CCScene::create();
	
	// 'layer' is an autorelease object
	CardSlot *layer = CardSlot::create();
	
	// add layer as a child to scene
	scene->addChild(layer);
	
	// return the scene
	return scene;
}

bool CardSlot::init()
{
	bool bRet = false;
	do 
	{
		CC_BREAK_IF(! CCLayer::init());

		ignoreAnchorPointForPosition(false);
		cocos2d::CCSize screenSize = cocos2d::CCDirector::sharedDirector()->getWinSize();
		bRet = true;
	} while (0);

	return bRet;
}

void CardSlot::setSlotSize(int nSize) 
{
	m_nSize = nSize;

	m_Cards = new Card[m_nSize];
	m_CardSprites = new cocos2d::CCSprite*[m_nSize];

	for(int i=0; i<m_nSize; i++)
	{
		m_CardSprites[i] = cocos2d::CCSprite::create("card_deck.png", cocos2d::CCRect(0, 0, 165, 230));
		m_CardSprites[i]->setAnchorPoint(ccp(0, 0));
		m_CardSprites[i]->cocos2d::CCNode::setPosition(i*165, 0);

		addChild(m_CardSprites[i], 0);
	}
	setContentSize(ccp(m_nSize*165, 230));
	cocos2d::CCLog("%f, %f", getContentSize().width, getContentSize().height);
	setAnchorPoint(ccp(0.5f, 0.5f));
	
	update();
}

void CardSlot::update()
{
	for(int i=0; i<m_nSize; i++)
	{
		int row = 0, col = 0;
	
		if(m_Cards[i].getSuit() == -1 || m_Cards[i].getRank() == -1)
		{
			row = 4;
			col = 0;
		}
		else if(m_Cards[i].getSuit() == 0 || m_Cards[i].getRank() == 0)
		{
			row = 4;
			col = 1;
		}
		else
		{
			row = m_Cards[i].getSuit()-1;
			col = m_Cards[i].getRank()-2;
		}
		
		m_CardSprites[i]->setTextureRect(cocos2d::CCRect(col*165, row*230, 165, 230));
	}
}

void CardSlot::setCard(int idx, const char* name)
{
	m_Cards[idx].setCard(name);
	
	update();
}