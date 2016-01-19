//
//  Messager.m
//  NinjiaRush
//
//  Created by 马 颂文 on 12-5-3.
//  Copyright (c) 2012年 TrinGame. All rights reserved.
//

#include "Messager.h"
#include "GameConstants.h"
#include "GamePlay.h"
#include "Item.h"
#include "GameRecord.h"
#include "UniversalFit.h"
#include "FootPrint.h"

void Messager::onCreate() 
{
	mCollisionCircles.push_back(Circle(cocos2d::Vec2(1, 13), 8));
	mCollisionCircles.push_back(Circle(cocos2d::Vec2(-1, 32.5), 13));

	Role::onCreate();

	mSprite->setAnchorPoint(cocos2d::Vec2(0.5438f, 0.0625f));

  switchStateDelegate(Entering, RoleStateDelegate::CreateDelegate<MessagerEnteringStateDelegate>());
  switchStateDelegate(Repositioning, RoleStateDelegate::CreateDelegate<MessagerRepositioningStateDelegate>());
  switchStateDelegate(Running, RoleStateDelegate::CreateDelegate<MessagerRunningStateDelegate>());
  switchStateDelegate(Fleeing, RoleStateDelegate::CreateDelegate<MessagerFleeStateDelegate>());
  switchStateDelegate(Dead, RoleStateDelegate::CreateDelegate<DeadStateDelegate>());

  switchToState(Entering);
}

void Messager::onUpdate(float delta) 
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

  //删除检查
  if (mSprite->getPosition().x < -LIMITE_OFFSET ||
      mSprite->getPosition().x > LIMITE_OFFSET+UniversalFit::sharedUniversalFit()->playSize.width)
  {
    play->manager->removeGameObject(this);
    //TODO: 下面这行代码可以重现Bug
    //play->mainrole->fire(cocos2d::Vec2(0, 0));
  }
}

void Messager::onDestroy() 
{ 
	mParent->removeChild(mSprite, true);
}


bool Messager::deliverHit(int type, cocos2d::Point dir) 
{
  if( dir.x > 0 )
  {
    mSprite->playGTAnimation(5, false);
    static_cast<DeadStateDelegate*>(mStateDelegate[Dead])->mIsDieForward = true;
  }
  else {
    static_cast<DeadStateDelegate*>(mStateDelegate[Dead])->mIsDieForward = false;
    mSprite->playGTAnimation(4, false);
  }
  //add sp
  GamePlay *play = GamePlay::sharedGamePlay();
  play->mainrole->SP+=4;
  if( play->mainrole->SP > play->mainrole->maxSP )
  {
    play->mainrole->SP = play->mainrole->maxSP;
  }

  mState = Dead;
  //随机掉落道具
  play->manager->addGameObject(Item::item(1, mSprite->getPosition(), mParent, false));

  //achievement killing
  GameRecord::sharedGameRecord()->task->dispatchTask(ACH_KILLING, 1);
  play->killingCount++;
  GameRecord::sharedGameRecord()->task->dispatchTask(ACH_KILLMESSAGER, 1);
  if( play->runwithoutkill == 0 )
  {
    play->runwithoutkill = play->distance/PLAY_DISMETER;
  }


  return  true;
}

cocos2d::Point Messager::center() 
{
	return ccpAdd(mSprite->getPosition(), Vec2(0, 21));
}

bool Messager::supportAimAid() 
{
	if( mState != Dead )
	{
		return  true;
	}
	else {
		return false;
	}
}
