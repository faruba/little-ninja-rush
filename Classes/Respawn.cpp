//
//  Respawn.m
//  NinjiaRush
//
//  Created by 马 颂文 on 12-5-5.
//  Copyright (c) 2012年 TrinGame. All rights reserved.
//

#include "Respawn.h"
#include "GamePlay.h"
#include "NewbieNinja.h"
#include "MiddleNinja.h"
#include "HighNinja.h"
#include "Messager.h"
#include "Samuri.h"
#include "Merchant.h"
#include "Archer.h"
#include "Mechanic.h"
#include "Pumpkin.h"
#include "Santa.h"
#include "UniversalFit.h"
#include "GameScene.h"

//*** Classic Mode ***
#define ENEMIES (6)
#define LEVELCOUNT (10)

int gLevel[LEVELCOUNT][ENEMIES] =
{
    {88, 10,  2,  0,  0,  0},
    {85, 10,  3,  1,  1,  0},
    {78, 12,  5,  2,  2,  1},
    {70, 15,  7,  3,  3,  2},
    {50, 20, 15,  4,  6,  5},
    {40, 22, 17,  6,  8,  7},
    {30, 24, 19,  8, 10,  9},
    {20, 26, 21, 10, 12, 11},
    {15, 29, 23, 10, 12, 11},
    {10, 32, 25, 10, 12, 11},
};

//*** Arcade Mode ***
#define ARCLEVCOUNT (9)

int gArcLev[ARCLEVCOUNT][ENEMIES] =
{
    {88, 10,  2,  0,  0,  0},
    {85, 10,  3,  1,  1,  0},
    {78, 12,  5,  2,  2,  1},
    {70, 15,  7,  3,  3,  2},
    {50, 20, 15,  4,  6,  5},
    {40, 22, 17,  6,  8,  7},
    {30, 24, 19,  8, 10,  9},
    {20, 26, 21, 10, 12, 11},
    {15, 29, 23, 10, 12, 11},
};

float gCd[ENEMIES] = 
{ 
    NNINJA_RESPAWNCD,//下忍 0
    MNINJA_RESPAWNCD,//中忍 1
    HNINJA_RESPAWNCD,//上忍 2
    SAMURAI_RESPAWNCD,//武士 3
    ARCHER_RESPAWNCD,//弓箭手 4
    MECHANIC_RESPAWNCD,//机械师 5
};



void Respawn::setPipleLine(int pc) 
{
    if( pc > MAXPIPLECOUNT )
    {
        pc = MAXPIPLECOUNT;
    }
    int old = mPiple;
    mPiple = pc;
    CCLog("PIPLE = %d", mPiple);//hammer debug
    for(int i=old; i<pc; ++i)
    {
        mPT[i] = i - old;
    }
}

void Respawn::setMode(int mod) 
{
    mMode = mod;
    if( mMode == MODE_CLASSIC )
    {
        mHalloween = UniversalFit::isDuringHalloween();
        mChristmas = UniversalFit::isDuringChristmas();
    }
    else
    {
        mHalloween = false;
        mChristmas = false;
    }
}

