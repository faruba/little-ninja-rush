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
CCScene* CollectionMenu::scene()
{
  CCScene *ret = CCScene::create();

  CollectionMenu *tm = CollectionMenu::create();

  //universal
  ret->setScale(UniversalFit::sharedUniversalFit()->scaleFactor);
  ret->setAnchorPoint(ccp(0, 0));
  ret->setPosition(UniversalFit::sharedUniversalFit()->sceneOffset);

  CEClipedNode *clip = CEClipedNode::create();
  clip->setClipRect(&(UniversalFit::sharedUniversalFit()->clipRect));

  clip->addChild(tm);

  ret->addChild(clip);
  return ret;
}

void CollectionMenu::setNavBack(int nid)
{
  gNavBack = nid;
}

void CollectionMenu::setModal(const char * title, const char * desc, CCObject* target, SEL_CallFunc sel)
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

  CCSprite *spbg = CCSprite::createWithSpriteFrameName("sp-tc.png");
  spbg->setPosition(ccp(SCREEN_WIDTH/2, SCREEN_HEIGHT/2));
  mMask->addChild(spbg);

  CCSprite *titlesp = CCSprite::createWithSpriteFrameName(title);
  titlesp->setPosition(ccp( spbg->getContentSize().width/2, 105));
  spbg->addChild(titlesp);

  CCSprite *descsp = CCSprite::createWithSpriteFrameName(desc);
  descsp->setPosition(ccp( spbg->getContentSize().width/2, 75));
  spbg->addChild(descsp);

  CCMenuItemImage *menuConfirm = CCMenuItemImage::create("pu-confirm1.png", "pu-confirm2.png", this, menu_selector(CollectionMenu::onModalConfirm));
  menuConfirm->setPosition(ccp(75, 35));
  CCMenuItemImage *menuCancel = CCMenuItemImage::create("pu-cancel1.png", "pu-cancel2.png", this, menu_selector(CollectionMenu::onModalCancel));
  menuCancel->setPosition(ccp(200, 35));
  CCMenu *menu = CCMenu::create(menuConfirm, menuCancel, NULL);
  menu->setPosition(ccp(0, 0));
  spbg->addChild(menu);
}

void CollectionMenu::onModalConfirm()
{
  mModal = false;
  mMask->setVisible(false);
  //mItems->setTouchEnabled(true;//1.2.0)
  mTouchEnabled = true;
  mMenu->setTouchEnabled(true);
  mPowerupMenu->setTouchEnabled(true);

  CCCallFunc *callSelectorAction = CCCallFunc::create(mModalTarget, mModalSel);
  this->runAction(callSelectorAction);
  SimpleAudioEngine::sharedEngine()->playEffect(CCFileUtils::sharedFileUtils()->fullPathForFilename("sound/charge.mp3").c_str());
}

void CollectionMenu::onModalCancel()
{
  mModal = false;
  mMask->setVisible(false);
  //mItems->setTouchEnabled(true;//1.2.0)
  mTouchEnabled = true;
  mMenu->setTouchEnabled(true);
  mPowerupMenu->setTouchEnabled(true);
}

void CollectionMenu::onUseLife()
{
  int role = GameRecord::sharedGameRecord()->curr_char;
  if( GameData::roleCurrHP(role) < GameData::roleMaxHP(role) )
  {
    GameRecord::sharedGameRecord()->collection->life_piece -= 9;
    GameRecord::sharedGameRecord()->setCharacterHp(GameRecord::sharedGameRecord()->char_hp[role]+1, role);
    updateCharacterInfo(role, 1);
    int lc = GameRecord::sharedGameRecord()->collection->life_piece/9;
    mLifeCount->setString(CCString::createWithFormat("%d", lc)->getCString());
    if( GameData::roleCurrHP(role) >= GameData::roleMaxHP(role) )
    {
      mUse->setNormalImage(CCSprite::createWithSpriteFrameName("sc_use2.png"));
      mUse->setSelectedImage(CCSprite::createWithSpriteFrameName("sc_use2.png"));
    }
  }
}

void CollectionMenu::onUseDart()
{
  int role = GameRecord::sharedGameRecord()->curr_char;
  if( GameData::roleCurrDart(role) < GameData::roleMaxDart(role) )
  {
    GameRecord::sharedGameRecord()->collection->dart_piece -= 9;
    GameRecord::sharedGameRecord()->setCharacterDart(GameRecord::sharedGameRecord()->char_dart[role]+1, role);
    updateCharacterInfo(role, 2);
    int lc = GameRecord::sharedGameRecord()->collection->dart_piece/9;
    mDartCount->setString(CCString::createWithFormat("%d", lc)->getCString());
    if( GameData::roleCurrDart(role) >= GameData::roleMaxDart(role) )
    {
      mUse->setNormalImage(CCSprite::createWithSpriteFrameName("sc_use2.png"));
      mUse->setSelectedImage(CCSprite::createWithSpriteFrameName("sc_use2.png"));
    }
  }
} 

