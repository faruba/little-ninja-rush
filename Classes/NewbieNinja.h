#ifndef ___CLASSES_NEWBIENINJA_H_
#define ___CLASSES_NEWBIENINJA_H_
//
//  NewbieNinja.h
//  NinjiaRush
//
//  Created by 马 颂文 on 12-4-16.
//  Copyright (c) 2012年 TrinGame. All rights reserved.
//

#include "GameTool.h"
#include "Role.h"

class NewbieNinja:
  public Role
{
  public:
    CREATE_FUNC(NewbieNinja);

    virtual const char* animationSetName() { return "enemy"; }

    virtual void onCreate();
    virtual void onUpdate(float delta);
    virtual void onDestroy();

    virtual bool deliverHit(int type, cocos2d::Point dir);
    virtual cocos2d::Point center();
    virtual bool supportAimAid();

    float  mTimer;
    int    mDartCount;
    float  mTargetPos;
    bool   mFlag;
    float  mSpeed;

    float mStepSnow;


};
#endif
