#include "ShopMenu.h"
#include "TitleMenu.h"
#include "SimpleAudioEngine.h"
#include "GameRecord.h"
#include "Tasks.h"
#include "GameData.h"
//#include "IAPHelper.h"
#include "SelectMenu.h"
#include "GamePlay.h"
#include "PublicLoad.h"
#include "CoinsMenu.h"
#include "UniversalFit.h"

#define PADDING (6)

using namespace CocosDenshion;

int gSupplySequence[] = {0, 1, 7, 6, 8, 5, 2, 4, 3};
int gUtilityPrice[4] = {100, 400, 50, 200};
int gJoykitCost[3] = {5000, 12000, 36000};
const char *gIAPCost[] =
{
    "free",
    "$ 0.99",
    "$ 1.99",
    "$ 2.99",
    "$ 3.99",
    "$ 4.99",
    "$ 5.99",
};
char const *gDetails[] =
{
    "在游戏开始的时候送你800米的距离。",//0
    "促销优惠包，买四送一！",//1
    "立即让你蓄满查克拉！",//2
    "清仓大甩卖，物美价廉的宝贝。",//3
    "永远不再需要为雇佣樱同学而付钱了。\n她现在是你的人了。",//4
    "永远不再需要为雇佣武藏而付钱了。",//5
    "永远不再需要为雇佣马陇而付钱了。\n来我的宝贝妈咪⋯⋯哎呀！",//6
    "立即扔出多枚手里剑。\n升级：增加投出的手里剑数量。",//7
    "在短时间内无敌。\n升级：增加无敌时间。",//8
    "短时间内可以无限扔飞镖。\n升级：增加持续时间。",//9
    "召唤数波从天而降的火球。\n升级：增加火球的波数。",//10
    "以闪电般的速度秒杀数名敌人。\n升级：增加秒数数量。",//11
    "瞬间消除所有敌人的手里剑。\n升级：提高此道具出现的概率。",//12
    "发动忍者的意念，让自己摆脱时空的束缚。\n升级：提高此道具出现的概率。",//13
    "立即蓄满查克拉槽。\n升级：提高此道具出现的概率。",//14
    "立即回复一点生命值。\n升级：提高此道具的出现概率。",//15
    "瞬间消除所有敌人的手里剑。\n升级：开启此道具。",//16
    "发动忍者的意念，让自己摆脱时空的束缚。\n升级：开启此道具。",//17
    "立即蓄满查克拉槽。\n升级：开启此道具。",//18
    "立即回复一点生命值。\n升级：开启此道具。",//19
    "立即扔出多枚手里剑。",//20
    "在短时间内无敌。",//21
    "短时间内可以无限扔飞镖。",//22
    "召唤数波从天而降的火球。",//23
    "以闪电般的速度秒杀数名敌人。",//24
    "瞬间消除所有敌人的手里剑。",//25
    "发动忍者的意念，\n让自己摆脱时空的束缚。",//26
    "立即蓄满查克拉槽。",//27
    "立即回复一点生命值。",//28
    "在死后可以让你满状态复活一次。\n（有5次机会）",//29
    "三倍乐趣。（共15次机会）",//30
    "冲击冠军榜必须的道具。\n（有50次使用机会）",//31
};

static int gNavBack = 0;

void ShopMenu::setNavBack(int sid) 
{
    gNavBack = sid;
}

bool ShopMenu::init() 
{

  auto listener = EventListenerTouchOneByOne::create();
  listener->onTouchBegan = CC_CALLBACK_2(ShopMenu::onTouchBegan, this);
  listener->onTouchEnded = CC_CALLBACK_2(ShopMenu::onTouchEnded, this);
  listener->onTouchMoved = CC_CALLBACK_2(ShopMenu::onTouchMoved, this);
  listener->onTouchCancelled = CC_CALLBACK_2(ShopMenu::onTouchEnded, this);
  _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

  this->setTouchEnabled(true);
  this->scheduleUpdate();
  return true;
}