void CollectionMenu::onEnter()
{
  PublicLoad::menuCollection()->loadAll();
  mSceneIntro = NULL;
  mIntroFlag = false;
  mModal = false;

  //do some initialization
  CCNode *taskcomplete = CCNode::create();
  taskcomplete->setPosition(ccp(SCREEN_WIDTH/2, SCREEN_HEIGHT));
  this->addChild(taskcomplete, 5);
  GamePlay::setTaskCompleteNode(taskcomplete);
  //----------
  CCNode * node = createUIByCCBI("menu-collection", "CollectionMenu", CollectionMenuLayerLoader::loader(), this);
  if(node != NULL) {
    this->addChild(node, 5);
  }

  //1.2.0 SinaWeibo
  if( UniversalFit::sharedUniversalFit()->shouldUsingSinaWeibo() )
  {
    mTwitter->setDisabledSpriteFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("go_weibo.png"));
    mTwitter->setNormalSpriteFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("go_weibo.png"));
    mTwitter->setSelectedSpriteFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("go_weibo.png"));
    mTwitterAction->setDisplayFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("go_weibo2.png"));
  }

  mScroll = ABScrollContent::create();
  CCRect clip = CCRectMake(mScrollPoint->getPosition().x, mScrollPoint->getPosition().y, 322, 177.5f);
  mScroll->clickTarget = this;
  mScroll->clickMethod = callfunc_selector(CollectionMenu::clickMethod);
  mScroll->setClipRect(clip);
  mScrollPoint->addChild(mScroll, -1);


  mShadowDir = ccpNormalize(ccp(-12, 17));
  this->scheduleUpdate();
  this->setTouchEnabled(true);

  mMask = CCLayerColor::create(ccc4(0, 0, 0, 128));
  mMask->setOpacity(128);
  mMask->setVisible(false);
  this->addChild(mMask, 10);

  //初始化人物按钮
  int currRole = GameRecord::sharedGameRecord()->curr_char;
  for(int i=0; i<GAME_CHARCOUNT; ++i)
  {
    CCMenuItemImage *btn = character(i);
    if( i == currRole )
    {
      btn->setNormalImage(CCSprite::createWithSpriteFrameName(CCString::createWithFormat("sc_role%d%d.png", i, 0)->getCString()));
      btn->setSelectedImage(CCSprite::createWithSpriteFrameName(CCString::createWithFormat("sc_role%d%d.png", i, 1)->getCString()));
    }
    else {
      btn->setNormalImage(CCSprite::createWithSpriteFrameName(CCString::createWithFormat("sc_role%d%d.png", i, 1)->getCString()));
      btn->setSelectedImage(CCSprite::createWithSpriteFrameName(CCString::createWithFormat("sc_role%d%d.png", i, 0)->getCString()));
    }
  }
  //初始化分类按钮
  switch (gNavBack) {
    case 2:
      {
        mCurrType = 1;
        setTypeButton(0, false);
        setTypeButton(1, true);
        setTypeButton(2, false);
        setTypeButton(3, false);
        this->updateKatanas();
      }
      break;
    case 3:
      {
        mCurrType = 2;
        setTypeButton(0, false);
        setTypeButton(1, false);
        setTypeButton(2, true);
        setTypeButton(3, false);
        this->updateSpecials();
      }
      break;
    default:
      {
        mCurrType = 0;
        setTypeButton(0, true);
        setTypeButton(1, false);
        setTypeButton(2, false);
        setTypeButton(3, false);
        this->updateShurikens();
      }
      break;
  }

  mScrollCount->setString(CCString::createWithFormat("x%d", GameRecord::sharedGameRecord()->collection->magic_piece)->getCString());

  this->updateItemInfo();
  mUse->setNormalImage(CCSprite::createWithSpriteFrameName("sc_equiped2.png"));
  mUse->setSelectedImage(CCSprite::createWithSpriteFrameName("sc_equiped2.png"));

  this->setSceneIntro();

  if(!SimpleAudioEngine::sharedEngine()->isBackgroundMusicPlaying())
  {
    SimpleAudioEngine::sharedEngine()->playBackgroundMusic("menu.mp3", true);
  }

  mPostingFacebook = false;
  mPostingTwitter = false;

     //MidBridge::sharedMidbridge()->setFacebookCallback(this, menu_selector(CollectionMenu::doneFacebook));
     //MidBridge::sharedMidbridge()->setTwitterCallback(this, menu_selector(CollectionMenu::doneTwitter:));
     //toggleShare(true);

  CCLayer::onEnter();
}

void CollectionMenu::onExit()
{
  GameRecord::sharedGameRecord()->collection->cleanNewList();
  GamePlay::setTaskCompleteNode(NULL);

     //MidBridge::sharedMidbridge()->setFacebookCallback(NULL, menu_selector(CollectionMenu::doneFacebook));
     //MidBridge::sharedMidbridge()->setTwitterCallback(NULL, menu_selector(CollectionMenu::doneTwitter));

  CCLayer::onExit();
}

void CollectionMenu::updateShurikens()
{
  //clean nodes
  mScroll->contentNode->removeAllChildrenWithCleanup(true);

  //create equiped mark
  mEquipedMark = CCSprite::createWithSpriteFrameName("sc_equiped.png");
  mEquipedMark->setVisible(false);
  mScroll->contentNode->addChild(mEquipedMark, 5);
  mCurrentMark = CCSprite::createWithSpriteFrameName("sc_xz.png");
  mCurrentMark->setVisible(false);
  mScroll->contentNode->addChild(mCurrentMark, 4);

  CCArray *shurikens = GameData::fetchShurikens();
  int count = 0;

  CCSize contentSize;//update contentSize
  CCObject *obj;
  CCARRAY_FOREACH (shurikens, obj)
  {
    Shuriken *sh = (Shuriken*)obj;
    //获取对应飞镖玩家的收集数据
    int piece = GameRecord::sharedGameRecord()->collection->pieces[sh->uiid];
    bool completed = GameRecord::sharedGameRecord()->collection->isItemCompleted(sh->uiid);

    CCSprite *item = CCSprite::createWithSpriteFrameName("sc_fbbg2.png");
    int x = count%4;
    int y = count/4;
    item->setPosition(ccp(42 + 79*x, -29.5 - 59*y));
    mScroll->contentNode->addChild(item, 1, count);
    //extend contentSize
    {
      float cw = item->getPosition().x + item->getContentSize().width/2;
      float ch = -item->getPosition().y + item->getContentSize().height/2;
      if( contentSize.width < cw )
      {
        contentSize.width = cw;
      }
      if( contentSize.height < ch )
      {
        contentSize.height = ch;
      }
    }
    //ITEM
    CCSprite *icon = NULL;
    if( piece == 0 && !completed )
    {
      icon = CCSprite::createWithSpriteFrameName("sc_fbbg.png");
    }
    else{
      icon = CCSprite::create(sh->icon->getCString());
    }
    icon->setPosition(item->getPosition());
    mScroll->contentNode->addChild(icon, 2);
    //MASK
    if( piece > 0 && !completed )
    {
      CCString *filename = CCString::createWithFormat("sc_sp%d.png", piece);
      CCSprite *mask = CCSprite::createWithSpriteFrameName(filename->getCString());
      mask->setPosition(item->getPosition());
      mScroll->contentNode->addChild(mask, 3, count);
    }
    //NEW?
    if( GameRecord::sharedGameRecord()->collection->isContainedInNewList(sh->uiid) )
    {
      CCSprite *newsp = CCSprite::createWithSpriteFrameName("sc_new.png");
      newsp->setPosition(ccpAdd(item->getPosition(), ccp(33, 24)));
      mScroll->contentNode->addChild(newsp, 6, 100+count);
    }
    count++;
  }
  mItemCount = count;
  mScroll->setContentSize(contentSize);//apply contentSize

  //当前配置
  int cc = GameRecord::sharedGameRecord()->curr_char;
  int eq = GameRecord::sharedGameRecord()->char_equip_dart[cc];
  mEquipedItem = -1;
  mCurrItem = -1;
  markUsing(eq);
  markCurrent(-1);
}

