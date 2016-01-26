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
#define FLOAT_GUN_COUNT 10


class Coroutine
{
public:
  typedef std::function<bool()> callbackFunction;
private:
  struct excuteRecord{
  public:
    excuteRecord(float time,float delay, callbackFunction cb){
      timer = 0 - delay;
      excuteTime = time;
      callback = cb;
    }
    excuteRecord(){
      timer = excuteTime = 0;
      callback = nullptr;
    }
    float timer;
    float excuteTime;
    callbackFunction callback = nullptr;
  };
public:
  Coroutine(){
    resetCoroutine();
  }
  // return true if loop
  void resetCoroutine(){
    list.clear();
  }
  void startAfterSecond(float time,float delay, callbackFunction cb){
    list.push_back(excuteRecord(time, delay,cb));
  }
  void onUpdateCoroutine(float delta){
    for(excuteRecord& rec : list){
      rec.timer += delta;
      if(rec.timer >= rec.excuteTime){
        if(!rec.callback){
          continue;
        }
        if(rec.callback()){
          rec.timer = 0;
        }else{
          rec.callback = nullptr;
        }
      }
    }
    auto pend = std::remove_if(list.begin(), list.end(),[](excuteRecord& rec) ->bool{return !rec.callback;});
    list.erase(pend, list.end());
  }
private:
  std::vector<excuteRecord> list;

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
  void setMaxHp(int hp){
    this->hp = hp;
  }
  
  void changeState(int state){
    if(mState != Dead){
      mState = state;
    }
  }
  int currentState(){
    return mState;
  }
  
protected:
  Vec2 getAttackDir(bool isAim = false);
  void playPrepareAnimation();
  
  virtual void onEntering(float dt, bool playend);
  virtual void onRunning(float dt, bool playend);
  virtual void onPreparingToShoot(float dt);
  virtual void onShooting();
  virtual void onFleeing(){}
  virtual bool onDead(float delta, bool playend);
  void shootDart(const std::vector<Vec2>& dirList, const std::string& shape = "dart.png");
  
  typedef std::function<void (int index)> repeatCB;
  void repeatAction(int times, float timeInterval, repeatCB cb, repeatCB onFinished, float delay = 0);
  
  virtual void afterDamage() = 0;

  
  //run and speed
  const float speedRate = 2;
  float  mSpeed;
  bool isHighSpeedMode  = false;
  float speed();
  const Vec2& currentPos(){return mSprite->getPosition();}
  void setCurrentPos(const Vec2& pos) { mSprite->setPosition(pos);}
  
  TargetPositionPlan mTargetPos;

  //attack and speed
  const float attackRate = 0.5;
  Range attackTimeIntervalRange;

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
  virtual void onUpdate(float delta);
  void onFloatGunDead(FloatGun* floatGun);
  virtual const char* animationSetName() { return "boss"; }
  virtual bool supportAimAid(){return false;}
  virtual void onShooting();
  void onSpecialShoot();
  void releaseFloatGun(const Vec2& pos, int& index);
protected:
  virtual void afterDamage();
  void playShellEffect(){
    shell = GTAnimatedSprite::spriteWithGTAnimation(GTAnimation::loadedAnimationSet("bossEffect"));
    shell->setScale(2);
    shell->setPosition(80,80);
    shell->playGTAnimation(0, true);
    mSprite->addChild(shell);
  }
  void removeShellEffect(){
    if(shell != NULL){
      mSprite->removeChild(shell, true);
      shell = NULL;
    }
  }
private:
  virtual bool onDead(float delta, bool playend);
  std::vector<FloatGun*> floatGunGroup;
  bool isAllFloatGunDead();
  //flag
  bool isMakedSpecialShoot = false;
  bool isState2 = false;
  void clearFloatGun();
  GTAnimatedSprite* shell = NULL;
};
class FloatGun :public  MoveAndAttackRole
{
public:
  CREATE_FUNC(FloatGun);
  virtual void onCreate();
  virtual const char* animationSetName() { return "floatGun"; }
  virtual bool supportAimAid(){return false;}
  void setOwner(Boss* boss,int index,bool isOneStage){
    owner = boss;
    idx = index;
    isOneStageAttackMode   = isOneStage;
  }
  virtual void afterDamage();
  int idx;
  bool isOneStageAttackMode  = true;
protected:
  virtual void onShooting();
private:
  void markIndex(){
    auto mark = cocos2d::Label::createWithBMFont("ab34.fnt", std::string_format("%d",idx));
    mark->setPositionX(20);
    mSprite->addChild(mark);
  }
  Boss* owner;
};

class LittleBoss :public  Boss
{
public:
  CREATE_FUNC(LittleBoss);
  virtual void onCreate();
  virtual const char* animationSetName() { return "little_boss"; }
  virtual bool supportAimAid(){return false;}
  virtual void onShooting();
  virtual void onUpdate(float delta);
protected:
  virtual void afterDamage();
private:
  void playBeamEffect(const Vec2& dir){
    beam = GTAnimatedSprite::spriteWithGTAnimation(GTAnimation::loadedAnimationSet("littlebossEffect"));
    beam->setScale(2);
    beam->setAnchorPoint(Vec2(0.5,1));
    beam->setPosition(90,50);
    beam->setRotation(-(CC_RADIANS_TO_DEGREES(dir.getAngle())+90));
    beam->playGTAnimation(0, true);
    mSprite->addChild(beam);
    beamSoundId = GameTool::PlaySound("beem.mp3");
  }
  void stopBeamEffect(){
    mSprite->removeChild(beam, true);
    beam = NULL;
    if(beamSoundId != 0){
      GameTool::StopSound(beamSoundId);
    }
  }
  GTAnimatedSprite* beam;
  int beamSoundId = 0;

 
};
#endif /* defined(__little_ninja_rush__Boss__) */

