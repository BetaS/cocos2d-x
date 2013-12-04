//
//  Card.cpp
//  PokerGame
//
//  Created by BetaS on 13. 11. 10..
//
//

#include "Card.h"

void Card::setCard(const char name[2])
{
	if(name[1] == 's')
		m_nSuit = 1;
	else if(name[1] == 'h')
		m_nSuit = 2;
	else if(name[1] == 'd')
		m_nSuit = 3;
	else if(name[1] == 'c')
		m_nSuit = 4;
	
	if(name[0] == 'T')
		m_nRank = 10;
	else if(name[0] == 'J')
		m_nRank = 11;
	else if(name[0] == 'Q')
		m_nRank = 12;
	else if(name[0] == 'K')
		m_nRank = 13;
	else if(name[0] == 'A')
		m_nRank = 14;
	else
		m_nRank = (int)(name[0])-48;
}

void Card::getCard(char*& dest)
{
	if(m_nSuit == 1)
		dest[1] = 's';
	else if(m_nSuit == 2)
		dest[1] = 'h';
	else if(m_nSuit == 3)
		dest[1] = 'd';
	else if(m_nSuit == 4)
		dest[1] = 'c';
	
	if(m_nRank == 10)
		dest[0] = 'T';
	else if(m_nRank == 11)
		dest[0] = 'J';
	else if(m_nRank == 12)
		dest[0] = 'Q';
	else if(m_nRank == 13)
		dest[0] = 'K';
	else if(m_nRank == 14)
		dest[0] = 'A';
	else
		dest[0] = m_nRank+48;
}

int Card::GetSpriteIndex()
{
	int result = -1;
	
	result = (m_nSuit-1)*13;
	result += m_nRank-2;
	
	if(result < 0 || result > 13*4)
		return -1;
	
	return result;
}