void CollectionMenu::updateKatanas()
{
  //clean nodes
  mScroll->contentNode->removeAllChildrenWithCleanup(true);

  int cc = GameRecord::sharedGameRecord()->curr_char;

  //create equiped mark
  mEquipedMark = CCSprite::createWithSpriteFrameName("sc_equiped.png");
  mEquipedMark->setVisible(false);
  mScroll->contentNode->addChild(mEquipedMark, 5);
  mCurrentMark = CCSprite::createWithSpriteFrameName("sc_xz.png");
  mCurrentMark->setVisible(false);
  mScroll->contentNode->addChild(mCurrentMark, 4);

  CCArray *katanas = GameData::fetchKatanas();
  int count = 0;
  int i = 0;
  CCSize contentSize;//update contentSize
  CCObject *obj;
  CCARRAY_FOREACH (katanas, obj)
  {
    Katana *sh = (Katana *) obj;
    //获取对应飞镖玩家的收集数据
    int piece = GameRecord::sharedGameRecord()->collection->pieces[sh->uiid];
    bool completed = GameRecord::sharedGameRecord()->collection->isItemCompleted(sh->uiid);
    //BG
    if( i < GAME_CHARCOUNT )
    {
      if( i != cc )
      {
        ++i;
        continue;
      }
    }
    CCSprite *item = CCSprite::createWithSpriteFrameName("sc_fbbg2.png");
    int x = count%4;
    int y = count/4;
    item->setPosition(ccp( 42 + 79*x, -29.5 - 59*y ));
    mScroll->contentNode->addChild(item, 1, count);
    //extend contentSize
    {
      float cw = item->getPosition().x + item->getContentSize().width/2;
      float ch = -item->getPosition().y + item->getContentSize().height/2;
      if( contentSize.width < cw )
      {
        contentSize.width = cw;
      }
      if( contentSize.height < ch )
      {
        contentSize.height = ch;
      }
    }
    //ITEM
    CCSprite *icon = NULL;
    if( piece == 0 && !completed )
    {
      icon = CCSprite::createWithSpriteFrameName("sc_ktnbg.png");
    }
    else{
      icon = CCSprite::create(sh->icon->getCString());
    }
    icon->setPosition(item->getPosition());
    mScroll->contentNode->addChild(icon, 2);
    //MASK
    if( piece > 0 && !completed )
    {
      CCString *filename = CCString::createWithFormat("sc_sp%d.png", piece);
      CCSprite *mask = CCSprite::createWithSpriteFrameName(filename->getCString());
      mask->setPosition(item->getPosition());
      mScroll->contentNode->addChild(mask, 3, count);
    }
    //NEW?
    if( GameRecord::sharedGameRecord()->collection->isContainedInNewList(sh->uiid) )
    {
      CCSprite *newsp = CCSprite::createWithSpriteFrameName("sc_new.png");
      newsp->setPosition(ccpAdd(item->getPosition(), ccp(33, 24)));
      mScroll->contentNode->addChild(newsp, 6, 100+count);
    }
    count++;
    ++i;
  }
  mItemCount = count;
  mScroll->setContentSize(contentSize);//apply contentSize
  //当前配置
  int eq = GameRecord::sharedGameRecord()->char_equip_blade[cc];
  if( eq < GAME_CHARCOUNT )
  {
    eq = 0;
  }
  else {
    eq -= GAME_CHARCOUNT-1;
  }
  mEquipedItem = -1;
  mCurrItem = -1;
  markUsing(eq);
  markCurrent(-1);
}

void CollectionMenu::updateSpecials()
{
  mScroll->contentNode->removeAllChildren();

  //create equiped mark
  mEquipedMark = CCSprite::createWithSpriteFrameName("sc_equiped.png");
  mEquipedMark->setVisible(false);

  mScroll->contentNode->addChild(mEquipedMark, 5);
  mCurrentMark = CCSprite::createWithSpriteFrameName("sc_xz.png");
  mCurrentMark->setVisible(false);

  mScroll->contentNode->addChild(mCurrentMark, 4);

  CCArray *specials = GameData::fetchSpecials();
  int count = 0;
  CCSize contentSize;//update contentSize
  CCObject *obj;
  CCARRAY_FOREACH (specials, obj)
  {
    Special *sh = (Special *) obj;
    //获取对应飞镖玩家的收集数据
    int piece = GameRecord::sharedGameRecord()->collection->pieces[sh->uiid];
    bool completed = GameRecord::sharedGameRecord()->collection->isItemCompleted(sh->uiid);
    //BG
    CCSprite *item = CCSprite::createWithSpriteFrameName("sc_fbbg2.png");
    int x = count%4;
    int y = count/4;
    item->setPosition(ccp( 42 + 79*x, -29.5 - 59*y ));

    mScroll->contentNode->addChild(item, 1, count);

    //extend contentSize
    {
      float cw = item->getPosition().x + item->getContentSize().width/2;
      float ch = -item->getPosition().y + item->getContentSize().height/2;
      if( contentSize.width < cw )
      {
        contentSize.width = cw;
      }
      if( contentSize.height < ch )
      {
        contentSize.height = ch;
      }
    }

    //ITEM
    CCSprite *icon = NULL;
    if( piece == 0 && !completed )
    {
      icon = CCSprite::createWithSpriteFrameName("sc_jnbg.png");
    }
    else{
      icon = CCSprite::create(sh->icon->getCString());
    }
    icon->setPosition(item->getPosition());
    mScroll->contentNode->addChild(icon, 2);
    //MASK
    if( piece > 0 && !completed )
    {
      CCString *filename = CCString::createWithFormat("sc_sp%d.png", piece);
      CCSprite *mask = CCSprite::createWithSpriteFrameName(filename->getCString());
      mask->setPosition(item->getPosition());
      mScroll->contentNode->addChild(mask, 3, count);
    }
    //NEW?
    if( GameRecord::sharedGameRecord()->collection->isContainedInNewList(sh->uiid) )
    {
      CCSprite *newsp = CCSprite::createWithSpriteFrameName("sc_new.png");
      newsp->setPosition(ccpAdd(item->getPosition(), ccp(33, 24)));
      mScroll->contentNode->addChild(newsp, 6, 100+count);
    }
    count++;
  }
  mItemCount = count;
  mScroll->setContentSize(contentSize);//apply contentSize
  //当前配置
  int cc = GameRecord::sharedGameRecord()->curr_char;
  int eq = GameRecord::sharedGameRecord()->char_equip_spell[cc];
  mEquipedItem = -1;
  mCurrItem = -1;
  markUsing(eq);
  markCurrent(-1);
}

