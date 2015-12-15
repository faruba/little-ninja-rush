//
//  TimedTrigger.m
//  NinjiaRush
//
//  Created by 马 颂文 on 12-5-23.
//  Copyright (c) 2012年 TrinGame. All rights reserved.
//

#include "TimedTrigger.h"
#include "GamePlay.h"



TimedTrigger* TimedTrigger::triggerWithDelay(cocos2d::CCNode*target, SEL_CallFunc selector, float delay) 
{
    TimedTrigger *ret = TimedTrigger::create();
    ret->mTarget = target;
    ret->mSelector = selector;
    ret->mMode = 0;
    ret->mTime = delay;
    return ret;
}

TimedTrigger* TimedTrigger::triggerWithCycle(cocos2d::CCNode*target, SEL_CallFunc selector, float cycle, int times) 
{
    TimedTrigger *ret = TimedTrigger::create();
    ret->mTarget = target;
    ret->mSelector = selector;
    ret->mMode = times;
    ret->mTime = cycle;
    return ret;
}

void TimedTrigger::onCreate() 
{
    mTimer = 0;
}

void TimedTrigger::onUpdate(float delta) 
{
    mTimer += delta;
    if( mTimer >= mTime )
    {
cocos2d::CCCallFunc *callSelectorAction = cocos2d::CCCallFunc::create(mTarget, mSelector);
      GamePlay::sharedGamePlay()->runAction(callSelectorAction);
      mTimer -= mTime;
      if( mMode >= 0 )
      {
        mMode--;
        if(mMode<0)
        {
          GamePlay::sharedGamePlay()->manager->removeGameObject(this);
        }
      }
    }
}

void TimedTrigger::onDestroy() 
{
     
}


