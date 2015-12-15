//
//  Samuri.m
//  NinjiaRush
//
//  Created by 马 颂文 on 12-4-24.
//  Copyright (c) 2012年 TrinGame. All rights reserved.
//

#include "Samuri.h"
#include "GameConstants.h"
#include "GamePlay.h"
#include "SimpleAudioEngine.h"
#include "GameRecord.h"
#include "UniversalFit.h"

Samuri* Samuri::role(CCNode * parent) 
{
    Samuri *em = Samuri::create();
    em->mParent = parent;
    return em;
}

void Samuri::onCreate() 
{
    GamePlay *play = GamePlay::sharedGamePlay();
    mSprite = GTAnimatedSprite::spriteWithGTAnimation(GTAnimation::loadedAnimationSet("samurai"));
    mSprite->setAnchorPoint(ccp(0.694f, 0.08125f));
    mSprite->setPosition(ccp(UniversalFit::sharedUniversalFit()->playSize.width+play->runspeed*SAMURAI_WARNING, PLAY_PLAYERLINE));
    mSprite->playGTAnimation(0, true);
    mParent->addChild(mSprite, LAYER_ROLE);
    
    //add smark
    //mMark = CCSprite::createWithSpriteFrameName("smark.png");
    //mMark->setAnchorPoint(ccp( 0.5f, 0.5f));
    //mMark->setPosition(ccp( SCREEN_WIDTH/2, SCREEN_HEIGHT/2 ));
    //mParent->addChild(mMark, LAYER_UI);
    //CCBlink *blink = CCBlink::create(SAMURAI_WARNING, SAMURAI_WARNING*5);
    //mMark->runAction(blink);
    mHint = GTAnimatedSprite::spriteWithGTAnimation(GTAnimation::loadedAnimationSet("misc"));
    mHint->playGTAnimation(1, true);
    mHint->setAnchorPoint(ccp(1, 0.5f));
    mHint->setPosition(ccp(UniversalFit::sharedUniversalFit()->playSize.width, PLAY_PLAYERLINE+20));
    mParent->addChild(mHint, LAYER_ROLE);
    CCMoveBy *mb1 = CCMoveBy::create(0.2f, ccp(-20, 0));
    CCMoveBy *mb2 = CCMoveBy::create(0.2f, ccp(20, 0));
    CCSequence *sq = CCSequence::create(mb1,mb2, NULL);
    CCRepeatForever *rp = CCRepeatForever::create(sq);
    mHint->runAction(rp);
    //CCBlink *blink2 = CCBlink::create(SAMURAI_WARNING, SAMURAI_WARNING*5);
    //mHint->runAction(blink2);
    
    mState = 0;
    mTimer = 0;
    mFlag = false;
    
    //play samurai warning
    SimpleAudioEngine::sharedEngine()->playEffect(CCFileUtils::sharedFileUtils()->fullPathForFilename("sound/samurai_warning.mp3").c_str());
}

void Samuri::onUpdate(float delta) 
{
    GamePlay *play = GamePlay::sharedGamePlay();
    bool playend = mSprite->updateGTAnimation(delta);
    //fix pos
    CCPoint np = mSprite->getPosition();
    np.x -= play->runspeed*delta;
    mSprite->setPosition(np);
    switch (mState) {
        case 0:
        {
            if( !mFlag && mSprite->getPosition().x < UniversalFit::sharedUniversalFit()->playSize.width )
            {
                SimpleAudioEngine::sharedEngine()->playEffect(CCFileUtils::sharedFileUtils()->fullPathForFilename("sound/samurai_attack.mp3").c_str());
                mFlag = true;
            }
            float rds = play->runspeed*0.5f;
            if( mSprite->getPosition().x - play->mainrole->position().x < rds )
            {
                mState = 1;
                mSprite->playGTAnimation(1, false);
                mFlag = false;
            }
        }
            break;
        case 1:
        {
            if( mFlag == false )
            {
                bool hit = false;
                if( mSprite->getPosition().x < play->mainrole->position().x )
                {
                    hit = true;
                }
                if( play->mainrole2 != NULL && mSprite->getPosition().x < play->mainrole2->position().x )
                {
                    hit = true;
                }
                if( playend )
                {
                    hit = true;
                }
                if( hit )
                {
                    SimpleAudioEngine::sharedEngine()->playEffect(CCFileUtils::sharedFileUtils()->fullPathForFilename("sound/hit").c_str());
                    play->mainrole->deliverHit(HIT_BLADE, ccp(0, 0));//武士总是对付第一个人物造成伤害
                    mFlag = true;
                }
            }
        }
            break;
        case 2:
        {
            if(mFlag)
            {
                mTimer += delta;
                if(mTimer>0.3f)
                {
                    mTimer = 0;
                    mFlag = false;
                    SimpleAudioEngine::sharedEngine()->playEffect(CCFileUtils::sharedFileUtils()->fullPathForFilename("sound/samurai_die.mp3").c_str());
                }
            }
        }
            break;
    }
    //clean mark
    if( mHint != NULL && mSprite->getPosition().x < UniversalFit::sharedUniversalFit()->playSize.width )
    {
        //mParent->removeChild(mMark, true);
        mParent->removeChild(mHint, true);
        //mMark = NULL;
        mHint = NULL;
    }
    if( mHint != NULL )
    {
        mHint->updateGTAnimation(delta);
    }
    //clean this
    if( mSprite->getPosition().x < -100 )
    {
        play->enemies->removeObject(this);
        play->manager->removeGameObject(this);
        play->flag_samurai = false;
    }
}

