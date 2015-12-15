//
//  TraceDart.m
//  NinjiaRush
//
//  Created by 马 颂文 on 12-6-25.
//  Copyright (c) 2012年 TrinGame. All rights reserved.
//

#include "TraceDart.h"
#include "GamePlay.h"
#include "UniversalFit.h"



TraceDart* TraceDart::dart(CCPoint pos, CCPoint dir) 
{
    TraceDart *ret = TraceDart::create();
    ret->mPos = pos;
    ret->mDir = dir;
    return ret;
}

void TraceDart::onCreate() 
{
    mSprite = CCSprite::createWithSpriteFrameName("xfshuriken.png");
    mSprite->setAnchorPoint(ccp(0.5f, 0.5f));
    mSprite->setPosition(mPos);
    CCRotateBy* rb = CCRotateBy::create(0.5f, 720);
    CCRepeatForever* rf = CCRepeatForever::create(rb);
    mSprite->runAction(rf);
    GamePlay::sharedGamePlay()->addChild(mSprite, LAYER_MAINROLE+1);
    
    mTraceTimer = 10;//追踪10秒
    mPaused = false;
    
    mTrace.index = -1;
    mTrace.uid = -1;
}

void TraceDart::onUpdate(float delta) 
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
    //计时
    mTraceTimer -= delta;
    if( mTraceTimer >= 0 )
    {
        Role* traced = (Role *)play->manager->gameObjectWithHandler(mTrace);
        //寻找目标
        if( traced == NULL || !traced->supportAimAid() )
        {
            traced = (Role*)play->nearestEnemy(mDir);
            if( traced == NULL )
            {
                mTrace.index = -1;
                mTrace.uid = -1;
            }
            else {
                mTrace = traced->handler();
            }
        }
        //跟踪
        if( traced != NULL )
        {
            CCPoint rdir = ccpSub(traced->center(), mSprite->getPosition());
            float angle = ccpAngleSigned(mDir, rdir);
            if( angle == angle )
            {
                float limit = CC_DEGREES_TO_RADIANS(180.0f*delta);
                if( fabsf(angle) <= limit )
                {
                    mDir = ccpNormalize(rdir);
                }
                else {
                    float oa = ccpToAngle(mDir);
                    if( angle > 0 )
                    {
                        oa += limit;
                    }
                    else {
                        oa -= limit;
                    }
                    mDir = ccpForAngle(oa);
                }
            }
        }
    }
    //飞行
    CCPoint ds = ccpMult(mDir, 400.0f*delta);
    CCPoint np = ccpAdd(ds, mSprite->getPosition());
    mSprite->setPosition(np);
    //伤害判定
  CCObject* node = NULL;
  CCARRAY_FOREACH(play->enemies, node)
  {
    Role * em = (Role*)node;
        if( em->collisionWithCircle(mSprite->getPosition(), 20) )//杀伤半径
        {
            bool hit = em->deliverHit(HIT_MAGIC, mDir);
            if( hit )
            {
                SimpleAudioEngine::sharedEngine()->playEffect(CCFileUtils::sharedFileUtils()->fullPathForFilename("sound/hit.mp3").c_str());
                GTAnimatedEffect *hiteff = GTAnimatedEffect::create(GTAnimation::loadedAnimationSet("effect"), 1, false);
                hiteff->setAnchorPoint(ccp(0.5f, 0.5f));
                hiteff->setPosition(em->center());
                //hiteff->setRotation(90 - CC_RADIANS_TO_DEGREES( ccpToAngle(direction) ));
                hiteff->setRotation(60 - CC_RADIANS_TO_DEGREES( ccpToAngle(mDir) ) + 60*CCRANDOM_0_1());
                play->addChild(hiteff, LAYER_MAINROLE+1);
                
                GTAnimatedEffect *hiteff2 = GTAnimatedEffect::create(GTAnimation::loadedAnimationSet("effect"), 2, false);
                hiteff2->setAnchorPoint(ccp(0.5f, 0.5f));
                hiteff2->setPosition(em->center());
                play->addChild(hiteff2, LAYER_ROLE);
            }
            break;
        }
    }
    //越界判定
    if( mTraceTimer < 0 )
    {
        if( mSprite->getPosition().x < -100 || mSprite->getPosition().x > UniversalFit::sharedUniversalFit()->playSize.width+100
           || mSprite->getPosition().y < -100 || mSprite->getPosition().y > SCREEN_HEIGHT+100 )
        {
            play->manager->removeGameObject(this);
        }
    }
}

void TraceDart::onDestroy() 
{
    GamePlay *play = GamePlay::sharedGamePlay();
    play->removeChild(mSprite, true);
}


