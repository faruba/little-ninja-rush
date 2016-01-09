#include "ExtraMenu.h"
#include "ExtraSelect.h"
#include "GameConstants.h"
#include "Tasks.h"
#include "GameRecord.h"
#include "UniversalFit.h"
#include "PublicLoad.h"
#include "patch.h"

#define PADDING (6)

cocos2d::Scene* ExtraMenu::scene(int mode) 
{
	ExtraMenu *tm = NULL;
	auto ret = GameTool::scene(&tm);

	tm->mMode = mode;

	return ret;
}

bool ExtraMenu::init() 
{
	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = CC_CALLBACK_2(ExtraMenu::onTouchBegan, this);
	listener->onTouchEnded = CC_CALLBACK_2(ExtraMenu::onTouchEnded, this);
	listener->onTouchMoved = CC_CALLBACK_2(ExtraMenu::onTouchMoved, this);
	listener->onTouchCancelled = CC_CALLBACK_2(ExtraMenu::onTouchEnded, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);


	this->setTouchEnabled(true);
	this->scheduleUpdate();


	return true;
}

void ExtraMenu::onEnter() 
{
	mUISwapper.onEnter();
	PublicLoad::menuExtra()->loadAll();
	cocos2d::Node * node = createUIByCCBI("menu-extra", "ExtraMenu", ExtraMenuLayerLoader::loader(), this);
	if(node != NULL) {
		this->addChild(node);
	}

	mClipedList = CEClipedNode::create();
	mClipedList->setClipRect(new CCRect((UniversalFit::sharedUniversalFit()->transformRect(cocos2d::Rect(12, 12, 455, 264)))));
	mList->addChild(mClipedList);
	mItemList = cocos2d::Node::create();
	mClipedList->addChild(mItemList);
	mFly = false;

	mScrollBody = cocos2d::Sprite::createWithSpriteFrameName("sp_scroll2.png");
	mScrollBody->setAnchorPoint(cocos2d::Vec2(0, 1));
	mClipedList->addChild(mScrollBody);

	//--------------
	mOffset = 0;
	mItemList->removeAllChildrenWithCleanup(true);
	mCurrUnFold = -1;

	mCurrState = -1;

	switch (mMode) {
		case 0:
			{
				mBanner->setDisplayFrame(GameTool::getSpriteFrameByName("exb6.png"));
				this->loadAchievements();
			}
			break;
		case 1:
			{
				mBanner->setDisplayFrame(GameTool::getSpriteFrameByName("exb7.png"));
				this->loadStatistics();
			}
			break;
		case 2:
			{
				mBanner->setDisplayFrame(GameTool::getSpriteFrameByName("exb5.png"));
				this->loadAboutUs();
			}
			break;
	}

	//update scroll
	this->updateScorll();

	mUISwapper.setSceneIntro(this);
	cocos2d::Layer::onEnter();
}

void ExtraMenu::activate(int cid) 
{
	FoldItem *newitem = (FoldItem*)(mItemList->getChildByTag(cid));
	bool unfold = false;
	if( newitem->isFoldable() )
	{
		GameTool::PlaySound("ui-cancel.mp3");
		if( cid == mCurrUnFold )
		{
			//fold
			FoldItem *item = (FoldItem*)(mItemList->getChildByTag(cid));
			item->toggleFold(true);
			mCurrUnFold = -1;
		}
		else {
			//unfold
			if( mCurrUnFold >= 0 )
			{
				FoldItem *item = (FoldItem*)(mItemList->getChildByTag(mCurrUnFold));
				item->toggleFold(true);
				mCurrUnFold = -1;
			}
			newitem->toggleFold(false);
			mCurrUnFold = cid;
			unfold = true;
		}
		//rearrange items
		mOffset = 0;
		for (Node* node :mItemList->getChildren())
		{
			FoldItem *it = (FoldItem*)node;
			it->setPosition(cocos2d::Vec2(0, mOffset));
			mOffset -= it->getContentSize().height + PADDING;
		}
		if( unfold )
		{
			float upbound = -newitem->getPosition().y - mItemList->getPosition().y;
			if( upbound < 0 )
			{
				cocos2d::Point np = mItemList->getPosition();
				np.y = -newitem->getPosition().y;
				mItemList->setPosition(np);
			}
			float downbound = upbound + newitem->getContentSize().height;
			if( downbound > 264 )
			{
				cocos2d::Point np = mItemList->getPosition();
				np.y = -newitem->getPosition().y + newitem->getContentSize().height - 264;
				mItemList->setPosition(np);
			}
		}
	}
}

void ExtraMenu::onExit() 
{
	PublicLoad::menuExtra()->unloadAll();
	cocos2d::Layer::onExit();
}

