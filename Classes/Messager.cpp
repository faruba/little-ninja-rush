//
//  Messager.m
//  NinjiaRush
//
//  Created by 马 颂文 on 12-5-3.
//  Copyright (c) 2012年 TrinGame. All rights reserved.
//

#include "Messager.h"
#include "GameConstants.h"
#include "GamePlay.h"
#include "Item.h"
#include "GameRecord.h"
#include "UniversalFit.h"
#include "FootPrint.h"

//摔死的概率(%)
#define FALLDOWN (15)
//追上的速度
#define CHASE_SPEED (150.0f)
//掉队的速度
#define DROPOUT_SPEED (-50.0f)
//加速速度
#define ACCELER_SPEED (250.0f)


Messager* Messager::role(cocos2d::Node * parent) 
{
    Messager *ret = Messager::create();
    ret->mParent = parent;
    return  ret;
}

void Messager::onCreate() 
{
    mSprite = GTAnimatedSprite::spriteWithGTAnimation(GTAnimation::loadedAnimationSet("messager"));
    mSprite->setAnchorPoint(cocos2d::Vec2(0.5438f, 0.0625f));
    int y = CCRANDOM_0_1()*RESPAWN_Y;
    mSprite->setPosition(cocos2d::Vec2(-80, RESPAWN_YMIN+y));
    mParent->addChild(mSprite, LAYER_ROLE+RESPAWN_Y-y);
    
    //预设模式
    int mode = randomInt(2);
    if( mode == 0 )//chase mode
    {
        mSprite->playGTAnimation(1, true);
        mSprite->setPosition(cocos2d::Vec2( -80, RESPAWN_YMIN+y));
        mSpeed = CHASE_SPEED;
        mState = 0;
    }
    else {//drop out mode
        mSprite->playGTAnimation(0, true);
        mSprite->setPosition(cocos2d::Vec2( 80+UniversalFit::sharedUniversalFit()->playSize.width, RESPAWN_YMIN+y));
        mState = 1;
        mSpeed = DROPOUT_SPEED;
        mAwake = 0.4f*UniversalFit::sharedUniversalFit()->playSize.width + 0.3f*CCRANDOM_0_1()*UniversalFit::sharedUniversalFit()->playSize.width;
    }
    if( randomInt(100) < FALLDOWN )
    {
        mFlagFalldown = true;
        mFalldown = 0.1f*UniversalFit::sharedUniversalFit()->playSize.width + 0.8f*CCRANDOM_0_1()*UniversalFit::sharedUniversalFit()->playSize.width;
    }
    else {
        mFlagFalldown = false;
        mFalldown = 0;
    }
}

