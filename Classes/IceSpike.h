#ifndef ___CLASSES_ICESPIKE_H_
#define ___CLASSES_ICESPIKE_H_
//
//  IceSpike.h
//  NinjiaRush
//
//  Created by 马 颂文 on 12-5-18.
//  Copyright (c) 2012年 TrinGame. All rights reserved.
//

#include "GameTool.h"
#include "Role.h"

class IceSpike:
  public Role
{
  public:
    static IceSpike* spike(cocos2d::Point pos, CCNode* parent);
    CREATE_FUNC(IceSpike);

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
cocos2d::CCNode *mParent;

    GTAnimatedSprite *mSprite;
    cocos2d::Point mPos;

    int mState;
    bool mFlag;


};
#endif
