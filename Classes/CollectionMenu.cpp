#include "CollectionMenu.h"
#include "GameTool.h"
#include "GameRecord.h"
#include "GameConstants.h"
#include "TitleMenu.h"
#include "SelectMenu.h"
#include "GameData.h"
#include "UniversalFit.h"
#include "CEClipedNode.h"
#include "PublicLoad.h"
#include "SimpleAudioEngine.h"
#include "GTAnimatedEffect.h"
#include "GamePlay.h"
//#include "MidBridge.h"

using namespace CocosDenshion;
static int gNavBack = 0;

void CollectionMenu::setNavBack(int nid) {
  gNavBack = nid;
}

void CollectionMenu::setModal(const char * title, const char * desc, Ref* target, SEL_CallFunc sel)
{
  if( mModal )
    return ;
  mModal = true;
  mMask->setVisible(true);
  mModalTarget = target;
  mModalSel = sel;
  mMask->removeAllChildrenWithCleanup(true);
  //mItems->setTouchEnabled(false;//1.2.0)
  mTouchEnabled = false;
  mMenu->setTouchEnabled(false);
  mPowerupMenu->setTouchEnabled(false);
  cocos2d::Sprite *spbg = cocos2d::Sprite::createWithSpriteFrameName("sp-tc.png");
  spbg->setPosition(cocos2d::Vec2(SCREEN_WIDTH/2, SCREEN_HEIGHT/2));
  mMask->addChild(spbg);
  cocos2d::Sprite *titlesp = cocos2d::Sprite::createWithSpriteFrameName(title);
  titlesp->setPosition(cocos2d::Vec2( spbg->getContentSize().width/2, 105));
  spbg->addChild(titlesp);
  cocos2d::Sprite *descsp = cocos2d::Sprite::createWithSpriteFrameName(desc);
  descsp->setPosition(cocos2d::Vec2( spbg->getContentSize().width/2, 75));
  spbg->addChild(descsp);
  cocos2d::CCMenuItemImage *menuConfirm = cocos2d::CCMenuItemImage::create("pu-confirm1.png", "pu-confirm2.png", this, menu_selector(CollectionMenu::onModalConfirm));
  menuConfirm->setPosition(cocos2d::Vec2(75, 35));
  cocos2d::CCMenuItemImage *menuCancel = cocos2d::CCMenuItemImage::create("pu-cancel1.png", "pu-cancel2.png", this, menu_selector(CollectionMenu::onModalCancel));

  menuCancel->setPosition(cocos2d::Vec2(200, 35));
  cocos2d::CCMenu *menu = cocos2d::CCMenu::create(menuConfirm, menuCancel, NULL);
  menu->setPosition(cocos2d::Vec2(0, 0));
  spbg->addChild(menu);
}

void CollectionMenu::onModalConfirm(cocos2d::Ref*)
{
  mModal = false;
  mMask->setVisible(false);
  //mItems->setTouchEnabled(true;//1.2.0)
  mTouchEnabled = true;
  mMenu->setTouchEnabled(true);
  mPowerupMenu->setTouchEnabled(true);
  this->runAction(cocos2d::CallFunc::create(mModalTarget, mModalSel));
  GameTool::PlaySound("sound/charge.mp3");
}

void CollectionMenu::onModalCancel(cocos2d::Ref*Hjj)
{
  mModal = false;
  mMask->setVisible(false);
  //mItems->setTouchEnabled(true;//1.2.0)
  mTouchEnabled = true;
  mMenu->setTouchEnabled(true);
  mPowerupMenu->setTouchEnabled(true);
}

