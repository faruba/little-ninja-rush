//
//  AnimatedParticle.m
//  NinjiaRush
//
//  Created by 马 颂文 on 12-6-6.
//  Copyright (c) 2012年 TrinGame. All rights reserved.
//

#include "AnimatedParticle.h"
#include "GamePlay.h"

bool AnimatedParticle::init()
{
    if( !GameObject::init() )
    {
        return false;
    }
    //some self init code
    
    return true;
}

AnimatedParticle* AnimatedParticle::particleCoinUI(cocos2d::Point pos, int di, CCNode* parent)
{
    AnimatedParticle *ret = AnimatedParticle::create();
    ret->mDir = ccpForAngle(0.628318f*di);
    ret->mLife = 0.3f;
    ret->mSpeed = 100 + 30*CCRANDOM_0_1();
    ret->mSprite = GTAnimatedSprite::spriteWithGTAnimation(GTAnimation::loadedAnimationSet("effect"));
    ret->mSprite->retain();
    ret->mSprite->playGTAnimation(0,false);
    //位置抖动
    cocos2d::Point diter = ccp( -10 + 20*CCRANDOM_0_1(), -10 + 20*CCRANDOM_0_1() );
    ret->mSprite->setPosition(ccpAdd(pos, diter));
    ret->mStickScene = false;
    ret->mLayer = LAYER_MAINROLE+1;
    ret->mFlipX = 0;
    ret->mGravityX = -200;
    ret->mGravityY = 0;
    ret->mStickMainRole = true;
    ret->mLayer = 1;
    ret->mParent = parent;
    return ret;
}

AnimatedParticle* AnimatedParticle::particleCoin(cocos2d::Point pos, int di) 
{
    AnimatedParticle *ret = AnimatedParticle::create();
    ret->mDir = ccpForAngle(0.628318f*di);
    ret->mLife = 0.3f;
    ret->mSpeed = 100 + 30*CCRANDOM_0_1();
    ret->mSprite = GTAnimatedSprite::spriteWithGTAnimation(GTAnimation::loadedAnimationSet("effect"));
    ret->mSprite->retain();
    ret->mSprite->playGTAnimation(0, false);
    //位置抖动
    cocos2d::Point diter = ccp( -10 + 20*CCRANDOM_0_1(), -10 + 20*CCRANDOM_0_1() );
    ret->mSprite->setPosition(ccpAdd(pos, diter));
    ret->mStickScene = false;
    ret->mLayer = LAYER_MAINROLE+1;
    ret->mFlipX = 0;
    ret->mGravityX = -200;
    ret->mGravityY = 0;
    ret->mStickMainRole = true;
    ret->mLayer = LAYER_ROLE+1;
    ret->mParent = GamePlay::sharedGamePlay();
    return ret;
}

AnimatedParticle* AnimatedParticle::particleDeadSlide(cocos2d::Point pos) 
{
    AnimatedParticle *ret = AnimatedParticle::create();
    ret->mDir = ccpForAngle(cocos2d::CC_DEGREES_TO_RADIANS(150 + 30*CCRANDOM_0_1()));
    ret->mLife = 0.5f;
    ret->mSpeed = 100 + 50*CCRANDOM_0_1();
    ret->mSprite = GTAnimatedSprite::spriteWithGTAnimation(GTAnimation::loadedAnimationSet("effect"));
    ret->mSprite->retain();
    ret->mSprite->playGTAnimation(4,false);
    ret->mSprite->setPosition(pos);
    ret->mLayer = LAYER_ROLE;
    ret->mStickScene = true;
    ret->mFlipX = 0;
    ret->mGravityX = 0;
    ret->mGravityY = 0;
    ret->mLayer = LAYER_ROLE+1;
    ret->mParent = GamePlay::sharedGamePlay();
    return ret;
}

AnimatedParticle* AnimatedParticle::particleButterfly(cocos2d::Point pos) 
{
    AnimatedParticle *pat = AnimatedParticle::create();
    pat->mSprite = GTAnimatedSprite::spriteWithGTAnimation(GTAnimation::loadedAnimationSet("misc"));
    pat->mSprite->playGTAnimation(2+randomInt(4) ,true);
    pat->mSprite->setPosition(pos);
    pat->mSprite->retain();
    pat->mDir = ccpForAngle(cocos2d::CC_DEGREES_TO_RADIANS(180.0f*CCRANDOM_0_1()));
    pat->mSpeed = 100.0f + 50.0f*CCRANDOM_0_1();
    pat->mLife = 2.0f + 2.0f*CCRANDOM_0_1();
    pat->mRotate = 0;
    pat->mFade = true;
    pat->mFlipX = 0;
    pat->mGravityY = -100+200*CCRANDOM_0_1();
    pat->mGravityX = -300+200*CCRANDOM_0_1();
    pat->mStickScene = false;
    pat->mStickMainRole = false;
    pat->mLayer = LAYER_ROLE+1;
    pat->mParent = GamePlay::sharedGamePlay();
    return pat;
}

