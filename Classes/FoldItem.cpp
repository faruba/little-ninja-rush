#include "FoldItem.h"
#include "GameConstants.h"

extern const char *gIAPCost[];

FoldItem* FoldItem::foldItem(const char* title, const char* icon, const char* detail, int achieve, int counter, FoldItemController* controller) 
{
    FoldItem *ret = FoldItem::create();
    
    {
        ret->mController = controller;
        
        ret->mBgButton = cocos2d::Sprite::createWithSpriteFrameName("shoplist.png");
        ret->mBgButton->setAnchorPoint(cocos2d::ccp(0, 0));
        ret->addChild(ret->mBgButton);
        ret->setContentSize(ret->mBgButton->getContentSize());
        ret->setAnchorPoint(cocos2d::ccp(0, 1));
        
        //draw content
        if( counter < achieve )
        {
            ret->mIcon = cocos2d::Sprite::createWithSpriteFrameName("cj_unlocked.png");
            ret->mIcon->setPosition(cocos2d::ccp(43, ret->getContentSize().height - 28 ));
        }
        else {
            ret->mIcon = cocos2d::Sprite::createWithSpriteFrameName("cj-bg.png");
            ret->mIcon->setPosition(cocos2d::ccp(43, ret->getContentSize().height - 28 ));
            cocos2d::Sprite *real = cocos2d::Sprite::createWithSpriteFrameName(icon);
            real->setPosition(cocos2d::ccp( ret->mIcon->getContentSize().width/2, ret->mIcon->getContentSize().height/2));
            ret->mIcon->addChild(real);
        }
        ret->addChild(ret->mIcon);
        
        //draw cost
        if( counter < achieve )
        {
            if( achieve > 1 )
            {
                cocos2d::CCString *progress = cocos2d::CCString::createWithFormat("%d / %d", counter, achieve);
                cocos2d::CCLabelBMFont *font = cocos2d::CCLabelBMFont::create(progress->getCString(), "ra40.fnt");
                font->setAnchorPoint(cocos2d::ccp(0.5f , 0.6f));
                font->setPosition(cocos2d::ccp( ret->getContentSize().width - 60, ret->getContentSize().height - 26));
                ret->mCost = font;
                ret->addChild(font);
            }
        }
        else {
            cocos2d::Sprite *mark = cocos2d::Sprite::createWithSpriteFrameName("sp_completed.png");
            mark->setAnchorPoint(cocos2d::ccp(0, 0.5f));
            mark->setPosition(cocos2d::ccp( ret->getContentSize().width - 60, ret->getContentSize().height - 26));
            ret->mCost = mark;
            ret->addChild(mark);
        }
        
        //draw title
        {
            ret->mTitleSp = cocos2d::Sprite::create(title);
            ret->mTitleSp->setAnchorPoint(cocos2d::ccp(0, 0.5f));
            ret->mTitleSp->setPosition(cocos2d::ccp(100, ret->getContentSize().height - 26));
            ret->addChild(ret->mTitleSp);
        }
        
        //draw desc
        if( detail != NULL )
        {
            //ret->mDetailSp = cocos2d::CCLabelBMFont::create(detail, "ab34.fnt");
            ret->mDetailSp = cocos2d::CCLabelTTF::create(detail, GFONT_NAME, GFONT_SIZE_NORMAL);
            ret->mDetailSp->setAnchorPoint(cocos2d::ccp(0, 0.5f));
            ret->mDetailSp->setPosition(cocos2d::ccp(80, 25));
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
    cocos2d::CCString *filename = cocos2d::CCString::createWithFormat("sp_category%d.png", icon);
    cocos2d::Sprite *sp = cocos2d::Sprite::createWithSpriteFrameName(filename->getCString());
    sp->setAnchorPoint(cocos2d::ccp(0, 1));
    ret->addChild(sp);
    ret->setContentSize(sp->getContentSize());
  }
  else
  {
    ret->mDetail = cocos2d::CCString::create(detail);
    ret->mDetail->retain();
    ret->mBgButton = cocos2d::Sprite::createWithSpriteFrameName("shoplist.png");
    ret->mBgButton->setAnchorPoint(cocos2d::ccp(0, 0));
    ret->addChild(ret->mBgButton);

    if( btntyp == 1 )
    {
      ret->mPurchase = cocos2d::CCMenuItemImage::create();
      ret->mPurchase->setNormalSpriteFrame(cocos2d::SpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("sp_buy1.png"));
      ret->mPurchase->setSelectedSpriteFrame(cocos2d::SpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("sp_buy2.png"));
      ret->mPurchase->setTarget(ret, menu_selector(FoldItem::purchase));
    }
    else {
      ret->mPurchase = cocos2d::CCMenuItemImage::create();
      ret->mPurchase->setNormalSpriteFrame(cocos2d::SpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("sp_upgrade1.png"));
      ret->mPurchase->setSelectedSpriteFrame(cocos2d::SpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("sp_upgrade2.png"));
      ret->mPurchase->setTarget(ret, menu_selector(FoldItem::purchase));
    }

    ret->mPurchase->setVisible(false);
    ret->mMenu = cocos2d::CCMenu::create(ret->mPurchase, NULL);
    ret->mMenu->setPosition(cocos2d::ccp(0, 0));
    ret->addChild(ret->mMenu);
    ret->setContentSize(ret->mBgButton->getContentSize());
    ret->setAnchorPoint(cocos2d::ccp(0, 1));

    //draw content
    if( icon < 0 )
    {
      switch (icon) {
        case -1:
          {
            ret->mIcon = cocos2d::Sprite::createWithSpriteFrameName("sp_girl.png");
          }
          break;
        case -2:
          {
            ret->mIcon = cocos2d::Sprite::createWithSpriteFrameName("sp_musashi.png");
          }
          break;
        case -3:
          {
            ret->mIcon = cocos2d::Sprite::createWithSpriteFrameName("sp_cat.png");
          }
          break;
      }
      ret->mIcon->setPosition(cocos2d::ccp(43, ret->getContentSize().height - 28 ));
    }
    else {
      cocos2d::CCString *filename = cocos2d::CCString::createWithFormat("icon%d.png", icon);
      ret->mIcon = cocos2d::Sprite::create(filename->getCString());
      ret->mIcon->setPosition(cocos2d::ccp(33, ret->getContentSize().height - 28 ));
    }
    ret->addChild(ret->mIcon);

    //draw cost
    if( cost > 0 )
    {
      if( iap )
      {
        cocos2d::CCLabelBMFont *font = cocos2d::CCLabelBMFont::create(gIAPCost[ret->mCostNumber], "ra40.fnt");
        font->setAnchorPoint(cocos2d::ccp(0, 0.6f));
        font->setPosition(cocos2d::ccp( ret->getContentSize().width - 60, ret->getContentSize().height - 26));
        ret->mCost = font;
        ret->addChild(font);
      }
      else {
        cocos2d::Sprite *coin = cocos2d::Sprite::createWithSpriteFrameName("go_coin.png");
        coin->setPosition(cocos2d::ccp( ret->getContentSize().width - 60, ret->getContentSize().height - 26));
        cocos2d::CCLabelBMFont *font = cocos2d::CCLabelBMFont::create(cocos2d::CCString::createWithFormat("%d", cost)->getCString(), "ra40.fnt");
        font->setAnchorPoint(cocos2d::ccp(0, 0.5f));
        font->setPosition(cocos2d::ccp(20, 5));
        coin->addChild(font);
        ret->mCost = coin;
        ret->addChild(coin);
      }
    }
    else {
cocos2d::Sprite *mark = cocos2d::Sprite::createWithSpriteFrameName("sp_completed.png");
      mark->setAnchorPoint(cocos2d::ccp(0, 0.5f));
      mark->setPosition(cocos2d::ccp( ret->getContentSize().width - 60, ret->getContentSize().height - 26));
      ret->mCost = mark;
      ret->addChild(mark);
    }

    //draw title
    {
cocos2d::CCString *name = cocos2d::CCString::createWithFormat("shop%d.png", ret->mTitle);
      ret->mTitleSp = cocos2d::Sprite::createWithSpriteFrameName(name->getCString());
      ret->mTitleSp->setAnchorPoint(cocos2d::ccp(0, 0.5f));
      ret->mTitleSp->setPosition(cocos2d::ccp(100, ret->getContentSize().height - 26));
      ret->addChild(ret->mTitleSp);
    }

    //draw desc
    if( detail != NULL )
    {
      //ret->mDetailSp = cocos2d::CCLabelBMFont::create(detail, "ab34.fnt");
        ret->mDetailSp = cocos2d::CCLabelTTF::create(detail, GFONT_NAME, GFONT_SIZE_NORMAL);
      ret->mDetailSp->setAnchorPoint(cocos2d::ccp(0, 0.5f));
      ret->mDetailSp->setPosition(cocos2d::ccp(80, 25));
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
cocos2d::SpriteFrameCache *cache = cocos2d::SpriteFrameCache::sharedSpriteFrameCache();
        mBgButton->setDisplayFrame(cache->spriteFrameByName("shoplist.png"));
        mBgButton->setContentSize(cocos2d::CCSizeMake(435, 55));
        setContentSize(mBgButton->getContentSize());
        
        //update components
        mIcon->setPosition(cocos2d::ccp(mIcon->getPosition().x, getContentSize().height - 28 ));
        mTitleSp->setPosition(cocos2d::ccp(100, getContentSize().height - 26));
        mDetailSp->setVisible(false);
        
        if( !mIsAchievement )
        {
            mPurchase->setVisible(false);
            mCost->setPosition(cocos2d::ccp( getContentSize().width - 60, getContentSize().height - 26));
        }
    }
    else {
cocos2d::SpriteFrameCache *cache = cocos2d::SpriteFrameCache::sharedSpriteFrameCache();
        mBgButton->setDisplayFrame(cache->spriteFrameByName("shoplist2.png"));
        mBgButton->setContentSize(cocos2d::CCSizeMake(435, 100));
        setContentSize(mBgButton->getContentSize());
        
        //update components
        mIcon->setPosition(cocos2d::ccp(mIcon->getPosition().x, getContentSize().height - 28 ));
        mTitleSp->setPosition(cocos2d::ccp(100, getContentSize().height - 26));
        mDetailSp->setVisible(true);
        
        if( !mIsAchievement )
        {
            mPurchase->setPosition(cocos2d::ccp(getContentSize().width - 48, 25));
            mPurchase->setVisible(true);
            mCost->setPosition(cocos2d::ccp( getContentSize().width - 60, getContentSize().height - 26));
            
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
cocos2d::CCString *filename = cocos2d::CCString::createWithFormat("icon%d.png", icon);
            mIcon->setDisplayFrame(cocos2d::Sprite::create(filename->getCString())->displayFrame());
        }
    }
    if( cost > 0 )
    {
        if( !mIsIAP )
        {
            //update cost
            removeChild(mCost, false);
cocos2d::Sprite *coin = cocos2d::Sprite::createWithSpriteFrameName("go_coin.png");
            coin->setPosition(cocos2d::ccp( getContentSize().width - 60, getContentSize().height - 26));
cocos2d::CCLabelBMFont *font = cocos2d::CCLabelBMFont::create(cocos2d::CCString::createWithFormat("%d", cost)->getCString(), "ra40.fnt");
            font->setAnchorPoint(cocos2d::ccp(0, 0.5f));
            font->setPosition(cocos2d::ccp(20, 5));
            coin->addChild(font);
            mCost = coin;
            this->addChild(coin);
        }
    }
    else {
        removeChild(mCost, false);
cocos2d::Sprite *mark = cocos2d::Sprite::createWithSpriteFrameName("sp_completed.png");
        mark->setAnchorPoint(cocos2d::ccp(0, 0.5f));
        mark->setPosition(cocos2d::ccp( getContentSize().width - 60, getContentSize().height - 26));
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
