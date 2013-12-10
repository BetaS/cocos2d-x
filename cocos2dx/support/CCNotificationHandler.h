#pragma once

#ifndef __CCNOTIFICATIONHANDLER_H__
#define __CCNOTIFICATIONHANDLER_H__

#include "CCDirector.h"
#include "CCNotificationCenter.h"
#include "cocoa/CCObject.h"
#include "CCScheduler.h"

using namespace cocos2d;

class CC_DLL CCNotificationHandler : public CCObject {

public:
	static void postNotificationOnMainThread(const char *name);
	static void postNotificationOnMainThread(const char *name, CCObject *object);

private:
	std::string _name;
	CCObject *_object;

	~CCNotificationHandler();
	CCNotificationHandler(const char *name, CCObject *object);
	void postNotification(float dt);

};

#endif