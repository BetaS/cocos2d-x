#include "CCNotificationHandler.h"

void CCNotificationHandler::postNotificationOnMainThread(const char *name)
{
	postNotificationOnMainThread(name, NULL);
}

void CCNotificationHandler::postNotificationOnMainThread(const char *name, CCObject *object)
{
	CCObject *target = new CCNotificationHandler(name, object);
	target->retain();
	target->autorelease();

	CCDirector::sharedDirector()->getScheduler()->scheduleSelector(schedule_selector(CCNotificationHandler::postNotification), target, 0, 0, 0, false);
}

CCNotificationHandler::~CCNotificationHandler()
{
	if (_object) _object->release();
}

CCNotificationHandler::CCNotificationHandler(const char *name, CCObject *object)
{
	_name = name;
	_object = object;
	if (_object) _object->retain();
}

void CCNotificationHandler::postNotification(float dt)
{
	CCNotificationCenter::sharedNotificationCenter()->postNotification(_name.c_str(), _object);
//	this->autorelease();
	// CCTimer will unschedule this selector later, and 'this' will be autoreleased by main loop
}