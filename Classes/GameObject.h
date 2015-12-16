#ifndef ___CLASSES_GAMEOBJECT_H_
#define ___CLASSES_GAMEOBJECT_H_
//
//  GameObject.h
//  NinjiaRush
//
//  Created by 马 颂文 on 12-5-31.
//  Copyright (c) 2012年 TrinGame. All rights reserved.
//

/*** TRANSLATED BY HAMMER @2013-3-25 ***/

#include "cocos2d.h"

using namespace cocos2d;

typedef struct
{
    int index;
    int uid;
}GOHandler;

class GameObject:
  public Ref
{
  private:
    GOHandler mHandler;

  public:
    static GameObject* dummyObject();

    void setHandler(int index, int uid);
    GOHandler handler();

    virtual void onCreate();
    virtual void onUpdate(float delta);
    virtual void onDestroy();

    virtual bool init();
    CREATE_FUNC(GameObject);
};
#endif
