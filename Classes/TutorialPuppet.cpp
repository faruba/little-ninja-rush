//
//  TutorialPuppet.m
//  NinjiaRush
//
//  Created by 马 颂文 on 12-7-4.
//  Copyright (c) 2012年 TrinGame. All rights reserved.
//

#include "TutorialPuppet.h"
#include "GamePlay.h"
#include "GameConstants.h"
#include "Dart.h"
#include "SimpleAudioEngine.h"



TutorialPuppet* TutorialPuppet::puppet() 
{
  return TutorialPuppet::create();
}

void TutorialPuppet::reset() 
{
  if( mSprite != NULL )
  {
    mSprite->setPosition(ccp(-100, RESPAWN_YMIN+RESPAWN_Y/2));
    mSprite->playGTAnimation(0, true);
  }
  mState = 0;
  mCallback = NULL;
  mKillable = false;
}

void TutorialPuppet::setPos(CCPoint pos) 
{
  mSprite->setPosition(pos);
}

void TutorialPuppet::moveTo(CCPoint pos, GameObject* tar, SEL_CallFunc sel) 
{
  mTarget = pos;
  mCallback = tar;
  mSelector = sel;
  mState = 1;
}

void TutorialPuppet::fireAt(CCPoint pos, GameObject* tar, SEL_CallFunc sel) 
{

  mTarget = ccpNormalize(ccpSub(pos, this->center()));
  mCallback = tar;
  mSelector = sel;
  mState = 2;
  mTimer = 0;

  mSprite->playGTAnimation(6, true);
  //play effect
  GTAnimatedEffect *eff = GTAnimatedEffect::create(GTAnimation::loadedAnimationSet("effect"), 7, false);
  eff->setPosition(ccp(47, 19));
  mSprite->addChild(eff);
}

void TutorialPuppet::waitKilled(GameObject* tar, SEL_CallFunc sel) 
{
  mKillable = true;
  mKillCallback = tar;
  mKillSelector = sel;
}

void TutorialPuppet::waitCleanup(GameObject* tar, SEL_CallFunc sel) 
{
  mCleanCallback = tar;
  mCleanSelector = sel;
}

void TutorialPuppet::onCreate() 
{
  GamePlay *play = GamePlay::sharedGamePlay();
  mSprite = GTAnimatedSprite::spriteWithGTAnimation(GTAnimation::loadedAnimationSet("enemy"));
  mSprite->setAnchorPoint(ccp(0.4f, 0.0625f));
  play->addChild(mSprite, LAYER_ROLE+RESPAWN_Y/2);

  this->reset();
}

void TutorialPuppet::onUpdate(float delta) 
{
  GamePlay *play = GamePlay::sharedGamePlay();
  bool playend = mSprite->updateGTAnimation(delta);
  switch (mState) {
    case 1://move to
      {
        float step = delta*ENEMY_NNRUNSPEED;
        CCPoint dis = ccpSub(mTarget, mSprite->getPosition());
        if( ccpLengthSQ(dis) <= step*step )
        {
          mSprite->setPosition(mTarget);
          mState = 0;
          if( mCallback != NULL )
          {
            GameObject* calltarget = mCallback;
            mCallback = NULL;
            CCCallFunc *callSelectorAction = CCCallFunc::create(calltarget, mSelector);
            GamePlay::sharedGamePlay()->runAction(callSelectorAction);
          }
        }
        else {
          CCPoint dir = ccpNormalize(dis);
          CCPoint np = ccpAdd(mSprite->getPosition(), ccpMult(dir, step));
          mSprite->setPosition(np);
        }
      }
      break;
    case 2://fire
      {
        mTimer += delta;
        if( mSprite->animationId() == 6 )
        {
          if( mTimer > 1 )
          {
            mTimer = 0;
            mSprite->playGTAnimation(5, false);
            play->darts->addObject(play->manager->addGameObject(Dart::dart(CCString::create("dart.png"), this->center(), mTarget, -1, play)));
            mState = 0;
            if( mCallback != NULL )
            {
              GameObject* calltarget = mCallback;
              mCallback = NULL;
              CCCallFunc *callSelectorAction = CCCallFunc::create(calltarget, mSelector);
              GamePlay::sharedGamePlay()->runAction(callSelectorAction);
            }
          }
        }
      }
      break;
    case 3://dead
      {
        if( playend )
        {
          //fix pos
          float ds = delta*play->runspeed;
          CCPoint np = mSprite->getPosition();
          np.x -= ds;
          mSprite->setPosition(np);

          if( mSprite->getPosition().x < -100 )
          {
            mState = 0;
            if( mCleanCallback != NULL )
            {
              GameObject* calltarget = mCleanCallback;
              mCleanCallback = NULL;
              CCCallFunc *callSelectorAction = CCCallFunc::create(calltarget, mCleanSelector);
              GamePlay::sharedGamePlay()->runAction(callSelectorAction);
            }
          }
          playend = false;
        }
        else {
          mTimer += delta;
          if(mTimer>0.3f && !mFlag)
          {
            int n = 1 + randomInt(3);
            SimpleAudioEngine::sharedEngine()->playEffect(CCString::createWithFormat("ahh%d.mp3", n)->getCString());
            mFlag = true;
          }
          //fix pos
          CCPoint np = mSprite->getPosition();
          float ra = mTimer/mSprite->playBackTime();
          if( ra > 1 )
          {
            ra = 1;
          }
          ra = 1 - ra;
          float ds = delta*200*ra;
          np.x += ds;
          mSprite->setPosition(np);
        }
      }
      break;
  }
  if( playend )
  {
    mSprite->playGTAnimation(0, true);
  }
}

void TutorialPuppet::onDestroy() 
{
  GamePlay *play = GamePlay::sharedGamePlay();
  play->removeChild(mSprite, true);
}

bool TutorialPuppet::collisionWithCircle(CCPoint cc, float rad) 
{
  if( mState == 5 )
  {
    return false;
  }
  else {
    if( exCollisionWithCircles(mSprite->getPosition(), 6, 12, 9, cc, rad) ||
        exCollisionWithCircles(mSprite->getPosition(), 17, 27, 13, cc, rad) )
    {
      return true;
    }
  }  
  return false;
}

CCPoint TutorialPuppet::center() 
{
  return ccpAdd(mSprite->getPosition(), ccp(9, 20));
}

CCPoint TutorialPuppet::position() 
{
  return mSprite->getPosition();
}

void TutorialPuppet::toggleVisible(bool flag) 
{
  mSprite->setVisible(flag);
}

void TutorialPuppet::setPosition(CCPoint pos) 
{
  mSprite->setPosition(pos);
}

bool TutorialPuppet::deliverHit(int type, CCPoint dir) 
{
  if( mKillable )
  {
    mSprite->playGTAnimation(3, false);
    mTimer = 0;
    mFlag = false;
    mState = 3;
    if( mKillCallback != NULL )
    {
      GameObject* calltarget = mKillCallback;
      mKillCallback = NULL;
      CCCallFunc *callSelectorAction = CCCallFunc::create(calltarget, mKillSelector);
      GamePlay::sharedGamePlay()->runAction(callSelectorAction);
      mKillable = false;
    }

    return true;
  }
  return false;
}

bool TutorialPuppet::supportAimAid() 
{
  if( mState != 3 )
  {
    return true;
  }
  return false;
}