void CollectionMenu::updatePowerups()
{
  mScroll->contentNode->removeAllChildren();

  mPowerUp->removeChildByTag(999, false);
  mCurrentMark = CCSprite::createWithSpriteFrameName("sc_xz.png");
  mCurrentMark->setVisible(false);
  mPowerUp->addChild(mCurrentMark, 1, 999);
  mUse->setVisible(false);

  //update mask
  CCSpriteFrameCache *cache = CCSpriteFrameCache::sharedSpriteFrameCache();
  int lm = GameRecord::sharedGameRecord()->collection->life_piece%9;
  int lc = GameRecord::sharedGameRecord()->collection->life_piece/9;
  CCString *lfn = CCString::createWithFormat("sc_sp%d.png", lm);
  mLifeMask->setDisplayFrame(cache->spriteFrameByName(lfn->getCString()));
  mLifeCount->setString(CCString::createWithFormat("%d", lc)->getCString());

  int dm = GameRecord::sharedGameRecord()->collection->dart_piece%9;
  int dc = GameRecord::sharedGameRecord()->collection->dart_piece/9;
  CCString *dfn = CCString::createWithFormat("sc_sp%d.png", dm);
  mDartMask->setDisplayFrame(cache->spriteFrameByName(dfn->getCString()));
  mDartCount->setString(CCString::createWithFormat("%d", dc)->getCString());
  mCurrItem = 0;

  updateCharacterInfo(GameRecord::sharedGameRecord()->curr_char, 0);
}

void CollectionMenu::markUsing(int i)
{
  if( mEquipedItem < 0 )
  {
    mEquipedMark->setVisible(true);
  }
  if( i >= 0 )
  {
    CCSprite *item = (CCSprite*)(mScroll->contentNode->getChildByTag(i));
    mEquipedMark->setPosition(item->getPosition());
  }
  else {
    mEquipedMark->setVisible(false);
  }
  mEquipedItem = i;
}

void CollectionMenu::markCurrent(int i)
{
  if( mCurrItem < 0 )
  {
    mCurrentMark->setVisible(true);
  }
  if( i >= 0 )
  {
    CCSprite *item = (CCSprite*)(mScroll->contentNode->getChildByTag(i));
    mCurrentMark->setPosition(item->getPosition());
  }
  else {
    mCurrentMark->setVisible(false);
  }
  mCurrItem = i;
  //获取当前道具的信息
  if( mCurrType < 3 && mCurrItem >= 0 )
  {
    bool collected = false;
    switch (mCurrType) {
      case 0:
        {
          Shuriken *sh = (Shuriken*)(GameData::fetchShurikens()->objectAtIndex(mCurrItem));
          collected = GameRecord::sharedGameRecord()->collection->isItemCompleted(sh->uiid);
        }
        break;
      case 1:
        {
          int index = mCurrItem + GAME_CHARCOUNT - 1;
          if( mCurrItem == 0 )
          {
            index = GameRecord::sharedGameRecord()->curr_char;
          }
          Katana *sh = (Katana*)(GameData::fetchKatanas()->objectAtIndex(index));
          collected = GameRecord::sharedGameRecord()->collection->isItemCompleted(sh->uiid);
        }
        break;
      case 2:
        {
          Special *sh = (Special*)(GameData::fetchSpecials()->objectAtIndex(mCurrItem));
          collected = GameRecord::sharedGameRecord()->collection->isItemCompleted(sh->uiid);
        }
        break;
    }
    if( i != mEquipedItem )
    {
      if( !collected )
      {
        if( GameRecord::sharedGameRecord()->collection->magic_piece > 0 )
        {
          mUse->setNormalImage(CCSprite::createWithSpriteFrameName("sc_unlock1.png"));
          mUse->setSelectedImage(CCSprite::createWithSpriteFrameName("sc_unlock2.png"));
        }
        else
        {
          mUse->setNormalImage(CCSprite::createWithSpriteFrameName("sc_unlock2.png"));
          mUse->setSelectedImage(CCSprite::createWithSpriteFrameName("sc_unlock2.png"));
        }
        toggleShare(false);
      }
      else {
        mUse->setNormalImage(CCSprite::createWithSpriteFrameName("sc_equip1.png"));
        mUse->setSelectedImage(CCSprite::createWithSpriteFrameName("sc_equip2.png"));
        toggleShare(false);
      }
    }
    else {
      mUse->setNormalImage(CCSprite::createWithSpriteFrameName("sc_equiped2.png"));
      mUse->setSelectedImage(CCSprite::createWithSpriteFrameName("sc_equiped2.png"));
      toggleShare(false);
    }
  }
}
CCMenuItemImage* CollectionMenu::character(int rid)
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

void CollectionMenu::setTypeButton(int typ, bool mod)
{
  switch (typ) {
    case 0://shuriken
      {
        if( mod )
        {
          mShuriken->setNormalImage(CCSprite::createWithSpriteFrameName("sc_shuriken1.png"));
          mShuriken->setSelectedImage(CCSprite::createWithSpriteFrameName("sc_shuriken2.png"));
        }
        else {
          mShuriken->setNormalImage(CCSprite::createWithSpriteFrameName("sc_shuriken2.png"));
          mShuriken->setSelectedImage(CCSprite::createWithSpriteFrameName("sc_shuriken1.png"));
        }
      }
      break;
    case 1://katana
      {
        if( mod )
        {
          mKatana->setNormalImage(CCSprite::createWithSpriteFrameName("sc_katana1.png"));
          mKatana->setSelectedImage(CCSprite::createWithSpriteFrameName("sc_katana2.png"));
        }
        else {
          mKatana->setNormalImage(CCSprite::createWithSpriteFrameName("sc_katana2.png"));
          mKatana->setSelectedImage(CCSprite::createWithSpriteFrameName("sc_katana1.png"));
        }
      }
      break;
    case 2://special
      {
        if( mod )
        {
          mSpecial->setNormalImage(CCSprite::createWithSpriteFrameName("sc_special1.png"));
          mSpecial->setSelectedImage(CCSprite::createWithSpriteFrameName("sc_special2.png"));
        }
        else {
          mSpecial->setNormalImage(CCSprite::createWithSpriteFrameName("sc_special2.png"));
          mSpecial->setSelectedImage(CCSprite::createWithSpriteFrameName("sc_special1.png"));
        }
      }
      break;
    case 3://powerup
      {
        if( mod )
        {
          mPowerup->setNormalImage(CCSprite::createWithSpriteFrameName("sc_powerup1.png"));
          mPowerup->setSelectedImage(CCSprite::createWithSpriteFrameName("sc_powerup2.png"));
        }
        else {
          mPowerup->setNormalImage(CCSprite::createWithSpriteFrameName("sc_powerup2.png"));
          mPowerup->setSelectedImage(CCSprite::createWithSpriteFrameName("sc_powerup1.png"));
        }
      }
      break;
  }
}

