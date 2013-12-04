#include "RPCClient.h"
#include "cocos2d.h"

RPCClient g_Server(SERVER_PATH, SERVER_PORT);

RPCClient::RPCClient(string host, int port)
	:m_szHost(host), m_nPort(port)
{
#ifdef WIN32
	WSADATA wsaData;
	WSAStartup(MAKEWORD(2,2),&wsaData);
#endif
	_setup();
}


RPCClient::~RPCClient(void)
{
#ifdef WIN32
	WSACleanup();
#endif
}

void RPCClient::_setup() {
	struct hostent * host = gethostbyname(m_szHost.c_str());

	if ( (host == NULL) || (host->h_addr == NULL) ) {
		cocos2d::CCLog("Can not reach host.");
	}

	memset(&m_Client, 0, sizeof(m_Client));
	m_Client.sin_family = AF_INET;
	m_Client.sin_port = htons( m_nPort );
	memcpy(&m_Client.sin_addr, host->h_addr, host->h_length);

	m_nSock = socket(AF_INET, SOCK_STREAM, 0);

	if (m_nSock < 0) {
		cocos2d::CCLog("can not open socket.");
	}

	_connect();
}

void RPCClient::_connect() {
	if ( connect(m_nSock, (struct sockaddr *)&m_Client, sizeof(m_Client)) < 0 ) {
#ifdef WIN32
		closesocket(m_nSock);
#else
		close(m_nSock);
#endif
		cocos2d::CCLog("can not open connect");
	}
}

bool RPCClient::_send(string data) {
	cocos2d::CCLog("%s", data.c_str());

	char length[15];
	sprintf(length, "%d", data.length());
	cocos2d::CCLog("length: %s", length);

	string request = "";
	request += "POST / HTTP/1.1\r\n"; 
	request += "Host: "+string(SERVER_PATH)+"\r\n";
	request += "User-Agent: cocos2dx\r\n";
	request += "Content-Type: text/json\r\n";
	request += "Content-Length: "+string(length)+"\r\n";
	request += "Accept-Charset: utf-8\r\n\r\n";
	request += string(data);
	request += "\r\n";
	
	if (send(m_nSock, request.c_str(), request.length(), 0) != (int)request.length()) {
		cocos2d::CCLog("can not send data");
		return false;
	}
	return true;
}
void RPCClient::_recv(string& str) {
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

	cocos2d::CCLog("size: %d", size);
	size = str.find("\r\n\r\n")+4;
	str = str.substr(size);

	cocos2d::CCLog("result: %s", str.c_str());
}

void RPCClient::request(JsonBox::Value& json, char* method, JsonBox::Object params)
{
	string result = "";
	stringstream sstream("");

	JsonBox::Object data;
	data["jsonrpc"] = JsonBox::Value("2.0");
	data["id"] = JsonBox::Value("1");
	data["method"] = JsonBox::Value(method);
	data["params"] = JsonBox::Value(params);

	sstream << data;
//
//	_connect();
	if(_send(sstream.str()))
		_recv(result);
//	
//#ifdef WIN32
//	closesocket(m_nSock);
//#else
//	close(m_nSock);
//#endif

	json.loadFromString(result);
}
