//
//  IceSpike.m
//  NinjiaRush
//
//  Created by 马 颂文 on 12-5-18.
//  Copyright (c) 2012年 TrinGame. All rights reserved.
//

#include "IceSpike.h"
#include "GameConstants.h"
#include "GamePlay.h"
#include "GameRecord.h"



IceSpike* IceSpike::spike(cocos2d::Point pos, Node* parent) 
{
	IceSpike *ret = IceSpike::create();
	ret->mPos = pos;
	ret->mParent = parent;

	return ret;
}

void IceSpike::onCreate() 
{
	mSprite = GTAnimatedSprite::spriteWithGTAnimation(GTAnimation::loadedAnimationSet("bullets"));
	mSprite->setAnchorPoint(cocos2d::Vec2(0.5f, 0.085f));
	mSprite->setPosition(mPos);
	mSprite->playGTAnimation(2, false);
	mParent->addChild(mSprite, LAYER_ROLE);
	mState = 0;
	mFlag = false;

	GameTool::PlaySound("freeze_up.mp3");
}

void IceSpike::onUpdate(float delta) 
{
	mSprite->updateGTAnimation(delta);
	GamePlay *play = GamePlay::sharedGamePlay();
	if( mState == 0 )
	{
		if( !mFlag )
		{
			if( play->mainrole->collisionWithCircle(ccpAdd(mSprite->getPosition(), Vec2(0, 17)), 13) )
			{
				play->mainrole->deliverHit(HIT_ICESPIKE, Vec2(0, 0));
				mFlag = true;
			}
			//分身术
			if( play->mainrole2 != NULL && play->mainrole2->collisionWithCircle(ccpAdd(mSprite->getPosition(), Vec2(0, 17)), 13) )
			{
				play->mainrole2->deliverHit(HIT_ICESPIKE, Vec2(0, 0));
				mFlag = true;
			}
		}
	}
	//offset
	float ds = delta*play->runspeed;
	cocos2d::Point np = mSprite->getPosition();
	np.x -= ds;
	mSprite->setPosition(np);
	if( np.x < -100 )
	{
		play->enemies->removeObject(this);
		play->manager->removeGameObject(this);
	}
}

bool IceSpike::collisionWithCircle(cocos2d::Point cc, float rad) 
{
	if( mSprite != NULL )
	{
		if( exCollisionWithCircles(mSprite->getPosition(), 0, 17, 13, cc, rad) ||
				exCollisionWithCircles(mSprite->getPosition(), 0, 43, 13, cc, rad) )
		{
			return true;
		}
	}
	return false;
}

bool IceSpike::deliverHit(int type, cocos2d::Point dir) 
{
	if( mState == 0 )
	{
		mState = 1;
		mSprite->playGTAnimation(3, false);
		GameTool::PlaySound("freeze_break.mp3");
		//achievement ice spike
		if( type == HIT_BLADE )
		{
			GameRecord::sharedGameRecord()->task->dispatchTask(ACH_SLICEICESPIKE, 1);
		}
		if( type == HIT_DART )
		{
			GameRecord::sharedGameRecord()->task->dispatchTask(ACH_DARTONGROUND, 1);
		}

		GamePlay *play = GamePlay::sharedGamePlay();
		if( play->mode == MODE_ARCADE )
		{
			play->arcade->addScore(ARCADE_BOUNS_ICEPIKE, this->center());
		}

		return true;
	}
	return false;
}

cocos2d::Point IceSpike::position() 
{
	return mSprite->getPosition();
}

void IceSpike::toggleVisible(bool flag) 
{
	mSprite->setVisible(flag);
}

void IceSpike::setPosition(cocos2d::Point pos) 
{
	mSprite->setPosition(pos);
}

cocos2d::Point IceSpike::center() 
{
	return ccpAdd(mSprite->getPosition(), Vec2(0, 20));
}

bool IceSpike::supportAimAid() 
{
	return false;
}

void IceSpike::onDestroy() 
{
	mParent->removeChild(mSprite, true);
}