void CollectionMenu::updateItemInfo()
{
  int uid = mCurrItem;
  if( mCurrItem < 0 )
  {
    uid = mEquipedItem;
  }
  switch (mCurrType) {
    case 0:
      {
        Shuriken *sh = (Shuriken*)(GameData::fetchShurikens()->objectAtIndex(uid));
        mItemTitle->setDisplayFrame(CCSprite::create(sh->name->getCString())->displayFrame());
        mItemDesc->setString(sh->desc->getCString());
      }
      break;
    case 1:
      {
        int index = uid + GAME_CHARCOUNT - 1;
        if( uid == 0 )
        {
          index = GameRecord::sharedGameRecord()->curr_char;
        }
        Katana *sh = (Katana*)(GameData::fetchKatanas()->objectAtIndex(index));
        mItemTitle->setDisplayFrame(CCSprite::create(sh->name->getCString())->displayFrame());
        mItemDesc->setString(sh->desc->getCString());
      }
      break;
    case 2:
      {
        Special *sh = (Special*)(GameData::fetchSpecials()->objectAtIndex(uid));
        mItemTitle->setDisplayFrame(CCSprite::create(sh->name->getCString())->displayFrame());
        mItemDesc->setString(sh->desc->getCString());
      }
      break;
  }
}
void CollectionMenu::update(float delta)
{
  //update shadow
  CCPoint np = ccpAdd(mShadow->getPosition(), ccpMult(mShadowDir, delta*7));
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
void CollectionMenu::onBack()
{
  if( !mIntroFlag )
  {
    SimpleAudioEngine::sharedEngine()->playEffect(CCFileUtils::sharedFileUtils()->fullPathForFilename("sound/menu-change.mp3").c_str());
    if( gNavBack == 0 )
    {
      setSceneOutro(TitleMenu::scene());
    }
    else {
      setSceneOutro(SelectMenu::scene());
    }
  }
}

void CollectionMenu::onUse()
{
  if( mCurrType < 3 )
  {
    if( mCurrItem != mEquipedItem && mCurrItem>=0 )
    {
      bool collected = false;
      int uiid = -1;
      switch (mCurrType) {
        case 0:
          {
            Shuriken *sh = (Shuriken*)(GameData::fetchShurikens()->objectAtIndex(mCurrItem));
            uiid = sh->uiid;
            collected = GameRecord::sharedGameRecord()->collection->isItemCompleted(sh->uiid);
          }
          break;
        case 1:
          {
            int index = mCurrItem + GAME_CHARCOUNT - 1;
            if( mCurrItem == 0 )
            {
              index = GameRecord::sharedGameRecord()->curr_char;
            }
            Katana *sh = (Katana*)GameData::fetchKatanas()->objectAtIndex(index);
            uiid = sh->uiid;
            collected = GameRecord::sharedGameRecord()->collection->isItemCompleted(sh->uiid);
          }
          break;
        case 2:
          {
            Special *sh = (Special*)GameData::fetchSpecials()->objectAtIndex(mCurrItem);
            uiid = sh->uiid;
            collected = GameRecord::sharedGameRecord()->collection->isItemCompleted(sh->uiid);
          }
          break;
      }

      if( collected )
      {
        markUsing(mCurrItem);
        //保存数据
        int cc = GameRecord::sharedGameRecord()->curr_char;
        switch (mCurrType) {
          case 0:
            {
              GameRecord::sharedGameRecord()->char_equip_dart[cc] = mEquipedItem;
              SimpleAudioEngine::sharedEngine()->playEffect(CCFileUtils::sharedFileUtils()->fullPathForFilename("sound/equip.mp3").c_str());
            }
            break;
          case 1:
            {
              int index = mEquipedItem + GAME_CHARCOUNT - 1;
              if( mEquipedItem == 0 )
              {
                index = GameRecord::sharedGameRecord()->curr_char;
              }
              GameRecord::sharedGameRecord()->char_equip_blade[cc] = index;
              SimpleAudioEngine::sharedEngine()->playEffect(CCFileUtils::sharedFileUtils()->fullPathForFilename("sound/equip.mp3").c_str());
            }
            break;
          case 2:
            {
              GameRecord::sharedGameRecord()->char_equip_spell[cc] = mEquipedItem;
              SimpleAudioEngine::sharedEngine()->playEffect(CCFileUtils::sharedFileUtils()->fullPathForFilename("sound/equip.mp3").c_str());
            }
            break;
        }
        mUse->setNormalImage(CCSprite::createWithSpriteFrameName("sc_equiped2.png"));
        mUse->setSelectedImage(CCSprite::createWithSpriteFrameName("sc_equiped2.png"));

        //achievement change equipment
        if( mCurrType < 3 )
        {
          GameRecord::sharedGameRecord()->task->dispatchTask(ACH_CHANGEEQUIPMENT, 1);
        }
      }
      else {
        //如果没有解锁就使用万能卷轴
        if( GameRecord::sharedGameRecord()->collection->magic_piece > 0 )
        {
          GameRecord::sharedGameRecord()->collection->magic_piece--;
          GameRecord::sharedGameRecord()->collection->gainItemPiece(uiid);
          //update info
          CCSprite *mask = (CCSprite*)(mScroll->contentNode->getChildByTag(mCurrItem));
          if( GameRecord::sharedGameRecord()->collection->isItemCompleted(uiid) )
          {
            SimpleAudioEngine::sharedEngine()->playEffect(CCFileUtils::sharedFileUtils()->fullPathForFilename("sound/getscroll.mp3").c_str());
            mScroll->contentNode->removeChild(mask);
            mUse->setNormalImage(CCSprite::createWithSpriteFrameName("sc_equip1.png"));
            mUse->setSelectedImage(CCSprite::createWithSpriteFrameName("sc_equip2.png"));
          }
          else {
            SimpleAudioEngine::sharedEngine()->playEffect(CCFileUtils::sharedFileUtils()->fullPathForFilename("sound/getitem.mp3").c_str());
            int piece = GameRecord::sharedGameRecord()->collection->itemTotalPiece(uiid) - GameRecord::sharedGameRecord()->collection->itemLostPiece(uiid);
            CCString *filename = CCString::createWithFormat("sc_sp%d.png", piece);
            mask->setDisplayFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(filename->getCString()));
            //bugfix
            if( GameRecord::sharedGameRecord()->collection->magic_piece <= 0 )
            {
              mUse->setNormalImage(CCSprite::createWithSpriteFrameName("sc_unlock2.png"));
              mUse->setSelectedImage(CCSprite::createWithSpriteFrameName("sc_unlock2.png"));
            }
          }
          mScrollCount->setString(CCString::createWithFormat("x%d", GameRecord::sharedGameRecord()->collection->magic_piece)->getCString());
        }
        else {
          SimpleAudioEngine::sharedEngine()->playEffect(CCFileUtils::sharedFileUtils()->fullPathForFilename("sound/error.mp3").c_str());
        }
      }
    }
  }
  else {
    int roleid = GameRecord::sharedGameRecord()->curr_char;
    bool enable = true;
    SEL_CallFunc selector = NULL;
    if( mCurrItem == 1 )
    {

      selector = callfunc_selector(CollectionMenu::onUseLife);
      if( GameData::roleCurrHP(roleid) >= GameData::roleMaxHP(roleid) || GameRecord::sharedGameRecord()->collection->life_piece < 9 )
      {
        enable = false;
      }
    }
    else {
      selector = callfunc_selector(CollectionMenu::onUseDart);
      if( GameData::roleCurrDart(roleid) >= GameData::roleMaxDart(roleid) || GameRecord::sharedGameRecord()->collection->dart_piece < 9 )
      {
        enable = false;
      }
    }
    const char *desc = NULL;

    switch (roleid) {
      case 0:
        {
          desc = "xr_kt.png";
        }
        break;
      case 1:
        {
          desc = "xr_sr.png";
        }
        break;
      case 2:
        {
          desc = "xr_ms.png";
        }
        break;
      case 3:
        {
          desc = "xr_mr.png";
        }
        break;
    }
    if( enable )
    {
      SimpleAudioEngine::sharedEngine()->playEffect(CCFileUtils::sharedFileUtils()->fullPathForFilename("sound/click.mp3").c_str());
      setModal("pu-tc1.png", desc, this, selector);
    }
    else {
      SimpleAudioEngine::sharedEngine()->playEffect(CCFileUtils::sharedFileUtils()->fullPathForFilename("sound/error.mp3").c_str());
    }
  }
}