void CollectionMenu::onEnter()
{
  mUISwapper.onEnter();
  PublicLoad::menuCollection()->loadAll();
  mModal = false;

  //do some initialization
  cocos2d::Node *taskcomplete = cocos2d::Node::create();
  taskcomplete->setPosition(cocos2d::Vec2(SCREEN_WIDTH/2, SCREEN_HEIGHT));
  this->addChild(taskcomplete, 5);
  GamePlay::setTaskCompleteNode(taskcomplete);
  //----------
  cocos2d::Node * node = createUIByCCBI("menu-collection", "CollectionMenu", CollectionMenuLayerLoader::loader(), this);
  if(node != NULL) {
    this->addChild(node, 5);
  }

  //1.2.0 SinaWeibo
  if( UniversalFit::sharedUniversalFit()->shouldUsingSinaWeibo() )
  {
    mTwitter->setDisabledSpriteFrame(cocos2d::SpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("go_weibo.png"));
    mTwitter->setNormalSpriteFrame(cocos2d::SpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("go_weibo.png"));
    mTwitter->setSelectedSpriteFrame(cocos2d::SpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("go_weibo.png"));
    mTwitterAction->setDisplayFrame(cocos2d::SpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("go_weibo2.png"));
  }

  mScroll = ABScrollContent::create();
  cocos2d::CCRect clip = cocos2d::CCRectMake(mScrollPoint->getPosition().x, mScrollPoint->getPosition().y, 322, 177.5f);
  mScroll->clickTarget = this;
  mScroll->clickMethod = callfunc_selector(CollectionMenu::clickMethod);
  mScroll->setClipRect(clip);
  mScrollPoint->addChild(mScroll, -1);


  CollectionMenuDelegate::mScroll = mScroll;
  CollectionMenuDelegate::mItemTitle = mItemTitle;
  CollectionMenuDelegate::mItemDescription = mItemDesc;

  mShadowDir = ccpNormalize(cocos2d::Vec2(-12, 17));
  this->scheduleUpdate();
  this->setTouchEnabled(true);

  mMask = cocos2d::LayerColor::create(Color4B(0, 0, 0, 128));
  mMask->setOpacity(128);
  mMask->setVisible(false);
  this->addChild(mMask, 10);

  //初始化人物按钮
  int currRole = GameRecord::sharedGameRecord()->curr_char;
  for(int i=0; i<GAME_CHARCOUNT; ++i) {
    cocos2d::CCMenuItemImage *btn = character(i);
    btn->setNormalImage(cocos2d::Sprite::createWithSpriteFrameName(cocos2d::CCString::createWithFormat("sc_role%d%d.png", i, i==currRole?0:1)->getCString()));
    btn->setSelectedImage(cocos2d::Sprite::createWithSpriteFrameName(cocos2d::CCString::createWithFormat("sc_role%d%d.png", i, i==currRole?1:0)->getCString()));
  }
  //初始化分类按钮
  // TODO:Refactor this
  mShurikenDelegate.updateButtonImage(false);
  mKatanaDelegate.updateButtonImage(false);
  mSpecialDelegate.updateButtonImage(false);
  mPowerUpDelegate.updateButtonImage(false);
  switch (gNavBack) {
    case 2:
      mCurrentDelegate = &mKatanaDelegate;
      break;
    case 3:
      mCurrentDelegate = &mSpecialDelegate;
      break;
    default:
      mCurrentDelegate = &mShurikenDelegate;
      break;
  }
  mCurrentDelegate->updateButtonImage(true);
  mCurrentDelegate->updateScroll();

  mScrollCount->setString(cocos2d::CCString::createWithFormat("x%d", GameRecord::sharedGameRecord()->collection->magic_piece)->getCString());

  mCurrentDelegate->updateItemInfo();
  mUse->setNormalImage(cocos2d::Sprite::createWithSpriteFrameName("sc_equiped2.png"));
  mUse->setSelectedImage(cocos2d::Sprite::createWithSpriteFrameName("sc_equiped2.png"));

  mUISwapper.setSceneIntro(this);

  GameTool::PlayBackgroundMusic("menu.mp3");

  mPostingFacebook = false;
  mPostingTwitter = false;


  //MidBridge::sharedMidbridge()->setFacebookCallback(this, menu_selector(CollectionMenu::doneFacebook));
  //MidBridge::sharedMidbridge()->setTwitterCallback(this, menu_selector(CollectionMenu::doneTwitter:));
  //toggleShare(true);
  cocos2d::Layer::onEnter();
}

