#include "SelectMenu.h"
#include "TitleMenu.h"
#include "UniversalFit.h"
#include "CEClipedNode.h"
#include "SimpleAudioEngine.h"
#include "GamePlay.h"
#include "GameData.h"
#include "GameTool.h"
#include "GameRecord.h"
#include "CollectionMenu.h"
#include "Tasks.h"
#include "ShopMenu.h"
#include "AnimationRes.h"
#include "PublicLoad.h"
#include "Loading.h"

const char *gHeroDesc[] =
{
  "rwjs1.png",
  "rwjs2.png",
  "rwjs3.png",
  "rwjs4.png",
};

const char *gBouns[] =
{
  "bouns30.png",
  "bouns10.png",
  "bouns60.png",
  "bouns80.png",
};

using namespace CocosDenshion;

bool SelectMenu::init()
{
  if (!Layer::init())
    return false;

  auto listener = EventListenerTouchOneByOne::create();
  listener->onTouchBegan = CC_CALLBACK_2(SelectMenu::onTouchBegan, this);
  listener->onTouchEnded = CC_CALLBACK_2(SelectMenu::onTouchEnded, this);
  listener->onTouchMoved = CC_CALLBACK_2(SelectMenu::onTouchMoved, this);
  listener->onTouchCancelled = CC_CALLBACK_2(SelectMenu::onTouchEnded, this);
  _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);


  this->scheduleUpdate();
  this->setTouchEnabled(true);
cocos2d::Node *taskcomplete = cocos2d::Node::create();
  taskcomplete->setPosition(cocos2d::Vec2(SCREEN_WIDTH/2, SCREEN_HEIGHT));
  this->addChild(taskcomplete, 5);

  GamePlay::sharedGamePlay()->setTaskCompleteNode(taskcomplete);

  mButtoned = false;

  return true;
}

void SelectMenu::onEnter()
{
  PublicLoad::menuSelect()->loadAll();
  mUISwapper.onEnter();
cocos2d::Node *ui = createUIByCCBI("menu-select", "SelectMenu", SelectMenuLayerLoader::loader(), this);
  this->addChild(ui);
  mStart = NULL;

  if( GamePlay::sharedGamePlay()->getGameMode() == MODE_CLASSIC )
  {
    mTitle->setDisplayFrame(cocos2d::SpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("classict.png"));
    mParam->setDisplayFrame(cocos2d::SpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("xr_jz-life.png"));
  }
  else
  {
    mTitle->setDisplayFrame(cocos2d::SpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("arcadet.png"));
    mParam->setDisplayFrame(cocos2d::SpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("xr_jz-bouns.png"));
  }

  mProtraits[0] = cocos2d::Sprite::createWithSpriteFrameName("xr_main.png");
  mProtraits[0]->setAnchorPoint(cocos2d::Vec2(0.5f, 0));
  mProtraits[0]->setVisible(false);
  mHero->addChild(mProtraits[0]);
  mProtraits[1] = cocos2d::Sprite::createWithSpriteFrameName("xr_girl.png");
  mProtraits[1]->setAnchorPoint(cocos2d::Vec2(0.5f, 0));
  mProtraits[1]->setVisible(false);
  mHero->addChild(mProtraits[1]);
  mProtraits[2] = cocos2d::Sprite::createWithSpriteFrameName("xr_musashi.png");
  mProtraits[2]->setAnchorPoint(cocos2d::Vec2(0.5f, 0));
  mProtraits[2]->setVisible(false);
  mHero->addChild(mProtraits[2]);
  mProtraits[3] = cocos2d::Sprite::createWithSpriteFrameName("xr_cat.png");
  mProtraits[3]->setAnchorPoint(cocos2d::Vec2(0.5f, 0));
  mProtraits[3]->setVisible(false);
  mHero->addChild(mProtraits[3]);

  mSliding = false;
  mSlideTimer = -1;
  mTargetRole = -1;
  //添加动画
cocos2d::CCMoveBy *lmb1 = cocos2d::CCMoveBy::create(0.2f, Vec2(-10, 0));
cocos2d::CCMoveBy *lmb2 = cocos2d::CCMoveBy::create(0.2f, Vec2(10, 0));
cocos2d::CCSequence *lseq = (cocos2d::CCSequence*)CCSequence::create(lmb1, lmb2, NULL);
cocos2d::CCRepeatForever *lrep = cocos2d::CCRepeatForever::create(lseq);
  mLeftMark->runAction(lrep);
cocos2d::CCMoveBy *rmb1 = cocos2d::CCMoveBy::create(0.2f, Vec2(10, 0));
cocos2d::CCMoveBy *rmb2 = cocos2d::CCMoveBy::create(0.2f, Vec2(-10, 0));
cocos2d::CCSequence *rseq = (cocos2d::CCSequence*)CCSequence::create(rmb1, rmb2, NULL);
cocos2d::CCRepeatForever *rrep = cocos2d::CCRepeatForever::create(rseq);
  mRightMark->runAction(rrep);
  mCurrRole = GameRecord::sharedGameRecord()->curr_char;
  this->updateCharacterInfo(mCurrRole);
  mShadowDir = ccpNormalize(cocos2d::Vec2(-12, 17));
  //创建角色肖像
  mCurrAngle = mCurrRole*90;
  this->updateAngle();

  mUISwapper.setSceneIntro(this);


    GameTool::PlayBackgroundMusic("menu.mp3");

cocos2d::Layer::onEnter();
}

