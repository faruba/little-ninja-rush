//
//  CommitScroll.m
//  NinjiaRush
//
//  Created by 马 颂文 on 12-6-19.
//  Copyright (c) 2012年 TrinGame. All rights reserved.
//

#include "CommitScroll.h"
#include "GamePlay.h"
#include "GameOver.h"
#include "GameRecord.h"
#include "Collections.h"
#include "SimpleAudioEngine.h"
#include "GameConstants.h"
#include "Tasks.h"
#include "GameData.h"
#include "ObjectiveComplete.h"
#include "UniversalFit.h"
#include "ClassicMode.h"

#define PRIZE_COUNT (11)

float gPrizeRates[PRIZE_COUNT] = 
{
    0.5f,//无
    0.0f,//10金币
    0.08f,//100金币
    0.02f,//500金币
    0.05f,//复活
    0.05f,//下局双倍
    0.015f,//飞镖碎片
    0.015f,//刀刃碎片
    0.015f,//技能碎片
    0.02f,//血上限
    0.02f,//飞镖上限
};

const char *gPrizeItem[PRIZE_COUNT] = 
{
    "",
    "10枚金币",
    "100枚金币",
    "500枚金币",
    "原地复活",
    "下局双倍金币",
    "飞镖碎片",
    "忍者刀碎片",
    "技能卷轴碎片",
    "生命强化碎片",
    "飞镖强化碎片",
};



CommitScroll* CommitScroll::commitScorll() 
{
    CommitScroll *ret = CommitScroll::create();
    return ret;
}

void CommitScroll::onCreate() 
{
  //hot load
  cocos2d::SpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("ui-scroll.plist");

  GamePlay *play = GamePlay::sharedGamePlay();
  play->scheduleMask(Color3B(0, 0, 0), 128, 0);
  cocosbuilder::NodeLoaderLibrary *pNodeLib = cocosbuilder::NodeLoaderLibrary::getInstance();
  cocosbuilder::CCBReader *pReader = new cocosbuilder::CCBReader(pNodeLib, this, this);
  mNode = pReader->readNodeGraphFromFile("ui-scroll", this);
  pReader->release();

  mNode->setPosition(cocos2d::Vec2(UniversalFit::sharedUniversalFit()->baseLeft, 0));
  play->addChild(mNode, LAYER_MASK+1);

  //TODO: 
  mFlash->setBlendFunc({ GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA });//fix flash bug

  mScrollCount->setString(cocos2d::CCString::createWithFormat("x%d", play->scrolls)->getCString());
  mOver = false;
  cocos2d::RotateBy *rb1 = cocos2d::RotateBy::create(1, 60);
  cocos2d::RepeatForever *rf1 = cocos2d::RepeatForever::create(rb1);
  mRotateCW->runAction(rf1);
  cocos2d::RotateBy *rb2 = cocos2d::RotateBy::create(1, -30);
  cocos2d::RepeatForever *rf2 = cocos2d::RepeatForever::create(rb2);
  mRotateACW->runAction(rf2);
  cocos2d::RotateBy *rb3 = cocos2d::RotateBy::create(1, 20);
  cocos2d::RepeatForever *rf3 = cocos2d::RepeatForever::create(rb3);
  mLightBack->runAction(rf3);
  mState = 0;
  mPrize = 0;

  mEffect = GTAnimatedSprite::spriteWithGTAnimation(GTAnimation::loadedAnimationSet("effect"));
  mEffect->setScale(2.42f);
  mEffect->setVisible(false);
  mEffect->setPosition(mScroll->getPosition());
  mNode->addChild(mEffect);

  mReviveFlag = false;
}

