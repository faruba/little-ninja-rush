//
//  TutorialDirector.m
//  NinjiaRush
//
//  Created by 马 颂文 on 12-7-4.
//  Copyright (c) 2012年 TrinGame. All rights reserved.
//

#include "TutorialDirector.h"
#include "GamePlay.h"
#include "GameRecord.h"
#include "GameConstants.h"
#include "Item.h"
#include "UniversalFit.h"



TutorialDirector* TutorialDirector::tutorial() 
{
    return TutorialDirector::create();
}

void TutorialDirector::onCreate() 
{
  GamePlay *play = GamePlay::sharedGamePlay();

  //cache User Setting
  int rid = GameRecord::sharedGameRecord()->curr_char;

  cacheDartId = GameRecord::sharedGameRecord()->char_equip_dart[rid];
  cacheBladeId = GameRecord::sharedGameRecord()->char_equip_blade[rid];
  cacheSpellId = GameRecord::sharedGameRecord()->char_equip_spell[rid];
  play->mainrole->setEquipDart(0);
  play->mainrole->setEquipBlade(rid);
  play->mainrole->setEquipSpell(0);
cocos2d::CCLog("begin tutorial: role id(%d), cacheDartId(%d)", rid, cacheDartId);

  play->state = STATE_TUTORIAL;
  play->count_respawn++;

  mLayer = cocos2d::LayerColor::create(Color4B(0, 0, 0, 128), UniversalFit::sharedUniversalFit()->playSize.width, 75);
  mLayer->setPosition(cocos2d::Vec2(0, 122.5f));
  play->addChild(mLayer, LAYER_UI);

  mAnimateNode = cocos2d::Node::create();
  mAnimateNode->setPosition(cocos2d::Vec2( 80, 160 ));
  play->addChild(mAnimateNode, LAYER_UI);
  mAnimateSprite = NULL;

  mDummyEnemy = TutorialPuppet::puppet();
  play->enemies->addObject(play->manager->addGameObject(mDummyEnemy));
  for( int i=0; i<3; ++i)
  {
    mDummies[i] = TutorialPuppet::puppet();
    play->enemies->addObject(play->manager->addGameObject(mDummies[i]));
  }
  //mTeachText = cocos2d::CCLabelBMFont::create("", "ab40.fnt");
    mTeachText = cocos2d::Label::create("", GFONT_NAME, GFONT_SIZE_NORMAL);
    
  mTeachText->setAnchorPoint(cocos2d::Vec2( 0, 0.5f));
  mTeachText->setPosition(cocos2d::Vec2(UniversalFit::sharedUniversalFit()->playSize.width/2 - 60, SCREEN_HEIGHT/2));
  play->addChild(mTeachText, LAYER_UI);
cocos2d::CCMenuItemImage * skip = cocos2d::CCMenuItemImage::create();
  skip->setNormalSpriteFrame(cocos2d::SpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("jcskip.png"));
  skip->setSelectedSpriteFrame(cocos2d::SpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("sp_jcskip.png"));
  /*TODO:Uncomment this 
   skip->setTarget(this, menu_selector(TutorialDirector::onSkip));
   */
  skip->setAnchorPoint(cocos2d::Vec2(1, 0));
  skip->setPosition(cocos2d::Vec2(UniversalFit::sharedUniversalFit()->playSize.width-5, 200));
  mMenu = cocos2d::CCMenu::create(skip, NULL);
  mMenu->setPosition(cocos2d::Vec2(0, 0));
  play->addChild(mMenu, LAYER_UI);

  this->begin();
}

void TutorialDirector::onSkip() 
{
    this->skip();
}

void TutorialDirector::onUpdate(float delta) 
{
  GamePlay *play = GamePlay::sharedGamePlay();
  if( mAnimateSprite != NULL )
  {
    mAnimateSprite->updateGTAnimation(delta);
  }
  mTimer += delta;
  switch (mState) {
    case 0:
      {
cocos2d::CCObject* node = NULL;
CCARRAY_FOREACH(play->darts, node)
        {
          Darts *d = (Darts*)node;
          if( d->isEnemy())
          {
            if( d->position().y <= PLAY_PLAYERLINE + 50 )
            {
              this->waitBlade();
            }
          }
        }
      }
      break;
    case 1:
      {
        bool found = false;
cocos2d::CCObject* node = NULL;
CCARRAY_FOREACH(play->darts, node)
        {
          Darts *d = (Darts*)node;
          if( d->isEnemy())
          {
            found = true;
          }
        }
        if( !found )
        {
          mState = 2;
          this->doneBlade();
        }
      }
      break;
    case 2:
      {
        if( play->mainrole->SP == play->mainrole->maxSP )
        {
          mState = 4;
          this->doneCharge();
        }
      }
      break;
    case 3:
      {
        mTimer += delta;
        if( mTimer > 5 )
        {
          if( mCount < 3 )
          {
            if( play->mainrole->HP < play->mainrole->maxHP )
            {
              play->mainrole->HP++;
            }
            mDummyEnemy->fireAt(play->mainrole->position(), NULL, NULL);
            mCount++;
          }
          else {
            mState = 2;
            this->doneEvade();
          }
          mTimer = 0;
        }
      }
      break;
    case 4:
      {
        if( play->mainrole->SP < play->mainrole->maxSP )
        {
          mTimer = 0;
          mState = 5;
          this->doneTutorial();
        }
      }
      break;
    case 5:
      {
        mTimer += delta;
        if( mTimer > 8 )
        {
          this->skip();
        }
      }
      break;
  }
}

