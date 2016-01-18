#include "NewbieNinja.h"
#include "GameConstants.h"
#include "GamePlay.h"
#include "Item.h"
#include "GameRecord.h"
#include "UniversalFit.h"
#include "FootPrint.h"

void NewbieNinja::onCreate() {
	mCollisionCircles.push_back(Circle(cocos2d::Vec2(6, 12), 9));
	mCollisionCircles.push_back(Circle(cocos2d::Vec2(17, 27), 13));

	Role::onCreate();

  RoleStateDelegate *delegate = RoleStateDelegate::CreateDelegate<RepositioningStateDelegate>();
  NinjaRunningStateDelegate *running = RoleStateDelegate::CreateDelegate<NinjaRunningStateDelegate>();
  running->mPollTime = NNINJA_POLLTIME;
  running->mAggressive = NNINJA_AGGRISIVE;

  PreparingStateDelegate *preparing = RoleStateDelegate::CreateDelegate<PreparingStateDelegate>();
  preparing->mPreparingTime = NNINJA_PREPARE;

  switchStateDelegate(Entering, RoleStateDelegate::CreateDelegate<BasicEnteringStateDelegate>());
  switchStateDelegate(Repositioning, delegate);
  switchStateDelegate(Running, running);
  switchStateDelegate(PreparingToShoot, preparing);
  switchStateDelegate(Shooting, RoleStateDelegate::CreateDelegate<ShootStateDelegate>());
  switchStateDelegate(Fleeing, RoleStateDelegate::CreateDelegate<FleeStateDelegate>());
  switchStateDelegate(Dead, RoleStateDelegate::CreateDelegate<DeadStateDelegate>());

  switchToState(Entering);

	mDartCount = 0;
  mDartCapacity = NNINJA_MAXDART;
	mSpeed = ENEMY_NNRUNSPEED;
}

void NewbieNinja::onUpdate(float delta) {
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

  //snow step
  if( mState != Dead ) {
    FootPrint::goFootPrint(&mStepSnow, mSprite->getPosition());
  }

  //删除检查
  if (mSprite->getPosition().x < -LIMITE_OFFSET ||
      mSprite->getPosition().x > LIMITE_OFFSET+UniversalFit::sharedUniversalFit()->playSize.width)
  {
    play->manager->removeGameObject(this);
  }
}

//受到伤害
bool NewbieNinja::deliverHit(int type, cocos2d::Point dir) 
{
	if( mState == Dead )
	{
		mSprite->playGTAnimation(4, false);
	} else {
		if ( dir.x > 0 ) {
			mSprite->playGTAnimation(3 , false);
		} else {
			mSprite->playGTAnimation(1+randomInt(2), false);
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
			play->arcade->killScore(NNINJA_SCORE, play->combo, this->center());
		}
		else
		{
			play->arcade->killScore(NNINJA_SCORE, 0, this->center());
		}
	}

	//SP
	play->mainrole->gainSP(2);

	switchToState(Dead);

	//随机掉落道具
	Item::triggerItem(0, mSprite->getPosition());

	//achievement killing
	GameRecord::sharedGameRecord()->task->dispatchTask(ACH_KILLING, 1);
	play->killingCount++;
	GameRecord::sharedGameRecord()->task->dispatchTask(ACH_KILLGENIN, 1);
	if( play->runwithoutkill == 0 )
	{
		play->runwithoutkill = play->distance/PLAY_DISMETER;
	}

	return true;
}

cocos2d::Point NewbieNinja::center() 
{
	return ccpAdd(mSprite->getPosition(), Vec2(9, 20));
}

bool NewbieNinja::supportAimAid() 
{
	return !( mState == Dead );
}

void NewbieNinja::onDestroy() 
{
	GamePlay *play = GamePlay::sharedGamePlay();
	play->enemies->removeObject(this);
	mParent->removeChild(mSprite, false);
}