void CollectionMenu::onExit()
{
  GameRecord::sharedGameRecord()->collection->cleanNewList();
  GamePlay::setTaskCompleteNode(NULL);

  //MidBridge::sharedMidbridge()->setFacebookCallback(NULL, menu_selector(CollectionMenu::doneFacebook));
  //MidBridge::sharedMidbridge()->setTwitterCallback(NULL, menu_selector(CollectionMenu::doneTwitter));
  cocos2d::Layer::onExit();
}

cocos2d::CCMenuItemImage* CollectionMenu::character(int rid)
{
  switch (rid) {
    case 0:
      return mCharacter1;
    case 1:
      return mCharacter2;
    case 2:
      return mCharacter3;
    case 3:
      return mCharacter4;
  }
  return NULL;
}

void CollectionMenu::update(float delta)
{
  //update shadow
  cocos2d::Point np = ccpAdd(mShadow->getPosition(), ccpMult(mShadowDir, delta*7));
  if( np.x < -10 )
  {
    np.x = -10;
    mShadowDir.x *= -1;
  }
  if( np.x > 0 )
  {
    np.x = 0;
    mShadowDir.x *= -1;
  }
  if( np.y < 320 )
  {
    np.y = 320;
    mShadowDir.y *= -1;
  }
  if( np.y > 330 )
  {
    np.y = 330;
    mShadowDir.y *= -1;
  }
  mShadow->setPosition(np);
}
void CollectionMenu::onBack(cocos2d::Ref*)
{
  if(mUISwapper.isDone())
  {
    GameTool::PlaySound("sound/menu-change.mp3");
    if( gNavBack == 0 ) {
      mUISwapper.setSceneOutro(GameTool::scene<TitleMenu>(), this);
    } else {
      mUISwapper.setSceneOutro(GameTool::scene<SelectMenu>(), this);
    }
  }
}

void CollectionMenu::onUse(cocos2d::Ref*) {
  mCurrentDelegate->onUse();
}

void CollectionMenu::onCharacter(int nRole)
{
  int orole = GameRecord::sharedGameRecord()->curr_char;
  if( orole == nRole )
    return;
  GameTool::PlaySound("sound/click.mp3");
cocos2d::CCMenuItemImage *orl = character(orole);
  orl->setNormalImage(cocos2d::Sprite::createWithSpriteFrameName(cocos2d::CCString::createWithFormat("sc_role%d%d.png", orole, 1)->getCString()));
  orl->setSelectedImage(cocos2d::Sprite::createWithSpriteFrameName(cocos2d::CCString::createWithFormat("sc_role%d%d.png", orole, 0)->getCString()));

  int nrole = nRole;
cocos2d::CCMenuItemImage *nr = character(nrole);
  nr->setNormalImage(cocos2d::Sprite::createWithSpriteFrameName(cocos2d::CCString::createWithFormat("sc_role%d%d.png", nrole, 0)->getCString()));
  nr->setSelectedImage(cocos2d::Sprite::createWithSpriteFrameName(cocos2d::CCString::createWithFormat("sc_role%d%d.png", nrole, 1)->getCString()));
  GameRecord::sharedGameRecord()->curr_char = nrole;

  if( mCurrentDelegate != &mPowerUpDelegate )
  {
    mCurrentDelegate->markCurrent(-1);
    mCurrentDelegate->updateUsing();
    mCurrentDelegate->updateItemInfo();
  } else {
    updateCharacterInfo(nRole, mCurrItem);
    updatePowerUpButton();
  }

  mCurrentDelegate->updateScroll();
}



