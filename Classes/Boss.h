//
//  Boss.h
//  little-ninja-rush
//
//  Created by tringame on 16/1/14.
//
//

#ifndef __little_ninja_rush__Boss__
#define __little_ninja_rush__Boss__

#include <stdio.h>
#include "GameConstants.h"
#include "Role.h"
#define FLOAT_GUN_COUNT 5


class Coroutine
{
public:
  Coroutine(){
    callback = NULL;
    timer = excuteTime = 0;
  }
  // return true if loop
  typedef std::function<bool()> callbackFunction;
  bool startAfterSecond(float time, callbackFunction cb,bool replaceExist){
    if(!replaceExist && callback != NULL){
      return false;
    }
    
    timer = 0;
    excuteTime = time;
    callback  =cb;
    return true;
  }
  void safeStartAfterSecond(float time, callbackFunction cb){ assert(startAfterSecond(time, cb, true));}
  void onUpdateCoroutine(float delta){
    if(callback == NULL ){
      return;
    }
    timer += delta;
    if(timer >= excuteTime){
      if(callback()){
        timer = 0;
      }else{
        callback = NULL;
      }
    }
  }
private:
  float timer;
  float excuteTime;
  callbackFunction callback = NULL;
};

typedef Vec2 Range;


class TargetPositionPlan
{
public:
  void init(const Range& moveXRange, const Range& moveYRange) {
    this->positionXRange =  moveXRange;
    this->positionYRange =  moveYRange;
    genNextPlan();
  }
  void genNextPlan()
  {
    target.x = cocos2d::random(positionXRange.x, positionXRange.y);
    target.y = cocos2d::random(positionYRange.x, positionYRange.y);
  }
  const Vec2& getTarget(){ return target;}
private:
  Range positionYRange;
  Range positionXRange;
  Vec2 target;
};

class MoveAndAttackRole: public Role, public Coroutine
{
public:
  
  virtual void onUpdate(float delta);
  virtual void onDestroy();
  
  
  virtual bool deliverHit(int type, cocos2d::Point dir);
  virtual cocos2d::Point center();
  virtual bool supportAimAid();
  float  mTimer;
  int    mDartCount;
  bool   mFlag;
  
  float mStepSnow;
  
  
protected:
  Vec2 getAttackDir();
  void playPrepareAnimation();
  
  virtual void onEntering(float dt, bool playend);
  virtual void onRunning(float dt, bool playend);
  virtual void onPreparingToShoot(float dt);
  virtual void onShooting();
  virtual bool onDead(float delta, bool playend);
  
  virtual void afterDamage() = 0;

  //run and speed
  const float speedRate = 2;
  float  mSpeed;
  bool isHighSpeedMode  = false;
  float speed(){ return isHighSpeedMode? mSpeed * speedRate : mSpeed;}
  const Vec2& currentPos(){return mSprite->getPosition();}
  void setCurrentPos(const Vec2& pos) { mSprite->setPosition(pos);}
  
  TargetPositionPlan mTargetPos;
  
  //attack and speed
  const float attackRate = 0.5;
  Range attackTimeIntervalRange;
  bool isHighAttackSpeedMode = false;
  
  float nextAttackTimeInterval(){
    int minValue = attackTimeIntervalRange.x ;
    int maxValue = attackTimeIntervalRange.y ;
    if(isHighSpeedMode){
      minValue *= attackRate;
      maxValue *= attackRate;
    }
    return cocos2d::random(minValue, maxValue);
  }
  
  bool isGodmode = false;
   // life
  int hp ;
  bool isDead() {return hp <= 0;}
  
};

class FloatGun;
class Boss: public MoveAndAttackRole{
public:
  CREATE_FUNC(Boss);
  virtual void onCreate();
  void onFloatGunDead(FloatGun* floatGun);
  virtual const char* animationSetName() { return "mninja"; }
protected:
  virtual void onShooting();
  void onSpecialShoot(int count);
  void releaseFloatGun(const Vec2& pos, int index);
  virtual void afterDamage();
private:
  FloatGun* floatGunGroup[FLOAT_GUN_COUNT];
  //flag
  bool isMakedSpecialShoot = false;
  bool isState2 = false;
  void clearFloatGun();

};
class FloatGun :public  MoveAndAttackRole
{
public:
  CREATE_FUNC(FloatGun);
  virtual void onCreate();
  virtual const char* animationSetName() { return "hninja"; }
  void setOwner(Boss* boss,int index){
    owner = boss;
    idx = index;
  }
  virtual void afterDamage();
  int idx;
private:
  Boss* owner;
};
#endif /* defined(__little_ninja_rush__Boss__) */

