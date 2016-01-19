//
//  HighNinja.m
//  NinjiaRush
//
//  Created by 马 颂文 on 12-5-9.
//  Copyright (c) 2012年 TrinGame. All rights reserved.
//

#include "HighNinja.h"
#include "GameConstants.h"
#include "GamePlay.h"
#include "SimpleAudioEngine.h"
#include "PEffect.h"
#include "Item.h"
#include "GameRecord.h"
#include "UniversalFit.h"
#include "FootPrint.h"

void HighNinja::onCreate() {
  mCollisionCircles.push_back(Circle(cocos2d::Vec2(6, 12), 9));
  mCollisionCircles.push_back(Circle(cocos2d::Vec2(17, 27), 13));

  Role::onCreate();

  NinjaRunningStateDelegate *running = RoleStateDelegate::CreateDelegate<NinjaRunningStateDelegate>();
  running->mPollTime = HNINJA_POLLTIME;
  running->mAggressive = HNINJA_AGGRISIVE;

  PreparingStateDelegate *preparing = RoleStateDelegate::CreateDelegate<PreparingStateDelegate>();
  preparing->mPreparingTime = HNINJA_PREPARE;

  switchStateDelegate(Repositioning, RoleStateDelegate::CreateDelegate<RepositioningStateDelegate>());
  switchStateDelegate(Entering, RoleStateDelegate::CreateDelegate<HighNinjaEnteringStateDelegate>());
  switchStateDelegate(Running, running);
  switchStateDelegate(PreparingToShoot, preparing);
  switchStateDelegate(Shooting, RoleStateDelegate::CreateDelegate<HShootStateDelegate>());
  switchStateDelegate(Fleeing, RoleStateDelegate::CreateDelegate<FleeStateDelegate>());
  switchStateDelegate(Dummy, RoleStateDelegate::CreateDelegate<DummyStateDelegate>());
  switchStateDelegate(Dead, RoleStateDelegate::CreateDelegate<HDeadStateDelegate>());


  switchToState(Entering);

  mDartCount = 0;
  mDartCapacity = MNINJA_MAXDART;
  mFlag = true;
  mSpeed = ENEMY_NNRUNSPEED;
  mTimer = 0;
  mSpell = 0;
  mIsCoward = false;


  mWoodLink.uid=-1;
  mWoodLink.index=-1;
}

void HighNinja::onUpdate(float delta) 
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
bool HighNinja::deliverHit(int type, cocos2d::Point dir) 
{
	GamePlay *play = GamePlay::sharedGamePlay();
	//替身术
	if( mState != Dead && mState != PreparingToShoot && mState != Shooting && mSpell == 0 )
	{
		mSpell = 1;
		//发动替身术
		GameTool::PlaySound("smoke.mp3");
		GTAnimatedEffect *eff = GTAnimatedEffect::create(GTAnimation::loadedAnimationSet("effect"), 5, false);
		eff->setAnchorPoint(mSprite->getAnchorPoint());
		eff->setPosition(mSprite->getPosition());
		mParent->addChild(eff, LAYER_ROLE+RESPAWN_Y+RESPAWN_YMIN-mSprite->getPosition().y);
		mState = Dummy;
		mTimer = 0;
		mPoint = dir;

		//-------
		mSprite->setVisible(false);
		Wood*w = Wood::wood(this->center(), mPoint, mParent, this->handler(), mSprite->getAnchorPoint());
		mWoodLink = w->handler();
		play->manager->addGameObject(w);
		//-------
		return false;
	}
	mTimer = 0;
	if( mState == Dead )
	{
		mSprite->playGTAnimation(4, false);
		mFlag = false;
	}
	else {
		if( dir.x > 0 )
		{
			mSprite->playGTAnimation(3, false);
		}
		else {
			mSprite->playGTAnimation(1+randomInt(2 ), false);
		}
		mFlag = true;
	}
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
			play->arcade->killScore(HNINJA_SCORE, play->combo, this->center());
		}
		else
		{
			play->arcade->killScore(HNINJA_SCORE, 0, this->center());
		}
	}

	//SP
	play->mainrole->gainSP(4);

	mState = Dead;
	//随机掉落道具
	Item::triggerItem(2, mSprite->getPosition());

	//achievement killing
	GameRecord::sharedGameRecord()->task->dispatchTask(ACH_KILLING, 1);
	play->killingCount++;
	GameRecord::sharedGameRecord()->task->dispatchTask(ACH_KILLJONIN, 1);
	if( mSpell == 0 )
	{
		GameRecord::sharedGameRecord()->task->dispatchTask(ACH_JONINBYONESTRIKE, 1);
		play->killJoninCri++;
	}
	if( play->runwithoutkill == 0 )
	{
		play->runwithoutkill = play->distance/PLAY_DISMETER;
	}

	return true;
}

cocos2d::Point HighNinja::center() 
{
	return ccpAdd(mSprite->getPosition(), Vec2(9, 20));
}

bool HighNinja::supportAimAid() {
	return !( mState == Dead || !mSprite->isVisible());
}

void HighNinja::onDestroy() 
{
	GamePlay *play = GamePlay::sharedGamePlay();
	play->enemies->removeObject(this);
	mParent->removeChild(mSprite, false);

	GameObject *wood = play->manager->gameObjectWithHandler(mWoodLink);
	if( wood != NULL )
	{
		play->manager->removeGameObject(wood);
	}
}


