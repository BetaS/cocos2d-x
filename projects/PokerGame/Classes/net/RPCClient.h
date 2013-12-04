#pragma once
#ifndef __RPCCLIENT_H__
#define __RPCCLIENT_H__

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
#include "../json/JsonBox.h"

using namespace std;

#define SERVER_PATH "localhost"
#define SERVER_PORT 5303

class RPCClient
{
private:
	void _setup();
	void _connect();
	bool _send(string data);
	void _recv(string& str);

public:
	RPCClient(string host, int port);
	virtual ~RPCClient(void);

private:
	struct sockaddr_in m_Client;
	string m_szHost;
	int m_nPort;
	int m_nSock;

public:
	void request(JsonBox::Value& result, char* method, JsonBox::Object params);
};

extern RPCClient g_Server;

#endif
