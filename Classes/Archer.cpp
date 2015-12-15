//
//  Archer.m
//  NinjiaRush
//
//  Created by 马 颂文 on 12-6-11.
//  Copyright (c) 2012年 TrinGame. All rights reserved.
//

#include "Archer.h"
#include "GameConstants.h"
#include "GamePlay.h"
#include "SimpleAudioEngine.h"
#include "GameRecord.h"
#include "UniversalFit.h"



#define LOCKON_TIME (0.5f) //锁定时间
#define AIM_TIME (5) //瞄准时间
#define AIM_SPEED (80) //瞄准速度
#define AIM_ACCURATE (1) //瞄准精度
#define ARROW_HEIGHT (SCREEN_HEIGHT+60) //箭矢高度
#define ARROW_TIME (0.5f) //箭矢飞行的时间

Archer* Archer::role(CCNode * parent) 
{
    Archer *ret = Archer::create();
    ret->mParent = parent;
    return ret;
}

void Archer::onCreate() 
{
    mTarget = ccp(UniversalFit::sharedUniversalFit()->playSize.width/2, PLAY_PLAYERLINE);
    mTimer = 0;
    mState = 0;
    mLockTimer = 0;
    mAvaiable = true;
    
    mArrow = GTAnimatedSprite::spriteWithGTAnimation(GTAnimation::loadedAnimationSet("archer"));
    mArrow->playGTAnimation(0, false);
    mArrow->setPosition(mTarget);
    mParent->addChild(mArrow, LAYER_ROLE);
    SimpleAudioEngine::sharedEngine()->playEffect(CCFileUtils::sharedFileUtils()->fullPathForFilename("sound/shoot ready.mp3").c_str());
}

