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

void Samuri::onCreate() {
	mCollisionCircles.push_back(Circle(cocos2d::Vec2(1, 10), 15));
	mCollisionCircles.push_back(Circle(cocos2d::Vec2(0, 30), 14));
	mAnchor = cocos2d::Vec2(0.694f, 0.08125f);

	Role::onCreate();

	GamePlay *play = GamePlay::sharedGamePlay();
	mSprite->setPosition(cocos2d::Vec2(UniversalFit::sharedUniversalFit()->playSize.width+play->runspeed*SAMURAI_WARNING, PLAY_PLAYERLINE));
	mSprite->playGTAnimation(0, true);
	mParent->addChild(mSprite, LAYER_ROLE);

	mHint = GTAnimatedSprite::spriteWithGTAnimation(GTAnimation::loadedAnimationSet("misc"));
	mHint->playGTAnimation(1, true);
	mHint->setAnchorPoint(cocos2d::Vec2(1, 0.5f));
	mHint->setPosition(cocos2d::Vec2(UniversalFit::sharedUniversalFit()->playSize.width, PLAY_PLAYERLINE+20));
	mParent->addChild(mHint, LAYER_ROLE);
	cocos2d::MoveBy *mb1 = cocos2d::MoveBy::create(0.2f, Vec2(-20, 0));
	cocos2d::MoveBy *mb2 = cocos2d::MoveBy::create(0.2f, Vec2(20, 0));
	cocos2d::Sequence *sq = cocos2d::Sequence::create(mb1,mb2, NULL);
	cocos2d::RepeatForever *rp = cocos2d::RepeatForever::create(sq);
	mHint->runAction(rp);

	mTimer = 0;
	mFlag = false;

	GameTool::PlaySound("samurai_warning.mp3");
}

void Samuri::onUpdate(float delta) 
{
	GamePlay *play = GamePlay::sharedGamePlay();
	bool playend = mSprite->updateGTAnimation(delta);
	//fix pos
	cocos2d::Point np = mSprite->getPosition();
	np.x -= play->runspeed*delta;
	mSprite->setPosition(np);
	switch (mState) {
		case Entering:
			{
				if( !mFlag && mSprite->getPosition().x < UniversalFit::sharedUniversalFit()->playSize.width )
				{
					GameTool::PlaySound("samurai_attack.mp3");
					mFlag = true;
				}
				float rds = play->runspeed*0.5f;
				if( mSprite->getPosition().x - play->mainrole->position().x < rds )
				{
					mState = Running;
					mSprite->playGTAnimation(1, false);
					mFlag = false;
				}
			}
			break;
		case Running:
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
						GameTool::PlaySound("hit");
						play->mainrole->deliverHit(HIT_BLADE, Vec2(0, 0));//武士总是对付第一个人物造成伤害
						mFlag = true;
					}
				}
			}
			break;
		case Dead:
			{
				if(mFlag)
				{
					mTimer += delta;
					if(mTimer>0.3f)
					{
						mTimer = 0;
						mFlag = false;
						GameTool::PlaySound("samurai_die.mp3");
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
bool Samuri::deliverHit(int type, cocos2d::Point dir)
{
	if( mState != Dead )
	{
		GameTool::PlaySound(std::string_format("ahh%d.mp3", (randomInt(3)+1)).c_str());
		mState = Dead;
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

cocos2d::Point Samuri::center() 
{
	return ccpAdd(mSprite->getPosition(), Vec2(9, 20));
}

bool Samuri::supportAimAid() 
{
	return false;
}
