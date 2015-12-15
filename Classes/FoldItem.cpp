#include "FoldItem.h"
#include "GameConstants.h"

extern const char *gIAPCost[];

FoldItem* FoldItem::foldItem(const char* title, const char* icon, const char* detail, int achieve, int counter, FoldItemController* controller) 
{
    FoldItem *ret = FoldItem::create();
    
    {
        ret->mController = controller;
        
        ret->mBgButton = CCSprite::createWithSpriteFrameName("shoplist.png");
        ret->mBgButton->setAnchorPoint(ccp(0, 0));
        ret->addChild(ret->mBgButton);
        ret->setContentSize(ret->mBgButton->getContentSize());
        ret->setAnchorPoint(ccp(0, 1));
        
        //draw content
        if( counter < achieve )
        {
            ret->mIcon = CCSprite::createWithSpriteFrameName("cj_unlocked.png");
            ret->mIcon->setPosition(ccp(43, ret->getContentSize().height - 28 ));
        }
        else {
            ret->mIcon = CCSprite::createWithSpriteFrameName("cj-bg.png");
            ret->mIcon->setPosition(ccp(43, ret->getContentSize().height - 28 ));
            CCSprite *real = CCSprite::createWithSpriteFrameName(icon);
            real->setPosition(ccp( ret->mIcon->getContentSize().width/2, ret->mIcon->getContentSize().height/2));
            ret->mIcon->addChild(real);
        }
        ret->addChild(ret->mIcon);
        
        //draw cost
        if( counter < achieve )
        {
            if( achieve > 1 )
            {
                CCString *progress = CCString::createWithFormat("%d / %d", counter, achieve);
                CCLabelBMFont *font = CCLabelBMFont::create(progress->getCString(), "ra40.fnt");
                font->setAnchorPoint(ccp(0.5f , 0.6f));
                font->setPosition(ccp( ret->getContentSize().width - 60, ret->getContentSize().height - 26));
                ret->mCost = font;
                ret->addChild(font);
            }
        }
        else {
            CCSprite *mark = CCSprite::createWithSpriteFrameName("sp_completed.png");
            mark->setAnchorPoint(ccp(0, 0.5f));
            mark->setPosition(ccp( ret->getContentSize().width - 60, ret->getContentSize().height - 26));
            ret->mCost = mark;
            ret->addChild(mark);
        }
        
        //draw title
        {
            ret->mTitleSp = CCSprite::create(title);
            ret->mTitleSp->setAnchorPoint(ccp(0, 0.5f));
            ret->mTitleSp->setPosition(ccp(100, ret->getContentSize().height - 26));
            ret->addChild(ret->mTitleSp);
        }
        
        //draw desc
        if( detail != NULL )
        {
            //ret->mDetailSp = CCLabelBMFont::create(detail, "ab34.fnt");
            ret->mDetailSp = CCLabelTTF::create(detail, GFONT_NAME, GFONT_SIZE_NORMAL);
            ret->mDetailSp->setAnchorPoint(ccp(0, 0.5f));
            ret->mDetailSp->setPosition(ccp(80, 25));
            ret->mDetailSp->setVisible(false);
            ret->addChild(ret->mDetailSp);
        }
    }
    
    ret->mIsAchievement = true;
    
    return ret;
}

