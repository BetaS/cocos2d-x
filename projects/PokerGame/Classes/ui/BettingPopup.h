#include "cocos2d.h"
#include <functional>

USING_NS_CC;

class BettingPopup: public CCLayerColor
{
private:
	CCMenuItemLabel *label1, *label2, *label3, *label4;

public:
	virtual bool init();

	virtual bool ccTouchBegan(CCTouch *touch, CCEvent *event);
	
	void setAmount(int amount);

	void menuBtnCall(CCObject* pSender);
	void menuBtnDouble(CCObject* pSender);
	void menuBtnCheck(CCObject* pSender);
	void menuBtnDie(CCObject* pSender);
	
	CREATE_FUNC(BettingPopup);
};