void TutorialDirector::playSlideUp() 
{
    mAnimateNode->removeAllChildrenWithCleanup(true);
cocos2d::Sprite *panel = cocos2d::Sprite::createWithSpriteFrameName("jc1.png");
    panel->setAnchorPoint(cocos2d::Vec2(0.5f, 0.5f));
    panel->setPosition(cocos2d::Vec2(0, 0));
    mAnimateNode->addChild(panel);
cocos2d::Sprite *finger = cocos2d::Sprite::createWithSpriteFrameName("jc4.png");
    finger->setAnchorPoint(cocos2d::Vec2(0.065789f, 0.91666f));
    finger->setPosition(cocos2d::Vec2(37.5, 20.5f));
    panel->addChild(finger);
cocos2d::CCMoveTo *mt = cocos2d::CCMoveTo::create(1, Vec2(64, 36));
cocos2d::CCMoveTo *mt2 = cocos2d::CCMoveTo::create(0.1, Vec2(37.5, 20.5));
cocos2d::CCSequence *sq = cocos2d::CCSequence::create(mt, mt2, NULL);
cocos2d::CCRepeatForever *rf = cocos2d::CCRepeatForever::create(sq);
    finger->runAction(rf);
}

void TutorialDirector::stopSlide() 
{
    mAnimateNode->removeAllChildrenWithCleanup(true);
}

void TutorialDirector::playSlideForward() 
{
    mAnimateNode->removeAllChildrenWithCleanup(true);
cocos2d::Sprite *panel = cocos2d::Sprite::createWithSpriteFrameName("jc1.png");
    panel->setAnchorPoint(cocos2d::Vec2(0.5f, 0.5f));
    panel->setPosition(cocos2d::Vec2(0, 0));
    mAnimateNode->addChild(panel);
cocos2d::Sprite *finger = cocos2d::Sprite::createWithSpriteFrameName("jc4.png");
    finger->setAnchorPoint(cocos2d::Vec2(0.065789f, 0.91666f));
    finger->setPosition(cocos2d::Vec2(34.5f, 28));
    panel->addChild(finger);
cocos2d::CCMoveTo *mt = cocos2d::CCMoveTo::create(1, Vec2(63.5f, 28));
cocos2d::CCMoveTo *mt2 = cocos2d::CCMoveTo::create(0.1, Vec2(34.5f, 28));
cocos2d::CCSequence *sq = cocos2d::CCSequence::create(mt, mt2, NULL);
cocos2d::CCRepeatForever *rf = cocos2d::CCRepeatForever::create(sq);
    finger->runAction(rf);
}

void TutorialDirector::playSlideDown() 
{
    mAnimateNode->removeAllChildrenWithCleanup(true);
cocos2d::Sprite *panel = cocos2d::Sprite::createWithSpriteFrameName("jc1.png");
    panel->setAnchorPoint(cocos2d::Vec2(0.5f, 0.5f));
    panel->setPosition(cocos2d::Vec2(0, 0));
    mAnimateNode->addChild(panel);
cocos2d::Sprite *finger = cocos2d::Sprite::createWithSpriteFrameName("jc4.png");
    finger->setAnchorPoint(cocos2d::Vec2(0.065789f, 0.91666f));
    finger->setPosition(cocos2d::Vec2(50, 37.5f));
    panel->addChild(finger);
cocos2d::CCMoveTo *mt = cocos2d::CCMoveTo::create(1, Vec2(50, 20));
cocos2d::CCMoveTo *mt2 = cocos2d::CCMoveTo::create(0.1, Vec2(50, 37.5f));
cocos2d::CCSequence *sq = cocos2d::CCSequence::create(mt, mt2, NULL);
cocos2d::CCRepeatForever *rf = cocos2d::CCRepeatForever::create(sq);
    finger->runAction(rf);
}

