#pragma once
#ifndef __GAMECLIENT_H__
#define __GAMECLIENT_H__

#ifdef WIN32
#include <winsock2.h>
#else
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#endif

#include <string>

using namespace std;

class GameClient
{
private:
	void _setup();
	bool _send(const char* data, size_t size);
	void _recv(string& str);

public:
	GameClient(string host, int port);
	virtual ~GameClient(void);

private:
	struct sockaddr_in m_Client;
	string m_szHost;
	int m_nPort;
	int m_nSock;

public:
	void request(string& result, const char key[9], char type, string data);
};

extern GameClient *g_Client;

#define TYPE_PING		0
#define TYPE_STATUS		1
#define TYPE_JOIN		100
#define TYPE_LEAVE		101
#define TYPE_MYTURN		103
#define TYPE_DRAW_CARD	111
#define TYPE_BETTING	112

#endif