void Archer::onUpdate(float delta) 
{
    GamePlay *play = GamePlay::sharedGamePlay();
    bool playend = mArrow->updateGTAnimation(delta);
    switch (mState) {
        case 0://aim
        {
            if( playend )
            {
                mArrow->playGTAnimation(1, true);
            }
            bool lock = false;
            CCPoint pos = play->mainrole->position();
            if( play->mainrole2 != NULL)
            {//瞄准离它最近的一个
                float dis = pos.x - mTarget.x;
                float dis2 = play->mainrole2->center().x - mTarget.x;
                if( fabsf(dis2) < fabsf(dis) )
                {
                    pos = play->mainrole2->position();
                }
            }
            mTimer += delta;
            float dir = pos.x - mTarget.x;
            float len = fabsf(dir);
            if( len > AIM_ACCURATE )
            {
                float ds = AIM_SPEED*delta;
                if( len <= ds*ds )
                {
                    mTarget = pos;
                }
                else {
                    dir = dir/len;
                    mTarget.x = mTarget.x + dir*ds;
                }
                mLockTimer = 0;
            }
            else {
                mLockTimer += delta;
                if( mLockTimer >= LOCKON_TIME )
                {
                    lock = true;
                }
            }
            mArrow->setPosition(mTarget);
            if( mTimer >= AIM_TIME )
            {
                lock = true;
            }
            if( lock )
            {
                mArrow->playGTAnimation(2, true);
                mState = 1;
                mTimer = 0;
                if( mTarget.x < UniversalFit::sharedUniversalFit()->playSize.width/2 )
                {
                    mFrom = ccp(UniversalFit::sharedUniversalFit()->playSize.width, ARROW_HEIGHT);
                }
                else {
                    mFrom = ccp(0, ARROW_HEIGHT);
                }
                mMidPoint = ccpMidpoint(mFrom, mTarget);
                mMidPoint.y += 75 + CCRANDOM_0_1()*100;
                SimpleAudioEngine::sharedEngine()->playEffect(CCFileUtils::sharedFileUtils()->fullPathForFilename("sound/lock on.mp3").c_str());
            }
        }
            break;
        case 1://lock
        {
            mTimer += delta;
            if( mTimer > 1 && mArrow->animationId()==2 )
            {
                mTimer = 0;
                mArrow->playGTAnimation(3, false);
            }
            if( playend && mArrow->animationId()==3 )
            {
                mTimer = 0;
                mArrow->playGTAnimation(4, true);
                mArrow->setAnchorPoint(ccp( 0.5f, 1));
                mArrow->setPosition(mFrom);
                mState = 2;
                play->darts->addObject(this);
                SimpleAudioEngine::sharedEngine()->playEffect(CCFileUtils::sharedFileUtils()->fullPathForFilename("sound/shooting.mp3").c_str());
            }
        }
            break;
        case 2://shoot
        {
            mTimer += delta;
            if( mTimer < ARROW_TIME )
            {
                float k = mTimer/ARROW_TIME;
                CCPoint a = ccpLerp(mFrom, mMidPoint, k);
                CCPoint b = ccpLerp(mMidPoint, mTarget, k);
                CCPoint c = ccpLerp(a, b, k);
                CCPoint dir = ccpSub(c, mArrow->getPosition());
                //计算角度
                float rot = 90 - CC_RADIANS_TO_DEGREES(ccpToAngle(dir));
                mArrow->setRotation(rot);
                mArrow->setPosition(c);
                //伤害判定
                if( mAvaiable )
                {
                    bool hit = false;
                    CCPoint pos;
                    if( play->mainrole->collisionWithCircle(mArrow->getPosition(), 5) )
                    {
                        hit = play->mainrole->deliverHit(HIT_ARCHER, dir);
                        play->manager->removeGameObject(this);
                        pos = play->mainrole->center();
                    }
                    //分身术
                    if( play->mainrole2 != NULL && play->mainrole2->collisionWithCircle(mArrow->getPosition(), 5) )
                    {
                        hit = play->mainrole2->deliverHit(HIT_ARCHER, dir);
                        play->manager->removeGameObject(this);
                        pos = play->mainrole2->center();
                    }
                    if( hit )
                    {
                        SimpleAudioEngine::sharedEngine()->playEffect(CCFileUtils::sharedFileUtils()->fullPathForFilename("sound/hit.mp3").c_str());
                        GTAnimatedEffect *hiteff = GTAnimatedEffect::create(GTAnimation::loadedAnimationSet("effect"), 1, false);
                        hiteff->setAnchorPoint(ccp(0.5f, 0.5f));
                        hiteff->setPosition(pos);
                        hiteff->setRotation(60 - CC_RADIANS_TO_DEGREES( ccpToAngle(dir) ) + 60*CCRANDOM_0_1());
                        mParent->addChild(hiteff, LAYER_MAINROLE+1);
                        
                        GTAnimatedEffect *hiteff2 = GTAnimatedEffect::create(GTAnimation::loadedAnimationSet("effect"), 2, false);
                        hiteff2->setAnchorPoint(ccp(0.5f, 0.5f));
                        hiteff2->setPosition(pos);
                        mParent->addChild(hiteff2, LAYER_ROLE);
                    }
                }
            }
            else {
                mState = 3;
                mArrow->setPosition(mTarget);
                mArrow->playGTAnimation(5, true);
                mArrow->setAnchorPoint(ccp(0.5f, 1));
            }
        }
            break;
        case 3:
        {
            CCPoint np = mArrow->getPosition();
            float ds = play->runspeed*delta;
            np.x -= ds;
            mArrow->setPosition(np);
            
            //越界清除
            if( mArrow->getPosition().x < -100 )
            {
                play->manager->removeGameObject(this);
            }
        }
            break;
    }
}

bool Archer::isEnemy() 
{
    return true;
}

CCPoint Archer::position() 
{
    return mArrow->getPosition();
}

void Archer::setPosition(CCPoint pos) 
{
    mArrow->setPosition(pos);
}

void Archer::onHitback(CCPoint origin) 
{
    mAvaiable = false;
    mArrow->setVisible(false);
    GameRecord::sharedGameRecord()->task->dispatchTask(ACH_SLICEARROW, 1);
    
    GamePlay *play = GamePlay::sharedGamePlay();
    // arcade combo
    if( play->mode == MODE_ARCADE )
    {
        play->arcade->addScore(ARCHER_SCORE, mArrow->getPosition());
    }
    
}

void Archer::toggleVisible(bool flag) 
{
    mArrow->setVisible(flag);
}

void Archer::onDestroy() 
{
    mParent->removeChild(mArrow, true);
    GamePlay::sharedGamePlay()->darts->removeObject(this);
}