void SelectMenu::updateCharacterInfo(int rid) {
  if ( GamePlay::sharedGamePlay()->getGameMode() == MODE_CLASSIC ) {
    //更新血
    mHearts->removeAllChildrenWithCleanup(true);

    for (int i=0; i<GameData::roleCurrHP(rid); ++i) {
      cocos2d::Sprite *heart = cocos2d::Sprite::createWithSpriteFrameName("heart.png");
      heart->setPosition(cocos2d::Vec2(i*(1+heart->getContentSize().width), 0));
      mHearts->addChild(heart);
    }
    mBouns->setVisible(false);
  } else {
    mBouns->setDisplayFrame(cocos2d::SpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(gBouns[rid]));
    mHearts->setVisible(false);
  }
  //更新飞镖数
  mDarts->removeAllChildrenWithCleanup(true);
  float dartoff = 70.0f/GameData::roleCurrDart(rid);
  if( dartoff > 10 ) {
    dartoff = 10;
  }
  for (int i=0; i<GameData::roleCurrDart(rid); ++i) {
    cocos2d::Sprite *dart = cocos2d::Sprite::createWithSpriteFrameName("ui-dart.png");
    dart->setPosition(cocos2d::Vec2(i*dartoff, 0));
    mDarts->addChild(dart);
  }
  //更新飞镖 更新特技 更新刀刃
  int shid = GameRecord::sharedGameRecord()->char_equip_dart[rid];
  int blid = GameRecord::sharedGameRecord()->char_equip_blade[rid];
  int spid = GameRecord::sharedGameRecord()->char_equip_spell[rid];

  Shuriken sh = GameData::fetchShurikens()[shid];
  Katana ka = GameData::fetchKatanas()[blid];
  Special sp = GameData::fetchSpecials()[spid];

  mEquipDart->setDisplayFrame(cocos2d::Sprite::create(sh.icon.c_str())->displayFrame());
  mEquipBlade->setDisplayFrame(cocos2d::Sprite::create(ka.icon.c_str())->displayFrame());
  mEquipSpecial->setDisplayFrame(cocos2d::Sprite::create(sp.icon.c_str())->displayFrame());

  //更新人物显示
  switch (rid) {
    case 0:
      mDesc->setDisplayFrame(cocos2d::SpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("xr_kt.png"));
      mBtnStart->setNormalImage(cocos2d::Sprite::createWithSpriteFrameName("xr_st1.png"));
      mBtnStart->setSelectedImage(cocos2d::Sprite::createWithSpriteFrameName("xr_st1.png"));
      break;
    case 1:
      mDesc->setDisplayFrame(cocos2d::SpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("xr_sr.png"));
      mBtnStart->setNormalImage(cocos2d::Sprite::createWithSpriteFrameName("xr_st2.png"));
      mBtnStart->setSelectedImage(cocos2d::Sprite::createWithSpriteFrameName("xr_st2.png"));
      break;
    case 2:
      mDesc->setDisplayFrame(cocos2d::SpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("xr_ms.png"));
      mBtnStart->setNormalImage(cocos2d::Sprite::createWithSpriteFrameName("xr_st3.png"));
      mBtnStart->setSelectedImage(cocos2d::Sprite::createWithSpriteFrameName("xr_st3.png"));
      break;
    case 3:
      mDesc->setDisplayFrame(cocos2d::SpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("xr_mr.png"));
      mBtnStart->setNormalImage(cocos2d::Sprite::createWithSpriteFrameName("xr_st4.png"));
      mBtnStart->setSelectedImage(cocos2d::Sprite::createWithSpriteFrameName("xr_st4.png"));
      break;
  }

  mStartPos->removeAllChildrenWithCleanup(false);
  mStart = NULL;
  //更新人物动画
  if( mPreview != NULL )
  {
    mStartPos->removeChild(mPreview, true);
  }
  mPreview = GTAnimatedSprite::spriteWithGTAnimation(GTAnimation::loadedAnimationSet(GameData::roleAnimation(rid)->getCString()));
  mPreview->setAnchorPoint(cocos2d::Vec2(0.4f, 0.0625f));
  mPreview->setPosition(cocos2d::Vec2( -10, 10));
  mPreview->playGTAnimation(0, true);
  mStartPos->addChild(mPreview);
  if( GameRecord::sharedGameRecord()->char_contract[rid] == 0 )
  {
    mClickMe->setVisible(true);
    cocos2d::Sprite *hire = cocos2d::Sprite::createWithSpriteFrameName("hire.png");
    mStartPos->addChild(hire);
  }
  else {
    mClickMe->setVisible(false);

    mStart = GTAnimatedSprite::spriteWithGTAnimation(GTAnimation::loadedAnimationSet("ui"));
    mStart->playGTAnimation(2, true);
    mStartTimer = 0;
    mStartPos->addChild(mStart);
  }

  mHeroDesc->setDisplayFrame(cocos2d::SpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(gHeroDesc[rid]));
}