void CommitScroll::onUpdate(float delta) 
{
  GamePlay *play = GamePlay::sharedGamePlay();
  if( mReviveFlag )
  {
    play->manager->removeGameObject(this);
  }
  if( mOver )
  {
    mOverTimer += delta;
    if( mOverTimer > 1 )
    {
      play->manager->removeGameObject(this);
    }
  }
  if( mEffect->isVisible() )
  {
    if( mEffect->updateGTAnimation(delta) )
    {
      mEffect->setVisible(false);
    }
  }
  if( mState == 2 )
  {
    if( mPrize >= 2 )
    {
      mTimer+= delta;
      float r = 1.0f - mTimer/1;
      int op = 255*r;
      mLightBack->setOpacity(op);
      for (auto& node : mRotateCW->getChildren()) {
        cocos2d::Sprite *sp = (cocos2d::Sprite*)node;
        sp->setOpacity(op);
      }
      for (auto& node : mRotateACW->getChildren()) {
        cocos2d::Sprite *sp = (cocos2d::Sprite*)node;
        sp->setOpacity(op);
      }
      if( mTimer > 1 )
      {
        mLightBack->setOpacity(255);
        for (auto& node : mRotateCW->getChildren()) {
          cocos2d::Sprite *sp = (cocos2d::Sprite*)node;
          sp->setOpacity(255);
        }
        for (auto& node : mRotateACW->getChildren()) {
          cocos2d::Sprite *sp = (cocos2d::Sprite*)node;
          sp->setOpacity(255);
        }
        mFlash->setVisible(false);
        mFlash->setOpacity(255);
        mRotateNode->setVisible(false);
        mState = 0;

        if( play->scrolls <= 0 )
        {
          mOver = true;
          mOverTimer = 0.5f;
        }
        else {
          mScroll->stopAllActions();
          mScroll->setDisplayFrame(GameTool::getSpriteFrameByName("js_bjzd.png"));
          cocos2d::ScaleTo *sc1 = cocos2d::ScaleTo::create(0.3f, 1.2f);
          cocos2d::ScaleTo *sc2 = cocos2d::ScaleTo::create(0.1f, 1.0f);
          cocos2d::Sequence *sq = cocos2d::Sequence::create(sc1, sc2, NULL);
          mScroll->runAction(sq);
        }
      }
    } else {
      mState = 0;
      if( play->scrolls <= 0 )
      {
        mOver = true;
        mOverTimer = 0.5f;
      }
      else {
        mScroll->setVisible(true);
        mScroll->stopAllActions();
        mScroll->setDisplayFrame(GameTool::getSpriteFrameByName("js_bjzd.png"));
        cocos2d::ScaleTo *sc1 = cocos2d::ScaleTo::create(0.3f, 1.2f);
        cocos2d::ScaleTo *sc2 = cocos2d::ScaleTo::create(0.1f, 1.0f);
        cocos2d::Sequence *sq = cocos2d::Sequence::create(sc1, sc2, NULL);
        mScroll->runAction(sq);
      }
    }
  }
  if( mState == 1 && mPrize == 0 )
  {
    mTimer += delta;
    if( mTimer > 1 )
    {
      mState = 2;
      mTimer = 0;
    }
  }
}

void CommitScroll::onCashIn(cocos2d::Ref*) 
{
    GamePlay *play = GamePlay::sharedGamePlay();
    if( play->scrolls > 0 )
    {
        //achievement cash in
        GameRecord::sharedGameRecord()->task->dispatchTask(ACH_CASHEDIN, play->scrolls);
        
        GameTool::PlaySound("buy.mp3");
        play->coins += play->scrolls*PLAY_SCROLLPRICE;
        play->scrolls = 0;
        mScrollCount->setString(cocos2d::CCString::createWithFormat("x%d", play->scrolls)->getCString());
        mOver = true;
        mOverTimer = 0;
    }
}

void CommitScroll::onScroll(cocos2d::Ref*) 
{
  GamePlay *play = GamePlay::sharedGamePlay();
  if( mState == 0 )
  {
    if( play->scrolls > 0 )
    {
      play->scrolls--;
      mScrollCount->setString(cocos2d::CCString::createWithFormat("x%d", play->scrolls)->getCString());

      int prize = this->lottery();
      if( prize == 0 )
      {
        GameRecord::sharedGameRecord()->task->dispatchTask(ACH_EMPITYSCROLL, 1);
      }

      //弹出效果
      mScroll->stopAllActions();
      if( prize < 2 )
      {
        GameTool::PlaySound("smoke.mp3");
        mEffect->playGTAnimation(5, false);
        mEffect->setVisible(true);

        if( prize == 1 )
        {
          const char *filename = "prize-1.png";
          mScroll->setDisplayFrame(GameTool::getSpriteFrameByName(filename));
          mScroll->setScale(0.01f);
          cocos2d::ScaleTo *sc1 = cocos2d::ScaleTo::create(0.4f, 1.5f);
          cocos2d::ScaleTo *sc2 = cocos2d::ScaleTo::create(0.2f, 1.0f);
          cocos2d::Sequence *sq = cocos2d::Sequence::create(sc1, sc2, NULL);
          mScroll->runAction(sq);
        }
        else {
          mScroll->setVisible(false);
        }
      }
      else {
        GameTool::PlaySound("openscroll.mp3");
        mFlash->setVisible(true);
        cocos2d::FadeOut *fo1 = cocos2d::FadeOut::create(3);
        mFlash->runAction(fo1);
        mRotateNode->setVisible(true);

        const char *filename = cocos2d::CCString::createWithFormat("prize-%d.png", prize)->getCString();
        mScroll->setDisplayFrame(GameTool::getSpriteFrameByName(filename));
        mScroll->setScale(0.01f);
        cocos2d::ScaleTo *sc1 = cocos2d::ScaleTo::create(0.4f, 1.5f);
        cocos2d::ScaleTo *sc2 = cocos2d::ScaleTo::create(0.2f, 1.0f);
        cocos2d::Sequence *sq = cocos2d::Sequence::create(sc1, sc2, NULL);
        mScroll->runAction(sq);
      }

      mState = 1;
      mTimer = 0;
    }
  }
  else if (mState == 1 )
  {
    mItemName->setString("");
    this->implementPrize();

    mScroll->stopAllActions();
    cocos2d::ScaleTo *sc1 = cocos2d::ScaleTo::create(0.1f, 1.2f);
    cocos2d::ScaleTo *sc2 = cocos2d::ScaleTo::create(0.3f, 0);
    cocos2d::Sequence *sq = cocos2d::Sequence::create(sc1, sc2, NULL);
    mScroll->runAction(sq);

    mState = 2;
    mTimer = 0;
  }
}