void CollectionMenu::onItem(CollectionMenuDelegate *newDelegate)
{
  if( newDelegate == mCurrentDelegate )
    return ;

  GameTool::PlaySound("sound/click.mp3");
  mCurrentDelegate->updateButtonImage(false);
  newDelegate->updateButtonImage(true);
  mCurrentDelegate = newDelegate;

  mItemDesc->setVisible(true);

  mCurrentDelegate->updateScroll();
  mCurrentDelegate->updateItemInfo();

  mPowerUp->setVisible(false);
  mUse->setNormalImage(cocos2d::Sprite::createWithSpriteFrameName("sc_equiped2.png"));
  mUse->setSelectedImage(cocos2d::Sprite::createWithSpriteFrameName("sc_equiped2.png"));
  mUse->setVisible(true);
  mScroll->resetContentPosition();

  toggleShare(true);
}

void CollectionMenu::onPowerup(cocos2d::Ref*)
{
  CollectionMenuDelegate *newDelegate = &mPowerUpDelegate;
  if( newDelegate == mCurrentDelegate )
    return ;

  GameTool::PlaySound("sound/click.mp3");
  mCurrentDelegate->updateButtonImage(false);
  newDelegate->updateButtonImage(true);
  mCurrentDelegate = newDelegate;

  mCurrentDelegate->updateScroll();

  mPowerUp->setVisible(true);

  this->onSelectLife(nullptr);

  toggleShare(false);
}

void CollectionMenu::updateCharacterInfo(int rid, int bid)
{
   mLifeGuage->removeAllChildrenWithCleanup(false);
   for( int i=0; i<GameData::roleMaxHP(rid); ++i)
   {
     if( i<GameData::roleCurrHP(rid) )
     {
cocos2d::Sprite *sp = cocos2d::Sprite::createWithSpriteFrameName("heart.png");
       sp->setPosition(cocos2d::Vec2(i*18, 0));
       mLifeGuage->addChild(sp, 0, i);
     }
     else {
       if( bid == 1 && i == GameData::roleCurrHP(rid) )
       {
         GTAnimatedEffect *sp = GTAnimatedEffect::create(GTAnimation::loadedAnimationSet("ui"), 0, true);
         sp->setPosition(cocos2d::Vec2(i*18, 0));
         mLifeGuage->addChild(sp);
       }
       else {
cocos2d::Sprite *sp = cocos2d::Sprite::createWithSpriteFrameName("pulife2.png");
         sp->setPosition(cocos2d::Vec2(i*18, 0));
         mLifeGuage->addChild(sp);
       }
     }
   }

   mDartGuage->removeAllChildrenWithCleanup(false);
   for( int i=0; i<GameData::roleMaxDart(rid); ++i)
   {
     if( i<GameData::roleCurrDart(rid) )
     {
cocos2d::Sprite *sp = cocos2d::Sprite::createWithSpriteFrameName("ui-dart.png");
       sp->setPosition(cocos2d::Vec2(i*12, 0));
       mDartGuage->addChild(sp);
     }
     else {
       if( bid == 2 && i == GameData::roleCurrDart(rid) )
       {
         GTAnimatedEffect *sp = GTAnimatedEffect::create(GTAnimation::loadedAnimationSet("ui"), 1, true);
         sp->setPosition(cocos2d::Vec2(i*12, 0));
         mDartGuage->addChild(sp);
       }
       else {
cocos2d::Sprite *sp = cocos2d::Sprite::createWithSpriteFrameName("pushuriken2.png");
         sp->setPosition(cocos2d::Vec2(i*12, 0));
         mDartGuage->addChild(sp);
       }
     }
   }
}

