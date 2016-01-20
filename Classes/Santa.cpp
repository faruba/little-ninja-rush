//
//  Santa.m
//  NinjiaRush
//
//  Created by 马 颂文 on 12-11-26.
//  Copyright (c) 2012年 TrinGame. All rights reserved.
//

#include "Santa.h"
#include "GameConstants.h"
#include "GamePlay.h"
#include "SimpleAudioEngine.h"
#include "Item.h"
#include "GameRecord.h"
#include "UniversalFit.h"
#include "FootPrint.h"

void Santa::onCreate() {
  mCollisionCircles.push_back(Circle(cocos2d::Vec2(1, 13), 8));
  mCollisionCircles.push_back(Circle(cocos2d::Vec2(-1, 32.5), 13));
  mAnchor = cocos2d::Vec2(0.5438f, 0.0625f);

  Role::onCreate();

  switchStateDelegate(Entering, RoleStateDelegate::CreateDelegate<BasicEnteringStateDelegate>());
  switchStateDelegate(Repositioning, RoleStateDelegate::CreateDelegate<RepositioningStateDelegate>());
  switchStateDelegate(Running, RoleStateDelegate::CreateDelegate<SantaRunningStateDelegate>());
  switchStateDelegate(Fleeing, RoleStateDelegate::CreateDelegate<FleeStateDelegate>());
  switchStateDelegate(Dead, RoleStateDelegate::CreateDelegate<DeadStateDelegate>());

  switchToState(Entering);
  mSpeed = ENEMY_NNRUNSPEED;

  mCoinTimer = 0;
  mHited = false;
}

void Santa::onUpdate(float delta) {
  Role::onUpdate(delta);

  bool dropcoins = false;
  if (mStateDelegate[mState] != nullptr) {
    mStateDelegate[mState]->pre_update(delta);
    bool gameOver = handleGameOver(delta);
    if (!gameOver) {
      mStateDelegate[mState]->update(delta);

      switch (mState) {
        case Repositioning:
        case Running:
          dropcoins = true;
          if( mLifeSpan > SANTA_LIFE ) {
            switchToState(Fleeing);
            mSpeed = ENEMY_NNRUNSPEED;
          }
          break;
      }
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

  //snow step
  if( mState != Dead ) {
    FootPrint::goFootPrint(&mStepSnow, mSprite->getPosition());
  }

	//drop coins
	if( dropcoins ) {
		mCoinTimer+=delta;
		if( mCoinTimer > SANTA_COIN )
		{
			play->manager->addGameObject(Item::item(0, mSprite->getPosition(), play, true));
			mCoinTimer -= SANTA_COIN;
		}
	}

  //删除检查
  if (mSprite->getPosition().x < -LIMITE_OFFSET ||
      mSprite->getPosition().x > LIMITE_OFFSET+UniversalFit::sharedUniversalFit()->playSize.width)
  {
		if ( !mHited ) {
			//achievement : meet christmas
			play->meetchristmas++;
			GameRecord::sharedGameRecord()->task->dispatchTask(ACH_MEETCHRISTMAS, 1);
			GameRecord::sharedGameRecord()->task->dispatchTask(ACH_ONEMEETCHRISTMAS, play->meetchristmas);
		}
    play->manager->removeGameObject(this);
  }
}

//受到伤害
bool Santa::deliverHit(int type, cocos2d::Point dir) 
{
	mSprite->playGTAnimation(1, false);

	mState = Dead;
	mHited = true;
	mFlag = true;

	return true;
}

cocos2d::Point Santa::center() 
{
	return ccpAdd(mSprite->getPosition(), Vec2(0, 21));
}

bool Santa::supportAimAid() 
{
	return false;
}

void Santa::onDestroy() 
{
	GamePlay *play = GamePlay::sharedGamePlay();
	play->enemies->removeObject(this);
	mParent->removeChild(mSprite, false);
}
