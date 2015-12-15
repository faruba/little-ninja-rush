//
//  PumpkinCircle.m
//  NinjiaRush
//
//  Created by 马 颂文 on 12-9-21.
//  Copyright (c) 2012年 TrinGame. All rights reserved.
//

#include "PumpkinCircle.h"
#include "GamePlay.h"
#include "Item.h"
#include "PumpkinFire.h"
#include "UniversalFit.h"
#include "StaticParticle.h"
#include "AnimatedParticle.h"
#include "GameRecord.h"

PumpkinCircle *gPumpkinInst = NULL;

#define PUMPKIN_GOLDTIME (1.5f)
#define PUMPKIN_FIRETIME (1.5f)
#define STEEL_RADIUS (10)

#define PUMPKIN_ROTATE (3.1415926f*2/1)
#define PUMPKIN_FLYRADIUS (50)
#define PUMPKIN_FLYSPEED (450)
#define PUMPKIN_LIFE (30)
#define PUMPKIN_FADE (2)



void PumpkinCircle::summonPumpkin() 
{
    GamePlay *play = GamePlay::sharedGamePlay();
    if( gPumpkinInst == NULL )
    {
        gPumpkinInst = PumpkinCircle::create();
        play->manager->addGameObject(gPumpkinInst);
    }
    int pumpk;
    if( play->mode == MODE_ARCADE )
    {
        pumpk = randomInt(2) + 1;
    }
    else
    {
        pumpk = randomInt(3);
    }
    gPumpkinInst->addPumpkin(pumpk);
    
    //achievement
    play->pumpkinspell++;
    GameRecord::sharedGameRecord()->task->dispatchTask(ACH_SPELLPUMPKIN, 1);
    GameRecord::sharedGameRecord()->task->dispatchTask(ACH_ONESPELLPUMPKIN, play->pumpkinspell);
    GameRecord::sharedGameRecord()->task->dispatchTask(ACH_SUMMONPUMPKIN, gPumpkinInst->mPumpkinCount);
}

void PumpkinCircle::dismissPumpkins() 
{
    if( gPumpkinInst != NULL )
    {
        for( int i=0; i<gPumpkinInst->mPumpkinCount; ++i )
        {
            gPumpkinInst->mTimers[i] = PUMPKIN_LIFE;
        }
    }
}

bool PumpkinCircle::init() 
{
  //init
  mPumpkinCount = 0;
  mCircieAngle = 0;
  return GameObject::init();
}

void PumpkinCircle::updateAngles() 
{
    if( mPumpkinCount != 0 )
    {
        float u = 3.1415926f*2/mPumpkinCount;
        for( int i=0; i<mPumpkinCount; ++i )
        {
            mAngles[i] = i*u;
        }
    }
}

void PumpkinCircle::addPumpkin(int type) 
{
    if( mPumpkinCount >= PUMPKIN_NUM )
    {
        return;
    }
    GamePlay *play = GamePlay::sharedGamePlay();
    mPumpkinTypes[mPumpkinCount] = type;
    switch (type) {
        case 0:
        {
            mSprites[mPumpkinCount] = CCSprite::createWithSpriteFrameName("pumpkingold.png");
        }
            break;
        case 1:
        {
            mSprites[mPumpkinCount] = CCSprite::createWithSpriteFrameName("pumpkinsteel.png");
        }
            break;
        case 2:
        {
            mSprites[mPumpkinCount] = CCSprite::createWithSpriteFrameName("pumpkinfire.png");
        }
            break;
    }
    mTimers[mPumpkinCount] = 0;
    mPowerTimers[mPumpkinCount] = 0;
    mSprites[mPumpkinCount]->setPosition(play->mainrole->position());
    play->addChild(mSprites[mPumpkinCount], LAYER_MAINROLE+1);
    
    mPumpkinCount++;
    this->updateAngles();
}

void PumpkinCircle::removePumpkin(int pid) 
{
    if( pid < mPumpkinCount )
    {
        //remove object
        GamePlay::sharedGamePlay()->removeChild(mSprites[pid], true);
        mSprites[pid] = NULL;
        
        //move data
        for(int i=pid; i<mPumpkinCount-1; ++i)
        {
            mSprites[i] = mSprites[i+1];
            mTimers[i] = mTimers[i+1];
            mPowerTimers[i] = mPowerTimers[i+1];
            mPumpkinTypes[i] = mPumpkinTypes[i+1];
        }
        mPumpkinCount--;
        this->updateAngles();
    }
    else
    {
        CCLog("Error: Invaild pumpkin id.");
    }
}

