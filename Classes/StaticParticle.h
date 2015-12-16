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
cocos2d::Sprite *mSprite;
    cocos2d::Point mDir;
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

    static StaticParticle* particleSakura(cocos2d::Point pos);
    static StaticParticle* particleIce(cocos2d::Point pos, bool up);
    static StaticParticle* particleDart(cocos2d::Point pos, cocos2d::Point dir, int typ);
    static StaticParticle* particleShadow(cocos2d::Point pos);
    static StaticParticle* particleIceAir(cocos2d::Point pos, bool up);
    static StaticParticle* particleIcePiece(cocos2d::Point pos, cocos2d::Point dir);
    static StaticParticle* particleAddHP(cocos2d::Point pos);
    static StaticParticle* particleDash(cocos2d::Point pos);
    static StaticParticle* particlePumpkinTail(cocos2d::Point pos, int typ);

    static StaticParticle* particleStepDust(cocos2d::Point pos);

};
#endif