AnimatedParticle* AnimatedParticle::particleDart(cocos2d::Point pos, cocos2d::Point dir, int typ) 
{
    AnimatedParticle *pat = NULL;
    cocos2d::Point odir = ccpMult(dir, -1);
    float angle = cocos2d::CC_RADIANS_TO_DEGREES(ccpToAngle(odir));
    switch (typ) {
        case 1://金镖
        {
            pat = AnimatedParticle::create();
            pat->mDir = ccpForAngle(cocos2d::CC_DEGREES_TO_RADIANS(angle -60 + CCRANDOM_0_1()*120));
            pat->mLife = 0.3f;
            pat->mSpeed = 100 + 30*CCRANDOM_0_1();
            pat->mSprite = GTAnimatedSprite::spriteWithGTAnimation(GTAnimation::loadedAnimationSet("effect"));
            pat->mSprite->retain();
            pat->mSprite->playGTAnimation(0 ,false);
            pat->mSprite->setPosition(pos);
            pat->mStickScene = false;
            pat->mLayer = LAYER_MAINROLE+1;
            pat->mFlipX = 0;
            pat->mGravityX = -200;
            pat->mGravityY = 0;
            pat->mStickMainRole = true;
        }
            break;
        case 2://蝴蝶镖
        {
            pat = AnimatedParticle::create();
            pat->mSprite = GTAnimatedSprite::spriteWithGTAnimation(GTAnimation::loadedAnimationSet("misc"));
            pat->mSprite->playGTAnimation(2+randomInt(4) ,true);
            pat->mSprite->setPosition(pos);
            pat->mSprite->retain();
            pat->mDir = ccpForAngle(cocos2d::CC_DEGREES_TO_RADIANS(angle -30 + CCRANDOM_0_1()*60));
            pat->mSpeed = 100.0f + 50.0f*CCRANDOM_0_1();
            pat->mLife = 1.0f + 1.0f*CCRANDOM_0_1();
            pat->mRotate = 0;
            pat->mFade = true;
            pat->mFlipX = 0;
            pat->mGravityY = -100+200*CCRANDOM_0_1();
            pat->mGravityX = -300+200*CCRANDOM_0_1();
            pat->mStickScene = false;
            pat->mStickMainRole = false;
        }
            break;
    }
    pat->mLayer = LAYER_ROLE+1;
    pat->mParent = GamePlay::sharedGamePlay();
    return pat;
}

AnimatedParticle* AnimatedParticle::particleStepWater(cocos2d::Point pos) 
{
    AnimatedParticle *pat = AnimatedParticle::create();
    pat->mSprite = GTAnimatedSprite::spriteWithGTAnimation(GTAnimation::loadedAnimationSet("misc"));
    pat->mSprite->setAnchorPoint(cocos2d::ccp(0.5f, 0.25f));
    pat->mSprite->playGTAnimation(6 ,false);
    pat->mSprite->setPosition(pos);
    pat->mSprite->retain();
    pat->mDir = ccp(0, 0);
    pat->mSpeed = 0;
    pat->mLife = 0.35f;
    pat->mRotate = 0;
    pat->mFade = true;
    pat->mFlipX = 0;
    pat->mGravityY = 0;
    pat->mGravityX = 0;
    pat->mStickScene = true;
    pat->mStickMainRole = false;
    pat->mLayer = LAYER_ROLE-1;
    pat->mParent = GamePlay::sharedGamePlay();
    return pat;
}

void AnimatedParticle::onCreate() 
{
    GamePlay *play = GamePlay::sharedGamePlay();
    mParent->addChild(mSprite, mLayer);
    
    mPos = mSprite->getPosition();
    mMainRoleX = play->mainrole->position().x;
    mTimer = 0;
}

void AnimatedParticle::onUpdate(float delta) 
{
    GamePlay *play = GamePlay::sharedGamePlay();
    mSprite->updateGTAnimation(delta);
    mTimer += delta;
    if( mTimer > mLife )
    {
        play->manager->removeGameObject(this);
    }
    float k = mTimer/mLife;
    float r = 1.0f - k;
    //move
    float step = mSpeed*mTimer;
    cocos2d::Point np = ccpAdd(mPos, ccpMult(mDir, step));
    //gravity
    if( mGravityY != 0 )
    {
        float fy = mGravityY*mTimer*mTimer/2;
        np.y += fy;
    }
    if( mGravityX != 0 )
    {
        float fy = mGravityX*mTimer*mTimer/2;
        np.x += fy;
    }
    //stick
    if( mStickScene )
    {
        float dx = play->runspeed*mTimer;
        np.x -= dx;
    }
    if( mStickMainRole )
    {
        float dx = play->mainrole->position().x - mMainRoleX;
        np.x += dx;
    }
    mSprite->setPosition(np);
    //rotate
    if( mRotate != 0 )
    {
        mSprite->setRotation(mSprite->getRotation()+mRotate*delta);
    }
    //fade
    if( mFade )
    {
        mSprite->setOpacity(255*r);
    }
    //flip
    if( mFlipX != 0 )
    {
        int n = mTimer/mFlipX;
        float a = mTimer-mFlipX*n;
        float b = a - mFlipX/2;
        if( a < mFlipX/2 )
        {
            b = mFlipX/2 - a;
        }
        mSprite->setScaleX(b/(mFlipX/2)*2-1);
    }
}

void AnimatedParticle::onDestroy() 
{
    mSprite->release();
    mParent->removeChild(mSprite, true);
}



