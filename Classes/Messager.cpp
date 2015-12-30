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

void Messager::onCreate() 
{
  mCollisionCircles.push_back(Circle(cocos2d::Vec2(1, 13), 8));
  mCollisionCircles.push_back(Circle(cocos2d::Vec2(-1, 32.5), 13));
  Role::onCreate();
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
        mState = Entering;
    }
    else {//drop out mode
        mSprite->playGTAnimation(0, true);
        mSprite->setPosition(cocos2d::Vec2( 80+UniversalFit::sharedUniversalFit()->playSize.width, RESPAWN_YMIN+y));
        mState = Running;
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
    bool gameOver = handleGameOver(delta);

    if (!gameOver) {//主角死亡的处理
        switch (mState) {
            case Entering://chase
            {
                cocos2d::Point np = mSprite->getPosition();
                np.x += delta*mSpeed;
                mSprite->setPosition(np);
                if( mFlagFalldown && mSprite->getPosition().x > mFalldown )
                {
                    mState = Dead;
                    mSprite->playGTAnimation(5, false);
                    //随机掉落道具
                    play->manager->addGameObject(Item::item(1, mSprite->getPosition(), mParent, false));
                }
        
                FootPrint::goFootPrint(&mStepSnow, mSprite->getPosition());
            }
                break;
            case Running://drop out
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
                    mState = Fleeing;
                    mSprite->playGTAnimation(3, false);
                }
            }
                break;
            case Fleeing://awake
            {
                if( playend )
                {
                    mState = Entering;
                    mSprite->playGTAnimation(2, true);
                    mSpeed = ACCELER_SPEED;
                }
            }
                break;
            case Dead://dead
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
                        GameTool::PlaySound(cocos2d::CCString::createWithFormat("ahh%d.mp3", n)->getCString());
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
    
    mState = Dead;
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

cocos2d::Point Messager::center() 
{
    return ccpAdd(mSprite->getPosition(), Vec2(0, 21));
}

bool Messager::supportAimAid() 
{
    if( mState != Dead )
    {
        return  true;
    }
    else {
        return false;
    }
}