void Samuri::onDestroy() 
{
    //clean mark
    if( mHint != NULL )
    {
        //mParent->removeChild(mMark, true);
        mParent->removeChild(mHint, true);
        //mMark = NULL;
        mHint = NULL;
    }
    mParent->removeChild(mSprite, true);
}

//碰撞检测
bool Samuri::collisionWithCircle(CCPoint cc, float rad) 
{
    if( mState < 2 )
    {
        if( mSprite != NULL )
        {
            if( exCollisionWithCircles(mSprite->getPosition(), 1, 10, 15, cc, rad) ||
               exCollisionWithCircles(mSprite->getPosition(), 0, 31, 14, cc, rad) )
            {
                return true;
            }
        }
    }
    return false;
}

bool Samuri::deliverHit(int type, CCPoint dir) 
{
    if( mState < 2 )
    {
        SimpleAudioEngine::sharedEngine()->playEffect(CCString::createWithFormat("ahh%d.mp3", (randomInt(3)+1))->getCString());
        mState = 2;
        mSprite->playGTAnimation(2, false);
        mFlag = true;
        mTimer = 0;
        
        GamePlay *play = GamePlay::sharedGamePlay();
        play->mainrole->gainSP(5);
        
        //combo
        if( play->combo > 0 )
        {
            play->refreshCombo();
        }
        
        //arcade bouns
        if( play->mode == MODE_ARCADE )
        {
            play->arcade->killScore(SAMURAI_SCORE, 0, this->center());
        }
        
        
        //achievement killing
        GameRecord::sharedGameRecord()->task->dispatchTask(ACH_KILLING, 1);
        play->killingCount++;
        GameRecord::sharedGameRecord()->task->dispatchTask(ACH_KILLSAMURAI, 1);
        if( type == HIT_DART )
        {
            GameRecord::sharedGameRecord()->task->dispatchTask(ACH_DARTONGROUND, 1);
        }
        play->killsamurai++;
        GameRecord::sharedGameRecord()->task->dispatchTask(ACH_ONEKILLSAMURAI, play->killsamurai);
        if( play->roleId == 2 )
        {
            GameRecord::sharedGameRecord()->task->dispatchTask(ACH_ONEMUSASHIKILLSAMURAI, play->killsamurai);
            GameRecord::sharedGameRecord()->task->dispatchTask(ACH_MUSASHIKILLSAMURAI, 1);
        }
        
        if( play->runwithoutkill == 0 )
        {
            play->runwithoutkill = play->distance/PLAY_DISMETER;
        }
        
        return true;
    }
    return false;
}

CCPoint Samuri::position() 
{
    return mSprite->getPosition();
}

void Samuri::setPosition(CCPoint pos) 
{
    mSprite->setPosition(pos);
}

CCPoint Samuri::center() 
{
    return ccpAdd(mSprite->getPosition(), ccp(9, 20));
}

bool Samuri::supportAimAid() 
{
    return false;
}

void Samuri::toggleVisible(bool flag) 
{
    mSprite->setVisible(flag);
}


