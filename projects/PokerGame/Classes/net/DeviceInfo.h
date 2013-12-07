#pragma once
#ifndef __DEVICEINFO_H__
#define __DEVICEINFO_H__

#include <string>
#include "../json/JsonBox.h"

#define GAME_VERSION ("0.0.1a")

using namespace std;

class DeviceInfo
{
public:
	DeviceInfo(string device_code, string os_type, string os_ver, string model);
	~DeviceInfo(void);

private:
	string authkey;
	string device_code;
	string device_type;
	string os_type;
	string os_ver;
	string model;
	string version;

public:
	string getAuthKey();
	void setAuthKey(string authkey);
	void getJSONString(JsonBox::Object& dest);
};

#endif
