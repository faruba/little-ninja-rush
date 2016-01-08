//
//  ShaodwKilling.m
//  NinjiaRush
//
//  Created by 马 颂文 on 12-6-18.
//  Copyright (c) 2012年 TrinGame. All rights reserved.
//

#include "ShadowKilling.h"
#include "GamePlay.h"
#include "GameTool.h"
#include "GameData.h"
#include "GameRecord.h"
#include "UniversalFit.h"

#define TARGET_RANGE (0)
#define SLIDE_RANGE (100)
#define SLIDE_SPEED (1000)



ShadowKilling* ShadowKilling::shadowKilling(cocos2d::Node* parent) 
{
    ShadowKilling *ret = ShadowKilling::create();
    ret->mParent = parent;
    return ret;
}

void ShadowKilling::onCreate() 
{
    mState = 0;
    mDirection = -1;
    //创建角色动画
    mRole = GTAnimatedSprite::spriteWithGTAnimation(GTAnimation::loadedAnimationSet(GameData::roleAnimation(GameRecord::sharedGameRecord()->curr_char).c_str()));
    mRole->setAnchorPoint(cocos2d::Vec2(0.4f, 0.0625f));
    mShadow = GTGhostShadow::shadow(mRole, 0.02f, 5);
    mParent->addChild(mRole, LAYER_MAINROLE+2);
    mParent->addChild(mShadow, LAYER_MAINROLE+1);
    mRole->setVisible(false);
    mShadow->setVisible(false);
    mSteak = 0;
    
    int level = GameRecord::sharedGameRecord()->item_upgrade[8];
    mCount = 4 + 2*level;
}

void ShadowKilling::onUpdate(float delta) 
{
    GamePlay *play = GamePlay::sharedGamePlay();
    float time = play->deltaTime;
    
    mRole->updateGTAnimation(time);
    mShadow->syncShadow(time);
    
    switch (mState) {
        case 0://等待
        {
            this->seekTarget();
            if( mTarget != NULL )
            {
                mState = 1;
                play->mainrole->attachEffect("effect", 5, false, true);
                play->mainrole->toggleVisible(false);
                play->mainrole->toggleShadow(false);
                play->mainrole->toggleInvincible(true);
                if( play->mainrole2 != NULL )
                {
                    play->mainrole2->attachEffect("effect", 5, false, true);
                    play->mainrole2->toggleVisible(false);
                    play->mainrole2->toggleShadow(false);
                    play->mainrole2->toggleInvincible(true);
                }
                mState = 2;
            }
        }
            break;
        case 1:
        {
            bool done = false;
            if( mRole->getPosition().x != mTargetPos )
            {
                float dis = mTargetPos - mRole->getPosition().x;
                float dr = dis/fabsf(dis);
                float ds = dr*SLIDE_SPEED*time;
                if( fabsf(dis) <= fabsf(ds) )
                {
                    cocos2d::Point np = mRole->getPosition();
                    np.x = mTargetPos;
                    mRole->setPosition(np);
                    mTarget->deliverHit(HIT_MAGIC, Vec2(mDirection, 0));
                    mSteak++;
                    if( mSteak > 9 )
                    {
                        mSteak = 9;
                    }
std::string sound = std::string_format("combo%d.mp3", mSteak);
                    GameTool::PlaySound(sound.c_str());
                    
                    GTAnimatedEffect *hiteff = GTAnimatedEffect::create(GTAnimation::loadedAnimationSet("effect"), 28, false);
                    hiteff->setAnchorPoint(cocos2d::Vec2(0.5f, 0.5f));
                    hiteff->setPosition(mTarget->center());
                    hiteff->setRotation(90);
                    play->addChild(hiteff, LAYER_MAINROLE+1);
                }
                else {
                    mMoveDis += fabsf(ds);
                    //change alpha
                    float k = 0;
                    if( mMoveDis > SLIDE_RANGE/2 )
                    {
                        k = (SLIDE_RANGE-mMoveDis)/(SLIDE_RANGE/2);
                    }
                    else {
                        k = mMoveDis/(SLIDE_RANGE/2);
                    }
                    if( k > 0.7f )
                    {
                        k = 1;
                    }
                    mRole->setOpacity(100+155*k);
                    mShadow->opacity = mRole->getOpacity();
                    
                    cocos2d::Point np = mRole->getPosition();
                    np.x += ds;
                    mRole->setPosition(np);
                }
            }
            else {
                mRole->setOpacity(0);
                mRole->setVisible(false);
                mKillTimer += time;
                if( mKillTimer > 0.3f )
                {
                    done = true;
                    mCount--;
                }
            }
            if( done )
            {
                //-counter-
                this->seekTarget();
                if( mTarget != NULL && mCount > 0 )
                {
                    mRole->setOpacity(255);
                    mMoveDis = 0;
                    mKillTimer = 0;
                    mDirection *= -1;
                    mRole->setScaleX(mDirection);
                    cocos2d::Point pos = mTarget->position();
                    mTargetPos = pos.x + mDirection*(SLIDE_RANGE/2);
                    pos.x += mDirection*-(SLIDE_RANGE/2);
                    mRole->setPosition(pos);
                    mRole->playGTAnimation(8, false);
                    mRole->setVisible(true);
                }
                else {
                    play->count_control--;
                    mState = 0;
                    play->mainrole->attachEffect("effect", 5, false, true);
                    play->mainrole->toggleVisible(true);
                    play->mainrole->toggleShadow(true);
                    play->mainrole->toggleInvincible(false);
                    if( play->mainrole2 != NULL )
                    {
                        play->mainrole2->attachEffect("effect", 5, false, true);
                        play->mainrole2->toggleVisible(true);
                        play->mainrole2->toggleShadow(true);
                        play->mainrole2->toggleInvincible(false);
                    }
                    mRole->setVisible(false);
                    
                    //play->gamespeed += 0.9f;
                    play->unscheduleSpeed();
                    
                    play->manager->removeGameObject(this);
                }
            }
        }
            break;
        case 2:
        {//wait
            mKillTimer+=time;
            if( mKillTimer>0.2f )
            {
                mKillTimer = 0;
                play->count_control++;
                mDirection *= -1;
                mRole->setScaleX(mDirection);
                cocos2d::Point pos = mTarget->position();
                mTargetPos = pos.x + mDirection*(SLIDE_RANGE/2);
                pos.x += mDirection*-(SLIDE_RANGE/2);
                mRole->setPosition(pos);
                mRole->setVisible(true);
                mShadow->setVisible(true);
                mMoveDis = 0;
                mRole->playGTAnimation(8, false);
                mShadow->toggleShadow(true);
                
                //play->gamespeed -= 0.9f;
                play->scheduleSpeed(0.1f, -1, 1);
                
                mState = 1;
            }
        }
            break;
    }
}

void ShadowKilling::seekTarget() 
{
  GamePlay *play = GamePlay::sharedGamePlay();
  mTarget = NULL;
cocos2d::Ref* node = NULL;
CCARRAY_FOREACH(play->enemies, node)
  {
    Role * em = (Role*)node;
    if( em->supportAimAid() && em->position().x > TARGET_RANGE && em->position().x < UniversalFit::sharedUniversalFit()->playSize.width-TARGET_RANGE )
    {
      mTarget = em;
      break;
    }
  }
}

void ShadowKilling::onDestroy() 
{
    GamePlay *play = GamePlay::sharedGamePlay();
    play->removeChild(mRole, true);
    play->removeChild(mShadow, true);
}


