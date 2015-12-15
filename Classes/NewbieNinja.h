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

    virtual void onCreate();
    virtual void onUpdate(float delta);
    virtual void onDestroy();

    virtual bool collisionWithCircle(cocos2d::Point cc, float rad);
    virtual bool deliverHit(int type, cocos2d::Point dir);
    virtual cocos2d::Point position();
    virtual void setPosition(cocos2d::Point pos);
    virtual cocos2d::Point center();
    virtual bool supportAimAid();
    virtual void toggleVisible(bool flag);

    static NewbieNinja* role(cocos2d::CCNode* parent);

    CCNode *mParent;
    GTAnimatedSprite *mSprite;
    int    mState;
    float  mTimer;
    int    mDartCount;
    float  mTargetPos;
    bool   mFlag;
    float  mSpeed;

    float mStepSnow;


};
#endif
