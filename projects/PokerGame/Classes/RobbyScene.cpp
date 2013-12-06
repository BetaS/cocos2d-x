#include "RobbyScene.h"
#include "AppDelegate.h"
#include "ui/CustomTableViewCell.h"
#include "net/RPCClient.h"
#include "net/GameClient.h"

USING_NS_CC;
USING_NS_CC_EXT;

RobbyScene::~RobbyScene ()
{
    if(datalist) {
        datalist->release();
        datalist = NULL;
    }
}

CCScene* RobbyScene::scene()
{
    CCScene *scene = CCScene::create();
    RobbyScene *layer = RobbyScene::create();
    scene->addChild(layer);
    return scene;
}

bool RobbyScene::init()
{
    bar = NULL;
    if ( !CCLayerColor::initWithColor(ccc4(0, 0, 0, 255)) )
    {
        return false;
    }
    

    CCMenuItemImage *pCloseItem = CCMenuItemImage::create(
                                        "CloseNormal.png",
                                        "CloseSelected.png",
                                        this,
                                        menu_selector(RobbyScene::menuCloseCallback) );
    pCloseItem->setPosition( ccp(CCDirector::sharedDirector()->getWinSize().width - 20, 20) );
    
    CCMenuItemLabel* label = CCMenuItemLabel::create(CCLabelTTF::create(_AtoU8("방 만들기"), "Helvetica", 32), this, menu_selector(RobbyScene::menuCreateRoom));
	label->setPosition(ccp(900, 200));

    CCMenu* pMenu = CCMenu::create(pCloseItem, label, NULL);
    pMenu->setPosition( CCPointZero );
    this->addChild(pMenu, 1);

    datalist = CCArray::create();
    datalist->retain();
    
    count = 0;
	for(int i=0; i < 20; i++) {
		char str[10];
		sprintf(str, "data %d", i);
		datalist->addObject(CCString::create(str));
        count++;
    }
    
    CCTableView* tableView = CCTableView::create(this, CCSizeMake(720, 720));
    tableView->setDirection(kCCScrollViewDirectionVertical);
    tableView->setVerticalFillOrder(kCCTableViewFillTopDown);
    
    tableView->setPosition(ccp(0, 0));
    
    tableView->setDelegate(this);

    this->addChild(tableView, 1);
    tableView->reloadData();

	bar = ScrollBar::create(tableView, type_Vertical_in);
    
    return true;
}
//
//void RobbyScene::addDatalist(int num)
//{
//    char str[10] ;
//    sprintf(str, "data %d", num);
//    datalist->addObject(CCString::create(str));
//}

void RobbyScene::menuCloseCallback(CCObject* pSender)
{
    CCDirector::sharedDirector()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}

void RobbyScene::menuCreateRoom(CCObject* pSender)
{
	JsonBox::Value result;
	JsonBox::Object params, device;
	((AppDelegate*)CCApplication::sharedApplication())->getDeviceInfo()->getJSONString(device);

	params["device"] = JsonBox::Value(device);

	g_Server.request(result, "gameRoomCreate", params);
}

void RobbyScene::scrollViewDidScroll(CCScrollView* view) {
   if(bar != NULL) bar->setBarRefresh(view);
}

void RobbyScene::tableCellTouched(CCTableView* table, CCTableViewCell* cell)
{
    CCPoint offset = table->getContentOffset();
    CCSize contentSize = table->getContentSize();
    CCSize tvSize = table->cocos2d::CCNode::getContentSize();
    int p = cell->getIdx();
    CCLOG("cell touched at index: %i, tv content size (%3.2f, %3.2f) tv size : (%3.2f, %3.2f)", p, contentSize.width, contentSize.height, tvSize.width, tvSize.height);
    datalist->removeObjectAtIndex(p);
    table->removeCellAtIndex(p);

    table->reloadData();
   
	if(p > 4) table->setContentOffset(offset);
}

void RobbyScene::tableCellHighlight(CCTableView* table, CCTableViewCell* cell)
{
    CCLayerColor* mask;
	mask = CCLayerColor::create(ccc4(200, 0, 0, 100), 200, 80);
    cell->addChild(mask,100, 44);
}

void RobbyScene::tableCellUnhighlight(CCTableView* table, CCTableViewCell* cell)
{
    cell->removeChildByTag(44, true);
    
}
CCSize RobbyScene::cellSizeForTable(CCTableView *table)
{
    return CCSizeMake(200, 80);
}

CCTableViewCell* RobbyScene::tableCellAtIndex(CCTableView *table, unsigned int idx)
{
    CCTableViewCell *cell = table->dequeueCell();
    
    CCString* string = (CCString*) datalist->objectAtIndex(idx);
    
    if (!cell) {
        cell = new CustomTableViewCell();
        cell->autorelease();

        CCLabelTTF *label = CCLabelTTF::create(string->getCString(), "Helvetica", 20.0);
        label->setPosition(CCPointZero);
        label->setAnchorPoint(CCPointZero);
        label->setTag(123);
        cell->addChild(label);
    }
    else
    {
        CCLabelTTF *label = (CCLabelTTF*)cell->getChildByTag(123);
        label->setString(string->getCString());
    }
    
    return cell;
}

unsigned int RobbyScene::numberOfCellsInTableView(CCTableView *table)
{
    return datalist->count();
}

