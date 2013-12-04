//
//  Scrollbar.h
//  ExTableView
//
//  Created by ?댁젙??on 13. 6. 19..
//
//

#ifndef __ExTableView__Scrollbar__
#define __ExTableView__Scrollbar__

#include "cocos2d.h"
#include "cocos-ext.h"

USING_NS_CC;
USING_NS_CC_EXT;
enum {
    type_Vertical_in = 1,
    type_Vertical_out = 2,
    type_Horizontal_in = 3,
    type_Horizontal_out = 4,
};

class ScrollBar : public CCLayerColor {
public:
    ScrollBar(CCScrollView* table, int type);
    ~ScrollBar();

private:
	CCScale9Sprite* bar;

public:
    static ScrollBar* create(CCScrollView* table, int type);
    
    void setBarRefresh(CCScrollView* sv);
    
    void setCanFadeBar(bool isFade);
    void setFadeDelayTime(float fadeDelayTime);

private:
    int type;

    float fadeDelayTime;

    float svFixSize;
    float nowContentSize;
    float beforContentSize;
    float barSize;

    bool canFade;

public:
    void fadeOut(float dt);
    void fadeIn(float dt);

    void setBarPosition(float offset);

    void setBarScale();
};

#endif /* defined(__ExTableView__Scrollbar__) */
