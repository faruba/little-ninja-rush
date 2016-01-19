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

#define  NOMAL_SHOOT_RATE 8
#define MAX_BOSS_HP 10
#define BOSS_STATE2_HP 4
#define FLOAT_GUN_HP1 2
#define FLOAT_GUN_HP2 1



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
  if(!targetPos.fuzzyEquals(curPos, offset)){
    Vect distance = (targetPos - curPos);
    distance.normalize();
    curPos += distance * offset;
    setCurrentPos(curPos);
  }
  else {
    mTargetPos.genNextPlan();
    if( randomInt(100) < MNINJA_AGGRISIVE )
    {
      if( mDartCount < MNINJA_MAXDART && GamePlay::sharedGamePlay()->count_attack <= 0 )
      {
        //mState = PreparingToShoot;
        onPreparingToShoot(dt);
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

}

void MoveAndAttackRole::shootDart(std::vector<Vec2>& dirList){
  GamePlay* play = GamePlay::sharedGamePlay();
  mState = Shooting;
  std::string shape = "dart.png";
  for(Vec2& dir : dirList){
    play->darts->addObject(play->manager->addGameObject(Dart::dart(shape, this->center(), dir, -2, mParent)));
  }
//    mSprite->playGTAnimation(5, false);
}
void MoveAndAttackRole::repeatAction(int times, float timeInterval,callbackFunction cb, callbackFunction onFinised){
  int* currentCount = new int(0);
  safeStartAfterSecond(timeInterval, [times,currentCount,cb, onFinised](){
    if(*currentCount == times){
      delete currentCount;
      onFinised();
      return false;
    }
    cb();
    *currentCount += 1;
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
        //onPreparingToShoot(delta);
				break;
			case Shooting:// shoot
        //coroutine
				break;
			case Fleeing:// escape
        onFleeing();
				break;
			case Dead:// dead
        removeflag = onDead(delta, playend);
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
    //CCLOG("--remove %p", this);
		play->manager->removeGameObject(this);
	}
}

//受到伤害
bool MoveAndAttackRole::deliverHit(int type, cocos2d::Point dir)
{
  if(isGodmode){
    return true;
  }
	mTimer = 0;

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
    mSprite->playGTAnimation(4 , false);
//		if( dir.x > 0 )
//		{
//			mSprite->playGTAnimation(3 , false);
//		}
//		else {
//			mSprite->playGTAnimation(1+randomInt(2 ), false);
//		}
		mFlag = true;
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


Range bossMoveRange(SCREEN_WIDTH * 0.0,SCREEN_WIDTH * 1.0);
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
  isHighAttackSpeedMode = true;
  int *currentCount = new int(0);
  isMakedSpecialShoot = true;
  safeStartAfterSecond(0.1f, [count,currentCount,this](){
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
  //CCLOG("create Float %d %p",index, enemy);
  enemy->setOwner(this, index,isState2);
  floatGunGroup[index]  = enemy;
  play->enemies->addObject(play->manager->addGameObject(enemy));
}

void Boss::afterDamage()
{
  if(hp < BOSS_STATE2_HP && !isState2){
    isState2 = true;
    isGodmode = true;
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
        //CCLOG("%d still exsist recreate",i);
        releaseFloatGun(center(),idx);
        return ;
      }
    }

  }
  if(isAllFloatGunDead()){
    if(isState2){
      isGodmode = false;
    }
    isHighAttackSpeedMode = false;
  }
}
bool Boss::isAllFloatGunDead()
{
  for(FloatGun* gun : floatGunGroup){
    if(gun != NULL){
      return false;
    }
  }
  return true;
}
void Boss::clearFloatGun()
{
  GamePlay *play = GamePlay::sharedGamePlay();
  for(int i =0; i < FLOAT_GUN_COUNT; i++){
    FloatGun* gun = floatGunGroup[i];
    if(gun != NULL && gun->mState != Dead){
      play->manager->removeGameObject(gun);
    }
    floatGunGroup[i] = NULL;
  }
}
void FloatGun::onCreate() {
	mCollisionCircles.push_back(Circle(cocos2d::Vec2(6, 12), 9));
	mCollisionCircles.push_back(Circle(cocos2d::Vec2(17, 27), 13));

	Role::onCreate();

  
  attackTimeIntervalRange.set(4, 10);
  mTargetPos.init(bossMoveRange, Vec2(RESPAWN_YMIN * 0.4f, RESPAWN_YMIN + RESPAWN_Y));
	//计算起跳点

  const Vec2& pos =mTargetPos.getTarget();
	mSprite->setPosition(pos);
	//mSprite->playGTAnimation(7, true);
	mParent->addChild(mSprite,LAYER_ROLE+RESPAWN_Y);

  markIndex();
  
	mDartCount = 0;
	mFlag = true;
	mSpeed = ENEMY_NNRUNSPEED/2;
  setMaxHp(isOneStageAttackMode? FLOAT_GUN_HP1: FLOAT_GUN_HP2);
}


void FloatGun::afterDamage()
{
  if(isDead()){
    //CCLOG("--floatGun %d dead %p",idx, this);
    resetCoroutine();
    owner->onFloatGunDead(this);
  }
}

void FloatGun::onShooting(){
  mState = Shooting;
  mSprite->playGTAnimation(5, false);
  repeatAction(3,0.3, [this]() ->bool {
    std::vector<Vec2> dirList;
    if(this->isOneStageAttackMode){
      dirList.push_back(Vec2(-1,0));
    }else{
      dirList.push_back(Vec2(-1,-1));
      dirList.push_back(Vec2(1,-1));
    }
    this->shootDart(dirList);
    return true;
  }, [this]() -> bool{
    if( randomInt(3) < 2 ) {
      mState = Running;
      return false;
    } 
  });
  
}