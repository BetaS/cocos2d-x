//
//  Card.h
//  PokerGame
//
//  Created by BetaS on 13. 11. 9..
//
//

#ifndef PokerGame_Card_h
#define PokerGame_Card_h

class Card
{	
public:
	Card(): m_nSuit(0), m_nRank(0) {}
	virtual ~Card() {}

private:
	int m_nSuit;
	int m_nRank;
	
public:
	void setSuit(int suit) { m_nSuit = suit; }
	void setRank(int rank) { m_nRank = rank; }
	void setCard(int suit, int rank) { setSuit(suit); setRank(rank); }
	void setCard(const char name[2]);
	
	int getSuit() { return m_nSuit; }
	int getRank() { return m_nRank; }
	void getCard(char*& dest);
 	
public:
	int GetSpriteIndex();
};


#endif
