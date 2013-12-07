﻿#include "GameClient.h"
#include "cocos2d.h"

GameClient* g_Client;

GameClient::GameClient(string host, int port)
	:m_szHost(host), m_nPort(port)
{
#ifdef WIN32
	WSADATA wsaData;
	WSAStartup(MAKEWORD(2,2),&wsaData);
#endif
	_setup();
}


GameClient::~GameClient(void)
{
#ifdef WIN32
	WSACleanup();
#endif
}

void GameClient::_setup() {
	struct hostent * host = gethostbyname(m_szHost.c_str());

	if ( (host == NULL) || (host->h_addr == NULL) ) {
		cocos2d::CCLog("Can not reach host.");
	}

	memset(&m_Client, 0, sizeof(m_Client));
	m_Client.sin_family = AF_INET;
	m_Client.sin_port = htons( m_nPort );
	memcpy(&m_Client.sin_addr, host->h_addr, host->h_length);

	m_nSock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

	if (m_nSock < 0) {
		cocos2d::CCLog("can not open socket.");
	}
}

bool GameClient::_send(const char* data, size_t size) {
	cocos2d::CCLog("%s", data);

	if(sendto(m_nSock, data, size, 0, (struct sockaddr*)&m_Client, sizeof(m_Client)) == -1) {
		cocos2d::CCLog("can not send data");
		return false;
	}
	return true;
}

void GameClient::_recv(string& str) {
	str = "";

	char cur[1024]; int size=0;

	while( (size = recv(m_nSock, cur, sizeof(cur), 0)) > 0 ) 
	{
		str += string(cur, size);
		
		if (size == 0 || size < sizeof(cur)) 
		{
			cocos2d::CCLog("Connection closed\n");
			break;
		}
	}

	cocos2d::CCLog("result: %s", str.c_str());
}

void GameClient::request(string& result, const char key[9], char type, string data)
{
	result = "";
	
	int length = 9+data.length()+1;
	char* packet = new char[length];
	
	for(int i=0; i<8; i++)
		packet[i] = key[i];
	packet[8] = type;
	
	const char* pData = data.c_str();
	for(int i=0; i<data.length(); i++)
		packet[9+i] = pData[i];
	
	packet[length-1] = '\0';
	
	if(_send(packet, length))
		_recv(result);
		
	delete[] packet;
}