void CollectionMenu::updateUsing()
{
  int nrole = GameRecord::sharedGameRecord()->curr_char;
  switch (mCurrType) {
    case 0:
      {
        markUsing(GameRecord::sharedGameRecord()->char_equip_dart[nrole]);
      }
      break;
    case 1:
      {
        int index = GameRecord::sharedGameRecord()->char_equip_blade[nrole];
        if( index < GAME_CHARCOUNT )
        {
          index = 0;
        }
        else {
          index = index - GAME_CHARCOUNT + 1;
        }
        markUsing(index);
      }
      break;
    case 2:
      {
        markUsing(GameRecord::sharedGameRecord()->char_equip_spell[nrole]);
      }
      break;
    case 3:
      {
        markUsing(-1);
      }
      break;
  }
}

void CollectionMenu::onCharacter(int nRole)
{
  int orole = GameRecord::sharedGameRecord()->curr_char;
  if( orole == nRole )
    return;
  SimpleAudioEngine::sharedEngine()->playEffect(CCFileUtils::sharedFileUtils()->fullPathForFilename("sound/click.mp3").c_str());
  CCMenuItemImage *orl = character(orole);
  orl->setNormalImage(CCSprite::createWithSpriteFrameName(CCString::createWithFormat("sc_role%d%d.png", orole, 1)->getCString()));
  orl->setSelectedImage(CCSprite::createWithSpriteFrameName(CCString::createWithFormat("sc_role%d%d.png", orole, 0)->getCString()));

  int nrole = nRole;
  CCMenuItemImage *nr = character(nrole);
  nr->setNormalImage(CCSprite::createWithSpriteFrameName(CCString::createWithFormat("sc_role%d%d.png", nrole, 0)->getCString()));
  nr->setSelectedImage(CCSprite::createWithSpriteFrameName(CCString::createWithFormat("sc_role%d%d.png", nrole, 1)->getCString()));
  GameRecord::sharedGameRecord()->curr_char = nrole;

  if( mCurrType != 3 )
  {
    markCurrent(-1);
    this->updateUsing();
    this->updateItemInfo();
  }
  else {
    updateCharacterInfo(nRole, mCurrItem);
    updatePowerUpButton();
  }

  if( mCurrType == 1 )
  {
    this->updateKatanas();
  }
}



void CollectionMenu::onItem(int nItem)
{
  if( mCurrType == nItem )
    return ;

  SimpleAudioEngine::sharedEngine()->playEffect(CCFileUtils::sharedFileUtils()->fullPathForFilename("sound/click.mp3").c_str());
  setTypeButton(mCurrType, false);
  setTypeButton(nItem, true);
  mItemDesc->setVisible(true);
  mCurrType = nItem;
  if (nItem == 0)
    this->updateShurikens();
  else if (nItem == 1)
    this->updateKatanas();
  else if (nItem == 2)
    this->updateSpecials();
  this->updateItemInfo();
  mPowerUp->setVisible(false);
  mUse->setNormalImage(CCSprite::createWithSpriteFrameName("sc_equiped2.png"));
  mUse->setSelectedImage(CCSprite::createWithSpriteFrameName("sc_equiped2.png"));
  mUse->setVisible(true);
  mScroll->resetContentPosition();

  toggleShare(true);

  CCLog("CollectionMenu::onItem");
}

void CollectionMenu::onPowerup()
{
  if( mCurrType != 3 )
  {
    SimpleAudioEngine::sharedEngine()->playEffect(CCFileUtils::sharedFileUtils()->fullPathForFilename("sound/click.mp3").c_str());
    setTypeButton(mCurrType, false);
    setTypeButton(3, true);
    mItemDesc->setVisible(false);
    mCurrType = 3;
    this->updatePowerups();
    mPowerUp->setVisible(true);

    this->onSelectLife();

    toggleShare(false);
  }
}

void CollectionMenu::updateCharacterInfo(int rid, int bid)
{
   mLifeGuage->removeAllChildrenWithCleanup(false);
   for( int i=0; i<GameData::roleMaxHP(rid); ++i)
   {
     if( i<GameData::roleCurrHP(rid) )
     {
       CCSprite *sp = CCSprite::createWithSpriteFrameName("heart.png");
       sp->setPosition(ccp(i*18, 0));
       mLifeGuage->addChild(sp, 0, i);
     }
     else {
       if( bid == 1 && i == GameData::roleCurrHP(rid) )
       {
         GTAnimatedEffect *sp = GTAnimatedEffect::create(GTAnimation::loadedAnimationSet("ui"), 0, true);
         sp->setPosition(ccp(i*18, 0));
         mLifeGuage->addChild(sp);
       }
       else {
         CCSprite *sp = CCSprite::createWithSpriteFrameName("pulife2.png");
         sp->setPosition(ccp(i*18, 0));
         mLifeGuage->addChild(sp);
       }
     }
   }

   mDartGuage->removeAllChildrenWithCleanup(false);
   for( int i=0; i<GameData::roleMaxDart(rid); ++i)
   {
     if( i<GameData::roleCurrDart(rid) )
     {
       CCSprite *sp = CCSprite::createWithSpriteFrameName("ui-dart.png");
       sp->setPosition(ccp(i*12, 0));
       mDartGuage->addChild(sp);
     }
     else {
       if( bid == 2 && i == GameData::roleCurrDart(rid) )
       {
         GTAnimatedEffect *sp = GTAnimatedEffect::create(GTAnimation::loadedAnimationSet("ui"), 1, true);
         sp->setPosition(ccp(i*12, 0));
         mDartGuage->addChild(sp);
       }
       else {
         CCSprite *sp = CCSprite::createWithSpriteFrameName("pushuriken2.png");
         sp->setPosition(ccp(i*12, 0));
         mDartGuage->addChild(sp);
       }
     }
   }
}

