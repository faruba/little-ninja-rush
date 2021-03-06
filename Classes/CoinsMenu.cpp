#include "CoinsMenu.h"
#include "TitleMenu.h"
#include "GameRecord.h"
#include "Tasks.h"
#include "GameData.h"
#include "SelectMenu.h"
#include "GamePlay.h"
#include "ShopMenu.h"
#include "UniversalFit.h"

//#include "IAPHelper.h"
#include "PublicLoad.h"

#define PADDING (6)

const char *gPriceMatrix[] = 
{
	"$ 1.99",
	"$ 0.99",
	"$ 2.99",
	"$ 6.99",
	"$ 12.99",
};

const char *gSDetails[] = 
{
	"Once owned never lose. Double the coins\nyou collected in game.",
	"Deliver a pack of coins.",
	"What's that in that chest. Does it\nbelong to anyone?",
	"Are you dreaming of a friend coming\nto your home with a truck of coins?",
	"COINS, COINS, COINS...Oh, you have\nfound a mountain full of gold?",
};

typedef FoldItem CFoldItem;

bool CoinsMenu::init() 
{

	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = CC_CALLBACK_2(CoinsMenu::onTouchBegan, this);
	listener->onTouchEnded = CC_CALLBACK_2(CoinsMenu::onTouchEnded, this);
	listener->onTouchMoved = CC_CALLBACK_2(CoinsMenu::onTouchMoved, this);
	listener->onTouchCancelled = CC_CALLBACK_2(CoinsMenu::onTouchEnded, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

	this->setTouchEnabled(true);
	this->scheduleUpdate();
	return true;
}

void CoinsMenu::onEnter() 
{
	mUISwapper.onEnter();
	cocos2d::Node * node = createUIByCCBI("menu-coins", "CoinsMenu", CoinsMenuLayerLoader::loader(), this);
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

	mCoins->setString(std::string_format("%d", GameRecord::sharedGameRecord()->coins).c_str());
	mOffset = 0;
	mItemList->removeAllChildrenWithCleanup(true);
	mCurrUnFold = -1;

	//add item
	for(int i=1; i<=5; ++i)
	{
		addItem(i);
	}

	//achievement open store
	GameRecord::sharedGameRecord()->task->dispatchTask(ACH_OPENSTORE, 1);

	//update scroll
	this->updateScorll();

	//----------------
	mMask = cocos2d::LayerColor::create(Color4B(0, 0, 0, 128));
	mMask->setOpacity(128);
	mMask->setVisible(false);
	this->addChild(mMask, 10);
	//----------------
	mIsModal = false;
	mModalPurchase = -1;
	mModalTimer = -1;

	mUISwapper.setSceneIntro(this);
	cocos2d::Layer::onEnter();
}

void CoinsMenu::activate(int cid) 
{
	CFoldItem *newitem = (CFoldItem*)(mItemList->getChildByTag(cid));
	bool unfold = false;
	if( newitem->isFoldable() )
	{
		GameTool::PlaySound("ui-cancel.mp3");
		if( cid == mCurrUnFold )
		{
			//fold
			CFoldItem *item = (CFoldItem*)(mItemList->getChildByTag(cid));
			item->toggleFold(true);
			mCurrUnFold = -1;
		}
		else {
			//unfold
			if( mCurrUnFold >= 0 )
			{
				CFoldItem *item = (CFoldItem*)(mItemList->getChildByTag(mCurrUnFold));
				item->toggleFold(true);
				mCurrUnFold = -1;
			}
			newitem->toggleFold(false);
			mCurrUnFold = cid;
			unfold = true;
		}
		//rearrange items
		mOffset = 0;
		for(Node* node : mItemList->getChildren())
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

void CoinsMenu::addItem(int iid) 
{   
	int cost = iid - 1;
	if( iid == 1 && GameRecord::sharedGameRecord()->iap_flag[0] > 0 )
	{
		cost = -1;
	}
	CFoldItem *fi = CFoldItem::foldItem(iid, cost, true, iid, gSDetails[iid-1], 1, this);
	fi->setPosition(cocos2d::Vec2(0, mOffset));
	mItemList->addChild(fi, 1, mItemList->getChildrenCount());
	mOffset -= fi->getContentSize().height + PADDING;
}

void CoinsMenu::purchase(int cid) 
{
	setModal(cid);
}

void CoinsMenu::onExit() 
{
	cocos2d::Layer::onExit();
}

void CoinsMenu::onBack(cocos2d::Ref*)
{
	GameTool::PlaySound("menu-change.mp3");
	mUISwapper.setSceneOutro(GameTool::scene<ShopMenu>(), this);
}

void CoinsMenu::update(float delta) 
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

	if( mModalTimer >= 0 )
	{
		mModalTimer -= delta;
		if( mModalTimer < 0 )
		{
			this->modalOver();
		}
	}
}

void CoinsMenu::updateScorll() 
{
	float length = 264.0f*(264.0f/-mOffset)/100;
	float begin = -264.0f*(mItemList->getPosition().y/-mOffset);
	mScrollBody->setScaleY(length);
	mScrollBody->setPosition(cocos2d::Vec2(441, begin));
}


bool CoinsMenu::onTouchBegan(Touch * touch, Event * event) 
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

void CoinsMenu::onTouchMoved(Touch * touch, Event * event) 
{
	//    LNR_GET_TOUCH_POS;
	//
	//    float dy = pos.y - mBeginPressY;
	//    float y = mBeginNodeY + dy;
	//    cocos2d::Point np = mItemList->getPosition();
	//    np.y = y;
	//    mItemList->setPosition(np);
	//    //CFAbsoluteTime time = CFAbsoluteTimeGetCurrent();
	//    time_t time;
	//cocos2d::CCTime::gettimeofdayCocos2d(&time, NULL);
	//    
	//    if( mLastY > -10000 )
	//    {
	//        float ds = np.y - mLastY;
	//        //float dt = time - mLastTime;
	//        float dt = cocos2d::CCTime::timersubCocos2d(&mLastTime, &time)/1000.0;
	//        mFlySpeed = ds/dt;
	//    }
	//    mLastY = np.y;
	//    mLastTime = time;
	this->updateScorll();
}

void CoinsMenu::onTouchEnded(Touch * touch, Event * event) 
{
	//    mFly = true;
	//    LNR_GET_TOUCH_POS;
	//cocos2d::CCRect rect = cocos2d::Rect(12, 12, 455, 264);
	//    if( ccpLengthSQ(ccpSub(pos, mTouchBegin)) < 10*10 &&
	//            rect.containsPoint(pos) )
	//    {
	//        float dy = mList->getPosition().y - pos.y;
	//        float offset = 0;
	//        int index = 0;
	//cocos2d::Ref *node;
	//CCARRAY_FOREACH(mItemList->getChildren(), node)
	//        {
	//cocos2d::Node *item = (cocos2d::Node*) node;
	//            float upbound = offset - mItemList->getPosition().y;
	//            float downbound = upbound + item->getContentSize().height + PADDING;
	//            if( dy >= upbound && dy < downbound )
	//            {
	//                activate(index);
	//                break;
	//            }
	//            offset += item->getContentSize().height + PADDING;
	//            index++;
	//        }
	//    }
}

/*
	 void CoinsMenu::dealloc() 
	 {
	 PublicLoad->menuShop()->unloadAll();

	 super->dealloc();
	 }
	 */
void CoinsMenu::setModal(int pid) 
{
	if( !mIsModal )
	{
		/*TODO:付费?
			if ( ! IAPHelper->sharedIAPHelper()->makePurchase(pid, this) )
			{
			UIAlertView *alert = UIAlertView->alloc()->initWithTitle("Sorry"->message("In-App Purchase is not prepared. Please try it later.", NULL, "OK",  NULL) autorelease);
			alert->show();
			}
			else {
			mIsModal = true;
			mMask->setVisible(true);
			mMask->removeAllChildrenWithCleanup(true);
		//disable other operations
		mMenu->setTouchEnabled(false);
		cocos2d::Ref *node;
		CCARRAY_FOREACH(mItemList->getChildren(), node)
		{
		FoldItem *it = (FoldItem*)node;
		it->togglePurchaseButton(false);
		}
		cocos2d::Sprite *spbg = cocos2d::Sprite::createWithSpriteFrameName("sp-tc.png");
		spbg->setPosition(cocos2d::Vec2(SCREEN_WIDTH/2, SCREEN_HEIGHT/2)));
		mMask->addChild(spbg);
		cocos2d::Sprite *titlesp = cocos2d::Sprite::createWithSpriteFrameName("sp-tc3.png");
		titlesp->setPosition(cocos2d::Vec2( spbg->getContentSize().width/2, 105));
		spbg->addChild(titlesp);
		cocos2d::Sprite *descsp = cocos2d::Sprite::createWithSpriteFrameName("sp-tc2.png");
		descsp->setPosition(cocos2d::Vec2( spbg->getContentSize().width/2, 55));
		spbg->addChild(descsp);
		cocos2d::RotateBy *rb = cocos2d::RotateBy::create(1, 720);
		cocos2d::RepeatForever *rf = cocos2d::RepeatForever::create(rb);
		descsp->runAction(rf);

		mModalPurchase = pid;
		}
		*/
	}
}

void CoinsMenu::cancelModal() 
{
	if( mIsModal )
	{
		mIsModal = false;
		mMask->setVisible(false);
		mMenu->setTouchEnabled(true);
		for(Node* node : mItemList->getChildren())
		{
			FoldItem *it = (FoldItem*)node;
			it->togglePurchaseButton(true);
		}
	}
}

void CoinsMenu::purchaseVerified(int pid) 
{
	if( pid == mModalPurchase )
	{
		this->cancelModal();
		//do something to update item
		GameRecord::sharedGameRecord()->setIAPFlag(GameRecord::sharedGameRecord()->iap_flag[pid]+1, pid);
		if( pid > 0 )
		{
			switch (pid) {
				case 1:
					{
						GameRecord::sharedGameRecord()->makeCoins(COINS_VALUE1);
					}
					break;
				case 2:
					{
						GameRecord::sharedGameRecord()->makeCoins(COINS_VALUE2);
					}
					break;
				case 3:
					{
						GameRecord::sharedGameRecord()->makeCoins(COINS_VALUE3);
					}
					break;
				case 4:
					{
						GameRecord::sharedGameRecord()->makeCoins(COINS_VALUE4);
					}
					break;
			}
		}
		else {
			int cid = pid;
			CFoldItem *item = (CFoldItem*)(mItemList->getChildByTag(cid));
			item->updateInfo(-1, -1, NULL, -1);
		}
		GameRecord::sharedGameRecord()->checkPoint();
	}
	else {
		GameRecord::sharedGameRecord()->purchaseVerified(pid);
	}
	mCoins->setString(std::string_format("%d", GameRecord::sharedGameRecord()->coins).c_str());
	GameTool::PlaySound("getscroll.mp3");
}

void CoinsMenu::purchaseFailed(int pid) 
{
	if( pid == mModalPurchase )
	{
		//tell user purchase failed.
		mMask->removeAllChildrenWithCleanup(true);
		cocos2d::Sprite *spbg = cocos2d::Sprite::createWithSpriteFrameName("sp-tc.png");
		spbg->setPosition(cocos2d::Vec2(SCREEN_WIDTH/2, SCREEN_HEIGHT/2));
		mMask->addChild(spbg);
		cocos2d::Sprite *titlesp = cocos2d::Sprite::createWithSpriteFrameName("sp-tc5.png");
		titlesp->setPosition(cocos2d::Vec2( spbg->getContentSize().width/2, 65));
		spbg->addChild(titlesp);

		mModalTimer = 3;
	}
	else {
		GameRecord::sharedGameRecord()->purchaseFailed(pid);
	}
}

void CoinsMenu::purchaseCancelled(int pid) 
{
	if( pid == mModalPurchase )
	{
		//just cancel the modal
		mMask->removeAllChildrenWithCleanup(true);
		cocos2d::Sprite *spbg = cocos2d::Sprite::createWithSpriteFrameName("sp-tc.png");
		spbg->setPosition(cocos2d::Vec2(SCREEN_WIDTH/2, SCREEN_HEIGHT/2));
		mMask->addChild(spbg);
		cocos2d::Sprite *titlesp = cocos2d::Sprite::createWithSpriteFrameName("sp-tc4.png");
		titlesp->setPosition(cocos2d::Vec2( spbg->getContentSize().width/2, 65));
		spbg->addChild(titlesp);

		mModalTimer = 3;
	}
	else {
		GameRecord::sharedGameRecord()->purchaseCancelled(pid);
	}
}

void CoinsMenu::modalOver() 
{
	this->cancelModal();
}

SEL_MenuHandler CoinsMenu::onResolveCCBCCMenuItemSelector(cocos2d::Ref * pTarget, const char* pSelectorName)
{
	CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "onBack", CoinsMenu::onBack);
	return NULL;
}

cocos2d::extension::Control::Handler   CoinsMenu::onResolveCCBCCControlSelector(cocos2d::Ref * pTarget, const char* pSelectorName)
{
	return NULL;
}

bool CoinsMenu::onAssignCCBMemberVariable(cocos2d::Ref* pTarget, const char* pMemberVariableName, Node* pNode)
{
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "mCoins", Label *, mCoins);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "mList", Node *, mList);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "mMenu", CCMenu *, mMenu);

	return false;
}