void ExtraMenu::onBack(cocos2d::Ref*) 
{
	if(mUISwapper.isDone())
	{
		GameTool::PlaySound("menu-change.mp3");
		mUISwapper.setSceneOutro(GameTool::scene<ExtraSelect>(), this);
	}
}

void ExtraMenu::update(float delta) 
{
	if( mFly )
	{
		float yy = mItemList->getPosition().y;
		float minY = 0;
		float maxY = -mOffset-264;
		if( maxY < 0 )
		{
			maxY = 0;
		}

		//降速
		float fade = delta*3000;
		if( mFlySpeed > 0 )
		{
			mFlySpeed -= fade;
			if( mFlySpeed < 0 )
			{
				mFlySpeed = 0;
			}
		}
		if( mFlySpeed < 0 )
		{
			mFlySpeed += fade;
			if( mFlySpeed > 0 )
			{
				mFlySpeed = 0;
			}
		}
		//速度修正
		if( yy < minY )
		{
			float k = minY - yy;
			float a = delta*k*100;
			mFlySpeed += a;
			float ms = 50+k*20;
			if( mFlySpeed > ms )
			{
				mFlySpeed = ms;
			}
		}
		if( yy > maxY )
		{
			float k = yy - maxY;
			float a = delta*k*100;
			mFlySpeed -= a;
			float ms = -50-k*20;
			if( mFlySpeed < ms )
			{
				mFlySpeed = ms;
			}
		}

		//运动
		float dy = mFlySpeed*delta;
		cocos2d::Point np = mItemList->getPosition();
		np.y += dy;
		mItemList->setPosition(np);

		if( fabsf(mFlySpeed)<100 && np.y >= minY && np.y <= maxY )
		{
			mFly = false;
		}

		this->updateScorll();
	}
}

void ExtraMenu::updateScorll() 
{
	float length = 264.0f*(264.0f/-mOffset)/100;
	float begin = -264.0f*(mItemList->getPosition().y/-mOffset);
	mScrollBody->setScaleY(length);
	mScrollBody->setPosition(cocos2d::Vec2(441, begin));
}

void ExtraMenu::loadAchievements() 
{
	mCurrState = 0;
	mItemList->removeAllChildrenWithCleanup(true);
	mOffset = 0;
	for(Achievement ach : Tasks::getAchievements())
	{
		cocos2d::CCLog("*(%d) - %s", ach.achieveCode, ach.text.c_str());

		int achnum = ach.achieveNumber;
		if( ach.achieveCode > ACH_OVERLINE )
		{
			achnum = 1;
		}
		FoldItem *fi = FoldItem::foldItem(ach.name.c_str(), ach.icon.c_str(), ach.desc.c_str(), achnum, ach.achieveCount, this);
		fi->setPosition(cocos2d::Vec2(0, mOffset));
		mItemList->addChild(fi, 1, mItemList->getChildrenCount());
		mOffset -= fi->getContentSize().height + PADDING;
	}
}

void ExtraMenu::loadStatistics() 
{
	mCurrState = 1;
	mItemList->removeAllChildrenWithCleanup(true);
	mOffset = 0;
	for(Statistics sta : Tasks::getStatistics())
	{
		//CCLog("*(%d) - %s", sta->achieveCode, sta->name.c_str());

		if( sta.achieveCode != -1 )
		{//normal statistics
			//cocos2d::Label *title = cocos2d::Label::createWithBMFont("ab34.fnt", sta.name.c_str());
			cocos2d::Label *title = cocos2d::Label::createWithTTF(sta.name.c_str(), GFONT_NAME, GFONT_SIZE_NORMAL);
			title->setAnchorPoint(cocos2d::Vec2(0, 1));
			title->setPosition(cocos2d::Vec2(10, mOffset));
			mItemList->addChild(title, 1, mItemList->getChildrenCount());
			if( sta.achieveCode >= 0 )
			{
				std::string val = std::string_format("%d%s", sta.achieveCount, sta.psfx.c_str());
				cocos2d::Label *result = cocos2d::Label::createWithBMFont("ab34.fnt", val.c_str());
				result->setAnchorPoint(cocos2d::Vec2(1, 1));
				result->setPosition(cocos2d::Vec2(426, mOffset));
				mItemList->addChild(result, 1, mItemList->getChildrenCount());
			}
			else if( sta.achieveCode == -2 )
			{

				std::string val = std::string_format("%d%s", GameRecord::sharedGameRecord()->combo_high, sta.psfx.c_str());
				cocos2d::Label *result = cocos2d::Label::createWithBMFont("ab34.fnt", val.c_str());
				result->setAnchorPoint(cocos2d::Vec2(1, 1));
				result->setPosition(cocos2d::Vec2(426, mOffset));
				mItemList->addChild(result, 1, mItemList->getChildrenCount());
			}

			mOffset -= title->getContentSize().height + 5 + PADDING;
		}
		else {
			//category label
			cocos2d::Sprite *line = cocos2d::Sprite::createWithSpriteFrameName("ex-line.png");
			line->setAnchorPoint(cocos2d::Vec2(0.5f, 1));
			line->setPosition(cocos2d::Vec2(218, mOffset - 10));
			mItemList->addChild(line, 1, mItemList->getChildrenCount());
			cocos2d::Sprite *label = cocos2d::Sprite::createWithSpriteFrameName(sta.name.c_str());
			label->setAnchorPoint(cocos2d::Vec2(0.5f, 1));
			label->setPosition(cocos2d::Vec2(line->getContentSize().width/2, 20));
			line->addChild(label, 1, mItemList->getChildrenCount());

			mOffset -= line->getContentSize().height + PADDING + 20;
		}
	}
}