void CollectionMenu::onItemCallback(int i)
{
  //获取当前道具的信息
  if( mCurrType < 3 )
  {
    bool collected = false;
    switch (mCurrType) {
      case 0:
        {
          Shuriken *sh = (Shuriken*)(GameData::fetchShurikens()->objectAtIndex(i));
          collected = GameRecord::sharedGameRecord()->collection->isItemCompleted(sh->uiid);
          if( !collected && GameRecord::sharedGameRecord()->collection->itemLostPiece(sh->uiid) == GameRecord::sharedGameRecord()->collection->itemTotalPiece(sh->uiid) )
          {
            SimpleAudioEngine::sharedEngine()->playEffect(CCFileUtils::sharedFileUtils()->fullPathForFilename("sound/error.mp3").c_str());
            return;
          }
        }
        break;
      case 1:
        {
          int index = i + GAME_CHARCOUNT - 1;
          if( i == 0 )
          {
            index = GameRecord::sharedGameRecord()->curr_char;
          }
          Katana *sh = (Katana*)GameData::fetchKatanas()->objectAtIndex(index);
          collected = GameRecord::sharedGameRecord()->collection->isItemCompleted(sh->uiid);
          if( !collected && GameRecord::sharedGameRecord()->collection->itemLostPiece(sh->uiid) == GameRecord::sharedGameRecord()->collection->itemTotalPiece(sh->uiid) )
          {
            SimpleAudioEngine::sharedEngine()->playEffect(CCFileUtils::sharedFileUtils()->fullPathForFilename("sound/error.mp3").c_str());
            return;
          }
        }
        break;
      case 2:
        {
          Special *sh = (Special*)GameData::fetchSpecials()->objectAtIndex(i);
          collected = GameRecord::sharedGameRecord()->collection->isItemCompleted(sh->uiid);
          if( !collected && GameRecord::sharedGameRecord()->collection->itemLostPiece(sh->uiid) == GameRecord::sharedGameRecord()->collection->itemTotalPiece(sh->uiid) )
          {
            SimpleAudioEngine::sharedEngine()->playEffect(CCFileUtils::sharedFileUtils()->fullPathForFilename("sound/error.mp3").c_str());
            return;
          }
        }
        break;
    }
  }
  SimpleAudioEngine::sharedEngine()->playEffect(CCFileUtils::sharedFileUtils()->fullPathForFilename("sound/click.mp3").c_str());
  markCurrent(i);
  this->updateItemInfo();
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
    mUse->setNormalImage(CCSprite::createWithSpriteFrameName("sc_use1.png"));
    mUse->setSelectedImage(CCSprite::createWithSpriteFrameName("sc_use2.png"));
  }
  else {
    mUse->setNormalImage(CCSprite::createWithSpriteFrameName("sc_use2.png"));
    mUse->setSelectedImage(CCSprite::createWithSpriteFrameName("sc_use2.png"));
  }
}

void CollectionMenu::onSelectLife()
{
  if( mCurrItem != 1 )
  {
    SimpleAudioEngine::sharedEngine()->playEffect(CCFileUtils::sharedFileUtils()->fullPathForFilename("sound/click.mp3").c_str());
    mCurrentMark->setPosition(ccp(385, 145));
    mCurrentMark->setVisible(true);
    mCurrItem = 1;
    updateCharacterInfo(GameRecord::sharedGameRecord()->curr_char, mCurrItem);

    //buttons & title
    mItemTitle->setDisplayFrame(CCSprite::create("lifeplus.png")->displayFrame());
    this->updatePowerUpButton();
  }
}

void CollectionMenu::onSelectDart()
{
  if( mCurrItem != 2 )
  {
    SimpleAudioEngine::sharedEngine()->playEffect(CCFileUtils::sharedFileUtils()->fullPathForFilename("sound/click.mp3").c_str());
    mCurrentMark->setPosition(ccp(385, 65));
    mCurrentMark->setVisible(true);
    mCurrItem = 2;
    updateCharacterInfo(GameRecord::sharedGameRecord()->curr_char, mCurrItem);

    //buttons & title
    mItemTitle->setDisplayFrame(CCSprite::create("dartplus.png")->displayFrame());
    this->updatePowerUpButton();
  }
}

void CollectionMenu::clickMethod()
{
  CCPoint click = mScroll->clickPoint;
  int x = click.x / 79;
  int y = click.y / 59;
  int cid = x + y*4;
  if( cid < mItemCount && cid >= 0 )
  {
    onItemCallback(cid);
  }
}

