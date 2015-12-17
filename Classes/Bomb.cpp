//
//  Bomb.m
//  NinjiaRush
//
//  Created by 马 颂文 on 12-5-17.
//  Copyright (c) 2012年 TrinGame. All rights reserved.
//

#include "Bomb.h"
#include "GameConstants.h"
#include "GamePlay.h"



#define BOMB_V0 (-50.0f)
#define BOMB_G  (-400.0f)
#define BOMB_FLY (1.18f)
#define BOMB_RANGE (30)

Bomb* Bomb::bomb(cocos2d::Point pos, cocos2d::Point dir, Node * parent) 
{
    Bomb* ret = Bomb::create();
    ret->mParent = parent;
    ret->mPosition = pos;
    ret->mDirection = dir;
    return ret;
}

void Bomb::onCreate() 
{
    mSprite = GTAnimatedSprite::spriteWithGTAnimation(GTAnimation::loadedAnimationSet("bullets"));
    mSprite->playGTAnimation(1, true);
    mSprite->setAnchorPoint(cocos2d::Vec2(0.5f, 0.5f));
    mSprite->setPosition(mPosition);
    mParent->addChild(mSprite, LAYER_MAINROLE+1);
cocos2d::CCRotateBy* rb = cocos2d::CCRotateBy::create(1, 720);
cocos2d::CCRepeatForever* rf = cocos2d::CCRepeatForever::create(rb);
    mSprite->runAction(rf);
    //初始化direction
    mDirection.x = (mDirection.x - mPosition.x)/BOMB_FLY;
    SimpleAudioEngine::sharedEngine()->playEffect(cocos2d::CCFileUtils::sharedFileUtils()->fullPathForFilename("sound/bomb1.mp3").c_str());
    mTimer = 0;
}

void Bomb::onUpdate(float delta) 
{
    GamePlay* play = GamePlay::sharedGamePlay();
    //暂停和恢复动作
    if( play->paused && !mPaused )
    {
        mPaused = true;
        mSprite->pauseSchedulerAndActions();
    }
    if( mPaused && !play->paused )
    {
        mPaused = false;
        mSprite->resumeSchedulerAndActions();
    }
    mSprite->updateGTAnimation(delta);
    //飞行
    mTimer += delta;
    cocos2d::Point np;
    np.y = mPosition.y + BOMB_V0*mTimer + (BOMB_G*mTimer*mTimer)/2;
    np.x = mPosition.x + mDirection.x*mTimer;
    if ( np.y <= PLAY_PLAYERLINE-5 ) {
        //booom
        //伤害检定
        float dx = play->mainrole->position().x - np.x;
        if( BOMB_RANGE*BOMB_RANGE >= dx*dx )
        {
            play->mainrole->deliverHit(HIT_BOMB, Vec2(0, 0));
        }
        //分身术
        if( play->mainrole2 != NULL )
        {
            float dx2 = play->mainrole2->position().x - np.x;
            if( BOMB_RANGE*BOMB_RANGE >= dx2*dx2 )
            {
                play->mainrole2->deliverHit(HIT_BOMB, Vec2(0, 0));
            }
        }
        
        GTAnimatedEffect *eff = GTAnimatedEffect::create(GTAnimation::loadedAnimationSet("bullets"), 0, false);
        eff->setAnchorPoint(cocos2d::Vec2(0.5f, 0));
        eff->setPosition(np);
        mParent->addChild(eff, LAYER_ROLE);
        SimpleAudioEngine::sharedEngine()->playEffect(cocos2d::CCFileUtils::sharedFileUtils()->fullPathForFilename("sound/bomb2.mp3").c_str());
        
        play->manager->removeGameObject(this);
    }
    else {
        mSprite->setPosition(np);
    }
}

bool Bomb::isEnemy() 
{
    return true;
}

cocos2d::Point Bomb::position() 
{
    return mSprite->getPosition();
}

void Bomb::onHitback(cocos2d::Point origin) 
{
    //do nothing
}

void Bomb::onDestroy() 
{
    GamePlay *play = GamePlay::sharedGamePlay();
    play->darts->removeObject(this);
    mParent->removeChild(mSprite, true);
}