void ShopMenu::onEnter() 
{
  mUISwapper.onEnter();
    PublicLoad::menuShop()->loadAll();
cocos2d::Node *taskcomplete = cocos2d::Node::create();
    taskcomplete->setPosition(cocos2d::Vec2(SCREEN_WIDTH/2, SCREEN_HEIGHT));
    this->addChild(taskcomplete);
    GamePlay::sharedGamePlay()->setTaskCompleteNode(taskcomplete);
cocos2d::Node * node = createUIByCCBI("menu-shop", "ShopMenu", ShopMenuLayerLoader::loader(), this);
    if(node != NULL) {
      this->addChild(node);
    }
    mClipedList = CEClipedNode::create();
    mClipedList->setClipRect(new CCRect((UniversalFit::sharedUniversalFit()->transformRect(cocos2d::CCRectMake(12, 12, 455, 264)))));
    mList->addChild(mClipedList);
    mItemList = cocos2d::Node::create();
    mClipedList->addChild(mItemList);
    mFly = false;
    
    mScrollBody = cocos2d::Sprite::createWithSpriteFrameName("sp_scroll2.png");
    mScrollBody->setAnchorPoint(cocos2d::Vec2(0, 1));
    mClipedList->addChild(mScrollBody);
    
    mCoins->setString(cocos2d::CCString::createWithFormat("%d", GameRecord::sharedGameRecord()->coins)->getCString());
    mOffset = 0;
    mItemList->removeAllChildrenWithCleanup(true);
    mCurrUnFold = -1;
    
    //add content
    addCategory(0);
    addUtility(4);
    addUtility(5);
    addUtility(6);
    addUtility(0);
    addUtility(1);
    addUtility(2);
    addUtility(3);
    addCategory(1);
    addCharacter(1);
    addCharacter(2);
    addCharacter(3);
    addCategory(2);
    for(int i=0; i<9; ++i)
    {
        addSupply(gSupplySequence[i], i);
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

void ShopMenu::onExit() 
{
  GamePlay::sharedGamePlay()->setTaskCompleteNode(NULL);
    PublicLoad::menuShop()->unloadAll();
cocos2d::Layer::onExit();
}

void ShopMenu::addCategory(int cid) 
{
    FoldItem *fi = FoldItem::foldItem(-1, 0, false, cid, NULL, 0, this);
    fi->setPosition(cocos2d::Vec2(0, mOffset));
    mItemList->addChild(fi, 1, mItemList->getChildrenCount());
    mOffset -= fi->getContentSize().height + PADDING;
}

void ShopMenu::addCharacter(int cid) 
{
    int cost = 100000;
    if( GameRecord::sharedGameRecord()->char_contract[cid] > 0 )
    {
        cost = 0;
    }
    FoldItem *fi = FoldItem::foldItem(14, cost, false, -cid, gDetails[3+cid], 1, this);
    fi->setPosition(cocos2d::Vec2(0, mOffset));
    mItemList->addChild(fi, 1, mItemList->getChildrenCount());
    mOffset -= fi->getContentSize().height + PADDING;
}

void ShopMenu::addUtility(int cid) 
{
    if( cid >= 4 )
    {
        FoldItem *fi = FoldItem::foldItem(11+cid, gJoykitCost[cid-4], false, 100+cid, gDetails[25+cid], 1, this);
        fi->setPosition(cocos2d::Vec2(0, mOffset));
        mItemList->addChild(fi, 1, mItemList->getChildrenCount());
        mOffset -= fi->getContentSize().height + PADDING;
    }
    else
    {
        FoldItem *fi = FoldItem::foldItem(10+cid, gUtilityPrice[cid], false, 100+cid, gDetails[cid], 1, this);
        fi->setPosition(cocos2d::Vec2(0, mOffset));
        mItemList->addChild(fi, 1, mItemList->getChildrenCount());
        mOffset -= fi->getContentSize().height + PADDING;
    }
}

void ShopMenu::addSupply(int cid, int index) 
{
    int level = GameRecord::sharedGameRecord()->item_upgrade[cid];
    int icon = 4*cid + level;
    int cost = GameData::upgradeCost(cid, level);
    int detoff = 0;
    if( index > 4 && level == 0 )
    {
        detoff = 4;
    }
    if( level == 3 )
    {
        detoff = 13;
    }
    FoldItem *fi = FoldItem::foldItem(cid, cost, false, icon, gDetails[7+index+detoff], 0, this);
    fi->setPosition(cocos2d::Vec2(0, mOffset));
    mItemList->addChild(fi, 1, mItemList->getChildrenCount());
    mOffset -= fi->getContentSize().height + PADDING;
}

void ShopMenu::activate(int cid) 
{
    FoldItem *newitem = (FoldItem*)(mItemList->getChildByTag(cid));
    bool unfold = false;
    if( newitem->isFoldable() )
    {
        GameTool::PlaySound("sound/ui-cancel.mp3");
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
cocos2d::Ref* node = NULL;
//CCARRAY_FOREACH(mItemList->getChildren(), node)
//        {
//            FoldItem *it = (FoldItem*)node;
//            it->setPosition(cocos2d::Vec2(0, mOffset));
//            mOffset -= it->getContentSize().height + PADDING;
//        }
//        if( unfold )
//        {
//            float upbound = -newitem->getPosition().y - mItemList->getPosition().y;
//            if( upbound < 0 )
//            {
//                cocos2d::Point np = mItemList->getPosition();
//                np.y = -newitem->getPosition().y;
//                mItemList->setPosition(np);
//            }
//            float downbound = upbound + newitem->getContentSize().height;
//            if( downbound > 264 )
//            {
//                cocos2d::Point np = mItemList->getPosition();
//                np.y = -newitem->getPosition().y + newitem->getContentSize().height - 264;
//                mItemList->setPosition(np);
//            }
//        }
    }
}

void ShopMenu::purchase(int cid) 
{
cocos2d::CCLog("PID = %d", cid);
    if( cid >= 13 )
    {
        int index = gSupplySequence[cid - 13];
        int level = GameRecord::sharedGameRecord()->item_upgrade[index];
        if( level < 3 )
        {
            int cost = GameData::upgradeCost(index, level);
            if( GameRecord::sharedGameRecord()->coins >= cost )
            {
                level++;
                GameRecord::sharedGameRecord()->setItemUpgrade(level, index);
                GameTool::PlaySound("sound/buy.mp3");
                FoldItem *item = (FoldItem*)(mItemList->getChildByTag(cid));
                int icon = 4*index + level;
                int detbase = 7;
                if( level == 3 )
                {
                    detbase = 20;
                }
                item->updateInfo(icon, GameData::upgradeCost(index, level), gDetails[detbase+cid-10], 0);
                //cost coins
                GameRecord::sharedGameRecord()->makeCoins(-cost);
            }
            else {
                GameTool::PlaySound("sound/error.mp3");
            }
        }
    }
    else {
        if( cid ==9 || cid ==10 || cid==11 )
        {
            //setModal(cid-4);
            int cost = 100000;
            if( GameRecord::sharedGameRecord()->coins >= cost )
            {
                GameTool::PlaySound("sound/buy.mp3");
                GameRecord::sharedGameRecord()->makeCoins(-cost);
                GameRecord::sharedGameRecord()->setCharacterContract(1, cid-9+1);
            }
            else {
                GameTool::PlaySound("sound/error.mp3");
            }
        }
        else if( cid == 1 || cid == 2 || cid == 3 )
        {
            //setModal(cid+7);
            int cost = gJoykitCost[cid-1];
            if( GameRecord::sharedGameRecord()->coins >= cost )
            {
                GameTool::PlaySound("sound/buy.mp3");
                GameRecord::sharedGameRecord()->makeCoins(-cost);
                switch (cid) {
                    case 9:
                    {
                        GameRecord::sharedGameRecord()->item_credit+=5;
                    }
                        break;
                    case 10:
                    {
                        GameRecord::sharedGameRecord()->item_credit+=15;
                    }
                        break;
                    case 11:
                    {
                        GameRecord::sharedGameRecord()->item_credit+=50;
                    }
                        break;
                }
            }
            else {
                GameTool::PlaySound("sound/error.mp3");
            }
        }
        else
        {
            int cost = gUtilityPrice[cid-4];
            if( GameRecord::sharedGameRecord()->coins >= cost )
            {
                GameTool::PlaySound("sound/buy.mp3");
                GameRecord::sharedGameRecord()->makeCoins(-cost);
                switch (cid) {
                    case 4:
                    {
                        GameRecord::sharedGameRecord()->item_fly++;
                    }
                        break;
                    case 5:
                    {
                        GameRecord::sharedGameRecord()->item_fly+=5;
                    }
                        break;
                    case 6:
                    {
                        GameRecord::sharedGameRecord()->item_sp++;
                    }
                        break;
                    case 7:
                    {
                        GameRecord::sharedGameRecord()->item_sp+=5;
                    }
                        break;
                }
            }
            else {
                GameTool::PlaySound("sound/error.mp3");
            }
        }
    }
    mCoins->setString(cocos2d::CCString::createWithFormat("%d", GameRecord::sharedGameRecord()->coins)->getCString());
    GameRecord::sharedGameRecord()->checkPoint();
}

void ShopMenu::onBack(cocos2d::Ref*)
{
    if(mUISwapper.isDone()) {
      GameTool::PlaySound("sound/menu-change.mp3");
      if( gNavBack == 0 ) {
          mUISwapper.setSceneOutro(GameTool::scene<TitleMenu>(), this);
      } else {
          mUISwapper.setSceneOutro(GameTool::scene<SelectMenu>(), this);
      }
    }
}

void ShopMenu::onMoreCoins(cocos2d::Ref* )
{
    if(mUISwapper.isDone()) {
      GameTool::PlaySound("sound/menu-change.mp3");
      mUISwapper.setSceneOutro(GameTool::scene<CoinsMenu>(), this);
    }
}

void ShopMenu::update(float delta) 
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

void ShopMenu::updateScorll() 
{
    float length = 264.0f*(264.0f/-mOffset)/100;
    float begin = -264.0f*(mItemList->getPosition().y/-mOffset);
    mScrollBody->setScaleY(length);
    mScrollBody->setPosition(cocos2d::Vec2(441, begin));
}

bool ShopMenu::onTouchBegan(Touch * touch, Event * event) 
{
    cocos2d::Point pos = touch->getLocationInView();
    pos = cocos2d::CCDirector::sharedDirector()->convertToGL(pos);
    pos = UniversalFit::sharedUniversalFit()->restorePoint(pos);

    mTouchBegin = pos;
cocos2d::CCRect rect = cocos2d::CCRectMake(12, 12, 455, 264);
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

void ShopMenu::onTouchMoved(Touch * touch, Event * event) 
{
//    if( !mIsModal )
//    {
//        cocos2d::Point pos = touch->getLocationInView();
//        pos = cocos2d::CCDirector::sharedDirector()->convertToGL(pos);
//        pos = UniversalFit::sharedUniversalFit()->restorePoint(pos);
//
//        float dy = pos.y - mBeginPressY;
//        float y = mBeginNodeY + dy;
//        cocos2d::Point np = mItemList->getPosition();
//        np.y = y;
//        mItemList->setPosition(np);
//        //CFAbsoluteTime time = CFAbsoluteTimeGetCurrent();
//        time_t time;
//cocos2d::CCTime::gettimeofdayCocos2d(&time, NULL);
//        if( mLastY > -10000 )
//        {
//            float ds = np.y - mLastY;
//            //float dt = time - mLastTime;
//            float dt = cocos2d::CCTime::timersubCocos2d(&mLastTime, &time)/1000.0;
//            mFlySpeed = ds/dt;
//        }
//        mLastY = np.y;
//        mLastTime = time;
//        this->updateScorll();
//    }
}

void ShopMenu::onTouchEnded(Touch * touch, Event * event) 
{
    if( !mIsModal )
    {
        mFly = true;
        cocos2d::Point pos = touch->getLocationInView();
        pos = cocos2d::CCDirector::sharedDirector()->convertToGL(pos);
        pos = UniversalFit::sharedUniversalFit()->restorePoint(pos);
cocos2d::CCRect rect = cocos2d::CCRectMake(12, 12, 455, 264);
        if( ccpLengthSQ(ccpSub(pos, mTouchBegin)) < 10*10 &&
                rect.containsPoint(pos) )
        {
            float dy = mList->getPosition().y - pos.y;
            float offset = 0;
            int index = 0;
cocos2d::Ref *node;
//CCARRAY_FOREACH(mItemList->getChildren(), node)
//            {
//                FoldItem *item = (FoldItem*)node;
//                float upbound = offset - mItemList->getPosition().y;
//                float downbound = upbound + item->getContentSize().height + PADDING;
//                if( dy >= upbound && dy < downbound )
//                {
//                    activate(index);
//                    break;
//                }
//                offset += item->getContentSize().height + PADDING;
//                index++;
//            }
        }
    }
}

/*
void ShopMenu::dealloc() 
{
    PublicLoad->menuShop()->unloadAll();
    
    super->dealloc();
}
*/
void ShopMenu::setModal(int pid) 
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
cocos2d::CCRotateBy *rb = cocos2d::CCRotateBy::create(1, 720);
cocos2d::CCRepeatForever *rf = cocos2d::CCRepeatForever::create(rb);
            descsp->runAction(rf);
            
            mModalPurchase = pid;
        }
        */
    }
}