void CollectionMenu::setSceneIntro()
{
  doSceneIntro(mSceneIntro, this);
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
  if( mCurrType < 3 )
  {
    bool collected = false;
    id ret = NULL;
    switch (mCurrType) {
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
void CollectionMenu::onFacebook()
{/*
  if( !mPostingFacebook )
  {
    ABSocial *social = ABSocial->feedOnSinaWeibo();
    switch (mCurrType) {
      case 0:
        {
          Shuriken *sh = this->getCurrentCollectedItem();
          social.text = CCString::createWithFormat("I have just collected %@ in Little Ninja Rush!", sh->titl);
          social.image = UIImage->imageWithContentsOfFile(CCFileUtils->sharedFileUtils()->fullPathFromRelativePath( sh->icon));
        }
        break;
      case 1:
        {
          Katana *sh = this->getCurrentCollectedItem();
          social.text = CCString::createWithFormat("I have just collected %@ in Little Ninja Rush!", sh->titl);
          social.image = UIImage->imageWithContentsOfFile(CCFileUtils->sharedFileUtils()->fullPathFromRelativePath( sh->icon));
        }
        break;
      case 2:
        {
          Special *sh = this->getCurrentCollectedItem();
          social.text = CCString::createWithFormat("I have just collected %@ in Little Ninja Rush!", sh->titl);
          social.image = UIImage->imageWithContentsOfFile(CCFileUtils->sharedFileUtils()->fullPathFromRelativePath( sh->icon));
        }
        break;
    }
    social.callbackObject = MidBridge->sharedMidbridge();
    social.callbackSelector =, );
    AppController *del = (AppController*)UIApplication->sharedApplication().delegate;
    social->present(del.navController);

    mFacebookAction->setDisplayFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("ftloading.png"));
    mFacebookAction->stopAllActions();
    CCRotateBy *rb = CCRotateBy::create(1, 360);
    CCRepeatForever *rf = CCRepeatForever::create(rb);
    mFacebookAction->runAction(rf);

    mPostingFacebook = true;
  }
  */
}

void CollectionMenu::doneFacebook(CCInteger* res)
{
  /*
  mPostingFacebook = false;

  mFacebookAction->stopAllActions();
  mFacebookAction->setRotation(0);
  mFacebookAction->setDisplayFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("go_facebook2.png"));
  */
}

void CollectionMenu::onTwitter()
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
    switch (mCurrType) {
      case 0:
        {
          Shuriken *sh = this->getCurrentCollectedItem();
          if( flagSINA )
          {
            social.text = CCString::createWithFormat("刚刚在#LittleNinjaRush#里收集到了%@，一起来玩吧!", sh->titl);
          }
          else
          {
            social.text = CCString::createWithFormat("I have just collected %@ in #LittleNinjaRush!", sh->titl);
          }
          social.url = NSURL->URLWithString(TWITTER_URL);
          social.image = UIImage->imageWithContentsOfFile(CCFileUtils->sharedFileUtils()->fullPathFromRelativePath( sh->icon));
        }
        break;
      case 1:
        {
          Katana *sh = this->getCurrentCollectedItem();
          if( flagSINA )
          {
            social.text = CCString::createWithFormat("刚刚在#LittleNinjaRush#里收集到了%@，一起来玩吧!", sh->titl);
          }
          else
          {
            social.text = CCString::createWithFormat("I have just collected %@ in #LittleNinjaRush!", sh->titl);
          }
          social.url = NSURL->URLWithString(TWITTER_URL);
          social.image = UIImage->imageWithContentsOfFile(CCFileUtils->sharedFileUtils()->fullPathFromRelativePath( sh->icon));
        }
        break;
      case 2:
        {
          Special *sh = this->getCurrentCollectedItem();
          if( flagSINA )
          {
            social.text = CCString::createWithFormat("刚刚在#LittleNinjaRush#里收集到了%@，一起来玩吧!", sh->titl);
          }
          else
          {
            social.text = CCString::createWithFormat("I have just collected %@ in #LittleNinjaRush!", sh->titl);
          }
          social.url = NSURL->URLWithString(TWITTER_URL);
          social.image = UIImage->imageWithContentsOfFile(CCFileUtils->sharedFileUtils()->fullPathFromRelativePath( sh->icon));
        }
        break;
    }
    social.callbackObject = MidBridge->sharedMidbridge();
    social.callbackSelector =, );
    social->present(del.navController);

    mTwitterAction->setDisplayFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("ftloading.png"));
    mTwitterAction->stopAllActions();
    CCRotateBy *rb = CCRotateBy::create(1, 360);
    CCRepeatForever *rf = CCRepeatForever::create(rb);
    mTwitterAction->runAction(rf);
  }
*/
}

void CollectionMenu::doneTwitter(CCInteger* res)
{
  /*
  mPostingTwitter = false;

  mTwitterAction->stopAllActions();
  mTwitterAction->setRotation(0);

  if( UniversalFit::sharedUniversalFit()->shouldUsingSinaWeibo() )
  {
    mTwitterAction->setDisplayFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("go_weibo2.png"));
  }
  else
  {
    mTwitterAction->setDisplayFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("go_twitter2.png"));
  }
  */
}

void CollectionMenu::setSceneOutro(CCScene* newscene)
{
  if( mIntroFlag )
  {
    return;
  }

  mIntroFlag = true;

  mNewScene = doSceneOutro(newscene, mSceneIntro, (SEL_CallFunc)(&CollectionMenu::doneOutro), this);
}

void CollectionMenu::doneOutro()
{
  mIntroFlag = false;
  CCDirector::sharedDirector()->replaceScene(mNewScene);
  mNewScene->release();
}

bool CollectionMenu::onAssignCCBMemberVariable(CCObject* pTarget, const char* pMemberVariableName, CCNode* pNode)
{
  CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "mShadow",         CCSprite*       , mShadow)
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "mScrollCount",    CCLabelBMFont*  , mScrollCount)
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "mItemDesc",       CCLabelTTF*  , mItemDesc)
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "mPowerUp",        CCNode*         , mPowerUp)
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "mLifeGuage",      CCNode*         , mLifeGuage)
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "mDartGuage",      CCNode*         , mDartGuage)
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "mLifeMask",       CCSprite*       , mLifeMask)
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "mDartMask",       CCSprite*       , mDartMask)
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "mPowerupMenu",    CCMenu*         , mPowerupMenu)
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "mLifeCount",      CCLabelBMFont*  , mLifeCount)
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "mDartCount",      CCLabelBMFont*  , mDartCount)
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "mItemTitle",      CCSprite*       , mItemTitle)
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "mShare",          CCSprite*       , mShare)
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "mMenu",           CCMenu*         , mMenu)
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "mCharacter1",     CCMenuItemImage*, mCharacter1)
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "mCharacter2",     CCMenuItemImage*, mCharacter2)
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "mCharacter3",     CCMenuItemImage*, mCharacter3)
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "mCharacter4",     CCMenuItemImage*, mCharacter4)
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "mShuriken",       CCMenuItemImage*, mShuriken)
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "mKatana",         CCMenuItemImage*, mKatana)
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "mSpecial",        CCMenuItemImage*, mSpecial)
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "mPowerup",        CCMenuItemImage*, mPowerup)
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "mUse",            CCMenuItemImage*, mUse)
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "mFacebook",       CCMenuItemImage*, mFacebook)
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "mTwitter",        CCMenuItemImage*, mTwitter)
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "mTwitterAction",  CCSprite*       , mTwitterAction)
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "mFacebookAction", CCSprite*       , mFacebookAction)
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "mScrollPoint",    CCNode*         , mScrollPoint)

    CCLog(pMemberVariableName);

  return false;
}
SEL_MenuHandler CollectionMenu::onResolveCCBCCMenuItemSelector(CCObject * pTarget, const char* pSelectorName)
{
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

    CCLog(pSelectorName);
  return NULL;
}
SEL_CCControlHandler CollectionMenu::onResolveCCBCCControlSelector(CCObject * pTarget, const char* pSelectorName)
{
  //CCB_SELECTORRESOLVER_CCCONTROL_GLUE
  CCLog(pSelectorName);
  return NULL;
}

