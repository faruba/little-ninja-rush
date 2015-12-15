#ifndef ___CLASSES_ANIMATEDPARTICLE_H_
#define ___CLASSES_ANIMATEDPARTICLE_H_
//
//  AnimatedParticle.h
//  NinjiaRush
//
//  Created by 马 颂文 on 12-6-6.
//  Copyright (c) 2012年 TrinGame. All rights reserved.
//

/*** TRANSLATED BY HAMMER @2013-3-25 ***/

#include "cocos2d.h"
#include "GameTool.h"

using namespace cocos2d;

class AnimatedParticle:public GameObject
{
  private:
    CCNode *mParent;
    GTAnimatedSprite *mSprite;
    CCPoint mDir;
    CCPoint mPos;
    float mSpeed;
    float mLife;
    float mRotate;
    float mFlipX;
    float mGravityY;
    float mGravityX;
    bool mFade;
    bool mStickScene;
    bool mStickMainRole;

    float mMainRoleX;
    int mLayer;
    float mTimer;

  public:

    static AnimatedParticle* particleCoinUI(CCPoint pos, int di, CCNode* parent);
    static AnimatedParticle* particleCoin(CCPoint pos, int di);
    static AnimatedParticle* particleDeadSlide(CCPoint pos);
    static AnimatedParticle* particleButterfly(CCPoint pos);
    static AnimatedParticle* particleDart(CCPoint pos, CCPoint dir, int typ);
    static AnimatedParticle* particleStepWater(CCPoint pos);

    bool init();
    CREATE_FUNC(AnimatedParticle);

    void onCreate();
    void onUpdate(float delta);
    void onDestroy();
};

#endif