void TutorialDirector::playShake() 
{
    mAnimateNode->removeAllChildrenWithCleanup(true);
    mAnimateSprite = GTAnimatedSprite::spriteWithGTAnimation(GTAnimation::loadedAnimationSet("misc"));
    mAnimateSprite->playGTAnimation(8, true);
    mAnimateSprite->setAnchorPoint(cocos2d::Vec2(0.5f, 0.5f));
    mAnimateSprite->setPosition(cocos2d::Vec2(0, 0));
    mAnimateNode->addChild(mAnimateSprite);
}

void TutorialDirector::stopShake() 
{
    mAnimateNode->removeAllChildrenWithCleanup(true);
    mAnimateSprite = NULL;
}

void TutorialDirector::begin() 
{
    GamePlay *play = GamePlay::sharedGamePlay();
    mState = 0;
    mDummyEnemy->reset();
    mDummyEnemy->moveTo(cocos2d::Vec2(300, RESPAWN_YMIN+RESPAWN_Y/2), this, callfunc_selector(TutorialDirector::doneMove));
    play->count_control++;
    play->mainrole->flag_dart = false;
    play->mainrole->flag_blade = false;
    play->mainrole->flag_spell = false;
    play->mainrole->flag_move = false;
    
    mTeachText->setString("在战斗开始之前，让我们花一分钟\n的时间回顾一下基础的战斗操作。");
}

void TutorialDirector::doneMove() 
{
    mTeachText->setString("向上，朝着敌人滑动手指来投出一枚飞镖。\n试着杀死这个敌人。");
    GamePlay *play = GamePlay::sharedGamePlay();
    play->count_control--;
    mDummyEnemy->waitKilled(this, callfunc_selector(TutorialDirector::doneKilled));
    play->mainrole->flag_dart = true;
    this->playSlideUp();
}

void TutorialDirector::doneKilled() 
{
    mTeachText->setString("干得好！现在我们来看看如何应对敌人的飞镖。");
    GamePlay *play = GamePlay::sharedGamePlay();
    mDummyEnemy->waitCleanup(this, callfunc_selector(TutorialDirector::doneCleanup));
    play->count_control++;
    play->mainrole->flag_dart = false;
    this->stopSlide();
}

void TutorialDirector::doneCleanup() 
{
    mDummyEnemy->reset();
    mDummyEnemy->setPos(cocos2d::Vec2( UniversalFit::sharedUniversalFit()->playSize.width+100, RESPAWN_YMIN+RESPAWN_Y/2));
    mDummyEnemy->moveTo(cocos2d::Vec2(150, RESPAWN_YMIN+RESPAWN_Y/2), this, callfunc_selector(TutorialDirector::doneMove2));
}

void TutorialDirector::doneMove2() 
{
    mTeachText->setString("用忍者刀格挡敌人的飞镖。");
    GamePlay *play = GamePlay::sharedGamePlay();
    mDummyEnemy->fireAt(play->mainrole->position(), this, callfunc_selector(TutorialDirector::doneFire));
}

void TutorialDirector::doneFire() 
{
    mTimer = 0;
    mState = 0;
}

void TutorialDirector::waitBlade() 
{
  mTeachText->setString("手指向前滑动来使用忍者刀。");
  GamePlay *play = GamePlay::sharedGamePlay();
cocos2d::CCObject* node = NULL;
CCARRAY_FOREACH(play->darts, node)
  {
    Darts *d = (Darts*)node;
    if( d->isEnemy() )
    {
      //TODO 哪来的speed
      //d->speed = 0;
    }
  }
  play->count_control--;
  play->mainrole->flag_blade = true;
  mPosx = play->mainrole->position().x;
  mState = 1;
  this->playSlideForward();
}

void TutorialDirector::doneBlade() 
{
    mTeachText->setString("漂亮！现在左右倾斜你的设备来控制人物左右移动。\n尝试利用移动来躲开敌人的飞镖。");
    GamePlay *play = GamePlay::sharedGamePlay();
    play->mainrole->flag_blade = false;
    play->mainrole->flag_move = true;
    mState = 3;
    mCount = 0;
    mTimer = 0;
    
    this->stopSlide();
    this->playShake();
}

void TutorialDirector::doneEvade() 
{
    mTeachText->setString("太棒了！现在是时候还击了。\n用飞镖杀死这些敌人。");
    GamePlay *play = GamePlay::sharedGamePlay();
    play->mainrole->flag_dart = true;
    mDummyEnemy->waitKilled(this, callfunc_selector(TutorialDirector::doneKilled2));
    this->stopShake();
    this->playSlideUp();
}