void SelectMenu::updateAngle()
{
  for(int i=0; i<GAME_CHARCOUNT; ++i)
  {
    float angle = i*90;
    float disangle = fabsf(mCurrAngle-angle);
    if( disangle > 89 && disangle < 270 )
    {
      mProtraits[i]->setVisible(false);
    }
    else {
      float k = sinf(CC_DEGREES_TO_RADIANS(disangle));
      float m = 1-k*k;
      float pos = k*100;
      ccColor3B col = ccc3(255*m, 255*m, 255*m);
      float scale = 0.5f+0.5f*m;
      if( angle < mCurrAngle )
      {
        pos *= -1;
      }
      mHero->reorderChild(mProtraits[i], 100*m);
      mProtraits[i]->setOpacity(255*m);
      mProtraits[i]->setScale(scale);
      mProtraits[i]->setColor(col);
      mProtraits[i]->setPosition(cocos2d::Vec2(pos, 0));
      mProtraits[i]->setVisible(true);
    }
  }
}

void SelectMenu::update(float delta)
{
  if ( mSlideTimer >= 0 ) {
    mSlideTimer += delta;
  }

  mPreview->updateGTAnimation(delta);
  mStartTimer += delta;

  if( mStart != NULL )
  {
    if( mStart->updateGTAnimation(delta) )
    {
      mStart->playGTAnimation(2, true);
    }
    if( mStartTimer > 3 )
    {
      mStartTimer = 0;
      mStart->playGTAnimation(3, false);
    }
  }

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

  //move character
  while( mCurrAngle < 0 )
  {
    mCurrAngle += 360;
  }
  while ( mCurrAngle >= 360 ) {
    mCurrAngle -= 360;
  }
  //move to target
  if( mTargetRole >= 0 )
  {
    float ta = mTargetRole*90;
    float dis = ta - mCurrAngle;
    while( dis >= 180 )
    {
      dis -= 360;
    }
    while ( dis < -180 ) {
      dis += 360;
    }
    float step = delta*120;
    if( fabsf(dis) <= step )
    {
      mCurrAngle = ta;
      mCurrRole = mTargetRole;
      updateCharacterInfo(mCurrRole);
      mTargetRole = -1;
      GameRecord::sharedGameRecord()->curr_char = mCurrRole;
    }
    else {
      if( dis < 0 )
      {
        mCurrAngle -= step;
      }
      else {
        mCurrAngle += step;
      }
    }
  }
  this->updateAngle();

  {
//    //update money
//    char szTmp[64];
//    sprintf(szTmp, "%d", GameRecord::sharedGameRecord()->coins);
//    mMoney->setString(szTmp);
//    cocos2d::Point mp = mMoney->getPosition();
//    mp.x = 450 - 7.15f*strlen(mMoney->getString());
//    mMoney->setPosition( mp );
//    cocos2d::Point cm = mCoinMark->getPosition();
//    cm.x = mp.x - 10;
//    mCoinMark->setPosition(cm);
  }
cocos2d::Layer::update(delta);
}
void SelectMenu::onClickMe(cocos2d::Ref*)
{
  GameTool::PlaySound("sound/menu-change.mp3");
  ShopMenu::setNavBack(1);
  mUISwapper.setSceneOutro(ShopMenu::scene(), this);
}