void CollectionMenu::onItemCallback(int i)
{
//  //获取当前道具的信息
//  if( mCurrentType < 3 )
//  {
//    bool collected = false;
//    switch (mCurrentType) {
//      case 0:
//        {
//          Shuriken *sh = (Shuriken*)(GameData::fetchShurikens()->objectAtIndex(i));
//          collected = GameRecord::sharedGameRecord()->collection->isItemCompleted(sh->uiid);
//          if( !collected && GameRecord::sharedGameRecord()->collection->itemLostPiece(sh->uiid) == GameRecord::sharedGameRecord()->collection->itemTotalPiece(sh->uiid) )
//          {
//            GameTool::PlaySound("sound/error.mp3");
//            return;
//          }
//        }
//        break;
//      case 1:
//        {
//          int index = i + GAME_CHARCOUNT - 1;
//          if( i == 0 )
//          {
//            index = GameRecord::sharedGameRecord()->curr_char;
//          }
//          Katana *sh = (Katana*)GameData::fetchKatanas()->objectAtIndex(index);
//          collected = GameRecord::sharedGameRecord()->collection->isItemCompleted(sh->uiid);
//          if( !collected && GameRecord::sharedGameRecord()->collection->itemLostPiece(sh->uiid) == GameRecord::sharedGameRecord()->collection->itemTotalPiece(sh->uiid) )
//          {
//            GameTool::PlaySound("sound/error.mp3");
//            return;
//          }
//        }
//        break;
//      case 2:
//        {
//          Special *sh = (Special*)GameData::fetchSpecials()->objectAtIndex(i);
//          collected = GameRecord::sharedGameRecord()->collection->isItemCompleted(sh->uiid);
//          if( !collected && GameRecord::sharedGameRecord()->collection->itemLostPiece(sh->uiid) == GameRecord::sharedGameRecord()->collection->itemTotalPiece(sh->uiid) )
//          {
//            GameTool::PlaySound("sound/error.mp3");
//            return;
//          }
//        }
//        break;
//    }
//  }
//  GameTool::PlaySound("sound/click.mp3");
//  markCurrent(i);
//  mCurrentDelegate->updateItemInfo();
}

void CollectionMenu::updatePowerUpButton()
{
  bool flag = false;
  int rid = GameRecord::sharedGameRecord()->curr_char;
  if( mCurrItem == 1 )
  {
    if( GameRecord::sharedGameRecord()->collection->life_piece >= 9 &&
        GameData::roleCurrHP(rid) < GameData::roleMaxHP(rid) )
    {
      flag = true;
    }
    mUse->setVisible(true);
  }
  else if( mCurrItem == 2 )
  {
    if( GameRecord::sharedGameRecord()->collection->dart_piece >= 9 &&
        GameData::roleCurrDart(rid) < GameData::roleMaxDart(rid) )
    {
      flag = true;
    }
    mUse->setVisible(true);
  }
  else {
    mUse->setVisible(false);
  }
  if( flag )
  {
    mUse->setNormalImage(cocos2d::Sprite::createWithSpriteFrameName("sc_use1.png"));
    mUse->setSelectedImage(cocos2d::Sprite::createWithSpriteFrameName("sc_use2.png"));
  }
  else {
    mUse->setNormalImage(cocos2d::Sprite::createWithSpriteFrameName("sc_use2.png"));
    mUse->setSelectedImage(cocos2d::Sprite::createWithSpriteFrameName("sc_use2.png"));
  }
}

void CollectionMenu::onSelectLife(cocos2d::Ref*)
{
  if( mCurrItem != 1 )
  {
    GameTool::PlaySound("sound/click.mp3");
    mCurrentMark->setPosition(cocos2d::Vec2(385, 145));
    mCurrentMark->setVisible(true);
    mCurrItem = 1;
    updateCharacterInfo(GameRecord::sharedGameRecord()->curr_char, mCurrItem);

    //buttons & title
    mItemTitle->setDisplayFrame(cocos2d::Sprite::create("lifeplus.png")->displayFrame());
    this->updatePowerUpButton();
  }
}

void CollectionMenu::onSelectDart(cocos2d::Ref*)
{
  if( mCurrItem != 2 )
  {
    GameTool::PlaySound("sound/click.mp3");
    mCurrentMark->setPosition(cocos2d::Vec2(385, 65));
    mCurrentMark->setVisible(true);
    mCurrItem = 2;
    updateCharacterInfo(GameRecord::sharedGameRecord()->curr_char, mCurrItem);

    //buttons & title
    mItemTitle->setDisplayFrame(cocos2d::Sprite::create("dartplus.png")->displayFrame());
    this->updatePowerUpButton();
  }
}

