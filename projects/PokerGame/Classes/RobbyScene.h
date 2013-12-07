#ifndef __ROBBY_SCENE_H__
#define __ROBBY_SCENE_H__

#include "cocos2d.h"
#include "cocos-ext.h"
#include "ui/ScrollBar.h"
#include "GameRoomInfo.h"
#include <vector>

class RobbyScene : public cocos2d::CCLayerColor, public CCTableViewDataSource, public CCTableViewDelegate
{
public:
	~RobbyScene();

public:
	virtual bool init();
	static cocos2d::CCScene* scene();

	void menuCloseCallback(CCObject* pSender);
	void menuCreateRoom(CCObject* pSender);
	CREATE_FUNC(RobbyScene);

private:
	vector<GameRoomInfo*> *roomData;
	CCArray* datalist;
	ScrollBar* bar;
	int count;

public:
	void refreshDataList();
	
public:
	//tabview
	virtual void tableCellHighlight(CCTableView* table, CCTableViewCell* cell);
	virtual void tableCellUnhighlight(CCTableView* table, CCTableViewCell* cell);
	virtual void scrollViewDidScroll(CCScrollView* view);
	virtual void scrollViewDidZoom(CCScrollView* view) {}

	virtual void tableCellTouched(CCTableView* table, CCTableViewCell* cell);
	virtual CCSize cellSizeForTable(CCTableView *table);
	virtual CCTableViewCell* tableCellAtIndex(CCTableView *table, unsigned int idx);
	virtual unsigned int numberOfCellsInTableView(CCTableView *table);
};

#endif // __MENUSCENE_SCENE_H__