void SelectMenu::onStart(cocos2d::Ref*)
{
    {
    int cost = 0;
    int rid = GameRecord::sharedGameRecord()->curr_char;
    if( GameRecord::sharedGameRecord()->char_contract[rid] == 0 )
    {
      cost = 100;
    }
    if( GameRecord::sharedGameRecord()->coins >= cost )
    {
      GameRecord::sharedGameRecord()->makeCoins(-cost);
      if( cost > 0 )
      {
        GameTool::PlaySound("sound/buy.mp3");
      }

      GameTool::PlaySound("sound/menu-start.mp3");
      mUISwapper.setSceneOutro(Loading::loadTo(GamePlay::sharedGamePlay()->scene(), PublicLoad::gameLoadingList(), PublicLoad::menuSelect(), true), this);
    }
    else {
      GameTool::PlaySound("sound/error.mp3");
    }
  }
}

void SelectMenu::onChangeDart(cocos2d::Ref*)
{
  if( mUISwapper.isDone() )
  {
    GameTool::PlaySound("sound/menu-change.mp3");
    CollectionMenu::setNavBack(1);
    mUISwapper.setSceneOutro(CollectionMenu::scene(), this);
  }
}

void SelectMenu::onChangeBlade(cocos2d::Ref*)
{
  if( mUISwapper.isDone() )
  {
    GameTool::PlaySound("sound/menu-change.mp3");
    CollectionMenu::setNavBack(2);
    mUISwapper.setSceneOutro(CollectionMenu::scene(), this);
  }
}

void SelectMenu::onChangeSpecial(cocos2d::Ref*)
{
  if( mUISwapper.isDone() )
  {
    GameTool::PlaySound("sound/menu-change.mp3");
    CollectionMenu::setNavBack(3);
    mUISwapper.setSceneOutro(CollectionMenu::scene(), this);
  }
}

void SelectMenu::onBack(cocos2d::Ref*)
{
  if(mUISwapper.isDone())
  {
    GameTool::PlaySound("sound/menu-change.mp3");
    mUISwapper.setSceneOutro(GameTool::scene<TitleMenu>(), this);
  }
}

void SelectMenu::onLeftRole(cocos2d::Ref*)
{
  mTargetRole = mCurrRole==0 ? GAME_CHARCOUNT-1 : mCurrRole-1;
  mSliding = false;

  //achievement select role
  GameRecord::sharedGameRecord()->task->dispatchTask(ACH_SELECTBUTTON, 1);
  GameTool::PlaySound("sound/flip.mp3");
}

void SelectMenu::onRightRole(cocos2d::Ref*)
{
  if (mCurrRole==GAME_CHARCOUNT-1)
    mTargetRole =  0;
  else
    mTargetRole =  mCurrRole+1;

  mSliding = false;

  //achievement select role
  GameRecord::sharedGameRecord()->task->dispatchTask(ACH_SELECTBUTTON, 1);
  GameTool::PlaySound("sound/flip.mp3");
}

