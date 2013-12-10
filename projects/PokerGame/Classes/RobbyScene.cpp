#include "RobbyScene.h"
#include "ui/CustomTableViewCell.h"
#include "net/RPCClient.h"
#include "net/GameClient.h"
#include "GameScene.h"

USING_NS_CC;
USING_NS_CC_EXT;

enum Views
{
	vTableView
};

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
	
	roomData = new vector<GameRoomInfo*>;
	
    datalist = CCArray::create();
    datalist->retain();
	
    CCTableView* tableView = CCTableView::create(this, CCSizeMake(720, 720));
    tableView->setDirection(kCCScrollViewDirectionVertical);
    tableView->setVerticalFillOrder(kCCTableViewFillTopDown);
    
    tableView->setPosition(ccp(0, 0));
    
    tableView->setDelegate(this);

    this->addChild(tableView, 1, vTableView);
    tableView->reloadData();
	
	bar = ScrollBar::create(tableView, type_Vertical_in);
    
	refreshDataList();
	
    return true;
}

void RobbyScene::refreshDataList()
{
	roomData->clear();
	datalist->removeAllObjects();
	
	JsonBox::Value result;
	JsonBox::Object params;
	
	g_Server.request(result, "gameRoomList", params);
	
	if(result["result"]["code"].getInt() == 0)
	{
		count = 0;
		CCLog("%d", result["result"]["size"].getInt());
		
		for(int i=0; i < result["result"]["size"].getInt(); i++) {
			GameRoomInfo* pInfo = new GameRoomInfo(result["result"]["items"][i]);
			roomData->push_back(pInfo);
			
			stringstream title("");
			title << "#" << i << " (" << pInfo->getSessionKey() << ") - " << pInfo->getAddress() << ":" << pInfo->getPort();
			datalist->addObject(CCString::create(title.str()));
			title.flush();
			count++;
		}
	}
	
    ((CCTableView*)getChildByTag(vTableView))->reloadData();
}


void RobbyScene::joinGameRoom(GameRoomInfo info)
{
	if(info.isFull())
	{
		CCMessageBox("Room is Full", "Error");
		return;
	}
	
	if(g_Client != NULL)
		delete g_Client;
	
	string result;
	g_Client = new GameClient(info.getAddress(), info.getPort());
	g_Client->request(result, info.getSessionKey(), TYPE_JOIN, "");

	vector<string> elems;
	stringstream ss(result);
	string item;
	while (std::getline(ss, item, '|')) {
		elems.push_back(item);
	}
	ss.flush();

	if(elems[0] == "ok")
	{
		CCDirector::sharedDirector()->replaceScene(CCTransitionFade::create(0.5f, GameScene::scene(info.getSessionKey(), atoi(elems[1].c_str()), elems[2], atoi(elems[3].c_str()))));
	}
	else
	{
		CCMessageBox("Can't join the room", "Error");
	}
}

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
	JsonBox::Object params;

	g_Server.request(result, "gameRoomCreate", params);
	
	refreshDataList();
}

void RobbyScene::scrollViewDidScroll(CCScrollView* view) {
   if(bar != NULL) bar->setBarRefresh(view);
}

void RobbyScene::tableCellTouched(CCTableView* table, CCTableViewCell* cell)
{
	joinGameRoom(*(*roomData)[cell->getIdx()]);
}

void RobbyScene::tableCellHighlight(CCTableView* table, CCTableViewCell* cell)
{
    CCLayerColor* mask;
	mask = CCLayerColor::create(ccc4(180, 180, 0, 100), 720, 80);
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

