//
//  Mechanic.m
//  NinjiaRush
//
//  Created by 马 颂文 on 12-7-26.
//  Copyright (c) 2012年 TrinGame. All rights reserved.
//
#include "Mechanic.h"
#include "GameConstants.h"
#include "GamePlay.h"
#include "SimpleAudioEngine.h"
#include "Item.h"
#include "GameRecord.h"
#include "UniversalFit.h"

void Mechanic::onCreate() {
	mCollisionCircles.push_back(Circle(cocos2d::Vec2(6, 12), 9));
	mCollisionCircles.push_back(Circle(cocos2d::Vec2(17, 27), 13));

	Role::onCreate();

  switchStateDelegate(Entering, RoleStateDelegate::CreateDelegate<MechanicEnteringStateDelegate>());
  switchStateDelegate(Running, RoleStateDelegate::CreateDelegate<MechanicRunningStateDelegate>());
  switchStateDelegate(PreparingToShoot, RoleStateDelegate::CreateDelegate<MPreparingStateDelegate>());
  switchStateDelegate(Shooting, RoleStateDelegate::CreateDelegate<MechanicShootStateDelegate>());
  switchStateDelegate(Dead, RoleStateDelegate::CreateDelegate<DeadStateDelegate>());

  switchToState(Entering);
	mSpeed = 0.2f*ENEMY_NNRUNSPEED;
}

void Mechanic::onUpdate(float delta) 
{
  Role::onUpdate(delta);

  if (mStateDelegate[mState] != nullptr) {
    mStateDelegate[mState]->pre_update(delta);
    bool gameOver = handleGameOver(delta);
    if (!gameOver) {
      mStateDelegate[mState]->update(delta);
    }
    mStateDelegate[mState]->after_update(delta);
  }

  GamePlay* play = GamePlay::sharedGamePlay();
	//rush offset
	if( play->state == STATE_RUSH ) {
		float offset = (play->runspeed - play->levelspeed)*delta;
		cocos2d::Point np = mSprite->getPosition();
		np.x -= offset;
		mSprite->setPosition(np);
	}

  if (mSprite->getPosition().x < -LIMITE_OFFSET ||
      mSprite->getPosition().x > LIMITE_OFFSET+UniversalFit::sharedUniversalFit()->playSize.width)
  {
    play->manager->removeGameObject(this);
  }
}

//受到伤害
bool Mechanic::deliverHit(int type, cocos2d::Point dir) 
{
	GamePlay *play = GamePlay::sharedGamePlay();

	if( dir.x > 0 && type == HIT_DART )
	{
		//block darts
		GameTool::PlaySound("reflect.mp3");

		GTAnimatedEffect *hiteff2 = GTAnimatedEffect::create(GTAnimation::loadedAnimationSet("effect"), 2, false);
		hiteff2->setScale(0.5f);
		hiteff2->setAnchorPoint(cocos2d::Vec2(0.5f, 0.5f));
		hiteff2->setPosition(ccpAdd(center(), ccpMult(dir, -17)));
		play->addChild(hiteff2, LAYER_MAINROLE+1);

		return false;
	}

	if( mState != Dead )
	{
		mSprite->playGTAnimation(3, false);

		//combo
		bool isCombo = false;
		if( mState == PreparingToShoot || mState == Shooting )
		{
			play->makeCombo();
			isCombo = true;
		}
		// arcade combo
		if( play->mode == MODE_ARCADE )
		{
			if( isCombo )
			{
				play->arcade->killScore(MECHANIC_SCORE, play->combo, this->center());
			}
			else
			{
				play->arcade->killScore(MECHANIC_SCORE, 0, this->center());
			}
		}


		//add sp
		play->mainrole->gainSP(4);

		mState = Dead;
		mTimer = 0;
		mFlag = true;

		//achievement killing
		GameRecord::sharedGameRecord()->task->dispatchTask(ACH_KILLMECHANICS, 1);
		GameRecord::sharedGameRecord()->task->dispatchTask(ACH_KILLING, 1);
		play->killingCount++;
		if( play->runwithoutkill == 0 )
		{
			play->runwithoutkill = play->distance/PLAY_DISMETER;
		}

		return  true;    
	}
	return false;
}

cocos2d::Point Mechanic::center() 
{
	return ccpAdd(mSprite->getPosition(), Vec2(9, 20));
}

bool Mechanic::supportAimAid() 
{
	if( mState == Dead )
	{
		return false;
	}
	return  true;
}

void Mechanic::onDestroy() 
{
	GamePlay *play = GamePlay::sharedGamePlay();
	play->removeChild(mSprite, false);
}