FoldItem* FoldItem::foldItem(int title, int cost, bool iap, int icon, const char * detail, int btntyp, FoldItemController* controller) 
{
  FoldItem *ret = FoldItem::create();
  ret->mTitle = title;
  ret->mIconId = icon;
  ret->mIsIAP = iap;
  ret->mCostNumber = cost;
  ret->mController = controller;
  ret->mBtnType = btntyp;
  if( title < 0 )
  {
    CCString *filename = CCString::createWithFormat("sp_category%d.png", icon);
    CCSprite *sp = CCSprite::createWithSpriteFrameName(filename->getCString());
    sp->setAnchorPoint(ccp(0, 1));
    ret->addChild(sp);
    ret->setContentSize(sp->getContentSize());
  }
  else
  {
    ret->mDetail = CCString::create(detail);
    ret->mDetail->retain();
    ret->mBgButton = CCSprite::createWithSpriteFrameName("shoplist.png");
    ret->mBgButton->setAnchorPoint(ccp(0, 0));
    ret->addChild(ret->mBgButton);

    if( btntyp == 1 )
    {
      ret->mPurchase = CCMenuItemImage::create();
      ret->mPurchase->setNormalSpriteFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("sp_buy1.png"));
      ret->mPurchase->setSelectedSpriteFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("sp_buy2.png"));
      ret->mPurchase->setTarget(ret, menu_selector(FoldItem::purchase));
    }
    else {
      ret->mPurchase = CCMenuItemImage::create();
      ret->mPurchase->setNormalSpriteFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("sp_upgrade1.png"));
      ret->mPurchase->setSelectedSpriteFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("sp_upgrade2.png"));
      ret->mPurchase->setTarget(ret, menu_selector(FoldItem::purchase));
    }

    ret->mPurchase->setVisible(false);
    ret->mMenu = CCMenu::create(ret->mPurchase, NULL);
    ret->mMenu->setPosition(ccp(0, 0));
    ret->addChild(ret->mMenu);
    ret->setContentSize(ret->mBgButton->getContentSize());
    ret->setAnchorPoint(ccp(0, 1));

    //draw content
    if( icon < 0 )
    {
      switch (icon) {
        case -1:
          {
            ret->mIcon = CCSprite::createWithSpriteFrameName("sp_girl.png");
          }
          break;
        case -2:
          {
            ret->mIcon = CCSprite::createWithSpriteFrameName("sp_musashi.png");
          }
          break;
        case -3:
          {
            ret->mIcon = CCSprite::createWithSpriteFrameName("sp_cat.png");
          }
          break;
      }
      ret->mIcon->setPosition(ccp(43, ret->getContentSize().height - 28 ));
    }
    else {
      CCString *filename = CCString::createWithFormat("icon%d.png", icon);
      ret->mIcon = CCSprite::create(filename->getCString());
      ret->mIcon->setPosition(ccp(33, ret->getContentSize().height - 28 ));
    }
    ret->addChild(ret->mIcon);

    //draw cost
    if( cost > 0 )
    {
      if( iap )
      {
        CCLabelBMFont *font = CCLabelBMFont::create(gIAPCost[ret->mCostNumber], "ra40.fnt");
        font->setAnchorPoint(ccp(0, 0.6f));
        font->setPosition(ccp( ret->getContentSize().width - 60, ret->getContentSize().height - 26));
        ret->mCost = font;
        ret->addChild(font);
      }
      else {
        CCSprite *coin = CCSprite::createWithSpriteFrameName("go_coin.png");
        coin->setPosition(ccp( ret->getContentSize().width - 60, ret->getContentSize().height - 26));
        CCLabelBMFont *font = CCLabelBMFont::create(CCString::createWithFormat("%d", cost)->getCString(), "ra40.fnt");
        font->setAnchorPoint(ccp(0, 0.5f));
        font->setPosition(ccp(20, 5));
        coin->addChild(font);
        ret->mCost = coin;
        ret->addChild(coin);
      }
    }
    else {
      CCSprite *mark = CCSprite::createWithSpriteFrameName("sp_completed.png");
      mark->setAnchorPoint(ccp(0, 0.5f));
      mark->setPosition(ccp( ret->getContentSize().width - 60, ret->getContentSize().height - 26));
      ret->mCost = mark;
      ret->addChild(mark);
    }

    //draw title
    {
      CCString *name = CCString::createWithFormat("shop%d.png", ret->mTitle);
      ret->mTitleSp = CCSprite::createWithSpriteFrameName(name->getCString());
      ret->mTitleSp->setAnchorPoint(ccp(0, 0.5f));
      ret->mTitleSp->setPosition(ccp(100, ret->getContentSize().height - 26));
      ret->addChild(ret->mTitleSp);
    }

    //draw desc
    if( detail != NULL )
    {
      //ret->mDetailSp = CCLabelBMFont::create(detail, "ab34.fnt");
        ret->mDetailSp = CCLabelTTF::create(detail, GFONT_NAME, GFONT_SIZE_NORMAL);
      ret->mDetailSp->setAnchorPoint(ccp(0, 0.5f));
      ret->mDetailSp->setPosition(ccp(80, 25));
      ret->mDetailSp->setVisible(false);
      ret->addChild(ret->mDetailSp);
    }
  }

    ret->mIsAchievement = false;
    
  return ret;
}

