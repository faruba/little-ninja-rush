//
//  Merchant.m
//  NinjiaRush
//
//  Created by 马 颂文 on 12-5-10.
//  Copyright (c) 2012年 TrinGame. All rights reserved.
//

#include "Merchant.h"
#include "GameConstants.h"
#include "GamePlay.h"
#include "SimpleAudioEngine.h"
#include "MerchantPanel.h"
#include "GameRecord.h"
#include "UniversalFit.h"

void Merchant::onCreate() 
{
  Role::onCreate();
    mSprite->setAnchorPoint(cocos2d::Vec2(0.5f , 0));
    mSprite->setPosition(cocos2d::Vec2(-50, PLAY_PLAYERLINE+5));
    mSprite->playGTAnimation(0, true);
    mParent->addChild(mSprite, LAYER_MAINROLE-1);
    
    mFlag = true;
    mStepTimer = 0;
}

void Merchant::onUpdate(float delta) 
{
    GamePlay *play = GamePlay::sharedGamePlay();
    mSprite->updateGTAnimation(delta);
    if( play->state != STATE_MERCHANT )
    {
        cocos2d::Point np = mSprite->getPosition();
        np.x += delta*100.0f;
        mSprite->setPosition(np);
        
        //当与主角擦身而过时，触发商人界面
        if( np.x > play->mainrole->position().x && mFlag && play->mainrole->HP > 0 )
        {
            //弹出界面
            play->state = STATE_MERCHANT;
            mFlag = false;
            //scripting
            play->manager->addGameObject(MerchantPanel::merchantPanel());
            //achievement merchant met
            GameRecord::sharedGameRecord()->task->dispatchTask(ACH_MERCHANTMET, 1);
        }
        //删除检查
        if( mSprite->getPosition().x < -100 || mSprite->getPosition().x > 100+UniversalFit::sharedUniversalFit()->playSize.width )
        {
            play->count_respawn--;
            play->manager->removeGameObject(this);
        }
        
        //step dust
        mStepTimer += delta;
        if( mStepTimer > 0.12f )
        {
            play->stepDust(mSprite->getPosition());
            mStepTimer = 0;
        }
    }
}

void Merchant::onDestroy() 
{ 
    mParent->removeChild(mSprite, true);
}

bool Merchant::deliverHit(int type, cocos2d::Point dir) 
{
    return false;
}

cocos2d::Point Merchant::center() 
{
    return ccpAdd(mSprite->getPosition(), Vec2(42.5f, 26.0f));
}

bool Merchant::supportAimAid() 
{
    return  false;
}
