#ifndef ___CLASSES_BOMBDART_H_
#define ___CLASSES_BOMBDART_H_
//
//  BombDart.h
//  NinjiaRush
//
//  Created by 马 颂文 on 12-6-25.
//  Copyright (c) 2012年 TrinGame. All rights reserved.
//

#include "GameObject.h"
#include "GameTool.h"

class BombDart:
  public GameObject
{
  public:
    static BombDart* dart(cocos2d::Point pos, cocos2d::Point dir);

    CREATE_FUNC(BombDart);

    virtual void onCreate();
    virtual void onUpdate(float delta);
    virtual void onDestroy();

    GTAnimatedSprite *mSprite;
    cocos2d::Point mPos;
    cocos2d::Point mDir;
    cocos2d::Point mDst;
    cocos2d::Point mMid;
    float   mTimer;

    bool mPaused;
};
#endif
