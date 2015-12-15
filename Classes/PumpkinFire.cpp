//
//  PumpkinFire.m
//  NinjiaRush
//
//  Created by 马 颂文 on 12-9-24.
//  Copyright (c) 2012年 TrinGame. All rights reserved.
//

#include "PumpkinFire.h"
#include "GameConstants.h"
#include "GamePlay.h"
#include "SimpleAudioEngine.h"
#include "GameRecord.h"
#include "UniversalFit.h"

#define PUMPKINFIRE_SPEED (700)
#define PUMPKINHIT_EFF1 (11)
#define PUMPKINHIT_EFF2 (1)



PumpkinFire* PumpkinFire::dart(CCPoint pos, CCPoint dir, CCNode* parent) 
{
    PumpkinFire* ret = PumpkinFire::create();
    ret->mParent = parent;
    ret->pos = pos;
    ret->direction = dir;
    return ret;
}

void PumpkinFire::onCreate() 
{
    mSprite = CCSprite::createWithSpriteFrameName("fbh_tail.png");
    mSprite->setAnchorPoint(ccp(0.5f, 0.95f));
    mSprite->setPosition(pos);
    mSprite->setRotation(90-CC_RADIANS_TO_DEGREES(ccpToAngle(direction)));
    mParent->addChild(mSprite, LAYER_MAINROLE+1);
    
    mIsEnemy = false;
    mIsEnemy = false;
    speed = PUMPKINFIRE_SPEED;
    
    mRemoved = false;
}

void PumpkinFire::onUpdate(float delta) 
{
    GamePlay* play = GamePlay::sharedGamePlay();
    bool removeflag = false;
    
    //碰撞检测
  CCObject* node = NULL;
  CCARRAY_FOREACH(play->enemies, node)
  {
    Role * em = (Role*)node;
        if( em->collisionWithCircle(pos, 7) )
        {
            bool hit = em->deliverHit(HIT_MAGIC, direction);
            removeflag = true;
            if( hit )
            {
                GTAnimatedEffect *hiteff = GTAnimatedEffect::create(GTAnimation::loadedAnimationSet("effect"), PUMPKINHIT_EFF2, false);
                hiteff->setAnchorPoint(ccp(0.5f, 0.5f));
                hiteff->setPosition(em->center());
                hiteff->setRotation(60 - CC_RADIANS_TO_DEGREES( ccpToAngle(direction) ) + 60*CCRANDOM_0_1());
                mParent->addChild(hiteff, LAYER_MAINROLE+1);
                
                GTAnimatedEffect *hiteff2 = GTAnimatedEffect::create(GTAnimation::loadedAnimationSet("effect"), PUMPKINHIT_EFF1, false);
                hiteff2->setAnchorPoint(ccp(0.5f, 0.5f));
                hiteff2->setPosition(em->center());
                mParent->addChild(hiteff2, LAYER_ROLE);
                
                SimpleAudioEngine::sharedEngine()->playEffect(CCFileUtils::sharedFileUtils()->fullPathForFilename("sound/hit-fire.mp3").c_str());
                //achievement enemy burnt
                GameRecord::sharedGameRecord()->task->dispatchTask(ACH_ENEMYBURNT, 1);
            }
            break;
        }
    }
    
        
    //飞行
    CCPoint ds = ccpMult(direction, speed*delta);
    CCPoint np = ccpAdd(ds, mSprite->getPosition());
    mSprite->setPosition(np);
    pos = np;
       
    //越界清除
    if( np.x < -50 || np.x > UniversalFit::sharedUniversalFit()->playSize.width+50 || np.y < -50 || np.y > SCREEN_HEIGHT+50)
    {
        removeflag = true;
    }
    
    if( mRemoved )
    {
        mRemoved = false;
        removeflag = true;
    }
    
    if( removeflag )
    {
        play->manager->removeGameObject(this);
    }
}

bool PumpkinFire::isEnemy() 
{
    return mIsEnemy;
}

CCPoint PumpkinFire::position() 
{
    return pos;
}

void PumpkinFire::onHitback(CCPoint origin) 
{
}

void PumpkinFire::onDestroy() 
{
    //GamePlay *play = GamePlay::sharedGamePlay();
    mParent->removeChild(mSprite, true);
}


