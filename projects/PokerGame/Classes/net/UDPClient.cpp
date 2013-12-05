#include "UDPClient.h"
#include "cocos2d.h"

UDPClient::UDPClient(string host, int port)
	:m_szHost(host), m_nPort(port)
{
#ifdef WIN32
	WSADATA wsaData;
	WSAStartup(MAKEWORD(2,2),&wsaData);
#endif
	_setup();
}


UDPClient::~UDPClient(void)
{
#ifdef WIN32
	WSACleanup();
#endif
}

void UDPClient::_setup() {
	struct hostent * host = gethostbyname(m_szHost.c_str());

	if ( (host == NULL) || (host->h_addr == NULL) ) {
		cocos2d::CCLog("Can not reach host.");
	}

	memset(&m_Client, 0, sizeof(m_Client));
	m_Client.sin_family = AF_INET;
	m_Client.sin_port = htons( m_nPort );
	memcpy(&m_Client.sin_addr, host->h_addr, host->h_length);

	m_nSock = socket(AF_INET, SOCK_DGRAM, 0);

	if (m_nSock < 0) {
		cocos2d::CCLog("can not open socket.");
	}

	_connect();
}

void UDPClient::_connect() {
	if ( connect(m_nSock, (struct sockaddr *)&m_Client, sizeof(m_Client)) < 0 ) {
#ifdef WIN32
		closesocket(m_nSock);
#else
		close(m_nSock);
#endif
		cocos2d::CCLog("can not open connect");
	}
}

bool UDPClient::_send(string data) {
	cocos2d::CCLog("%s", data.c_str());

	if (send(m_nSock, data.c_str(), data.length(), 0) != (int)data.length()) {
		cocos2d::CCLog("can not send data");
		return false;
	}
	return true;
}
void UDPClient::_recv(string& str) {
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

void UDPClient::request(string& result, string data)
{
	result = "";

	if(_send(data))
		_recv(result);
}