void CollectionMenu::clickMethod()
{
  cocos2d::Point click = mScroll->clickPoint;
  int x = click.x / 79;
  int y = click.y / 59;
  int cid = x + y*4;
  if( cid < mItemCount && cid >= 0 )
  {
    onItemCallback(cid);
  }
}

void CollectionMenu::toggleShare(bool flag)
{
  /*
  if( flag )
  {
    if( ABSocial->isAvailable() )
    {
      mShare->setVisible(true);
      mFacebook->setVisible(true);
      mTwitter->setVisible(true);
      mFacebookAction->setVisible(true);
      mTwitterAction->setVisible(true);
      mTwitter.isEnabled = true;
    }
    else
    {
      mTwitter->setColor(ccc3(128, 128, 128));
      mTwitterAction->setColor(ccc3(128, 128, 128));
      mTwitter.isEnabled = false;
      mFacebook->setColor(ccc3(128, 128, 128));
      mFacebookAction->setColor(ccc3(128, 128, 128));
      mFacebook.isEnabled = false;
    }
  }
  else
  {
    mShare->setVisible(false);
    mFacebook->setVisible(false);
    mTwitter->setVisible(false);
    mFacebookAction->setVisible(false);
    mTwitterAction->setVisible(false);
  }
  */
}
/*
id CollectionMenu::getCurrentCollectedItem()
{
  int idx = mCurrItem;
  if( idx < 0 )
  {
    idx = mEquipedItem;
  }
  if( mCurrentType < 3 )
  {
    bool collected = false;
    id ret = NULL;
    switch (mCurrentType) {
      case 0:
        {
          Shuriken *sh = (Shuriken*)GameData::fetchShurikens()->objectAtIndex(idx);
          collected = [GameRecord::sharedGameRecord()->collection->isItemCompleted:sh->uiid];
          ret = sh;
        }
        break;
      case 1:
        {
          int index = idx + GAME_CHARCOUNT - 1;
          if( idx == 0 )
          {
            index = GameRecord::sharedGameRecord()->curr_char;
          }
          Katana *sh = (Katana*)GameData::fetchKatanas()->objectAtIndex(index);
          collected = [GameRecord::sharedGameRecord()->collection->isItemCompleted:sh->uiid];
          ret = sh;
        }
        break;
      case 2:
        {
          Special *sh = (Special*)GameData::fetchSpecials()->objectAtIndex(idx);
          collected = [GameRecord::sharedGameRecord()->collection->isItemCompleted:sh->uiid];
          ret = sh;
        }
        break;
    }
    if( collected && ret != NULL )
    {
      return ret;
    }
    else
    {
      return NULL;
    }
  }
  else
  {
    return NULL;
  }
}
*/
void CollectionMenu::onFacebook(cocos2d::Ref*)
{/*
  if( !mPostingFacebook )
  {
    ABSocial *social = ABSocial->feedOnSinaWeibo();
    switch (mCurrentType) {
      case 0:
        {
          Shuriken *sh = this->getCurrentCollectedItem();
          social.text = cocos2d::CCString::createWithFormat("I have just collected %@ in Little Ninja Rush!", sh->titl);
          social.image = UIImage->imageWithContentsOfFile(cocos2d::CCFileUtils->sharedFileUtils()->fullPathFromRelativePath( sh->icon));
        }
        break;
      case 1:
        {
          Katana *sh = this->getCurrentCollectedItem();
          social.text = cocos2d::CCString::createWithFormat("I have just collected %@ in Little Ninja Rush!", sh->titl);
          social.image = UIImage->imageWithContentsOfFile(cocos2d::CCFileUtils->sharedFileUtils()->fullPathFromRelativePath( sh->icon));
        }
        break;
      case 2:
        {
          Special *sh = this->getCurrentCollectedItem();
          social.text = cocos2d::CCString::createWithFormat("I have just collected %@ in Little Ninja Rush!", sh->titl);
          social.image = UIImage->imageWithContentsOfFile(cocos2d::CCFileUtils->sharedFileUtils()->fullPathFromRelativePath( sh->icon));
        }
        break;
    }
    social.callbackObject = MidBridge->sharedMidbridge();
    social.callbackSelector =, );
    AppController *del = (AppController*)UIApplication->sharedApplication().delegate;
    social->present(del.navController);

    mFacebookAction->setDisplayFrame(cocos2d::SpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("ftloading.png"));
    mFacebookAction->stopAllActions();
cocos2d::CCRotateBy *rb = cocos2d::CCRotateBy::create(1, 360);
cocos2d::CCRepeatForever *rf = cocos2d::CCRepeatForever::create(rb);
    mFacebookAction->runAction(rf);

    mPostingFacebook = true;
  }
  */
}