void ExtraMenu::loadAboutUs() 
{
	mCurrState = 2;
	mItemList->removeAllChildrenWithCleanup(true);
	cocos2d::Node * node = createUIByCCBI("about", "ExtraMenu", ExtraMenuLayerLoader::loader(), this);
	if(node != NULL) {
		mItemList->addChild(node);
	}

	mOffset = -800;
}

void ExtraMenu::onFacebook() 
{
}

void ExtraMenu::onTwitter() 
{
}

void ExtraMenu::onMail() 
{
}

void ExtraMenu::onWeibo() 
{
}

bool ExtraMenu::onTouchBegan(Touch * touch, Event * event) 
{
	LNR_GET_TOUCH_POS;

	mTouchBegin = pos;
	cocos2d::CCRect rect = cocos2d::Rect(12, 12, 455, 264);
	if( rect.containsPoint(pos) )
	{
		mBeginPressY = pos.y;
		mBeginNodeY = mItemList->getPosition().y;
		mFly = false;
		mLastY = -20000;
		return true;
	}
	return false;
}

void ExtraMenu::onTouchMoved(Touch * touch, Event * event) 
{
	//TODO same as ShopMenu:onTouchMoved
	LNR_GET_TOUCH_POS;

	float dy = pos.y - mBeginPressY;
	float y = mBeginNodeY + dy;
	cocos2d::Point np = mItemList->getPosition();
	np.y = y;
	mItemList->setPosition(np);
	//CFAbsoluteTime time = CFAbsoluteTimeGetCurrent();



	timeval time;
	gettimeofday(&time, NULL);
	if( mLastY > -10000 )
	{
		float ds = np.y - mLastY;
		//float dt = time - mLastTime;
		float dt = GameTool::diffTimeval(time, mLastTime)/1000.0;
		if(dt <= 0.01){
			dt = 1.0f;
		}
		mFlySpeed = ds/dt;
	}
	mLastY = np.y;
	mLastTime = time;
	this->updateScorll();
}

void ExtraMenu::onTouchEnded(Touch * touch, Event * event) 
{
	mFly = true;
	LNR_GET_TOUCH_POS;
	cocos2d::CCRect rect = cocos2d::Rect(12, 12, 455, 264);
	if( ccpLengthSQ(ccpSub(pos, mTouchBegin)) < 10*10 &&
			rect.containsPoint(pos) && mCurrState == 0 )//only available in achievement state
	{
		float dy = mList->getPosition().y - pos.y;
		float offset = 0;
		int index = 0;
		for(cocos2d::Node* item:mItemList->getChildren())
		{
			float upbound = offset - mItemList->getPosition().y;
			float downbound = upbound + item->getContentSize().height + PADDING;
			if( dy >= upbound && dy < downbound )
			{
				this->activate(index);
				break;
			}
			offset += item->getContentSize().height + PADDING;
			index++;
		}
	}
}

SEL_MenuHandler ExtraMenu::onResolveCCBCCMenuItemSelector(cocos2d::Ref * pTarget, const char* pSelectorName)
{
	CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "onBack", ExtraMenu::onBack);
	return NULL;
}

cocos2d::extension::Control::Handler   ExtraMenu::onResolveCCBCCControlSelector(cocos2d::Ref * pTarget, const char* pSelectorName)
{
	return NULL;
}
bool ExtraMenu::onAssignCCBMemberVariable(cocos2d::Ref* pTarget, const char* pMemberVariableName, Node* pNode)
{
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "mCoins", Label *,  mCoins);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "mList", Node *, mList);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "mBanner", Sprite *, mBanner);

	return false;
}
