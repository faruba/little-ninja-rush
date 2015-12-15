//
//  PEffect.m
//  NinjiaRush
//
//  Created by 马 颂文 on 12-5-9.
//  Copyright (c) 2012年 TrinGame. All rights reserved.
//

#include "PEffect.h"
#include "GameConstants.h"
#include "GamePlay.h"

#include "UniversalFit.h"

#define EFFECT_Z (LAYER_MAINROLE+1)



Wood* Wood::wood(cocos2d::Point pos, cocos2d::Point dir, CCNode * parent, GOHandler target, cocos2d::Point anchor) 
{
    Wood *ret = Wood::create();
    ret->mParent = parent;
    ret->mPos = pos;
    ret->mDir = dir;
    ret->mTarget = target;
    ret->mAnchor = anchor;
    return ret;
}

void Wood::onCreate() 
{
    mTimer = 0;
    mSprite = cocos2d::CCSprite::createWithSpriteFrameName("wood.png");
    mSprite->setAnchorPoint(cocos2d::ccp(0.5f, 0.5f));
    mSprite->setPosition(mPos);
    mParent->addChild(mSprite, EFFECT_Z);
    if( mDir.x > 0 )
    {
        mDir.x = 1;
    }
    else {
        mDir.x = -1;
    }
    mFlag = false;
}

void Wood::onUpdate(float delta) 
{
    GamePlay *play = GamePlay::sharedGamePlay();
    Role *target = (Role*)play->manager->gameObjectWithHandler(mTarget);
    if( target == NULL )
    {
        play->manager->removeGameObject(this);
        return;
    }
    
    mTimer += delta;
    if( !mFlag )
    {
        float dx = mDir.x*delta*70.0f;
        float dy = 200.0f*mTimer + 0.5f*mTimer*mTimer*(-800.0f);
        cocos2d::Point np = mSprite->getPosition();
        np.x += dx;
        np.y = mPos.y + dy;
        if( np.x < 0 )
        {
            np.x = 0;
        }
        if( np.x > UniversalFit::sharedUniversalFit()->playSize.width )
        {
            np.x = UniversalFit::sharedUniversalFit()->playSize.width;
        }
        mSprite->setPosition(np);
        if( mSprite->getPosition().y < mPos.y )
        {
            mFlag = true;
            mTimer = 0;
            mSprite->setVisible(false);
            
            cocos2d::Point xp = target->position();
            xp.x = mSprite->getPosition().x;
            
            GTAnimatedEffect *eff = GTAnimatedEffect::create(GTAnimation::loadedAnimationSet("effect"), 5, false);
            eff->setAnchorPoint(mAnchor);
            eff->setPosition(xp);
            if( mPos.y > SCREEN_HEIGHT/2 )
            {//敌人
                mParent->addChild(eff, LAYER_ROLE+RESPAWN_Y+RESPAWN_YMIN-target->position().y);
            }
            else {//主角
                mParent->addChild(eff, LAYER_MAINROLE+1);
                play->count_control--;
            }
        }
    }
    else {
        if( mTimer >= 0.3f )
        {
            target->toggleVisible(true);
            cocos2d::Point np = target->position();
            np.x = mSprite->getPosition().x;
            target->setPosition(np);
            
            play->manager->removeGameObject(this);
        }
    }
}

void Wood::onDestroy() 
{
    mParent->removeChild(mSprite, false);
    mSprite = NULL;
}


