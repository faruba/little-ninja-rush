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



Merchant* Merchant::role(CCNode * parent) 
{
    Merchant *ret = Merchant::create();
    ret->mParent = parent;
    return  ret;
}

void Merchant::onCreate() 
{
    mSprite = GTAnimatedSprite::spriteWithGTAnimation(GTAnimation::loadedAnimationSet("merchant"));
    mSprite->setAnchorPoint(ccp(0.5f , 0));
    mSprite->setPosition(ccp(-50, PLAY_PLAYERLINE+5));
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
        CCPoint np = mSprite->getPosition();
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

bool Merchant::collisionWithCircle(CCPoint cc, float rad) 
{
    return false;
}

bool Merchant::deliverHit(int type, CCPoint dir) 
{
    return false;
}

CCPoint Merchant::position() 
{
    return mSprite->getPosition();
}

CCPoint Merchant::center() 
{
    return ccpAdd(mSprite->getPosition(), ccp(42.5f, 26.0f));
}

bool Merchant::supportAimAid() 
{
    return  false;
}

void Merchant::toggleVisible(bool flag) 
{
    mSprite->setVisible(flag);
}

void Merchant::setPosition(CCPoint pos) 
{
    mSprite->setPosition(pos);
}


