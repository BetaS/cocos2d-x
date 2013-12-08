#include "cocos2d.h"
#include <functional>

USING_NS_CC;

class StringPopup: public CCLayerColor
{
private:
	//std::function<void(void)> hidePopup;

public:
	virtual bool init();

	virtual bool ccTouchBegan(cocos2d::CCTouch *touch, cocos2d::CCEvent *event);

	//void setHideCallback(std::function<void(void)> callback) {this->hidePopup = callback;}

	CREATE_FUNC(StringPopup);
};
