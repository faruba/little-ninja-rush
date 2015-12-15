//
//  BombDart.m
//  NinjiaRush
//
//  Created by 马 颂文 on 12-6-25.
//  Copyright (c) 2012年 TrinGame. All rights reserved.
//

#include "BombDart.h"
#include "GamePlay.h"

#define BOMB_FLYTIME (1)



BombDart* BombDart::dart(CCPoint pos, CCPoint dir) 
{
    BombDart *ret = BombDart::create();
    ret->mPos = pos;
    ret->mDir = dir;
    return ret;
}

void BombDart::onCreate() 
{
    //创建sprite
    mSprite = GTAnimatedSprite::spriteWithGTAnimation(GTAnimation::loadedAnimationSet("bullets"));
    mSprite->playGTAnimation(1, true);
    mSprite->setAnchorPoint(ccp(0.5f, 0.5f));
    mSprite->setPosition(mPos);
    CCRotateBy* rb = CCRotateBy::create(0.5f, 720);
    CCRepeatForever* rf = CCRepeatForever::create(rb);
    mSprite->runAction(rf);
    GamePlay::sharedGamePlay()->addChild(mSprite, LAYER_MAINROLE+1);
    //计算落点
    float dsty = RESPAWN_YMIN+RESPAWN_Y/2 - mPos.y;
    float k = dsty/mDir.y;
    mDst = ccpAdd(mPos, ccpMult(mDir, k));
    mMid.x = (mPos.x + mDst.x)/2;
    mMid.y = 30 + dsty + CCRANDOM_0_1()*50;
    mTimer = 0;
    SimpleAudioEngine::sharedEngine()->playEffect(CCFileUtils::sharedFileUtils()->fullPathForFilename("sound/bomb1.mp3").c_str());
    
    mPaused = false;
}

void BombDart::onUpdate(float delta) 
{
    GamePlay* play = GamePlay::sharedGamePlay();
    //暂停和恢复动作
    if( play->paused && !mPaused )
    {
        mPaused = true;
        mSprite->pauseSchedulerAndActions();
    }
    if( mPaused && !play->paused )
    {
        mPaused = false;
        mSprite->resumeSchedulerAndActions();
    }
    mSprite->updateGTAnimation(delta);
    mTimer += delta;
    if( mTimer < BOMB_FLYTIME )
    {//fly
        float k = mTimer/BOMB_FLYTIME;
        CCPoint a = ccpLerp(mPos, mMid, k);
        CCPoint b = ccpLerp(mMid, mDst, k);
        CCPoint np = ccpLerp(a, b, k);
        mSprite->setPosition(np);
    }
    else {//explosive
        mSprite->setPosition(mDst);
        float minx = mSprite->getPosition().x - 75;
        float maxx = mSprite->getPosition().x + 75;
        //进行伤害判断
  CCObject* node = NULL;
  CCARRAY_FOREACH(play->enemies, node)
  {
    Role * em = (Role*)node;
            if( em->position().x > minx && em->position().x < maxx && em->supportAimAid() )//杀伤半径
            {
                bool hit = em->deliverHit(HIT_BOMB, mDir);
                if( hit )
                {
                    SimpleAudioEngine::sharedEngine()->playEffect(CCFileUtils::sharedFileUtils()->fullPathForFilename("sound/hit.mp3").c_str());
                    GTAnimatedEffect *hiteff = GTAnimatedEffect::create(GTAnimation::loadedAnimationSet("effect"), 1, false);
                    hiteff->setAnchorPoint(ccp(0.5f, 0.5f));
                    hiteff->setPosition(em->center());
                    //hiteff->setRotation(90 - CC_RADIANS_TO_DEGREES( ccpToAngle(direction) ));
                    hiteff->setRotation(60 - CC_RADIANS_TO_DEGREES( ccpToAngle(mDir) ) + 60*CCRANDOM_0_1());
                    play->addChild(hiteff, LAYER_MAINROLE+1);
                    
                    GTAnimatedEffect *hiteff2 = GTAnimatedEffect::create(GTAnimation::loadedAnimationSet("effect"), 2, false);
                    hiteff2->setAnchorPoint(ccp(0.5f, 0.5f));
                    hiteff2->setPosition(em->center());
                    play->addChild(hiteff2, LAYER_ROLE);
                    
                }
            }
        }
        GTAnimatedEffect *eff = GTAnimatedEffect::create(GTAnimation::loadedAnimationSet("bullets"), 0, false);
        eff->setAnchorPoint(ccp(0.5f, 0));
        CCPoint effpos = mSprite->getPosition();
        effpos.y -= 20;
        eff->setPosition(effpos);
        play->addChild(eff, LAYER_ROLE);
        SimpleAudioEngine::sharedEngine()->playEffect(CCFileUtils::sharedFileUtils()->fullPathForFilename("sound/bomb2.mp3").c_str());
        play->manager->removeGameObject(this);
    }
}

void BombDart::onDestroy() 
{
    GamePlay::sharedGamePlay()->removeChild(mSprite, true);
}