void Respawn::setLevel(int lv) 
{
    if( mMode == MODE_ARCADE )
    {//Arcade Mode
        mLevel = lv-1;
        mReady = -1;
        if( mLevel >= ARCLEVCOUNT )
        {
            mLevel = ARCLEVCOUNT-1;
        }
    }
    else
    {//Classic Mode
        mLevel = lv-1;
        mMerchantTimer = 0;
        this->setPipleLine(2+mLevel);
        
        mReady = -1;
        for(int i=0; i<mPiple; ++i)
        {
            mPT[i] = 2+i;
        }
        
        if( mLevel >= LEVELCOUNT )
        {
            mLevel = LEVELCOUNT-1;
            
        }
        if( mLevel == 0 )
        {//重新开始游戏
            mFlagMerchant = false;
        }
        
        //设定信使
        mMessagerPos[0] = -1;
        mMessagerPos[1] = -1;
        int n = randomInt(100);
        if( n < 70 )
        {
            mMessagerPos[0] = mLevel*PLAY_SCENELEN*PLAY_DISMETER + (0.15f + 0.7f*CCRANDOM_0_1())*PLAY_SCENELEN*PLAY_DISMETER;
            mMessagerPos[1] = -1;
        }
        else if ( n < 80 ) {
            mMessagerPos[0] = mLevel*PLAY_SCENELEN*PLAY_DISMETER + (0.15f + 0.7f*CCRANDOM_0_1())*PLAY_SCENELEN*PLAY_DISMETER;
            mMessagerPos[1] = mLevel*PLAY_SCENELEN*PLAY_DISMETER + (0.15f + 0.7f*CCRANDOM_0_1())*PLAY_SCENELEN*PLAY_DISMETER;
        }
        //设定商人
        if( !mFlagMerchant && randomInt(100) <10 && mLevel>0 )
        //if( !mFlagMerchant && randomInt(100) <100)//test code remains
        {
            mMerchant = mLevel*PLAY_SCENELEN*PLAY_DISMETER + (0.15f + 0.6f*CCRANDOM_0_1())*PLAY_SCENELEN*PLAY_DISMETER;
            //mMerchant = 100;//test code remains
            mMerchantTimer = 0;
        }
        else {
            mMerchant = -1;
        }
        //Halloween Pumpkin
        mFestivalEnemy = -1;
        {
            //halloween check
            {
                if( GameScene::getCurrentSceneId() == 5 )
                {
                    mFestivalEnemy = mLevel*PLAY_SCENELEN*PLAY_DISMETER + ( 0.6f*CCRANDOM_0_1())*PLAY_SCENELEN*PLAY_DISMETER;
                    mFestivalRole = 0;
                }
                else if( mHalloween )
                {
                    if( CCRANDOM_0_1() < 0.3f )
                    {
                        mFestivalEnemy = mLevel*PLAY_SCENELEN*PLAY_DISMETER + ( 0.6f*CCRANDOM_0_1())*PLAY_SCENELEN*PLAY_DISMETER;
                        mFestivalRole = 0;
                    }
                }
            }
            //christmas check
            {
                if( GameScene::getCurrentSceneId() == 6 )
                {
                    mFestivalEnemy = mLevel*PLAY_SCENELEN*PLAY_DISMETER + ( 0.6f*CCRANDOM_0_1())*PLAY_SCENELEN*PLAY_DISMETER;
                    mFestivalRole = 1;
                }
                else if( mChristmas )
                {
                    if( CCRANDOM_0_1() < 0.3f )
                    {
                        mFestivalEnemy = mLevel*PLAY_SCENELEN*PLAY_DISMETER + ( 0.6f*CCRANDOM_0_1())*PLAY_SCENELEN*PLAY_DISMETER;
                        mFestivalRole = 1;
                    }
                }
            }
        }
    }
}

void Respawn::updateClassic(float delta) 
{
    GamePlay *play = GamePlay::sharedGamePlay();
    if( play->count_respawn > 0 )
    {
        return;
    }
    
    //时间膨胀
    delta *= play->runspeed/play->levelspeed;
    
    if( mReady >= 0 )
    {
        if( mReady == ENEMIES+1 )
        {//刷商人
            mMerchantTimer += delta;
            if( play->enemies->count() == 0 )
            {
                this->gen(mReady);
                mFlagMerchant = true;
                mReady = -1;
            }
            if( mMerchantTimer > MERCHANT_WAIT+mLevel-1 )
            {
                mReady = -1;
            }
        }
        else if( play->enemies->count() < play->maxenemy )
        {
            this->gen(mReady);
            mReady = -1;
        }
    }
    //刷信使
    for( int i=0; i<2; ++i)
    {
        if( mMessagerPos[i] >= 0 && play->distance>mMessagerPos[i] )
        {
            this->gen(ENEMIES);//刷信使
            mMessagerPos[i] = -1;
        }
    }
    //刷商人
    if( mMerchant >= 0 && play->distance>mMerchant )
    {
        mReady = ENEMIES+1;
        mMerchantTimer = 0;
        mMerchant = -1;
    }
    //Halloween Pumpkin
    if( mFestivalEnemy >= 0 && play->distance > mFestivalEnemy )
    {
        mReady = ENEMIES+2;
        mFestivalEnemy = -1;
    }
    //run piple
    for(int i=0; i<mPiple; ++i)
    {
        if( mPT[i] > 0 )
        {
            mPT[i] -= delta;
        }
        if( mPT[i] <= 0 && mReady<0 )
        {
            bool accept = false;
            int tar = -1;
            while ( !accept )
            {
                //刷出概率
                int n = randomInt(100);
                int base = 0;
                for( int j=0; j<ENEMIES; ++j)
                {
                    if( n >= base && n < base+gLevel[mLevel][j] )
                    {
                        if( this->judge(j) )
                        {
                            accept = true;
                            tar = j;
                            break;
                        }
                    }
                    base += gLevel[mLevel][j];
                }
            }
            mReady = tar;
            mPT[i] = gCd[tar]/*/play->difficulty*/;//不再根据难度系数调整
        }
    }
}

