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

	GamePlay *play = GamePlay::sharedGamePlay();
	int y = CCRANDOM_0_1()*RESPAWN_Y;
	mSprite->setPosition(cocos2d::Vec2(UniversalFit::sharedUniversalFit()->playSize.width+100, RESPAWN_YMIN+y));
	mSprite->playGTAnimation(0, true);
	play->addChild(mSprite, LAYER_ROLE+RESPAWN_Y-y);

	mSpeed = 0.2f*ENEMY_NNRUNSPEED;
}

void Mechanic::onUpdate(float delta) 
{
	GamePlay* play = GamePlay::sharedGamePlay();
	bool playend = mSprite->updateGTAnimation(delta);
	bool gameOver = handleGameOver(delta);

	if (!gameOver) {//主角死亡的处理
		switch (mState) {
			case Entering:// walk
				{
					if( mTimer > 0 )
					{//attack cd
						mTimer -= delta;
					}
					float ds = mSpeed*delta;
					cocos2d::Point np = mSprite->getPosition();
					np.x -= ds;
					mSprite->setPosition(np);
					//may attack
					if( np.x > 30 && np.x < UniversalFit::sharedUniversalFit()->playSize.width - 30 && mTimer <= 0 )
					{
						if( randomInt(3) == 0 )
						{
							mState = PreparingToShoot;
							mSprite->playGTAnimation(1, true);
							mTimer = 0;
						}
						else {
							mTimer = MECHANIC_POLLTIME;
						}
					}
				}
				break;
			case PreparingToShoot:// prepare
				{
					mTimer += delta;
					float prepare = MECHANIC_PREPARE;
					if( play->mode == MODE_ARCADE )
					{
						prepare *= play->arcade->prepare;
					}
					if( mTimer > prepare )
					{
						mState = Shooting;
						mTimer = 0.5f;
						mCount = 0;
					}
				}
				break;
			case Shooting:// fire
				{
					if( mTimer > 0 )
					{
						mTimer -= delta;
						if( mTimer <= 0 )
						{
							GameTool::PlaySound("duofa.mp3");
						}
					}
					if( mTimer <= 0 )
					{
						if( mCount < MECHANIC_LAUNCH )
						{
							//launch dart
							//                        cocos2d::Point dir = ccpForAngle(CC_DEGREES_TO_RADIANS(-90-MECHANIC_ANGLE+CCRANDOM_0_1()*MECHANIC_ANGLE*2));
							//                        play->darts->addObject(play->manager->addGameObject(Dart::dart(cocos2d::CCString::create("dart.png"), this->center(), dir, -6, play)));
							mTimer = MECHANIC_LAUNCHCD;
							mCount++;
						}
						else {
							mSprite->playGTAnimation(0, true);
							mState = Entering;
						}
					}
				}
				break;
			case Dead:// dead
				{
					if( playend )
					{
						//fix pos
						float ds = delta*play->runspeed;
						cocos2d::Point np = mSprite->getPosition();
						np.x -= ds;
						mSprite->setPosition(np);

						if( mSprite->getPosition().x < -100 )
						{
							GamePlay *play = GamePlay::sharedGamePlay();
							play->enemies->removeObject(this);
							play->manager->removeGameObject(this);
						}
					}
					else {
						mTimer += delta;
						if(mTimer>0.3f && mFlag)
						{
							int n = 1 + randomInt(3);
							GameTool::PlaySound(std::string_format("ahh%d.mp3", n).c_str());
							mFlag = false;
						}
						//fix pos
						cocos2d::Point np = mSprite->getPosition();
						float ra = mTimer/mSprite->playBackTime();
						if( ra > 1 )
						{
							ra = 1;
						}
						float ds = delta*300*ra;
						np.x -= ds;
						mSprite->setPosition(np);
					}
				}
				break;
		}
	}

	if( playend && mState != Dead )
	{
		mSprite->playGTAnimation(0, true);
	}

	//rush offset
	if( play->state == STATE_RUSH )
	{
		float offset = (play->runspeed - play->levelspeed)*delta;
		cocos2d::Point np = mSprite->getPosition();
		np.x -= offset;
		mSprite->setPosition(np);
	}

	//删除检查
	if( mSprite->getPosition().x < -120 || mSprite->getPosition().x > 120+UniversalFit::sharedUniversalFit()->playSize.width )
	{
		play->enemies->removeObject(this);
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


