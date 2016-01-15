//
//  Boss.cpp
//  little-ninja-rush
//
//  Created by tringame on 16/1/14.
//
//

#include "Boss.h"
#include "GameConstants.h"
#include "GamePlay.h"
#include "Item.h"
#include "Bomb.h"
#include "SimpleAudioEngine.h"
#include "GameRecord.h"
#include "UniversalFit.h"
#include "FootPrint.h"

#define  NOMAL_SHOOT_RATE 80
#define MAX_BOSS_HP 10
#define BOSS_STATE2_HP 4



void MoveAndAttackRole::onEntering(float delta, bool playend){
  const Vec2& curPos = currentPos();
  const Vec2& targetPos = mTargetPos.getTarget();
  if(curPos.y > targetPos.y )
  {
    float dis = delta* speed();
    Vec2 dir =targetPos - curPos;
    if(dir.lengthSquared() <= dis*dis )
    {
      setCurrentPos(targetPos);
      mSprite->playGTAnimation(8, false);
    }
    else {
      dir = ccpForAngle(PI*3.0f/5.0f);
      dir.x *= -1;
      dir.y *= -1;
      mSprite->setPosition(curPos + dir * dis);
    }
  }
  else {
      mState = Running;
  }
}
bool fuzzyEquals(float a, float b , float variance){
  return fabs(a-b) < variance;
}

void MoveAndAttackRole::onRunning(float dt, bool playend)
{
  Vec2 curPos = currentPos();
  const Vec2& targetPos = mTargetPos.getTarget();
  
  float offset = dt * speed();
  if(!fuzzyEquals(targetPos.x , curPos.x, offset)) {
    if(targetPos.x < curPos.x){
      offset = -offset;
    }
    curPos.x += offset;
    setCurrentPos(curPos);
  }
  else {
    mTargetPos.genNextPlan();
    if( randomInt(100) < MNINJA_AGGRISIVE )
    {
      if( mDartCount < MNINJA_MAXDART && GamePlay::sharedGamePlay()->count_attack <= 0 )
      {
        mState = PreparingToShoot;
      }
    }
  }
  
  if( playend )
  {
    mSprite->playGTAnimation(0, true);
  }
}
void MoveAndAttackRole::playPrepareAnimation(){
   mSprite->playGTAnimation(6, true);
  //play effect
  GTAnimatedEffect *eff = GTAnimatedEffect::create(GTAnimation::loadedAnimationSet("effect"), 7, false);
  eff->setPosition(cocos2d::Vec2(47, 19));
  mSprite->addChild(eff); 
}

void MoveAndAttackRole::onPreparingToShoot(float dt){
  mState = Shooting;
  playPrepareAnimation();
  onShooting();
}



Vec2 MoveAndAttackRole::getAttackDir(){
  GamePlay* play = GamePlay::sharedGamePlay();
  Vec2 target = play->mainrole->center();
  if( play->mainrole2 != NULL )
  {
    if( randomInt(2) == 0 )
    {
      target = play->mainrole2->center();
    }
  }
  if( randomInt(3))
  {
    return target;
  }
  else {
    cocos2d::Point dir = ccpNormalize(ccpSub(target,center()));
    float angle = ccpToAngle(dir);
    angle += CC_DEGREES_TO_RADIANS(-MNINJA_ACCURATE)+CC_DEGREES_TO_RADIANS(2*MNINJA_ACCURATE)*CCRANDOM_0_1();
    dir = ccpForAngle(angle);
    return dir;
  }
}
void MoveAndAttackRole::onShooting(){
  safeStartAfterSecond(nextAttackTimeInterval(), [this]() ->bool {
    GamePlay* play = GamePlay::sharedGamePlay();
    mState = Shooting;
    Vec2 dir = getAttackDir();
    std::string shape = "dart.png";
    play->darts->addObject(play->manager->addGameObject(Dart::dart(shape, this->center(), dir, -2, mParent)));
    mSprite->playGTAnimation(5, false);
    if( randomInt(3) < 2 ) {
      mState = Running;
      return false;
    }
    return true;
  });
}

bool MoveAndAttackRole::onDead(float delta, bool playend) {
  bool  removeflag = false;
  if( playend )
  {
    //fix pos
    float ds = delta*speed();
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
  return removeflag;
}

void MoveAndAttackRole::onUpdate(float delta)
{
	GamePlay* play = GamePlay::sharedGamePlay();
	bool playend = mSprite->updateGTAnimation(delta);
	bool removeflag = false;
	bool gameOver = handleGameOver(delta);

	if (!gameOver) {//主角死亡的处理
		switch (mState) {
			case Entering:// onstage
        onEntering(delta,playend);
				break;
			case Running:// run
        onRunning(delta, playend);
				break;
			case PreparingToShoot:// prepare
        onPreparingToShoot(delta);
				break;
			case Shooting:// shoot
        //coroutine
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
        onDead(delta, playend);
				break;
		}
	}
  onUpdateCoroutine(delta);
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
bool MoveAndAttackRole::deliverHit(int type, cocos2d::Point dir)
{
	mTimer = 0;
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
			mSprite->playGTAnimation(1+randomInt(2 ), false);
		}
		mFlag = true;
	}
	GamePlay *play = GamePlay::sharedGamePlay();
	//combo
	bool isCombo = false;
  int damage = 1;
	if( mState == PreparingToShoot || mState == Fleeing )
	{
		play->makeCombo();
		isCombo = true;
    damage = 2;
	}
	// arcade combo
	if( play->mode == MODE_ARCADE )
	{
		if( isCombo )
		{
			play->arcade->killScore(MNINJA_SCORE, play->combo, this->center());
		}
		else
		{
			play->arcade->killScore(MNINJA_SCORE, 0, this->center());
		}
	}

	//SP
	play->mainrole->gainSP(3);

  hp -= damage;
  if(isDead()){
    mState = Dead;
    //随机掉落道具
    Item::triggerItem(1, mSprite->getPosition());
    
    //achievement killing
    GameRecord::sharedGameRecord()->task->dispatchTask(ACH_KILLING, 1);
    play->killingCount++;
    GameRecord::sharedGameRecord()->task->dispatchTask(ACH_KILLCHUNIN, 1);
    if( type == HIT_BOMB )
    {
      GameRecord::sharedGameRecord()->task->dispatchTask(ACH_CHUNINBYBOMB, 1);
    }
    if( play->runwithoutkill == 0 )
    {
      play->runwithoutkill = play->distance/PLAY_DISMETER;
    }
  }

  afterDamage();
	return true;
}

