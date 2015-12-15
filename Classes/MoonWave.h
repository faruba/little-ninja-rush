#ifndef ___CLASSES_MOONWAVE_H_
#define ___CLASSES_MOONWAVE_H_
//
//  MoonWave.h
//  NinjiaRush
//
//  Created by 马 颂文 on 12-6-25.
//  Copyright (c) 2012年 TrinGame. All rights reserved.
//

#include "GameObject.h"
#include "GameTool.h"

class MoonWave:public GameObject
{
  public:
    static MoonWave* wave(cocos2d::Point pos) ;

    CREATE_FUNC(MoonWave);

    virtual bool init(){return true;};
    virtual void onCreate();
    virtual void onUpdate(float delta);
    virtual void onDestroy();

    GTAnimatedSprite *mSprite;
    GTGhostShadow *mShadow;
    cocos2d::Point mPos;



};
#endif
