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

void Santa::onCreate() {
  mCollisionCircles.push_back(Circle(cocos2d::Vec2(1, 13), 8));
  mCollisionCircles.push_back(Circle(cocos2d::Vec2(-1, 32.5), 13));
  mAnchor = cocos2d::Vec2(0.5438f, 0.0625f);

  Role::onCreate();

  GamePlay *play = GamePlay::sharedGamePlay();
  int y = CCRANDOM_0_1()*RESPAWN_Y;
  int x = (play->state == STATE_RUSH) ? UniversalFit::sharedUniversalFit()->playSize.width+100:-100;

  mSprite->setPosition(cocos2d::Vec2(x, RESPAWN_YMIN+y));
  mSprite->playGTAnimation(0, true);
  mParent->addChild(mSprite, LAYER_ROLE+RESPAWN_Y-y);

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
    bool gameOver = handleGameOver(delta);

    if (!gameOver) {//主角死亡的处理
        switch (mState) {
            case Entering:// run to position
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
                    mState = Running;
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
                    mState = Fleeing;
                    mTargetPos = UniversalFit::sharedUniversalFit()->playSize.width+150;
                    mSpeed = ENEMY_NNRUNSPEED;
                }
            }
                break;
            case Running:// stop
            {
                mActionTimer -= delta;
                if( mActionTimer <= 0 )
                {
                    mState = Entering;
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
                    mState = Fleeing;
                    mTargetPos = UniversalFit::sharedUniversalFit()->playSize.width+150;
                    mSpeed = ENEMY_NNRUNSPEED;
                }
            }
                break;
            case Hit:// hit
            {
                if( playend )
                {
                    mState = Dead;
                }
            }
                break;
            case Fleeing:// run away
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
                        GameTool::PlaySound(std::string_format("ahh%d.mp3", n).c_str());
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
    if( mState != Hit )
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

//受到伤害
bool Santa::deliverHit(int type, cocos2d::Point dir) 
{
    mTimer = 0;
    mSprite->playGTAnimation(1, false);
    
    mState = Dead;
    mHited = true;
    mFlag = true;
    
    return true;
}

cocos2d::Point Santa::center() 
{
    return ccpAdd(mSprite->getPosition(), Vec2(0, 21));
}

bool Santa::supportAimAid() 
{
    return false;
}

void Santa::onDestroy() 
{
    GamePlay *play = GamePlay::sharedGamePlay();
    play->enemies->removeObject(this);
    mParent->removeChild(mSprite, false);
}