void ShopMenu::cancelModal() 
{
    if( mIsModal )
    {
        mIsModal = false;
        mMask->setVisible(false);
        mMenu->setTouchEnabled(true);
cocos2d::Ref *node;
//CCARRAY_FOREACH(mItemList->getChildren(), node)
//        {
//            FoldItem *it = (FoldItem*)node;
//            it->togglePurchaseButton(true);
//        }
    }
}

void ShopMenu::purchaseVerified(int pid) 
{
    if( pid == mModalPurchase )
    {
        this->cancelModal();
        //do something to update item
        GameRecord::sharedGameRecord()->setIAPFlag(GameRecord::sharedGameRecord()->iap_flag[pid]+1, pid);
        int contractid = pid - 4;
        GameRecord::sharedGameRecord()->setCharacterContract(1, contractid);
cocos2d::CCLog("Contract %d Purchased!", contractid);
        GameRecord::sharedGameRecord()->checkPoint();
        int cid = pid+1;
        FoldItem *item = (FoldItem*)(mItemList->getChildByTag(cid));
        item->updateInfo(-1, 0, gDetails[3+cid], -1);
        GameTool::PlaySound("sound/getscroll.mp3");
    }
    else {
        GameRecord::sharedGameRecord()->purchaseVerified(pid);
    }
}

void ShopMenu::purchaseFailed(int pid) 
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

void ShopMenu::purchaseCancelled(int pid) 
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

void ShopMenu::modalOver() 
{
    this->cancelModal();
}

SEL_MenuHandler ShopMenu::onResolveCCBCCMenuItemSelector(cocos2d::Ref * pTarget, const char* pSelectorName)
{
  CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "onBack", ShopMenu::onBack);
  CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "onMoreCoins", ShopMenu::onMoreCoins);
  return NULL;
}

cocos2d::extension::Control::Handler   ShopMenu::onResolveCCBCCControlSelector(cocos2d::Ref * pTarget, const char* pSelectorName)
{
  return NULL;
}

bool ShopMenu::onAssignCCBMemberVariable(cocos2d::Ref* pTarget, const char* pMemberVariableName, Node* pNode)
{
  CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "mCoins", Label *, mCoins);
  CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "mList", Node *, mList);
  CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "mMenu", CCMenu *, mMenu);
  return false;
}
