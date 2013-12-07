#include "DeviceInfo.h"

DeviceInfo::DeviceInfo(string device_code, string os_type, string os_ver, string model)
{
	this->device_code = device_code;
#ifdef WIN32
	this->device_type = "pc";
#else
	this->device_type = "phone";
#endif

	this->os_type = os_type;
	this->os_ver = os_ver;
	this->model = model;
	this->version = GAME_VERSION;
}

DeviceInfo::~DeviceInfo(void)
{
}

void DeviceInfo::setAuthKey(string authkey)
{
	this->authkey = authkey;
}
string DeviceInfo::getAuthKey()
{
	return authkey;
}

void DeviceInfo::getJSONString(JsonBox::Object& dest)
{
	dest["code"] = JsonBox::Value(this->device_code);
	dest["type"] = JsonBox::Value(this->device_type);
	dest["os_type"] = JsonBox::Value(this->os_type);
	dest["os_ver"] = JsonBox::Value(this->os_ver);
	dest["model"] = JsonBox::Value(this->model);
	dest["version"] = JsonBox::Value(this->version);
}
