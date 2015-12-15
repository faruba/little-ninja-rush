#ifndef ___CLASSES_SHADOWKILLING_H_
#define ___CLASSES_SHADOWKILLING_H_
//
//  ShaodwKilling.h
//  NinjiaRush
//
//  Created by 马 颂文 on 12-6-18.
//  Copyright (c) 2012年 TrinGame. All rights reserved.
//

#include "GameTool.h"
#include "GameObject.h"
#include "Role.h"

class ShadowKilling:
  public GameObject
{
  public:
    CREATE_FUNC(ShadowKilling);
    virtual bool init(){return true;};
    static ShadowKilling* shadowKilling(cocos2d::CCNode* parent);

    void seekTarget();
    virtual void onCreate();
    virtual void onUpdate(float delta);
    virtual void onDestroy();

    int mState;
    Role *mTarget;
    GTAnimatedSprite *mRole;
    GTGhostShadow *mShadow;
    float mKillTimer;
    float mDirection;
    float mTargetPos;
    float mMoveDis;
    int mSteak;
cocos2d::CCNode *mParent;

    int mCount;


};
#endif
