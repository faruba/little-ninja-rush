//
//  Pumpkin.m
//  NinjiaRush
//
//  Created by 马 颂文 on 12-9-20.
//  Copyright (c) 2012年 TrinGame. All rights reserved.
//

#include "Pumpkin.h"
#include "GamePlay.h"
#include "Item.h"
#include "UniversalFit.h"
#include "GameRecord.h"

#define FLOATING_TIME (45)
#define NEAR (50)
#define RANGE (27)//半径

#define HIT_EXPLODE (10)



Pumpkin* Pumpkin::role(cocos2d::Node * parent) 
{
    Pumpkin *em = Pumpkin::create();
    em->mParent = parent;
    return em;
}

void Pumpkin::onCreate() 
{
    mSprite = GTAnimatedSprite::spriteWithGTAnimation(GTAnimation::loadedAnimationSet("pumpkin"));
    mSprite->playGTAnimation(0, true);
    mSprite->setPosition(cocos2d::Vec2( -100, SCREEN_HEIGHT ));
    mParent->addChild(mSprite, LAYER_MAINROLE);
    
    mState = 0;
    mTimer = 0;
    mHitCount = 0;
    mHurtTimer = -1;
    
    //init parameters
    mTargetPos = Vec2(50, 210);
    mTargetSpeed = ccpMult(ccpForAngle(CC_DEGREES_TO_RADIANS(-45)), 100);
    mPumpkinSpeed = ccpMult(ccpForAngle(CC_DEGREES_TO_RADIANS(-90)), 50);
    
    GameTool::PlaySound("pumpkinstart.mp3");
    
    mFlySound = 3 + 7*CCRANDOM_0_1();
}

void Pumpkin::onUpdate(float delta) 
{
    GamePlay *play = GamePlay::sharedGamePlay();
    bool removeflag = false;
    bool playend = mSprite->updateGTAnimation(delta);
    
    if( mHurtTimer >= 0 )
    {
        mHurtTimer += delta;
        if( mHurtTimer > 0.2f )
        {
            mSprite->setColor(Color3B(255, 255, 255));
            mSprite->setScale(1);
            mHurtTimer = -1;
            mSprite->setRotation(0);
        }
        else
        {
            if( mHurtTimer > 0.1f )
            {//scale down
                mSprite->setScale(1.3f - (mHurtTimer-0.1f)/0.1f * 0.3f);
            }
            else
            {//scale up
                mSprite->setScale(mHurtTimer/0.1f * 0.3f + 1);
            }
        }
    }
    float displayer = play->mainrole->position().x - mSprite->getPosition().x;
    if( displayer*mSprite->getScaleX() > 0 )
    {
      mSprite->setScaleX(mSprite->getScaleX()*-1);
    }
    
    switch (mState) {
        case 0://floating
        {
            //tick timer
            mTimer += delta;
            if( mTimer > FLOATING_TIME)
            {
                mState = 1;
                mTimer = 0;
            }
            if( playend )
            {
                mSprite->playGTAnimation(0, true);
            }
            {//move target
                cocos2d::Point ntp = ccpAdd(mTargetPos, ccpMult(mTargetSpeed, delta));
                mTargetPos = ntp;
                //range check
                //left && right
                if( ( ntp.x < 50 && mTargetSpeed.x < 0 ) || ( ntp.x > 430 && mTargetSpeed.x > 0 ) )
                {
                    mTargetSpeed.x *= -1;
                }
                //up && down
                if( ( ntp.y < 210 && mTargetSpeed.y < 0 ) || ( ntp.y > 270 && mTargetSpeed.y > 0 ) )
                {
                    mTargetSpeed.y *= -1;
                }
            }
            
            //fly sound
            {
                mFlySound -= delta;
                if( mFlySound < 0 )
                {
                    GameTool::PlaySound("pumpkinfly.mp3");
                    mFlySound = 3 + 7*CCRANDOM_0_1();
                }
            }
        }
            break;
        case 1://escape
        {
            mTargetPos = Vec2(UniversalFit::sharedUniversalFit()->playSize.width + 100, SCREEN_HEIGHT/2);
            if( ccpDistance(mSprite->getPosition(), mTargetPos) < NEAR )
            {
                removeflag = true;
            }
            if( playend )
            {
                mSprite->playGTAnimation(0, true);
            }
        }
            break;
        case 2://dying
        {
            mTimer += delta;
            if( mTimer >= 1 && mSprite->animationId() == 1 )
            {
                mSprite->playGTAnimation(2, false);
            }
            if( mTimer < 1 )
            {
                if( mTimer > (1.0f/30)*mCoinsCounter )
                {
                    play->manager->addGameObject(Item::item(0, mSprite->getPosition(), play, true));
                    mCoinsCounter++;
                }
            }
            if( playend )
            {
                removeflag = true;
                if( !GameRecord::sharedGameRecord()->collection->isItemCompleted(38) )
                {
                    play->manager->addGameObject(Item::item(-1, mSprite->getPosition(), play, false));
                }
            }
            //shake
            mTargetPos = mSprite->getPosition();
            mTargetPos.x += -5 + CCRANDOM_0_1()*10;
            mTargetPos.y += -5 + CCRANDOM_0_1()*10;
            mSprite->setPosition(mTargetPos);
            mPumpkinSpeed = Vec2(0, 0);
        }
            break;
    }
    if( !ccpFuzzyEqual(mSprite->getPosition(), mTargetPos, 0.1f) && mHurtTimer<0 ){//move with target
        //merge speed
        cocos2d::Point dr = ccpSub(mTargetPos, mSprite->getPosition());
        float dt = ccpLength(dr);
        float at = dt*20;//magic k
        dr = ccpNormalize(dr);
        cocos2d::Point attract = ccpMult(dr, at*delta);
        mPumpkinSpeed = ccpAdd(mPumpkinSpeed, attract);
        
        //limit speed
        {
            float currsp = ccpLength(mPumpkinSpeed);
            if( currsp > 100 )
            {
                mPumpkinSpeed = ccpMult( ccpNormalize(mPumpkinSpeed), 100);
            }
        }
        
        //move pumpkin
        cocos2d::Point np = ccpAdd(mSprite->getPosition(), ccpMult(mPumpkinSpeed, delta));
        mSprite->setPosition(np);
    }
    if( removeflag )
    {
        play->manager->removeGameObject(this);
    }
}

