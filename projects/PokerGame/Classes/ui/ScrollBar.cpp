#include "ScrollBar.h"

#define barFixSize 10.0f
#define barMinSize 20.0f

ScrollBar::ScrollBar (CCScrollView* sv, int type) {
    this->type = type;
    fadeDelayTime = 1.0f;
    canFade = true;

    this->setAnchorPoint(ccp(0,0));
    if ( !CCLayerColor::initWithColor(ccc4(0, 255, 0, 0)) )
    {
        return;
    }
    
    CCPoint svPosition = sv->getPosition();

    float offset;
    if(type <= type_Vertical_out) {
        bar = CCScale9Sprite::create("barV.png",
                                     CCRectMake(0, 0, barFixSize, barMinSize),
                                     CCRectMake(1, 4, 8, 12));

        bar->setAnchorPoint(ccp(0.5,0));
        bar->setPosition(ccp(bar->getContentSize().width/2, 0));
        CCSize svSize = sv->cocos2d::CCNode::getContentSize();
        svFixSize = svSize.height;
        nowContentSize = beforContentSize = sv->getContentSize().height;
        offset = sv->getContentOffset().y;
        if(type == type_Vertical_in) {
            this->setPosition((svPosition.x+svSize.width - barFixSize), svPosition.y);
        } else {
            this->setPosition((svPosition.x+svSize.width), svPosition.y);
        }
    } else {
        bar = CCScale9Sprite::create("barH.png",
                                     CCRectMake(0, 0, barMinSize, barFixSize),
                                     CCRectMake(4, 1, 12, 8));

        bar->setAnchorPoint(ccp(0.0f,0.5f));
        CCSize svSize = sv->cocos2d::CCNode::getContentSize();
        svFixSize = svSize.width;
        nowContentSize = beforContentSize = sv->getContentSize().width;
        offset = sv->getContentOffset().x;
        if(type == type_Horizontal_in) {
            this->setPosition(svPosition.x, svPosition.y + ((bar->getContentSize().height/2)+1));
        } else {
            this->setPosition(svPosition.x, svPosition.y + (-1 * ((bar->getContentSize().height/2)+1)));
        }
    }
    
    if(nowContentSize <= svFixSize) this->setVisible(false);
    else this->setVisible(true);
    
    this->addChild(bar);
    setBarScale();
    setBarPosition(offset);

    CCNode* parent = sv->getParent();
    parent->addChild(this, 1000, 1000);
    scheduleOnce(schedule_selector(ScrollBar::fadeOut), fadeDelayTime);
}

ScrollBar::~ScrollBar() {
}

ScrollBar* ScrollBar::create(CCScrollView* sv, int type) {
    ScrollBar* obj = new ScrollBar(sv, type);
    if(obj) {
        obj->autorelease();

        return obj;
    } else {
		delete obj;

        return NULL;
    }
}

void ScrollBar::fadeOut (float dt) {
    if(canFade) {
        float x, y;
        if(type<= type_Vertical_out) {
            x = 0.0f;
            y = 1.0f;
        } else {
            x = 1.0f;
            y = 0.0f;
        }
       CCAction* action = CCScaleTo::create(0.1f, x, y);
        bar->runAction(action);
    }
}

void ScrollBar::fadeIn (float dt) {
    if(canFade) {
        CCAction* action = CCScaleTo::create(0.05f, 1.0f, 1.0f);
        bar->runAction(action);
    }
}

void ScrollBar::setBarPosition(float offset) {
    offset *= -1;

    float result = (((offset-svFixSize)/nowContentSize) * svFixSize) + barSize;
    CCLog("result : %3.2f offset : %3.2f contentSize : %3.2f svFixSize : %3.2f bar-height : %3.2f ", result, offset, nowContentSize, svFixSize, barSize);
    if(type <= type_Vertical_out) {
        if(result < 0|| result > (svFixSize - barSize + 1)) return;
        
        bar->setPosition(ccp(bar->getPosition().x, result));
    } else {
        bar->setPosition(ccp(result, bar->getPosition().y));
    }
}

void ScrollBar::setBarScale() {
    beforContentSize = nowContentSize;

    float result = (svFixSize/nowContentSize) * svFixSize;
    barSize = result;
    if(result < barMinSize || result >= svFixSize) return;
    
    if(type <= type_Vertical_out) {
        bar->setContentSize(CCSizeMake(barFixSize, result));
    } else {
        bar->setContentSize(CCSizeMake(result, barFixSize));
    }
}


void ScrollBar::setBarRefresh(CCScrollView* sv) {
//    if(bar == NULL) {
//        CCLog("bar NULL");
//        return;
//    }
	if((bar->getScaleX() == 0.0f || bar->getScaleY() == 0.0f)) {
		fadeIn(0);
	}

    unscheduleAllSelectors();
    scheduleOnce(schedule_selector(ScrollBar::fadeOut), fadeDelayTime);


    float offset;
    if(type <= type_Vertical_out) {
        offset = sv->getContentOffset().y;
        nowContentSize = sv->getContentSize().height;
    } else {
        offset = sv->getContentOffset().x;
        nowContentSize = sv->getContentSize().width;
    }
    
    if(nowContentSize <= svFixSize) this->setVisible(false);
    else this->setVisible(true);
    
    if(beforContentSize != nowContentSize) setBarScale();
    setBarPosition(offset);
}

void ScrollBar::setCanFadeBar(bool isFade) {
    this->canFade = isFade;
}

void ScrollBar::setFadeDelayTime(float fadeDelayTime) {
    this->fadeDelayTime = fadeDelayTime;
}