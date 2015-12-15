#ifndef ___CLASSES_STARFALL_H_
#define ___CLASSES_STARFALL_H_
//
//  Starfall.h
//  NinjiaRush
//
//  Created by 马 颂文 on 12-6-26.
//  Copyright (c) 2012年 TrinGame. All rights reserved.
//

#include "GameObject.h"

class Starfall:
  public GameObject
{
  public:
    CREATE_FUNC(Starfall);
    static Starfall* starfall();

    virtual void onCreate();
    virtual void onUpdate(float delta);
    virtual void onDestroy();

    float mTimer;
    int   mWave;
    int   mCount;
};
#endif