void TutorialDirector::doneKilled2() 
{
    mTeachText->setString("有的时候，杀死敌人会掉落一些\n有用的补给品。试试去接一个来。");
    GamePlay *play = GamePlay::sharedGamePlay();
    play->manager->addGameObject(Item::item(6, mDummyEnemy->center(), play, false));
    mDummyEnemy->waitCleanup(this, callfunc_selector(TutorialDirector::doneCleanup2));
}

void TutorialDirector::doneCleanup2() 
{
    GamePlay *play = GamePlay::sharedGamePlay();
    if( play->mainrole->SP != play->mainrole->maxSP )
    {
        mTeachText->setString("跑到敌人的下面，\n杀死敌人并接住掉下来的补给品。");
        if( randomInt(2) == 0 )
        {
            mDummyEnemy->setPos(cocos2d::Vec2(-100, RESPAWN_YMIN+RESPAWN_Y/2));
        }
        else {
            mDummyEnemy->setPos(cocos2d::Vec2(UniversalFit::sharedUniversalFit()->playSize.width+100, RESPAWN_YMIN+RESPAWN_Y/2));
        }
        mDummyEnemy->moveTo(cocos2d::Vec2((0.2f+CCRANDOM_0_1()*0.6f)*UniversalFit::sharedUniversalFit()->playSize.width, RESPAWN_YMIN+RESPAWN_Y/2), NULL, NULL);
        mDummyEnemy->waitKilled(this, callfunc_selector(TutorialDirector::doneKilled2));
    }
}

void TutorialDirector::doneCharge() 
{
    mTeachText->setString("好极了！你刚刚接到了一个查克拉补满的补给。\n手指向下滑动发动你的必杀技。");
    GamePlay *play = GamePlay::sharedGamePlay();
    for( int i=0; i<3; ++i)
    {
        mDummies[i]->reset();
        if( randomInt(2) == 0 )
        {
            mDummies[i]->setPos(cocos2d::Vec2(-100, RESPAWN_YMIN+RESPAWN_Y/2));
        }
        else {
            mDummies[i]->setPos(cocos2d::Vec2(UniversalFit::sharedUniversalFit()->playSize.width+100, RESPAWN_YMIN+RESPAWN_Y/2));
        }
        cocos2d::Point dst = Vec2((0.2f+0.2f*i)*UniversalFit::sharedUniversalFit()->playSize.width, RESPAWN_YMIN+RESPAWN_Y/2);
        mDummies[i]->moveTo(dst, NULL, NULL);
        mDummies[i]->waitKilled(NULL, NULL);
    }
    play->mainrole->flag_dart = false;
    play->mainrole->flag_blade = false;
    play->mainrole->setTilt(0);
    play->mainrole->flag_move = false;
    play->mainrole->flag_spell = true;
    
    this->stopSlide();
    this->playSlideDown();
}

void TutorialDirector::doneTutorial() 
{
    mTeachText->setAnchorPoint(cocos2d::Vec2(0.5f, 0.5f));
    mTeachText->setPosition(cocos2d::Vec2(UniversalFit::sharedUniversalFit()->playSize.width/2, SCREEN_HEIGHT/2));
    mTeachText->setString("恭喜，恭喜！\n你现在已经完全掌握了作为一名忍者所必须的要义。\n好好练习，充分发挥你的战斗力吧。祝你好运！");
    
    this->stopSlide();
}

void TutorialDirector::skip() 
{
    tutorialOver();
    GamePlay::sharedGamePlay()->manager->removeGameObject(this);
}

void TutorialDirector::tutorialOver() 
{
    GamePlay *play = GamePlay::sharedGamePlay();
    play->state = STATE_NORMAL;
    play->manager->removeGameObject(mDummyEnemy);
    mDummyEnemy = NULL;
    for(int i=0; i<3; ++i)
    {
        play->manager->removeGameObject(mDummies[i]);
    }
    play->enemies->removeAllObjects();
    play->mainrole->flag_dart = true;
    play->mainrole->flag_blade = true;
    play->mainrole->flag_spell = true;
    play->mainrole->flag_move = true;
    play->count_control = 0;//reset control counter
    play->count_respawn = 0;
    play->mainrole->HP = play->mainrole->maxHP;
    play->mainrole->SP = 0;
    play->mainrole->setEquipDart(cacheDartId);
    play->mainrole->setEquipBlade(cacheBladeId);
    play->mainrole->setEquipSpell(cacheSpellId);
}

void TutorialDirector::onDestroy() 
{
    GamePlay *play = GamePlay::sharedGamePlay();
    play->removeChild(mLayer, true);
    play->removeChild(mTeachText, true);
    play->removeChild(mAnimateNode, true);
    play->removeChild(mMenu, true);
cocos2d::CCLog("end tutorial");
    GameRecord::sharedGameRecord()->game_tutorial = 1;
}