void Respawn::updateArcade(float delta) 
{
    if( mPiple > 100 )
    {
        CCLog("XXX PIPLE = %d", mPiple);
        assert(false);
    }
    
    GamePlay *play = GamePlay::sharedGamePlay();
    if( play->count_respawn > 0 )
    {
        return;
    }
    
    //时间膨胀
    delta *= play->runspeed/play->levelspeed;
    
    if( mReady >= 0 )
    {
        if( play->enemies->count() < play->maxenemy )
        {
            this->gen(mReady);
            mReady = -1;
        }
    }
    //run piple
    for(int i=0; i<mPiple; ++i)
    {
        if( mPT[i] > 0 )
        {
            mPT[i] -= delta;
        }
        if( mPT[i] <= 0 && mReady<0 )
        {
            bool accept = false;
            int tar = -1;
            while ( !accept )
            {
                //刷出概率
                int n = randomInt(100);
                int base = 0;
                for( int j=0; j<ENEMIES; ++j)
                {
                    if( n >= base && n < base+gArcLev[mLevel][j] )
                    {
                        if( this->judge(j) )
                        {
                            accept = true;
                            tar = j;
                            break;
                        }
                    }
                    base += gArcLev[mLevel][j];
                }
            }
            mReady = tar;
            mPT[i] = gCd[tar]/*/play->difficulty*/;//不再根据难度系数调整
        }
    }
}

bool Respawn::judge(int tid) 
{
    GamePlay *play = GamePlay::sharedGamePlay();
    if( tid == 3 )
    {//武士的判定
        if( play->flag_samurai )
        {
            return false;
        }
        if( play->distance > play->changeflag - play->runspeed*5 )
        {
            return false;
        }
    }
    
    return true;
}

void Respawn::gen(int tid) 
{
  GamePlay *play = GamePlay::sharedGamePlay();
  switch (tid) {
    case 0://下忍
      {
        play->enemies->addObject(play->manager->addGameObject(NewbieNinja::role(play)));
      }
      break;
    case 1://中忍
      {
        play->enemies->addObject(play->manager->addGameObject(MiddleNinja::role(play)));
      }
      break;
    case 2://上忍
      {
        play->enemies->addObject(play->manager->addGameObject(HighNinja::role(play)));
      }
      break;
    case 3://武士
      {
        play->enemies->addObject(play->manager->addGameObject(Samuri::role(play)));
        play->flag_samurai = true;
      }
      break;
    case 4://弓箭手
      {
        play->manager->addGameObject(Archer::role(play));
      }
      break;
    case 5://神工
      {
        play->enemies->addObject(play->manager->addGameObject(Mechanic::role()));
      }
      break;
    case ENEMIES://信使
      {
        play->enemies->addObject(play->manager->addGameObject(Messager::role(play)));
      }
      break;
    case ENEMIES+1://商人
      {
        play->manager->addGameObject(Merchant::role(play));
        play->count_respawn++;
      }
      break;
    case ENEMIES+2://mFestivalEnemy
      {
        switch (mFestivalRole) {
          case 0://halloween
            {
              play->enemies->addObject(play->manager->addGameObject(Pumpkin::role(play)));
            }
            break;
          case 1:
            {
              play->enemies->addObject(play->manager->addGameObject(Santa::role(play)));
            }
            break;
        }
      }
      break;
  }
}
