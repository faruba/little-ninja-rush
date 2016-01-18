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

  mRepositionDelegate = RoleStateDelegate::CreateDelegate<RepositioningStateDelegate>();
  switchStateDelegate(Repositioning, mRepositionDelegate);

	int y = CCRANDOM_0_1()*RESPAWN_Y;

	mSprite->setPosition(cocos2d::Vec2(20+(UniversalFit::sharedUniversalFit()->playSize.width-40)*CCRANDOM_0_1(), RESPAWN_YMIN+y));
	mSprite->playGTAnimation(0, true);
	mSprite->setVisible(false);
	mParent->addChild(mSprite, LAYER_ROLE+RESPAWN_Y-y);

	mDartCount = 0;
	mFlag = true;
	mSpeed = ENEMY_NNRUNSPEED;
	mTimer = 0;
	mSpell = 0;

	//出场特效
	GTAnimatedEffect *eff = GTAnimatedEffect::create(GTAnimation::loadedAnimationSet("effect"), 5, false);
	eff->setAnchorPoint(mSprite->getAnchorPoint());
	eff->setPosition(mSprite->getPosition());
	mParent->addChild(eff, LAYER_ROLE+RESPAWN_Y-y);

	mWoodLink.uid=-1;
	mWoodLink.index=-1;
}

void HighNinja::onUpdate(float delta) 
{
  Role::onUpdate(delta);
	bool playend = mSprite->updateGTAnimation(delta);
	GamePlay* play = GamePlay::sharedGamePlay();
	bool removeflag = false;
	bool gameOver = handleGameOver(delta);

	if (!gameOver) {//主角死亡的处理
		switch (mState) {
			case Entering:// onstage
				{
					mTimer += delta;
					if( mTimer > 0.30f && mSprite->isVisible() == false )
					{
						mSprite->setVisible(true);
					}
					if( mTimer >= 0.4f )
					{
						mState = Running;
						mTimer = 0;
						mSprite->playGTAnimation(0, true);
					}
				}
				break;
      case Repositioning:
        mStateDelegate[Repositioning]->update(delta);
        break;
			case Running:// run
				{
          mTimer += delta;
          if( mTimer > HNINJA_POLLTIME )
          {
            if( randomInt(100) < HNINJA_AGGRISIVE )
            {
              if( GamePlay::sharedGamePlay()->count_attack <= 0 )
              {
                mState = PreparingToShoot;
                mTimer = 0;
                mSprite->playGTAnimation(6, true);
                //play effect
                GTAnimatedEffect *eff = GTAnimatedEffect::create(GTAnimation::loadedAnimationSet("effect"), 7, false);
                eff->setPosition(cocos2d::Vec2(47, 19));
                mSprite->addChild(eff);
              }
            }
            else {
              if( randomInt(2)== 0 )
              {
                mRepositionDelegate->reset();
                mState = Repositioning;
                mSpeed = (0.3f+0.4f*CCRANDOM_0_1())*ENEMY_NNRUNSPEED;
              }
            }
            mTimer = 0;
          }
					if( playend )
					{
						mSprite->playGTAnimation(0, true);
					}
				}
				break;
			case PreparingToShoot:// prepare
				{
					mTimer += delta;
					float prepare = HNINJA_PREPARE;
					if( play->mode == MODE_ARCADE )
					{
						prepare *= play->arcade->prepare;
					}
					if( mTimer > prepare && GamePlay::sharedGamePlay()->count_attack <= 0 )
					{
						mState = Shooting;
						mTimer = 0;
						mDartCount = 0;
						mFlag = false;
						if( randomInt(3) == 0 )
						{
							mFlag = true;
						}
					}
				}
				break;
			case Shooting:// shoot
				{
					if( playend )
					{
						mSprite->playGTAnimation(0, true);
					}
					mTimer -= delta;
					if( mTimer <= 0 )
					{
						cocos2d::Point target = play->mainrole->center();
						if( play->mainrole2 != NULL )
						{
							if( randomInt(2) == 0 )
							{
								target = play->mainrole2->center();
							}
						}
						if( mFlag )
						{//冰锥术
							cocos2d::Point dst;
							dst.x = UniversalFit::sharedUniversalFit()->playSize.width/2 + UniversalFit::sharedUniversalFit()->playSize.width*CCRANDOM_0_1()/2;
							dst.y = play->mainrole->position().y;
							cocos2d::Point dir = ccpNormalize(ccpSub(dst, this->center()));
							std::string shape = "dart.png";
							play->darts->addObject(play->manager->addGameObject(Dart::dart(shape, this->center(), dir, -4, mParent)));
						}
						else {
							cocos2d::Point dir = ccpNormalize(ccpSub(target, this->center()));
							float angle = ccpToAngle(dir);
							angle += CC_DEGREES_TO_RADIANS(-HNINJA_ACCURATE)+CC_DEGREES_TO_RADIANS(2*HNINJA_ACCURATE)*CCRANDOM_0_1();
							dir = ccpForAngle(angle);
							std::string shape = "dart.png";
							play->darts->addObject(play->manager->addGameObject(Dart::dart(shape, this->center(), dir, -3, mParent)));
						}

						mSprite->playGTAnimation(5, false);
						mTimer = 0.3f;

						if( mDartCount>=2 || mFlag )
						{
							if( randomInt(3) < 2 )
							{
                mRepositionDelegate->mTargetPos = UniversalFit::sharedUniversalFit()->playSize.width*(0.2f+0.6f*CCRANDOM_0_1());
								mState = Repositioning;
								mTimer = 0;
								mSpeed = (0.3f+0.4f*CCRANDOM_0_1())*ENEMY_NNRUNSPEED;
							}
							else {
								mState = Running;
								mTimer = 0;
							}
						}
						mDartCount++;
					}

				}
				break;
			case Fleeing:// escape
				{
					float ds = delta*ENEMY_NNRUNSPEED;
					if( mSprite->getPosition().x > -100 )
					{
						cocos2d::Point np = mSprite->getPosition();
						np.x -= ds;
						mSprite->setPosition(np);
					}
					else {
						//销毁对象
						removeflag = true;
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
							removeflag = true;
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
						if( mSprite->animationId() == 3 )
						{
							float ra = mTimer/mSprite->playBackTime();
							if( ra > 1 )
							{
								ra = 1;
							}
							ra = 1 - ra;
							float ds = delta*200*ra;
							np.x += ds;
						}
						else {
							float ra = mTimer/mSprite->playBackTime();
							if( ra > 1 )
							{
								ra = 1;
							}
							float ds = delta*300*ra;
							np.x -= ds;
						}
						mSprite->setPosition(np);
					}
				}
				break;
			case Dummy:
				{//替身术
					if( mSprite->isVisible() == true )
					{
						mState = Running;
					}
				}
				break;
		}  
	}
	//rush offset
	if( play->state == STATE_RUSH )
	{
		float offset = (play->runspeed - play->levelspeed)*delta;
		cocos2d::Point np = mSprite->getPosition();
		np.x -= offset;
		mSprite->setPosition(np);
	}

	if( mState != Dead )
	{
		FootPrint::goFootPrint(&mStepSnow, mSprite->getPosition());
	}

	//删除检查
	if( mSprite->getPosition().x < -120 || mSprite->getPosition().x > 120+UniversalFit::sharedUniversalFit()->playSize.width )
	{
		removeflag = true;
	}

	if( removeflag )
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


