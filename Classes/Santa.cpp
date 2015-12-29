//
//  Santa.m
//  NinjiaRush
//
//  Created by 马 颂文 on 12-11-26.
//  Copyright (c) 2012年 TrinGame. All rights reserved.
//

#include "Santa.h"
#include "GameConstants.h"
#include "GamePlay.h"
#include "SimpleAudioEngine.h"
#include "Item.h"
#include "GameRecord.h"
#include "UniversalFit.h"
#include "FootPrint.h"

void Santa::onCreate() 
{
    GamePlay *play = GamePlay::sharedGamePlay();
    mSprite = GTAnimatedSprite::spriteWithGTAnimation(GTAnimation::loadedAnimationSet("santa"));
    mSprite->setAnchorPoint(cocos2d::Vec2(0.5438f, 0.0625f));
    int y = CCRANDOM_0_1()*RESPAWN_Y;
    if( play->state == STATE_RUSH )
    {
        mSprite->setPosition(cocos2d::Vec2(UniversalFit::sharedUniversalFit()->playSize.width+100, RESPAWN_YMIN+y));
    }
    else {
        mSprite->setPosition(cocos2d::Vec2(-100, RESPAWN_YMIN+y));
    }
    mSprite->playGTAnimation(0, true);
    mParent->addChild(mSprite, LAYER_ROLE+RESPAWN_Y-y);
    
    mState = 0;//0 run 1 stop 2 hit 3 away
    mTargetPos = 20+(UniversalFit::sharedUniversalFit()->playSize.width-40)*CCRANDOM_0_1();
    mSpeed = ENEMY_NNRUNSPEED;
    
    mCoinTimer = 0;
    mActionTimer = 0;
    mHited = false;
}

void Santa::onUpdate(float delta) 
{
    bool playend = mSprite->updateGTAnimation(delta);
    GamePlay* play = GamePlay::sharedGamePlay();
    bool removeflag = false;
    bool dropcoins = false;
    
    if( play->gameOverTimer >= 0 )
    {//主角死亡的处理
        float ds = delta*(play->levelspeed - play->runspeed);
        cocos2d::Point np = mSprite->getPosition();
        np.x += ds;
        mSprite->setPosition(np);
    }
    else {
        switch (mState) {
            case 0:// run to position
            {
                float ds = delta*mSpeed;
                float dis = mTargetPos - mSprite->getPosition().x;
                cocos2d::Point np = mSprite->getPosition();
                if( fabsf(dis) > ds )
                {
                    if( dis > 0 )
                    {
                        np.x += ds;
                    }
                    else {
                        np.x -= ds;
                    }
                    mSprite->setPosition(np);
                }
                else {
                    np.x = mTargetPos;
                    //上场就奔跑
                    mState = 1;
                    mActionTimer = 2 + 3*CCRANDOM_0_1();
                }
                if( playend )
                {
                    mSprite->playGTAnimation(0, true);
                }
                
                dropcoins = true;
                mTimer += delta;
                if( mTimer > SANTA_LIFE )
                {
                    mState = 3;
                    mTargetPos = UniversalFit::sharedUniversalFit()->playSize.width+150;
                    mSpeed = ENEMY_NNRUNSPEED;
                }
            }
                break;
            case 1:// stop
            {
                mActionTimer -= delta;
                if( mActionTimer <= 0 )
                {
                    mState = 0;
                    mActionTimer = 0;
                    mTargetPos = 20+(UniversalFit::sharedUniversalFit()->playSize.width-40)*CCRANDOM_0_1();
                    mSpeed = (0.3f+0.4f*CCRANDOM_0_1())*ENEMY_NNRUNSPEED;
                }
                if( playend )
                {
                    mSprite->playGTAnimation(0, true);
                }
                
                dropcoins = true;
                mTimer += delta;
                if( mTimer > SANTA_LIFE )
                {
                    mState = 3;
                    mTargetPos = UniversalFit::sharedUniversalFit()->playSize.width+150;
                    mSpeed = ENEMY_NNRUNSPEED;
                }
            }
                break;
            case 2:// hit
            {
                if( playend )
                {
                    mState = 4;
                }
            }
                break;
            case 3:// run away
            {
                float ds = delta*mSpeed;
                float dis = mTargetPos - mSprite->getPosition().x;
                cocos2d::Point np = mSprite->getPosition();
                if( fabsf(dis) > ds )
                {
                    if( dis > 0 )
                    {
                        np.x += ds;
                    }
                    else {
                        np.x -= ds;
                    }
                    mSprite->setPosition(np);
                }
                else {
                    removeflag = true;
                }
                if( playend )
                {
                    mSprite->playGTAnimation(2, true);
                }
            }
                break;
            case 4://dead
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
                    {
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
    
    //drop coins
    if( dropcoins )
    {
        mCoinTimer+=delta;
        if( mCoinTimer > SANTA_COIN )
        {
            play->manager->addGameObject(Item::item(0, mSprite->getPosition(), play, true));
            mCoinTimer -= SANTA_COIN;
        }
    }
    
    //snow step
    if( mState != 2 )
    {
        FootPrint::goFootPrint(&mStepSnow, mSprite->getPosition());
    }
    
    //删除检查
    if( mSprite->getPosition().x < -120 || mSprite->getPosition().x > 120+UniversalFit::sharedUniversalFit()->playSize.width )
    {
        removeflag = true;
    }
    
    if( removeflag )
    {
        play->manager->removeGameObject(this);
        if( !mHited )
        {
            //achievement : meet christmas
            play->meetchristmas++;
            GameRecord::sharedGameRecord()->task->dispatchTask(ACH_MEETCHRISTMAS, 1);
            GameRecord::sharedGameRecord()->task->dispatchTask(ACH_ONEMEETCHRISTMAS, play->meetchristmas);
        }
    }
}

//碰撞检测
bool Santa::collisionWithCircle(cocos2d::Point cc, float rad) 
{
    if( mState > 1 )
    {
        return false;
    }
    else {
        if( exCollisionWithCircles(mSprite->getPosition(), 1.0f, 13.0f, 8, cc, rad) ||
           exCollisionWithCircles(mSprite->getPosition(), -1.0f, 32.5f, 13, cc, rad) )
        {
            return true;
        }
    }
    return false;
}

//受到伤害
bool Santa::deliverHit(int type, cocos2d::Point dir) 
{
    mTimer = 0;
    mSprite->playGTAnimation(1, false);
    
    mState = 4;
    mHited = true;
    mFlag = true;
    
    return true;
}

cocos2d::Point Santa::position() 
{
    return mSprite->getPosition();
}

void Santa::setPosition(cocos2d::Point pos) 
{
    mSprite->setPosition(pos);
}

cocos2d::Point Santa::center() 
{
    return ccpAdd(mSprite->getPosition(), Vec2(0, 21));
}

bool Santa::supportAimAid() 
{
    return false;
}

void Santa::toggleVisible(bool flag) 
{
    mSprite->setVisible(flag);
}

void Santa::onDestroy() 
{
    GamePlay *play = GamePlay::sharedGamePlay();
    play->enemies->removeObject(this);
    mParent->removeChild(mSprite, false);
}


