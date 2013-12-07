#ifndef __GAMEROOMINFO__
#define __GAMEROOMINFO__

#include "JsonBox.h"
#include <string>

using namespace std;

class GameRoomInfo
{
public:
	GameRoomInfo(JsonBox::Value data);
	virtual ~GameRoomInfo();
	
private:
	string key;
	
	string addr;
	int port;
	
	int curr_user;
	int max_user;
	
public:
	string getSessionKey() {return key;}
	
	string getAddress() {return addr;}
	int getPort() {return port;}
	
	int getUserCnt() {return curr_user;}
	int getMaxUser() {return max_user;}
	
	bool isFull() {return (curr_user >= max_user);}
};

#endif