int countUncollectedBetween(int from, int to) {
  int count = 0;
  for (int i = from; i < to; i+=1) {
    if ( !GameRecord::sharedGameRecord()->collection->isItemCompleted(i) ) {
      count += 1;
    }
  }
  return count;
}

int CommitScroll::lottery() {
  float sum = 0;
  float weight[PRIZE_COUNT];

  for (int i=0; i < 4; i += 1) { weight[i] = gPrizeRates[i]; }
  weight[4] =  GamePlay::sharedGamePlay()->reviveCount > 3 ? 0 : gPrizeRates[4];
  weight[5] = isPrizeAvailable(5) ? gPrizeRates[5] : 0;
  weight[9] = isPrizeAvailable(9) ? gPrizeRates[9] : 0;
  weight[10] = isPrizeAvailable(10) ? gPrizeRates[10] : 0;

  weight[6] = gPrizeRates[6]*countUncollectedBetween(ITEM_SHURIKENSTART, ITEM_SHURIKENEND);
  weight[7] = gPrizeRates[7]*countUncollectedBetween(ITEM_KATANASTART, ITEM_KATANAEND);
  weight[8] = gPrizeRates[8]*countUncollectedBetween(ITEM_SPECIALSTART, ITEM_SPECIALEND);

  for (int i = 0; i < PRIZE_COUNT; i += 1) {
    sum += weight[i];
  }

  // draw a lottey
  mPrize = 0;
  float res = 1.0f*randomInt(100000)/100000.0f;
  float roller = 0;
  for (int i=0; i<PRIZE_COUNT; ++i) {
    float key = weight[i]/sum;
    roller += key;

    if( res < roller ) {
      mPrize = i;
      break;
    }
  }

  mItemName->setString(gPrizeItem[mPrize]);
  mItemName->stopAllActions();
  cocos2d::FadeIn *fo = cocos2d::FadeIn::create(2);
  mItemName->runAction(fo);
  return mPrize;
}

void CommitScroll::implementPrize() {
    GamePlay *play = GamePlay::sharedGamePlay();
    //implement prize
    switch (mPrize) {
      case 1:
        play->coins += 10;
        GameTool::PlaySound("getcoin.mp3");
        break;
      case 2://Gold 100
        play->coins += 100;
        GameTool::PlaySound("getcoin.mp3");
        break;
      case 3://Gold 500
        play->coins += 500;
        GameTool::PlaySound("getcoin.mp3");
        break;
      case 4://Revive
        mReviveFlag = true;
        GameTool::PlaySound("charge.mp3");
        play->reviveCount++;
        break;
      case 5://x2 Gold Next Run
        GameRecord::sharedGameRecord()->game_doublecoin++;
        GameTool::PlaySound("getcoin.mp3");
        break;
      case 6:
        drawLottoryBetween(ITEM_SHURIKENSTART, ITEM_SHURIKENEND, GameData::fetchShurikens());
        break;
      case 7:
        drawLottoryBetween(ITEM_KATANASTART, ITEM_KATANAEND, GameData::fetchKatanas());
        break;
      case 8:
        drawLottoryBetween(ITEM_SPECIALSTART, ITEM_SPECIALEND, GameData::fetchSpecials());
        break;
      case 9://Power Up for HP
        GameRecord::sharedGameRecord()->collection->life_piece++;
        if( GameRecord::sharedGameRecord()->collection->life_piece%9 == 0 ) {
          GamePlay::sharedGamePlay()->pushNotification("lifeplus.png", "pulife.png", 4);
        }
        break;
      case 10://Power Up for Shuriken
        GameRecord::sharedGameRecord()->collection->dart_piece++;
        if ( GameRecord::sharedGameRecord()->collection->dart_piece%9 == 0 ) {
          GamePlay::sharedGamePlay()->pushNotification("dartplus.png", "pushuriken.png", 4);
        }
        break;
    }
}

