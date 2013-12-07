//
//  GameRoomInfo.cpp
//  PokerGame
//
//  Created by BetaS on 13. 12. 7..
//
//

#include "GameRoomInfo.h"

GameRoomInfo::GameRoomInfo(JsonBox::Value data)
{
	key = data["room_id"].getString();
	
	addr = data["addr"].getString();
	port = data["port"].getInt();
	
	curr_user = data["user_cnt"].getInt();
	max_user = data["max_user_cnt"].getInt();
}

GameRoomInfo::~GameRoomInfo()
{
}