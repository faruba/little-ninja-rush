//
//  MiddleNinja.m
//  NinjiaRush
//
//  Created by 马 颂文 on 12-5-4.
//  Copyright (c) 2012年 TrinGame. All rights reserved.
//

#include "MiddleNinja.h"
#include "GameConstants.h"
#include "GamePlay.h"
#include "Item.h"
#include "Bomb.h"
#include "SimpleAudioEngine.h"
#include "GameRecord.h"
#include "UniversalFit.h"
#include "FootPrint.h"


void MiddleNinja::onCreate() {
	mCollisionCircles.push_back(Circle(cocos2d::Vec2(6, 12), 9));
	mCollisionCircles.push_back(Circle(cocos2d::Vec2(17, 27), 13));

	Role::onCreate();

  NinjaRunningStateDelegate *running = RoleStateDelegate::CreateDelegate<NinjaRunningStateDelegate>();
  running->mPollTime = MNINJA_POLLTIME;
  running->mAggressive = MNINJA_AGGRISIVE;

  PreparingStateDelegate *preparing = RoleStateDelegate::CreateDelegate<PreparingStateDelegate>();
  preparing->mPreparingTime = MNINJA_PREPARE;

  switchStateDelegate(Repositioning, RoleStateDelegate::CreateDelegate<RepositioningStateDelegate>());
  switchStateDelegate(Entering, RoleStateDelegate::CreateDelegate<MiddleNinjaEnteringStateDelegate>());
  switchStateDelegate(Running, running);
  switchStateDelegate(PreparingToShoot, preparing);
  switchStateDelegate(Shooting, RoleStateDelegate::CreateDelegate<MShootStateDelegate>());
  switchStateDelegate(Fleeing, RoleStateDelegate::CreateDelegate<FleeStateDelegate>());
  switchStateDelegate(Dead, RoleStateDelegate::CreateDelegate<DeadStateDelegate>());

  switchToState(Entering);

	mDartCount = 0;
  mDartCapacity = MNINJA_MAXDART;
	mSpeed = ENEMY_NNRUNSPEED*3;
}

void MiddleNinja::onUpdate(float delta) 
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

	if( mState != Dead ) {
		FootPrint::goFootPrint(&mStepSnow, mSprite->getPosition());
	}

  if (mSprite->getPosition().x < -LIMITE_OFFSET ||
      mSprite->getPosition().x > LIMITE_OFFSET+UniversalFit::sharedUniversalFit()->playSize.width)
  {
    play->manager->removeGameObject(this);
  }
}

//受到伤害
bool MiddleNinja::deliverHit(int type, cocos2d::Point dir) 
{
	if( mState == Dead )
	{
		mSprite->playGTAnimation(4, false);
	}
	else {
		if( dir.x > 0 )
		{
			mSprite->playGTAnimation(3 , false);
		}
		else {
			mSprite->playGTAnimation(1+randomInt(2 ), false);
		}
	}
	GamePlay *play = GamePlay::sharedGamePlay();
	//combo
	bool isCombo = false;
	if( mState == PreparingToShoot || mState == Fleeing )
	{
		play->makeCombo();
		isCombo = true;
	}
	// arcade combo
	if( play->mode == MODE_ARCADE )
	{
		if( isCombo )
		{
			play->arcade->killScore(MNINJA_SCORE, play->combo, this->center());
		}
		else
		{
			play->arcade->killScore(MNINJA_SCORE, 0, this->center());
		}
	}

	//SP
	play->mainrole->gainSP(3);

	mState = Dead;
	//随机掉落道具
	Item::triggerItem(1, mSprite->getPosition());

	//achievement killing
	GameRecord::sharedGameRecord()->task->dispatchTask(ACH_KILLING, 1);
	play->killingCount++;
	GameRecord::sharedGameRecord()->task->dispatchTask(ACH_KILLCHUNIN, 1);
	if( type == HIT_BOMB )
	{
		GameRecord::sharedGameRecord()->task->dispatchTask(ACH_CHUNINBYBOMB, 1);
	}
	if( play->runwithoutkill == 0 )
	{
		play->runwithoutkill = play->distance/PLAY_DISMETER;
	}

	return true;
}

cocos2d::Point MiddleNinja::center() 
{
	return ccpAdd(mSprite->getPosition(), Vec2(9, 20));
}

bool MiddleNinja::supportAimAid() {
	return !( mState == Dead || mState == Initializing || mState == Entering );
}

void MiddleNinja::onDestroy() 
{
	GamePlay *play = GamePlay::sharedGamePlay();
	play->enemies->removeObject(this);
	mParent->removeChild(mSprite, false);
}