bool notAllItemCompletedBetween(int from, int to) {
  for (int i = from; i < to; i +=1) {
    if ( !GameRecord::sharedGameRecord()->collection->isItemCompleted(i) ) {
      return true;
    }
  }
  return false;
}

bool CommitScroll::isPrizeAvailable(int pid) {
  switch (pid) {
    case 5: return !(GameRecord::sharedGameRecord()->game_doublecoin > 0); break;
    case 6: notAllItemCompletedBetween(ITEM_SHURIKENEND, ITEM_SHURIKENSTART); break;
    case 7: notAllItemCompletedBetween(ITEM_KATANAEND, ITEM_KATANASTART); break;
    case 8: notAllItemCompletedBetween(ITEM_SPECIALEND, ITEM_SPECIALSTART); break;
    case 9:
            {//Piece Of HP Maxium
              int thl = 0;
              for (int i=0; i<GAME_CHARCOUNT; ++i) {
                thl += (GameData::roleMaxHP(i) - GameData::roleCurrHP(i))*9;
              }
              if( GameRecord::sharedGameRecord()->collection->life_piece >= thl )
              {
                return false;
              }
            }
            break;
    case 10:
            {//Piece Of Shuriken Maxium
              int thl = 0;
              for(int i=0; i<GAME_CHARCOUNT; ++i)
              {
                thl += (GameData::roleMaxDart(i) - GameData::roleCurrDart(i))*9;
              }
              if( GameRecord::sharedGameRecord()->collection->dart_piece >= thl )
              {
                return false;
              }
            }
            break;
    default: return true; break;
  }

  return true;
}

void CommitScroll::onDestroy() 
{
  GamePlay *play = GamePlay::sharedGamePlay();
  play->unscheduleMask();
  play->removeChild(mNode, true);
  if( !mReviveFlag )
  {
    play->classic->goOver();
  }
  else {//Revive
    play->state = STATE_REVIVE;
    play->substate = 0;
    play->stateTimer = 0;
    play->gameOverTimer = -1;
    play->mainrole->HP = 1;
    play->mainrole->customAction(1);
    play->count_runscene--;
    play->gameOverTimer=-1;
    play->mainrole->timedInvincible(5);
  }

  //hot release
  cocos2d::SpriteFrameCache::sharedSpriteFrameCache()->removeSpriteFramesFromFile("ui-scroll.plist");
  unloadTextureFromeSpriteFrameFile("ui-scroll.plist");
}

SEL_MenuHandler CommitScroll::onResolveCCBCCMenuItemSelector(cocos2d::Ref * pTarget, const char* pSelectorName)
{
  CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "onCashIn", CommitScroll::onCashIn)
  CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "onScroll", CommitScroll::onScroll)
  return NULL;
}

cocos2d::extension::Control::Handler   CommitScroll::onResolveCCBCCControlSelector(cocos2d::Ref * pTarget, const char* pSelectorName)
{
  return NULL;
}
bool CommitScroll::onAssignCCBMemberVariable(cocos2d::Ref* pTarget, const char* pMemberVariableName, Node* pNode)
{
  CCB_MEMBERVARIABLEASSIGNER_GLUE(this,"mArrow", Sprite *, mArrow)
  CCB_MEMBERVARIABLEASSIGNER_GLUE(this,"mScroll", Sprite *, mScroll)
  CCB_MEMBERVARIABLEASSIGNER_GLUE(this,"mTitle", Sprite *, mTitle)
  CCB_MEMBERVARIABLEASSIGNER_GLUE(this,"mScrollCount", Label *, mScrollCount)
  CCB_MEMBERVARIABLEASSIGNER_GLUE(this,"mRotateNode", Node *, mRotateNode)
  CCB_MEMBERVARIABLEASSIGNER_GLUE(this,"mLightBack", Sprite *, mLightBack)
  CCB_MEMBERVARIABLEASSIGNER_GLUE(this,"mRotateACW", Node *, mRotateACW)
  CCB_MEMBERVARIABLEASSIGNER_GLUE(this,"mRotateCW", Node *, mRotateCW)
  CCB_MEMBERVARIABLEASSIGNER_GLUE(this,"mFlash", LayerColor *, mFlash)
  CCB_MEMBERVARIABLEASSIGNER_GLUE(this,"mItemName", Label *, mItemName)

  return false;
}
