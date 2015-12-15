#ifndef ___CLASSES_STATICPARTICLE_H_
#define ___CLASSES_STATICPARTICLE_H_
//
//  StaticParticle.h
//  NinjiaRush
//
//  Created by 马 颂文 on 12-7-11.
//  Copyright (c) 2012年 TrinGame. All rights reserved.
//

#include "GameTool.h"

class StaticParticle:public GameObject{
  public:
    CREATE_FUNC(StaticParticle);
    virtual bool init(){return true;};

    virtual void onCreate();
    virtual void onUpdate(float delta);
    virtual void onDestroy();

    CCSprite *mSprite;
    CCPoint mDir;
    float mSpeed;
    float mLife;
    float mRotate;
    float mFlipX;
    float mGravityY;
    float mGravityX;
    bool mFade;
    bool mShurink;
    bool mStick;

    float mTimer;

    static StaticParticle* particleSakura(CCPoint pos);
    static StaticParticle* particleIce(CCPoint pos, bool up);
    static StaticParticle* particleDart(CCPoint pos, CCPoint dir, int typ);
    static StaticParticle* particleShadow(CCPoint pos);
    static StaticParticle* particleIceAir(CCPoint pos, bool up);
    static StaticParticle* particleIcePiece(CCPoint pos, CCPoint dir);
    static StaticParticle* particleAddHP(CCPoint pos);
    static StaticParticle* particleDash(CCPoint pos);
    static StaticParticle* particlePumpkinTail(CCPoint pos, int typ);

    static StaticParticle* particleStepDust(CCPoint pos);

};
#endif
