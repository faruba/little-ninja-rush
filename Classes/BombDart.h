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
    static BombDart* dart(CCPoint pos, CCPoint dir);

    CREATE_FUNC(BombDart);

    virtual void onCreate();
    virtual void onUpdate(float delta);
    virtual void onDestroy();

    GTAnimatedSprite *mSprite;
    CCPoint mPos;
    CCPoint mDir;
    CCPoint mDst;
    CCPoint mMid;
    float   mTimer;

    bool mPaused;
};
#endif