void PumpkinCircle::onCreate() 
{
    //do nothing
}

void PumpkinCircle::onUpdate(float delta) 
{
    GamePlay *play = GamePlay::sharedGamePlay();
    CCPoint mainpos = play->mainrole->center();
    //run the circle
    mCircieAngle += PUMPKIN_ROTATE*delta;
    
    for(int i=0; i<mPumpkinCount; ++i)
    {
        //update the target
        CCPoint target;
        {
            float curangle = mCircieAngle+mAngles[i];
            CCPoint radptr = ccpForAngle(curangle);
            target = ccpAdd(mainpos, ccpMult(radptr, PUMPKIN_FLYRADIUS));
        }
        //move to target
        {
            CCPoint dp = ccpSub(target, mSprites[i]->getPosition());
            float dis = ccpLength(dp);
            float step = PUMPKIN_FLYSPEED*delta;
            if( step < dis )
            {
                CCPoint dir = ccpNormalize(dp);
                CCPoint np = ccpAdd(mSprites[i]->getPosition(), ccpMult(dir, step));
                mSprites[i]->setPosition(np);
            }
            else
            {
                mSprites[i]->setPosition(target);
            }
        }
        
        //update each pumpkin
        mTimers[i] += delta;
        if( mTimers[i] > PUMPKIN_LIFE && mTimers[i] <= PUMPKIN_LIFE + PUMPKIN_FADE )
        {
            float k = 1 - (mTimers[i] - PUMPKIN_LIFE) / PUMPKIN_FADE;
            mSprites[i]->setOpacity(255*k);
        }
        if( mTimers[i] > PUMPKIN_LIFE + PUMPKIN_FADE )
        {
            this->removePumpkin(i);
            break;
        }
        switch( mPumpkinTypes[i] )
        {
            case 0://gold
            {
                mPowerTimers[i] += delta;
                if( mPowerTimers[i] > PUMPKIN_GOLDTIME )
                {
                    mPowerTimers[i] = 0;
                    //pop a gold
                    play->manager->addGameObject(Item::item(0, mSprites[i]->getPosition(), play, false));
                }
            }
                break;
            case 1://steel
            {
              bool hit = false;
              CCObject* node = NULL;
              CCARRAY_FOREACH(play->darts, node)
              {
                Darts *dr = (Darts*)node;
                if( dr->isEnemy() && exCollisionWithCircles(mSprites[i]->getPosition(), 0, 0, STEEL_RADIUS, dr->position(), 15) )
                {
                  dr->onHitback(play->mainrole->center());
                  hit = true;
                }
              }
              if( hit )
              {
                SimpleAudioEngine::sharedEngine()->playEffect(CCFileUtils::sharedFileUtils()->fullPathForFilename("sound/reflect.mp3").c_str());
              }
            }
                break;
            case 2://fire
            {
                mPowerTimers[i] += delta;
                if( mPowerTimers[i] > PUMPKIN_FIRETIME )
                {
                    //find a target
                    bool found = false;
                    CCPoint target;
                    {
  CCObject* node = NULL;
  CCARRAY_FOREACH(play->enemies, node)
  {
    Role * em = (Role*)node;
                            if( em->supportAimAid() && em->position().x > 0 && em->position().x < UniversalFit::sharedUniversalFit()->playSize.width )
                            {
                                found = true;
                                target = em->position();
                            } 
                        }

                    }
                    if( found )
                    {//ready to fire
                        mPowerTimers[i] = 0;
                        CCPoint dir = ccpSub(target, mSprites[i]->getPosition());
                        dir = ccpNormalize(dir);
                        play->manager->addGameObject(PumpkinFire::dart(mSprites[i]->getPosition(), dir, play));
                    }
                }
            }
                break;
        }
    }
    
    //clean up check
    if( mPumpkinCount == 0 )
    {
        play->manager->removeGameObject(this);
    }
}

void PumpkinCircle::onDestroy() 
{
    while(mPumpkinCount!=0)
    {
        this->removePumpkin(0);
    }
    gPumpkinInst = NULL;
}


