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

// TODO:Refactor this two: onUseLife(), onUseDart
void CollectionMenu::onUseLife()
{
  int role = GameRecord::sharedGameRecord()->curr_char;
  if( GameData::roleCurrHP(role) < GameData::roleMaxHP(role) )
  {
    GameRecord::sharedGameRecord()->collection->life_piece -= 9;
    GameRecord::sharedGameRecord()->setCharacterHp(GameRecord::sharedGameRecord()->char_hp[role]+1, role);
    updateCharacterInfo(role, 1);
    int lc = GameRecord::sharedGameRecord()->collection->life_piece/9;
    mLifeCount->setString(cocos2d::CCString::createWithFormat("%d", lc)->getCString());
    if( GameData::roleCurrHP(role) >= GameData::roleMaxHP(role) )
    {
      mUse->setNormalImage(cocos2d::Sprite::createWithSpriteFrameName("sc_use2.png"));
      mUse->setSelectedImage(cocos2d::Sprite::createWithSpriteFrameName("sc_use2.png"));
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
    mDartCount->setString(cocos2d::CCString::createWithFormat("%d", lc)->getCString());
    if( GameData::roleCurrDart(role) >= GameData::roleMaxDart(role) )
    {
      mUse->setNormalImage(cocos2d::Sprite::createWithSpriteFrameName("sc_use2.png"));
      mUse->setSelectedImage(cocos2d::Sprite::createWithSpriteFrameName("sc_use2.png"));
    }
  }
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
  setTypeButton(TypeShuriken, false);
  setTypeButton(TypeKatana, false);
  setTypeButton(TypeSpecial, false);
  setTypeButton(TypePowerUp, false);
  switch (gNavBack) {
    case 2:
      setTypeButton(TypeKatana, true);
      mCurrentType = TypeKatana;
      this->updateKatanas();
      break;
    case 3:
      mCurrentType = TypeSpecial;
      setTypeButton(TypeSpecial, true);
      this->updateSpecials();
      break;
    default:
      mCurrentType = TypeShuriken;
      setTypeButton(TypeShuriken, true);
      this->updateShurikens();
      break;
  }

  mScrollCount->setString(cocos2d::CCString::createWithFormat("x%d", GameRecord::sharedGameRecord()->collection->magic_piece)->getCString());

  this->updateItemInfo();
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

void CollectionMenu::updatePowerups()
{
  mScroll->contentNode->removeAllChildren();

  mPowerUp->removeChildByTag(999, false);
  mCurrentMark = cocos2d::Sprite::createWithSpriteFrameName("sc_xz.png");
  mCurrentMark->setVisible(false);
  mPowerUp->addChild(mCurrentMark, 1, 999);
  mUse->setVisible(false);

  //update mask
  //TODO:Refactor this
  cocos2d::SpriteFrameCache *cache = cocos2d::SpriteFrameCache::sharedSpriteFrameCache();
  int lm = GameRecord::sharedGameRecord()->collection->life_piece%9;
  int lc = GameRecord::sharedGameRecord()->collection->life_piece/9;
  cocos2d::CCString *lfn = cocos2d::CCString::createWithFormat("sc_sp%d.png", lm);
  mLifeMask->setDisplayFrame(cache->spriteFrameByName(lfn->getCString()));
  mLifeCount->setString(cocos2d::CCString::createWithFormat("%d", lc)->getCString());

  int dm = GameRecord::sharedGameRecord()->collection->dart_piece%9;
  int dc = GameRecord::sharedGameRecord()->collection->dart_piece/9;
  cocos2d::CCString *dfn = cocos2d::CCString::createWithFormat("sc_sp%d.png", dm);
  mDartMask->setDisplayFrame(cache->spriteFrameByName(dfn->getCString()));
  mDartCount->setString(cocos2d::CCString::createWithFormat("%d", dc)->getCString());
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
    cocos2d::Sprite *item = (cocos2d::Sprite*)(mScroll->contentNode->getChildByTag(i));
    mEquipedMark->setPosition(item->getPosition());
  }
  else {
    mEquipedMark->setVisible(false);
  }
  mEquipedItem = i;
}

void CollectionMenu::markCurrent(int i)
{
  if( mCurrItem < 0 ) {
    mCurrentMark->setVisible(true);
  }

  if ( i >= 0 ) {
    cocos2d::Sprite *item = (cocos2d::Sprite*)(mScroll->contentNode->getChildByTag(i));
    mCurrentMark->setPosition(item->getPosition());
  } else {
    mCurrentMark->setVisible(false);
  }

  mCurrItem = i;
  //获取当前道具的信息
  if( mCurrentType < 3 && mCurrItem >= 0 ) {
    bool collected = false;
    switch (mCurrentType) {
      case 0:
        {
          Shuriken sh = GameData::fetchShurikens()[mCurrItem];
          collected = GameRecord::sharedGameRecord()->collection->isItemCompleted(sh.uiid);
        }
        break;
      case 1:
        {
          int index = mCurrItem + GAME_CHARCOUNT - 1;
          if( mCurrItem == 0 ) {
            index = GameRecord::sharedGameRecord()->curr_char;
          }
          Katana sh = GameData::fetchKatanas()[mCurrItem];
          collected = GameRecord::sharedGameRecord()->collection->isItemCompleted(sh.uiid);
        }
        break;
      case 2:
        {
          Special sh = GameData::fetchSpecials()[mCurrItem];
          collected = GameRecord::sharedGameRecord()->collection->isItemCompleted(sh.uiid);
        }
        break;
    }

    if( i != mEquipedItem ) {
      if ( !collected ) {
        if ( GameRecord::sharedGameRecord()->collection->magic_piece > 0 ) {
          mUse->setNormalImage(cocos2d::Sprite::createWithSpriteFrameName("sc_unlock1.png"));
          mUse->setSelectedImage(cocos2d::Sprite::createWithSpriteFrameName("sc_unlock2.png"));
        } else {
          mUse->setNormalImage(cocos2d::Sprite::createWithSpriteFrameName("sc_unlock2.png"));
          mUse->setSelectedImage(cocos2d::Sprite::createWithSpriteFrameName("sc_unlock2.png"));
        }
        toggleShare(false);
      } else {
        mUse->setNormalImage(cocos2d::Sprite::createWithSpriteFrameName("sc_equip1.png"));
        mUse->setSelectedImage(cocos2d::Sprite::createWithSpriteFrameName("sc_equip2.png"));
        toggleShare(false);
      }
    } else {
      mUse->setNormalImage(cocos2d::Sprite::createWithSpriteFrameName("sc_equiped2.png"));
      mUse->setSelectedImage(cocos2d::Sprite::createWithSpriteFrameName("sc_equiped2.png"));
      toggleShare(false);
    }
  }
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

void CollectionMenu::setTypeButton(ButtonEnum type, bool isDisabled) {
  cocos2d::MenuItemImage *pItem = nullptr;
  std::string filename = "";

  switch (type) {
    case TypeShuriken:
      pItem = mShuriken;
      filename = "sc_shuriken";
      break;
    case TypeKatana:
      pItem = mKatana;
      filename = "sc_katana";
      break;
    case TypeSpecial:
      pItem = mSpecial;
      filename = "sc_special";
      break;
    case TypePowerUp:
      pItem = mPowerup;
      filename = "sc_powerup";
      break;
  }

  pItem->setNormalImage(cocos2d::Sprite::createWithSpriteFrameName(filename + (isDisabled?"1.png":"2.png")));
  pItem->setSelectedImage(cocos2d::Sprite::createWithSpriteFrameName(filename + (isDisabled?"2.png":"1.png")));
}

void CollectionMenu::updateItemInfo()
{/* TODO: fix this
  int uid = mCurrItem;
  if( mCurrItem < 0 )
  {
    uid = mEquipedItem;
  }
  switch (mCurrentType) {
    case 0:
      {
        Shuriken &sh = (Shuriken*)(GameData::fetchShurikens()->objectAtIndex(uid));
        mItemTitle->setDisplayFrame(cocos2d::Sprite::create(sh->name.c_str())->displayFrame());
        mItemDesc->setString(sh.desc.c_str());
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
        mItemTitle->setDisplayFrame(cocos2d::Sprite::create(sh->name->getCString())->displayFrame());
        mItemDesc->setString(sh->desc->getCString());
      }
      break;
    case 2:
      {
        Special *sh = (Special*)(GameData::fetchSpecials()->objectAtIndex(uid));
        mItemTitle->setDisplayFrame(cocos2d::Sprite::create(sh->name->getCString())->displayFrame());
        mItemDesc->setString(sh->desc->getCString());
      }
      break;
  }
  */
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

void CollectionMenu::onUse(cocos2d::Ref*)
{
//  if( mCurrentType < 3 )
//  {
//    if( mCurrItem != mEquipedItem && mCurrItem>=0 )
//    {
//      bool collected = false;
//      int uiid = -1;
//      switch (mCurrentType) {
//        case 0:
//          {
//            Shuriken *sh = (Shuriken*)(GameData::fetchShurikens()->objectAtIndex(mCurrItem));
//            uiid = sh->uiid;
//            collected = GameRecord::sharedGameRecord()->collection->isItemCompleted(sh->uiid);
//          }
//          break;
//        case 1:
//          {
//            int index = mCurrItem + GAME_CHARCOUNT - 1;
//            if( mCurrItem == 0 )
//            {
//              index = GameRecord::sharedGameRecord()->curr_char;
//            }
//            Katana *sh = (Katana*)GameData::fetchKatanas()->objectAtIndex(index);
//            uiid = sh->uiid;
//            collected = GameRecord::sharedGameRecord()->collection->isItemCompleted(sh->uiid);
//          }
//          break;
//        case 2:
//          {
//            Special *sh = (Special*)GameData::fetchSpecials()->objectAtIndex(mCurrItem);
//            uiid = sh->uiid;
//            collected = GameRecord::sharedGameRecord()->collection->isItemCompleted(sh->uiid);
//          }
//          break;
//      }
//
//      if( collected )
//      {
//        markUsing(mCurrItem);
//        //保存数据
//        int cc = GameRecord::sharedGameRecord()->curr_char;
//        switch (mCurrentType) {
//          case 0:
//            {
//              GameRecord::sharedGameRecord()->char_equip_dart[cc] = mEquipedItem;
//              GameTool::PlaySound("sound/equip.mp3");
//            }
//            break;
//          case 1:
//            {
//              int index = mEquipedItem + GAME_CHARCOUNT - 1;
//              if( mEquipedItem == 0 )
//              {
//                index = GameRecord::sharedGameRecord()->curr_char;
//              }
//              GameRecord::sharedGameRecord()->char_equip_blade[cc] = index;
//              GameTool::PlaySound("sound/equip.mp3");
//            }
//            break;
//          case 2:
//            {
//              GameRecord::sharedGameRecord()->char_equip_spell[cc] = mEquipedItem;
//              GameTool::PlaySound("sound/equip.mp3");
//            }
//            break;
//        }
//        mUse->setNormalImage(cocos2d::Sprite::createWithSpriteFrameName("sc_equiped2.png"));
//        mUse->setSelectedImage(cocos2d::Sprite::createWithSpriteFrameName("sc_equiped2.png"));
//
//        //achievement change equipment
//        if( mCurrentType < 3 )
//        {
//          GameRecord::sharedGameRecord()->task->dispatchTask(ACH_CHANGEEQUIPMENT, 1);
//        }
//      }
//      else {
//        //如果没有解锁就使用万能卷轴
//        if( GameRecord::sharedGameRecord()->collection->magic_piece > 0 )
//        {
//          GameRecord::sharedGameRecord()->collection->magic_piece--;
//          GameRecord::sharedGameRecord()->collection->gainItemPiece(uiid);
//          //update info
//cocos2d::Sprite *mask = (cocos2d::Sprite*)(mScroll->contentNode->getChildByTag(mCurrItem));
//          if( GameRecord::sharedGameRecord()->collection->isItemCompleted(uiid) )
//          {
//            GameTool::PlaySound("sound/getscroll.mp3");
//            mScroll->contentNode->removeChild(mask);
//            mUse->setNormalImage(cocos2d::Sprite::createWithSpriteFrameName("sc_equip1.png"));
//            mUse->setSelectedImage(cocos2d::Sprite::createWithSpriteFrameName("sc_equip2.png"));
//          }
//          else {
//            GameTool::PlaySound("sound/getitem.mp3");
//            int piece = GameRecord::sharedGameRecord()->collection->itemTotalPiece(uiid) - GameRecord::sharedGameRecord()->collection->itemLostPiece(uiid);
//cocos2d::CCString *filename = cocos2d::CCString::createWithFormat("sc_sp%d.png", piece);
//            mask->setDisplayFrame(cocos2d::SpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(filename->getCString()));
//            //bugfix
//            if( GameRecord::sharedGameRecord()->collection->magic_piece <= 0 )
//            {
//              mUse->setNormalImage(cocos2d::Sprite::createWithSpriteFrameName("sc_unlock2.png"));
//              mUse->setSelectedImage(cocos2d::Sprite::createWithSpriteFrameName("sc_unlock2.png"));
//            }
//          }
//          mScrollCount->setString(cocos2d::CCString::createWithFormat("x%d", GameRecord::sharedGameRecord()->collection->magic_piece)->getCString());
//        }
//        else {
//          GameTool::PlaySound("sound/error.mp3");
//        }
//      }
//    }
//  }
//  else {
//    int roleid = GameRecord::sharedGameRecord()->curr_char;
//    bool enable = true;
//    SEL_CallFunc selector = NULL;
//    if( mCurrItem == 1 )
//    {
//
//      selector = callfunc_selector(CollectionMenu::onUseLife);
//      if( GameData::roleCurrHP(roleid) >= GameData::roleMaxHP(roleid) || GameRecord::sharedGameRecord()->collection->life_piece < 9 )
//      {
//        enable = false;
//      }
//    }
//    else {
//      selector = callfunc_selector(CollectionMenu::onUseDart);
//      if( GameData::roleCurrDart(roleid) >= GameData::roleMaxDart(roleid) || GameRecord::sharedGameRecord()->collection->dart_piece < 9 )
//      {
//        enable = false;
//      }
//    }
//    const char *desc = NULL;
//
//    switch (roleid) {
//      case 0:
//        {
//          desc = "xr_kt.png";
//        }
//        break;
//      case 1:
//        {
//          desc = "xr_sr.png";
//        }
//        break;
//      case 2:
//        {
//          desc = "xr_ms.png";
//        }
//        break;
//      case 3:
//        {
//          desc = "xr_mr.png";
//        }
//        break;
//    }
//    if( enable )
//    {
//      GameTool::PlaySound("sound/click.mp3");
//      setModal("pu-tc1.png", desc, this, selector);
//    }
//    else {
//      GameTool::PlaySound("sound/error.mp3");
//    }
//  }
}

void CollectionMenu::updateUsing()
{
  int nrole = GameRecord::sharedGameRecord()->curr_char;
  switch (mCurrentType) {
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
  GameTool::PlaySound("sound/click.mp3");
cocos2d::CCMenuItemImage *orl = character(orole);
  orl->setNormalImage(cocos2d::Sprite::createWithSpriteFrameName(cocos2d::CCString::createWithFormat("sc_role%d%d.png", orole, 1)->getCString()));
  orl->setSelectedImage(cocos2d::Sprite::createWithSpriteFrameName(cocos2d::CCString::createWithFormat("sc_role%d%d.png", orole, 0)->getCString()));

  int nrole = nRole;
cocos2d::CCMenuItemImage *nr = character(nrole);
  nr->setNormalImage(cocos2d::Sprite::createWithSpriteFrameName(cocos2d::CCString::createWithFormat("sc_role%d%d.png", nrole, 0)->getCString()));
  nr->setSelectedImage(cocos2d::Sprite::createWithSpriteFrameName(cocos2d::CCString::createWithFormat("sc_role%d%d.png", nrole, 1)->getCString()));
  GameRecord::sharedGameRecord()->curr_char = nrole;

  if( mCurrentType != 3 )
  {
    markCurrent(-1);
    this->updateUsing();
    this->updateItemInfo();
  }
  else {
    updateCharacterInfo(nRole, mCurrItem);
    updatePowerUpButton();
  }

  if( mCurrentType == 1 )
  {
    this->updateKatanas();
  }
}



void CollectionMenu::onItem(ButtonEnum type)
{
  if( mCurrentType == type )
    return ;

  GameTool::PlaySound("sound/click.mp3");
  setTypeButton(mCurrentType, false);
  setTypeButton(type, true);
  mItemDesc->setVisible(true);
  mCurrentType = type;
  if (type == TypeShuriken)
    this->updateShurikens();
  else if (type == TypeKatana)
    this->updateKatanas();
  else if (type == TypeSpecial)
    this->updateSpecials();
  this->updateItemInfo();
  mPowerUp->setVisible(false);
  mUse->setNormalImage(cocos2d::Sprite::createWithSpriteFrameName("sc_equiped2.png"));
  mUse->setSelectedImage(cocos2d::Sprite::createWithSpriteFrameName("sc_equiped2.png"));
  mUse->setVisible(true);
  mScroll->resetContentPosition();

  toggleShare(true);
cocos2d::CCLog("CollectionMenu::onItem");
}

void CollectionMenu::onPowerup(cocos2d::Ref*)
{
  if ( mCurrentType != TypePowerUp )
    return ;

  GameTool::PlaySound("sound/click.mp3");
  setTypeButton(mCurrentType, false);
  setTypeButton(TypePowerUp, true);
  mItemDesc->setVisible(false);
  mCurrentType = TypePowerUp;
  this->updatePowerups();
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
//  this->updateItemInfo();
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
  CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "mShuriken",       CCMenuItemImage*, mShuriken)
  CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "mKatana",         CCMenuItemImage*, mKatana)
  CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "mSpecial",        CCMenuItemImage*, mSpecial)
  CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "mPowerup",        CCMenuItemImage*, mPowerup)
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
