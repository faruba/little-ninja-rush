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
    static ReflectDart* dart(CCPoint pos, CCPoint dir);

    CREATE_FUNC(ReflectDart);
    virtual void onCreate();
    virtual void onUpdate(float delta);
    virtual void onDestroy();

    CCSprite *mSprite;
    CCPoint mPos;
    CCPoint mDir;

    int mReflectCounter;
    bool mPaused;


};
#endif
