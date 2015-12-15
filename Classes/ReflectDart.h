#ifndef ___CLASSES_REFLECTDART_H_
#define ___CLASSES_REFLECTDART_H_
//
//  ReflectDart.h
//  NinjiaRush
//
//  Created by 马 颂文 on 12-6-25.
//  Copyright (c) 2012年 TrinGame. All rights reserved.
//

#include "GameObject.h"

class ReflectDart:
  public GameObject
{
  public:
    static ReflectDart* dart(cocos2d::Point pos, cocos2d::Point dir);

    CREATE_FUNC(ReflectDart);
    virtual void onCreate();
    virtual void onUpdate(float delta);
    virtual void onDestroy();
cocos2d::CCSprite *mSprite;
    cocos2d::Point mPos;
    cocos2d::Point mDir;

    int mReflectCounter;
    bool mPaused;


};
#endif
