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
  switchStateDelegate(Entering, delegate);
  switchStateDelegate(Repositioning, delegate);
  switchStateDelegate(Running, RoleStateDelegate::CreateDelegate<NinjaRunningStateDelegate>());
  switchStateDelegate(PreparingToShoot, RoleStateDelegate::CreateDelegate<PreparingStateDelegate>());
  switchStateDelegate(Shooting, RoleStateDelegate::CreateDelegate<ShootStateDelegate>());
  switchStateDelegate(Fleeing, RoleStateDelegate::CreateDelegate<FleeStateDelegate>());
  switchStateDelegate(Dead, RoleStateDelegate::CreateDelegate<DeadStateDelegate>());

	int y = CCRANDOM_0_1()*RESPAWN_Y ;
	int x = (GamePlay::sharedGamePlay()->state == STATE_RUSH) ? UniversalFit::sharedUniversalFit()->playSize.width+100 : -100;

	mSprite->setPosition(cocos2d::Vec2(x, y + RESPAWN_YMIN ));
	mSprite->playGTAnimation(0, true);
	mParent->addChild(mSprite, LAYER_ROLE+RESPAWN_Y-y);

	mDartCount = 0;
	mFlag = true;
	mSpeed = ENEMY_NNRUNSPEED;
}

void NewbieNinja::onUpdate(float delta) {
  Role::onUpdate(delta);
  GamePlay* play = GamePlay::sharedGamePlay();
  bool removeflag = false;
  bool gameOver = handleGameOver(delta);

  if (!gameOver) {
    if (mStateDelegate[mState] != nullptr) {
      mStateDelegate[mState]->pre_update(delta);
      mStateDelegate[mState]->update(delta);
      mStateDelegate[mState]->after_update(delta);
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

  //snow step
  if( mState != Dead )
  {
    FootPrint::goFootPrint(&mStepSnow, mSprite->getPosition());
  }

  //删除检查
  if( mSprite->getPosition().x < -120 || mSprite->getPosition().x > 120+UniversalFit::sharedUniversalFit()->playSize.width )
  {
    removeflag = true;
  }

  if (removeflag || !mSprite->isVisible()) {
    play->manager->removeGameObject(this);
  }
}

//受到伤害
bool NewbieNinja::deliverHit(int type, cocos2d::Point dir) 
{
	if( mState == Dead )
	{
		mSprite->playGTAnimation(4, false);
		mFlag = false;
	}
	else {
		if( dir.x > 0 )
		{
			mSprite->playGTAnimation(3 , false);
		}
		else {
			mSprite->playGTAnimation(1+randomInt(2), false);
		}
		mFlag = true;
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
