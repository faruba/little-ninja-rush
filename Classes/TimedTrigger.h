#ifndef ___CLASSES_TIMEDTRIGGER_H_
#define ___CLASSES_TIMEDTRIGGER_H_
//
//  TimedTrigger.h
//  NinjiaRush
//
//  Created by 马 颂文 on 12-5-23.
//  Copyright (c) 2012年 TrinGame. All rights reserved.
//

#include "GameTool.h"

class TimedTrigger:
  public GameObject
{
  public:
    CREATE_FUNC(TimedTrigger);
    static TimedTrigger* triggerWithDelay(cocos2d::CCNode* target, SEL_CallFunc selector, float delay);
    static TimedTrigger* triggerWithCycle(cocos2d::CCNode* target, SEL_CallFunc selector, float cycle, int times);

    virtual void onCreate();
    virtual void onUpdate(float delta);
    virtual void onDestroy();

    CCNode* mTarget;
    SEL_CallFunc mSelector;
    int mMode;
    float mTime;
    float mTimer;
};
#endif