void Messager::onUpdate(float delta) 
{
    bool playend = mSprite->updateGTAnimation(delta);
    GamePlay *play = GamePlay::sharedGamePlay();
    
    if( mState < 3 && play->gameOverTimer >= 0 )
    {//主角死亡的处理
        float ds = delta*(play->levelspeed - play->runspeed);
        cocos2d::Point np = mSprite->getPosition();
        np.x += ds;
        mSprite->setPosition(np);
    }
    else {
        switch (mState) {
            case 0://chase
            {
                cocos2d::Point np = mSprite->getPosition();
                np.x += delta*mSpeed;
                mSprite->setPosition(np);
                if( mFlagFalldown && mSprite->getPosition().x > mFalldown )
                {
                    mState = 3;
                    mSprite->playGTAnimation(5, false);
                    //随机掉落道具
                    play->manager->addGameObject(Item::item(1, mSprite->getPosition(), mParent, false));
                }
        
                FootPrint::goFootPrint(&mStepSnow, mSprite->getPosition());
            }
                break;
            case 1://drop out
            {
                cocos2d::Point np = mSprite->getPosition();
                np.x += delta*mSpeed;
                mSprite->setPosition(np);
                if( mSprite->getPosition().x < mAwake )
                {
                    GTAnimatedEffect *hiteff2 = GTAnimatedEffect::create(GTAnimation::loadedAnimationSet("effect"), 2, false);
                    hiteff2->setScale(0.5f);
                    hiteff2->setAnchorPoint(cocos2d::Vec2(0.5f, 0.5f));
                    hiteff2->setPosition(position());
                    play->addChild(hiteff2, LAYER_MAINROLE+1);
                    mState = 2;
                    mSprite->playGTAnimation(3, false);
                }
            }
                break;
            case 2://awake
            {
                if( playend )
                {
                    mState = 0;
                    mSprite->playGTAnimation(2, true);
                    mSpeed = ACCELER_SPEED;
                }
            }
                break;
            case 3://dead
            {
                if( playend )
                {
                    //fix pos
                    float ds = delta*play->runspeed;
                    cocos2d::Point np = mSprite->getPosition();
                    np.x -= ds;
                    mSprite->setPosition(np);
                    
                    if( mSprite->getPosition().x < -100 )
                    {
                        play->enemies->removeObject(this);
                        play->manager->removeGameObject(this);
                    }
                }
                else {
                    mTimer += delta;
                    if(mTimer>0.3f && mFlag)
                    {
                        int n = 1 + randomInt(3);
                        SimpleAudioEngine::sharedEngine()->playEffect(cocos2d::CCString::createWithFormat("ahh%d.mp3", n)->getCString());
                        mFlag = false;
                    }
                    //fix pos
                    cocos2d::Point np = mSprite->getPosition();
                    if( mSprite->animationId() == 5 )
                    {
                        float ra = mTimer/mSprite->playBackTime();
                        if( ra > 1 )
                        {
                            ra = 1;
                        }
                        ra = 1 - ra;
                        float ds = delta*200*ra;
                        np.x += ds;
                    }
                    else {
                        float ra = mTimer/mSprite->playBackTime();
                        if( ra > 1 )
                        {
                            ra = 1;
                        }
                        float ds = delta*300*ra;
                        np.x -= ds;
                    }
                    mSprite->setPosition(np);
                }
            }
                break;
        }
    }
    
    //rush offset
    if( play->state == STATE_RUSH )
    {
        float offset = (play->runspeed - play->levelspeed)*delta;
        cocos2d::Point np = mSprite->getPosition();
        np.x -= offset;
        mSprite->setPosition(np);
    }
    
    //主角死亡的处理
    if( mState < 3 && play->gameOverTimer >= 0 )
    {
        float ds = play->runspeed*delta;
        cocos2d::Point np = mSprite->getPosition();
        np.x += ds;
        mSprite->setPosition(np);
    }
    //删除检查
    if( mSprite->getPosition().x < -100 || mSprite->getPosition().x > 100+UniversalFit::sharedUniversalFit()->playSize.width )
    {
        play->enemies->removeObject(this);
        play->manager->removeGameObject(this); 
    }
}

void Messager::onDestroy() 
{ 
    mParent->removeChild(mSprite, true);
}

bool Messager::collisionWithCircle(cocos2d::Point cc, float rad) 
{
    if( mState == 3 )
    {
        return false;//禁止鞭尸
    }
    if( mSprite != NULL )//shame defense
    {
        if( exCollisionWithCircles(mSprite->getPosition(), 1.0f, 13.0f, 8, cc, rad) ||
           exCollisionWithCircles(mSprite->getPosition(), -1.0f, 32.5f, 13, cc, rad) )
        {
            return true;
        }
    }
    return false;
}

bool Messager::deliverHit(int type, cocos2d::Point dir) 
{
    if( dir.x > 0 )
    {
        mSprite->playGTAnimation(5, false);
    }
    else {
        mSprite->playGTAnimation(4, false);
    }
    //add sp
    GamePlay *play = GamePlay::sharedGamePlay();
    play->mainrole->SP+=4;
    if( play->mainrole->SP > play->mainrole->maxSP )
    {
        play->mainrole->SP = play->mainrole->maxSP;
    }
    
    mState = 3;
    mTimer = 0;
    //随机掉落道具
    play->manager->addGameObject(Item::item(1, mSprite->getPosition(), mParent, false));
    
    //achievement killing
    GameRecord::sharedGameRecord()->task->dispatchTask(ACH_KILLING, 1);
    play->killingCount++;
    GameRecord::sharedGameRecord()->task->dispatchTask(ACH_KILLMESSAGER, 1);
    if( play->runwithoutkill == 0 )
    {
        play->runwithoutkill = play->distance/PLAY_DISMETER;
    }
    
    mFlag = true;
    
    return  true;
}

cocos2d::Point Messager::position() 
{
    return mSprite->getPosition();
}

void Messager::setPosition(cocos2d::Point pos) 
{
    mSprite->setPosition(pos);
}

cocos2d::Point Messager::center() 
{
    return ccpAdd(mSprite->getPosition(), Vec2(0, 21));
}

bool Messager::supportAimAid() 
{
    if( mState != 3 )
    {
        return  true;
    }
    else {
        return false;
    }
}

void Messager::toggleVisible(bool flag) 
{
    mSprite->setVisible(flag);
}


