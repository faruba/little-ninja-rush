#ifndef ___CLASSES_PUMPKINCIRCLE_H_
#define ___CLASSES_PUMPKINCIRCLE_H_
//
//  PumpkinCircle.h
//  NinjiaRush
//
//  Created by 马 颂文 on 12-9-21.
//  Copyright (c) 2012年 TrinGame. All rights reserved.
//

#include "GameTool.h"

#define PUMPKIN_NUM (10)

class PumpkinCircle:public GameObject
{
  public:
  static void summonPumpkin();
  static void dismissPumpkins();

  CREATE_FUNC(PumpkinCircle);

  virtual bool init();
  virtual void onCreate();
  virtual void onUpdate(float delta);
  virtual void onDestroy();

  void updateAngles();
  void addPumpkin(int type);
  void removePumpkin(int pid);

  CCSprite *mSprites[PUMPKIN_NUM];
  float mTimers[PUMPKIN_NUM];
  float mPowerTimers[PUMPKIN_NUM];
  int mPumpkinTypes[PUMPKIN_NUM];

  float mAngles[PUMPKIN_NUM];

  int mPumpkinCount;
  float mCircieAngle;


};
#endif