void FoldItem::purchase(Ref* ref) 
{
    mController->purchase(getTag());
}

void FoldItem::toggleFold(bool flag) 
{
    if( mTitle < 0 )
        return;
    
    if( flag )
    {
        CCSpriteFrameCache *cache = CCSpriteFrameCache::sharedSpriteFrameCache();
        mBgButton->setDisplayFrame(cache->spriteFrameByName("shoplist.png"));
        mBgButton->setContentSize(CCSizeMake(435, 55));
        setContentSize(mBgButton->getContentSize());
        
        //update components
        mIcon->setPosition(ccp(mIcon->getPosition().x, getContentSize().height - 28 ));
        mTitleSp->setPosition(ccp(100, getContentSize().height - 26));
        mDetailSp->setVisible(false);
        
        if( !mIsAchievement )
        {
            mPurchase->setVisible(false);
            mCost->setPosition(ccp( getContentSize().width - 60, getContentSize().height - 26));
        }
    }
    else {
        CCSpriteFrameCache *cache = CCSpriteFrameCache::sharedSpriteFrameCache();
        mBgButton->setDisplayFrame(cache->spriteFrameByName("shoplist2.png"));
        mBgButton->setContentSize(CCSizeMake(435, 100));
        setContentSize(mBgButton->getContentSize());
        
        //update components
        mIcon->setPosition(ccp(mIcon->getPosition().x, getContentSize().height - 28 ));
        mTitleSp->setPosition(ccp(100, getContentSize().height - 26));
        mDetailSp->setVisible(true);
        
        if( !mIsAchievement )
        {
            mPurchase->setPosition(ccp(getContentSize().width - 48, 25));
            mPurchase->setVisible(true);
            mCost->setPosition(ccp( getContentSize().width - 60, getContentSize().height - 26));
            
            if(mCostNumber<= 0 )
            {
                mPurchase->setVisible(false);
            }
        }
    }
}

bool FoldItem::isFoldable() 
{
    if( mTitle >= 0 )
    {
        return true;
    }
    else
    {
        return false;
    }
}

void FoldItem::togglePurchaseButton(bool flag) 
{
    if( mMenu != NULL )
    {
        if(flag)
        {
            mMenu->setTouchEnabled(true);
        }
        else {
            mMenu->setTouchEnabled(false);
        }
    }
}

void FoldItem::updateInfo(int icon, int cost, const char * detail, int btntyp) 
{
    mCostNumber = cost;
    if( mTitle >= 0 )
    {
        //update icon
        if( mIconId >= 0 )
        {
            CCString *filename = CCString::createWithFormat("icon%d.png", icon);
            mIcon->setDisplayFrame(CCSprite::create(filename->getCString())->displayFrame());
        }
    }
    if( cost > 0 )
    {
        if( !mIsIAP )
        {
            //update cost
            removeChild(mCost, false);
            CCSprite *coin = CCSprite::createWithSpriteFrameName("go_coin.png");
            coin->setPosition(ccp( getContentSize().width - 60, getContentSize().height - 26));
            CCLabelBMFont *font = CCLabelBMFont::create(CCString::createWithFormat("%d", cost)->getCString(), "ra40.fnt");
            font->setAnchorPoint(ccp(0, 0.5f));
            font->setPosition(ccp(20, 5));
            coin->addChild(font);
            mCost = coin;
            this->addChild(coin);
        }
    }
    else {
        removeChild(mCost, false);
        CCSprite *mark = CCSprite::createWithSpriteFrameName("sp_completed.png");
        mark->setAnchorPoint(ccp(0, 0.5f));
        mark->setPosition(ccp( getContentSize().width - 60, getContentSize().height - 26));
        mCost = mark;
        this->addChild(mark);
        
        mPurchase->setVisible(false);
    }
    if( mDetail != NULL )
    {
        //update detail
        mDetailSp->setString(detail);
    }
}

/*
void FoldItem::dealloc() 
{
    mDetail->release();
    mDetail = NULL;
    super->dealloc();
}
*/

///////////////////////////////////////