bool SelectMenu::onTouchBegan(Touch * touch, Event * event)
{
  if( mTargetRole == -1 )
  {
    LNR_GET_TOUCH_POS;

    if( pos.x > 174 &&
        pos.x < 457 &&
        pos.y > 97 &&
        pos.y < 323 )
    {
      mSlideBegin = pos.x;
      mSlideBeginX = mSlideBegin;
      mSlideAngle = mCurrAngle;
      mSliding = true;
      mSlideTimer = 0;
    }
    return true;
  }
  return false;
}

void SelectMenu::onTouchMoved(Touch * touch, Event * event)
{
  if( mTargetRole == -1 && mSliding )
  {
    LNR_GET_TOUCH_POS;

    float dis = pos.x - mSlideBegin;
    float range = dis;
    float sp = range/mSlideTimer;
    if( fabsf(sp) > 600 )
    {
      range = mSlideTimer*600;
      if( dis < 0 )
      {
        range *= -1;
      }
    }
    mCurrAngle = mSlideAngle - range;
    if( fabsf(pos.x - mSlideBeginX) > 50 )
    {
      if( dis > 0 )
      {//--
        mTargetRole = mCurrRole==0 ? GAME_CHARCOUNT-1 : mCurrRole-1;
      }
      else {
        //++
        mTargetRole = mCurrRole==GAME_CHARCOUNT-1 ? 0 : mCurrRole+1;
      }
      mSliding = false;
      mSlideTimer = -1;
      //achievement
      GameRecord::sharedGameRecord()->task->dispatchTask(ACH_SELECTBUTTON, 1);
      GameTool::PlaySound("sound/flip.mp3");
    }

    mSlideAngle = mCurrAngle;
    mSlideBegin = pos.x;
    mSlideTimer = 0;
  }
}

void SelectMenu::onTouchEnded(Touch * touch, Event * event)
{
  if( mTargetRole == -1 && mSliding )
  {
    mTargetRole = mCurrRole;
  }
}

SEL_MenuHandler SelectMenu::onResolveCCBCCMenuItemSelector(cocos2d::Ref * pTarget, const char* pSelectorName)
{
  CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "onStart", SelectMenu::onStart)
  CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "onChangeDart", SelectMenu::onChangeDart)
  CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "onChangeBlade", SelectMenu::onChangeBlade)
  CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "onChangeSpecial", SelectMenu::onChangeSpecial)
  CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "onBack", SelectMenu::onBack)
  CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "onLeftRole", SelectMenu::onLeftRole)
  CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "onRightRole", SelectMenu::onRightRole)
  CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "onClickMe", SelectMenu::onClickMe)
  return NULL;
}

cocos2d::extension::Control::Handler   SelectMenu::onResolveCCBCCControlSelector(cocos2d::Ref * pTarget, const char* pSelectorName)
{
  return NULL;
}

bool SelectMenu::onAssignCCBMemberVariable(cocos2d::Ref* pTarget, const char* pMemberVariableName, Node* pNode)
{
  CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "mShadow", Sprite*,  mShadow)
  CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "mDesc", Sprite*,  mDesc)
  CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "mEquipDart", Sprite*,  mEquipDart)
  CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "mEquipBlade", Sprite*,  mEquipBlade)
  CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "mEquipSpecial", Sprite*,  mEquipSpecial)
  CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "mLeftMark", Sprite*,  mLeftMark)
  CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "mRightMark", Sprite*,  mRightMark)
  CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "mCoinMark", Sprite*,  mCoinMark)
  CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "mHeroDesc", Sprite*,  mHeroDesc)
  CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "mTitle", Sprite*,  mTitle)
  CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "mParam", Sprite*,  mParam)
  CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "mBouns", Sprite*,  mBouns)
  CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "mHero", Node*,  mHero)
  CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "mHearts", Node*,  mHearts)
  CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "mDarts", Node*,  mDarts)
  CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "mStartPos", Node*,  mStartPos)
  CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "mBtnStart", CCMenuItemImage*,  mBtnStart)
  CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "mBtnDart", CCMenuItemImage*,  mBtnDart)
  CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "mBtnBlade", CCMenuItemImage*,  mBtnBlade)
  CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "mBtnSpecial", CCMenuItemImage*,  mBtnSpecial)
  CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "mClickMe", CCMenuItemImage*,  mClickMe)
  CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "mMoney", cocos2d::Label*,  mMoney)

  return false;
}
