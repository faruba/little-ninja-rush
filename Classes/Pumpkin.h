#ifndef ___CLASSES_PUMPKIN_H_
#define ___CLASSES_PUMPKIN_H_

#include "GameTool.h"
#include "Role.h"

class Pumpkin:
  public Role
{
public:
  CREATE_FUNC(Pumpkin);
  
  virtual void onCreate();
  virtual void onUpdate(float delta);
  virtual void onDestroy();
  
  virtual const char* animationSetName() { return "pumpkin"; }
  
  virtual bool deliverHit(int type, cocos2d::Point dir);
  virtual cocos2d::Point center();
  virtual bool supportAimAid();
  
  void moveToTargetPos(float delta);
  void updateTargetPos(float delta);
  float mTimer;
  int mHitCount;
  float mHurtTimer;
  
  int mFacing;
  int mCoinsCounter;
  
  cocos2d::Vec2 mTargetPos;
  cocos2d::Vec2 mPumpkinSpeed;
  cocos2d::Vec2 mTargetSpeed;
};
#endif