void CollectionMenu::doneFacebook(cocos2d::CCInteger* res)
{
  /*
  mPostingFacebook = false;

  mFacebookAction->stopAllActions();
  mFacebookAction->setRotation(0);
  mFacebookAction->setDisplayFrame(cocos2d::SpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("go_facebook2.png"));
  */
}

void CollectionMenu::onTwitter(cocos2d::Ref*)
{/*
  if( !mPostingTwitter )
  {
    mPostingTwitter = true;
    AppController *del = (AppController*)UIApplication->sharedApplication().delegate;
    ABSocial *social = NULL;
    bool flagSINA = UniversalFit::sharedUniversalFit()->shouldUsingSinaWeibo();
    if( flagSINA )
    {
      social = ABSocial->feedOnSinaWeibo();
    }
    else
    {
      social = ABSocial->feedOnTwitter();
    }
    switch (mCurrentType) {
      case 0:
        {
          Shuriken *sh = this->getCurrentCollectedItem();
          if( flagSINA )
          {
            social.text = cocos2d::CCString::createWithFormat("刚刚在#LittleNinjaRush#里收集到了%@，一起来玩吧!", sh->titl);
          }
          else
          {
            social.text = cocos2d::CCString::createWithFormat("I have just collected %@ in #LittleNinjaRush!", sh->titl);
          }
          social.url = NSURL->URLWithString(TWITTER_URL);
          social.image = UIImage->imageWithContentsOfFile(cocos2d::CCFileUtils->sharedFileUtils()->fullPathFromRelativePath( sh->icon));
        }
        break;
      case 1:
        {
          Katana *sh = this->getCurrentCollectedItem();
          if( flagSINA )
          {
            social.text = cocos2d::CCString::createWithFormat("刚刚在#LittleNinjaRush#里收集到了%@，一起来玩吧!", sh->titl);
          }
          else
          {
            social.text = cocos2d::CCString::createWithFormat("I have just collected %@ in #LittleNinjaRush!", sh->titl);
          }
          social.url = NSURL->URLWithString(TWITTER_URL);
          social.image = UIImage->imageWithContentsOfFile(cocos2d::CCFileUtils->sharedFileUtils()->fullPathFromRelativePath( sh->icon));
        }
        break;
      case 2:
        {
          Special *sh = this->getCurrentCollectedItem();
          if( flagSINA )
          {
            social.text = cocos2d::CCString::createWithFormat("刚刚在#LittleNinjaRush#里收集到了%@，一起来玩吧!", sh->titl);
          }
          else
          {
            social.text = cocos2d::CCString::createWithFormat("I have just collected %@ in #LittleNinjaRush!", sh->titl);
          }
          social.url = NSURL->URLWithString(TWITTER_URL);
          social.image = UIImage->imageWithContentsOfFile(cocos2d::CCFileUtils->sharedFileUtils()->fullPathFromRelativePath( sh->icon));
        }
        break;
    }
    social.callbackObject = MidBridge->sharedMidbridge();
    social.callbackSelector =, );
    social->present(del.navController);

    mTwitterAction->setDisplayFrame(cocos2d::SpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("ftloading.png"));
    mTwitterAction->stopAllActions();
cocos2d::CCRotateBy *rb = cocos2d::CCRotateBy::create(1, 360);
cocos2d::CCRepeatForever *rf = cocos2d::CCRepeatForever::create(rb);
    mTwitterAction->runAction(rf);
  }
*/
}