//碰撞检测
bool Pumpkin::collisionWithCircle(cocos2d::Point cc, float rad) 
{
	if(mSprite == NULL){
		cocos2d::CCLog("???? %p", this);
    return false;
	}
    if( mState < 2 )
    {
        cocos2d::Point dp = ccpSub(mSprite->getPosition(), cc);
        float lensq = ccpLengthSQ(dp);
        if( lensq < (rad + RANGE)*(rad + RANGE) )
        {
            return true;
        }
        else
        {
            return false;
        }
    }
    else
    {
        return false;
    }
}

//受到伤害
bool Pumpkin::deliverHit(int type, cocos2d::Point dir) 
{
    GamePlay *play = GamePlay::sharedGamePlay();
    if( mState < 2 )
    {
        if( mHitCount < HIT_EXPLODE )
        {
            //pop coins
            play->manager->addGameObject(Item::item(0, mSprite->getPosition(), play, false));
            mSprite->playGTAnimation(1, false);
            mHurtTimer = 0;
            mSprite->setColor(Color3B(255, 142, 142));
            GameTool::PlaySound("pumpkinhurt.mp3");
            if( mSprite->getScaleX() > 0 )
            {
                mSprite->setRotation(30);
            }
            else
            {
                mSprite->setRotation(-30);
            }
        }
        else
        {
            mState = 2;
            // die
            mSprite->playGTAnimation(1, true);
            GameTool::PlaySound("pumpkindie.mp3");
            mTimer = 0;
            mCoinsCounter = 0;
            
            //achievement : kill pumpkin
            play->killpumpkins++;
            GameRecord::sharedGameRecord()->task->dispatchTask(ACH_KILLPUMPKINS, 1);
            GameRecord::sharedGameRecord()->task->dispatchTask(ACH_ONEKILLPUMPKINS, play->killpumpkins);
        }
        mHitCount++;
        return true;
    }
    else
    {
        return false;
    }
}

cocos2d::Point Pumpkin::position() 
{
    return mSprite->getPosition();
}

void Pumpkin::setPosition(cocos2d::Point pos) 
{
    mSprite->setPosition(pos);
}

cocos2d::Point Pumpkin::center() 
{
    return mSprite->getPosition();
}

bool Pumpkin::supportAimAid() 
{
    return  false;
}

void Pumpkin::toggleVisible(bool flag) 
{
    mSprite->setVisible(flag);
}

void Pumpkin::onDestroy() 
{
    GamePlay *play = GamePlay::sharedGamePlay();
    play->enemies->removeObject(this);
    mParent->removeChild(mSprite, false);
}


