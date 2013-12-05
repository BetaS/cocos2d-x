#pragma once
#ifndef __UDPCLIENT_H__
#define __UDPCLIENT_H__

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

class UDPClient
{
private:
	void _setup();
	void _connect();
	bool _send(string data);
	void _recv(string& str);

public:
	UDPClient(string host, int port);
	virtual ~UDPClient(void);

private:
	struct sockaddr_in m_Client;
	string m_szHost;
	int m_nPort;
	int m_nSock;

public:
	void request(string& result, string data);
};

#endif