void CollectionMenu::doneTwitter(cocos2d::CCInteger* res)
{
  mPostingTwitter = false;

  mTwitterAction->stopAllActions();
  mTwitterAction->setRotation(0);

  if( UniversalFit::sharedUniversalFit()->shouldUsingSinaWeibo() )
  {
    mTwitterAction->setDisplayFrame(cocos2d::SpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("go_weibo2.png"));
  }
  else
  {
    mTwitterAction->setDisplayFrame(cocos2d::SpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("go_twitter2.png"));
  }
}

bool CollectionMenu::onAssignCCBMemberVariable(cocos2d::Ref* pTarget, const char* pMemberVariableName, Node* pNode)
{
  CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "mShadow",         Sprite*       , mShadow)
  CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "mScrollCount",    Label*  , mScrollCount)
  CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "mItemDesc",       Label*  , mItemDesc)
  CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "mPowerUp",        Node*         , mPowerUp)
  CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "mLifeGuage",      Node*         , mLifeGuage)
  CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "mDartGuage",      Node*         , mDartGuage)
  CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "mLifeMask",       Sprite*       , mLifeMask)
  CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "mDartMask",       Sprite*       , mDartMask)
  CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "mPowerupMenu",    CCMenu*         , mPowerupMenu)
  CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "mLifeCount",      Label*  , mLifeCount)
  CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "mDartCount",      Label*  , mDartCount)
  CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "mItemTitle",      Sprite*       , mItemTitle)
  CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "mShare",          Sprite*       , mShare)
  CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "mMenu",           CCMenu*         , mMenu)
  CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "mCharacter1",     CCMenuItemImage*, mCharacter1)
  CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "mCharacter2",     CCMenuItemImage*, mCharacter2)
  CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "mCharacter3",     CCMenuItemImage*, mCharacter3)
  CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "mCharacter4",     CCMenuItemImage*, mCharacter4)
  CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "mShuriken",       CCMenuItemImage*, mShurikenDelegate.mMenuButtonRef)
  CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "mKatana",         CCMenuItemImage*, mKatanaDelegate.mMenuButtonRef)
  CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "mSpecial",        CCMenuItemImage*, mSpecialDelegate.mMenuButtonRef)
  CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "mPowerup",        CCMenuItemImage*, mPowerUpDelegate.mMenuButtonRef)
  CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "mUse",            CCMenuItemImage*, mUse)
  CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "mFacebook",       CCMenuItemImage*, mFacebook)
  CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "mTwitter",        CCMenuItemImage*, mTwitter)
  CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "mTwitterAction",  Sprite*       , mTwitterAction)
  CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "mFacebookAction", Sprite*       , mFacebookAction)
  CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "mScrollPoint",    Node*         , mScrollPoint)

  return false;
}

SEL_MenuHandler CollectionMenu::onResolveCCBCCMenuItemSelector(cocos2d::Ref * pTarget, const char* pSelectorName) {
  CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "onSelectLife", CollectionMenu::onSelectLife)
  CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "onSelectDart", CollectionMenu::onSelectDart)
  CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "onBack", CollectionMenu::onBack)
  CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "onCharacter1", CollectionMenu::onCharacter1)
  CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "onCharacter2", CollectionMenu::onCharacter2)
  CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "onCharacter3", CollectionMenu::onCharacter3)
  CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "onCharacter4", CollectionMenu::onCharacter4)
  CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "onShuriken", CollectionMenu::onShuriken)
  CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "onKatana", CollectionMenu::onKatana)
  CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "onSpecial", CollectionMenu::onSpecial)
  CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "onPowerup", CollectionMenu::onPowerup)
  CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "onUse", CollectionMenu::onUse)
  CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "onFacebook", CollectionMenu::onFacebook)
  CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "onTwitter", CollectionMenu::onTwitter)
  return NULL;
}

cocos2d::extension::Control::Handler   CollectionMenu::onResolveCCBCCControlSelector(cocos2d::Ref * pTarget, const char* pSelectorName)
{
  return NULL;
}
