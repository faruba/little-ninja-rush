//
//  Star.m
//  NinjiaRush
//
//  Created by 马 颂文 on 12-6-26.
//  Copyright (c) 2012年 TrinGame. All rights reserved.
//

#include "Star.h"
#include "GamePlay.h"
#include "SimpleAudioEngine.h"
#include "GameConstants.h"
#include "GameRecord.h"



Star* Star::star(cocos2d::Point pos, cocos2d::Point dir) 
{
    Star *ret = Star::create();
    ret->mPos = pos;
    ret->mDir = dir;
    return ret;
}

void Star::onCreate() 
{
    GamePlay *play = GamePlay::sharedGamePlay();
    mSprite = cocos2d::Sprite::createWithSpriteFrameName("lxy.png");
    mSprite->setRotation(-90 - CC_RADIANS_TO_DEGREES(ccpToAngle(mDir)));
    mSprite->setAnchorPoint(cocos2d::ccp(0.5f, 0.5f));
    mSprite->setPosition(mPos);
    play->addChild(mSprite, LAYER_MAINROLE+1);
    
    SimpleAudioEngine::sharedEngine()->playEffect(cocos2d::CCFileUtils::sharedFileUtils()->fullPathForFilename("sound/starfall.mp3").c_str());
}

void Star::onUpdate(float delta) 
{
    cocos2d::Point np = mSprite->getPosition();
    np = ccpAdd(np, ccpMult(mDir, delta*400));
    mSprite->setPosition(np);
    GamePlay *play = GamePlay::sharedGamePlay();
    //伤害判定
cocos2d::Ref* node = NULL;
CCARRAY_FOREACH(play->enemies, node)
  {
    Role * em = (Role*)node;
        if( em->collisionWithCircle(mSprite->getPosition(), 20) )//杀伤半径
        {
            bool hit = em->deliverHit(HIT_MAGIC, mDir);
            play->manager->removeGameObject(this);
            if( hit )
            {
                SimpleAudioEngine::sharedEngine()->playEffect(cocos2d::CCFileUtils::sharedFileUtils()->fullPathForFilename("sound/hit.mp3").c_str());
                GTAnimatedEffect *hiteff = GTAnimatedEffect::create(GTAnimation::loadedAnimationSet("effect"), 1, false);
                hiteff->setAnchorPoint(cocos2d::ccp(0.5f, 0.5f));
                hiteff->setPosition(em->center());
                hiteff->setRotation(60 - CC_RADIANS_TO_DEGREES( ccpToAngle(mDir) ) + 60*CCRANDOM_0_1());
                play->addChild(hiteff, LAYER_MAINROLE+1);
                
                GTAnimatedEffect *hiteff2 = GTAnimatedEffect::create(GTAnimation::loadedAnimationSet("effect"), 11, false);
                hiteff2->setAnchorPoint(cocos2d::ccp(0.5f, 0.5f));
                hiteff2->setPosition(em->center());
                play->addChild(hiteff2, LAYER_ROLE);
                
                //achievement enemy burnt
                GameRecord::sharedGameRecord()->task->dispatchTask(ACH_ENEMYBURNT, 1);
            }
            break;
        }
    }
    //越界删除
    if( mSprite->getPosition().y < -100 )
    {
        play->manager->removeGameObject(this);
    }
}

void Star::onDestroy() 
{
    GamePlay::sharedGamePlay()->removeChild(mSprite, true);
}


