//
//  Pumpkin.m
//  NinjiaRush
//
//  Created by 马 颂文 on 12-9-20.
//  Copyright (c) 2012年 TrinGame. All rights reserved.
//

#include "Pumpkin.h"
#include "GamePlay.h"
#include "Item.h"
#include "UniversalFit.h"
#include "GameRecord.h"

#define FLOATING_TIME (45)
#define NEAR (50)
#define RANGE (27)//半径

#define HIT_EXPLODE (10)

void Pumpkin::onCreate() {
	mCollisionCircles.push_back(Circle(cocos2d::Vec2(0, 0), RANGE));

	Role::onCreate();

	mSprite->playGTAnimation(0, true);
	mSprite->setPosition(cocos2d::Vec2( -100, SCREEN_HEIGHT ));
	mParent->addChild(mSprite, LAYER_MAINROLE);

	mTimer = 0;
	mHitCount = 0;
	mHurtTimer = -1;

	//init parameters
  switchStateDelegate(Entering, RoleStateDelegate::CreateDelegate< PumpkinEnteringStateDelegate>());
  switchStateDelegate(Fleeing, RoleStateDelegate::CreateDelegate<PumpkinFleeingStateDelegate>());
  switchStateDelegate(Dead, RoleStateDelegate::CreateDelegate< PumpkinDeadDelegate>());
  switchToState(Entering);

}
void Pumpkin::updateTargetPos(float delta){
  mTargetPos = mTargetPos + mTargetSpeed * delta;
  //range check
  //left && right
  if( ( mTargetPos.x < 50 && mTargetSpeed.x < 0 ) || ( mTargetPos.x > 430 && mTargetSpeed.x > 0 ) )
  {
    mTargetSpeed.x *= -1;
  }
  //up && down
  if( ( mTargetPos.y < 210 && mTargetSpeed.y < 0 ) || ( mTargetPos.y > 270 && mTargetSpeed.y > 0 ) )
  {
    mTargetSpeed.y *= -1;
  }
}

void Pumpkin::moveToTargetPos(float delta){
  if(!position().fuzzyEquals(mTargetPos, 0.1f)){//move with target
    //merge speed
    cocos2d::Point dr = mTargetPos - position();
    float at = dr.length()*20*delta;
    dr.normalize();
    mPumpkinSpeed = mPumpkinSpeed + dr* at;
    
    //limit speed
    {
      if(mPumpkinSpeed.lengthSquared() > 100*100)
      {
        mPumpkinSpeed.normalize();
        mPumpkinSpeed*=100;
      }
    }
    
    //move pumpkin
    cocos2d::Point np = position() + mPumpkinSpeed * delta;
    setPosition(np);
  }
}
void Pumpkin::playHurtAnimation(){
  mSprite->setColor(Color3B(255, 142, 142));
  if( mSprite->getScaleX() > 0 )
  {
				mSprite->setRotation(30);
  }
  else
  {
				mSprite->setRotation(-30);
  }
  auto seq = cocos2d::Sequence::create(
                               cocos2d::ScaleTo::create(0.1f,1.3f),
                               cocos2d::ScaleTo::create(0.1f,1.0f),
                               cocos2d::CallFunc::create([this](){
    mSprite->setColor(Color3B(255, 255, 255));
    mSprite->setScale(1);
    mSprite->setRotation(0);
  }), nullptr);
  mSprite->runAction(seq);
}
void Pumpkin::facePlayer(GamePlay* play){
 	float displayer = play->mainrole->position().x - mSprite->getPosition().x;
	if( displayer*mSprite->getScaleX() > 0 )
	{
		mSprite->setScaleX(mSprite->getScaleX()*-1);
	}
}
void Pumpkin::onUpdate(float delta) 
{
  Role::onUpdate(delta);
	GamePlay *play = GamePlay::sharedGamePlay();
  facePlayer(play);
  
  if (mStateDelegate[mState] != nullptr) {
    mStateDelegate[mState]->pre_update(delta);
    bool gameOver = handleGameOver(delta);
    if (!gameOver) {
      mStateDelegate[mState]->update(delta);
    }
    mStateDelegate[mState]->after_update(delta);
  }
	
  if (mSprite->getPosition().x < -LIMITE_OFFSET ||
      mSprite->getPosition().x > LIMITE_OFFSET+UniversalFit::sharedUniversalFit()->playSize.width)
  {
    play->manager->removeGameObject(this);
	}
}

//受到伤害
bool Pumpkin::deliverHit(int type, cocos2d::Point dir) 
{
	GamePlay *play = GamePlay::sharedGamePlay();
	if( mState != Dead )
	{
		if( mHitCount < HIT_EXPLODE )
		{
			//pop coins
			play->manager->addGameObject(Item::item(0, mSprite->getPosition(), play, false));
			mSprite->playGTAnimation(1, false);
      playHurtAnimation();
			GameTool::PlaySound("pumpkinhurt.mp3");

		}
		else
		{
      switchToState(Role::Dead);

			//achievement : kill pumpkin
			play->killpumpkins++;
			GameRecord::sharedGameRecord()->task->dispatchTask(ACH_KILLPUMPKINS, 1);
			GameRecord::sharedGameRecord()->task->dispatchTask(ACH_ONEKILLPUMPKINS, play->killpumpkins);
		}
		mHitCount++;
		return true;
	}
	else
	{
		return false;
	}
}

cocos2d::Point Pumpkin::center() 
{
	return mSprite->getPosition();
}

bool Pumpkin::supportAimAid() 
{
	return  false;
}

void Pumpkin::onDestroy() 
{
	GamePlay *play = GamePlay::sharedGamePlay();
	play->enemies->removeObject(this);
	mParent->removeChild(mSprite, false);
}