cocos2d::Point MoveAndAttackRole::center() 
{
	return ccpAdd(mSprite->getPosition(), Vec2(9, 20));
}

bool MoveAndAttackRole::supportAimAid() {
	return !( mState == Dead || mState == Initializing || mState == Entering );
}

void MoveAndAttackRole::onDestroy()
{
	GamePlay *play = GamePlay::sharedGamePlay();
	play->enemies->removeObject(this);
	mParent->removeChild(mSprite, false);
}


Range bossMoveRange(SCREEN_WIDTH * 0.2,SCREEN_WIDTH * 0.8);
void Boss::onCreate() {
	mCollisionCircles.push_back(Circle(cocos2d::Vec2(6, 12), 9));
	mCollisionCircles.push_back(Circle(cocos2d::Vec2(17, 27), 13));

	Role::onCreate();
  attackTimeIntervalRange.set(2, 5);
	int y = CCRANDOM_0_1()*RESPAWN_Y;
  
  mTargetPos.init(bossMoveRange, Vec2::ONE*(RESPAWN_YMIN + y));
	//计算起跳点
	cocos2d::Point rjp = ccpForAngle(PI*3.0f/5.0f);

	mSprite->setPosition(mTargetPos.getTarget() +  rjp * 100);
	mSprite->playGTAnimation(7, true);
	mParent->addChild(mSprite, LAYER_ROLE+RESPAWN_Y-y);

	mDartCount = 0;
	mFlag = true;
	mSpeed = ENEMY_NNRUNSPEED;
  hp = MAX_BOSS_HP;
}

void Boss::onShooting(){
  if(isMakedSpecialShoot || randomInt(100) < NOMAL_SHOOT_RATE){
    MoveAndAttackRole::onShooting();
  }else{
    onSpecialShoot(FLOAT_GUN_COUNT);
  }
}
void Boss::onSpecialShoot(int count){
  clearFloatGun();
  mState = Shooting;
  int *currentCount = new int(0);
  isMakedSpecialShoot = true;
  safeStartAfterSecond(0.5f, [count,currentCount,this](){
    if(*currentCount == count){
      mState = Running;
      delete currentCount;
      return false;
    }
    releaseFloatGun(center(),*currentCount);
    *currentCount += 1;
    return true;
  });
}
void Boss::releaseFloatGun(const Vec2& pos, int& index){
  GamePlay* play = GamePlay::sharedGamePlay();
  FloatGun* enemy =static_cast<FloatGun*>(Role::CreateRole<FloatGun>(play));
  enemy->setOwner(this, index);
  floatGunGroup[index]  = enemy;
  play->enemies->addObject(play->manager->addGameObject(enemy));
}

void Boss::afterDamage()
{
  if(hp < BOSS_STATE2_HP && !isState2){
    isState2 = true;
    onSpecialShoot(FLOAT_GUN_COUNT);
  }
  
}

void Boss::onFloatGunDead(FloatGun* floatGun){
  int idx =floatGun->idx;
  floatGunGroup[idx] = NULL;
  if(isState2){
    for (int i = 0; i < idx; i++) {
      if(floatGunGroup[i] != NULL){
        //add new one
        releaseFloatGun(center(),idx);
        return ;
      }
    }
  }
}
void Boss::clearFloatGun()
{
  for(int i =0; i < FLOAT_GUN_COUNT; i++){
    FloatGun* gun = floatGunGroup[i];
    if(gun != NULL){
      gun->mState = Dead;
    }
    floatGunGroup[i] = NULL;
  }
}
void FloatGun::onCreate() {
	mCollisionCircles.push_back(Circle(cocos2d::Vec2(6, 12), 9));
	mCollisionCircles.push_back(Circle(cocos2d::Vec2(17, 27), 13));

	Role::onCreate();

  
  attackTimeIntervalRange.set(4, 10);
  mTargetPos.init(bossMoveRange, Vec2(RESPAWN_YMIN, RESPAWN_YMIN + RESPAWN_Y));
	//计算起跳点

  const Vec2& pos =mTargetPos.getTarget();
	mSprite->setPosition(pos);
	//mSprite->playGTAnimation(7, true);
	mParent->addChild(mSprite,LAYER_ROLE+RESPAWN_Y);

	mDartCount = 0;
	mFlag = true;
	mSpeed = ENEMY_NNRUNSPEED/2;
  hp = MAX_BOSS_HP;
}


void FloatGun::afterDamage()
{
  if(isDead()){
    owner->onFloatGunDead(this);
  }
}