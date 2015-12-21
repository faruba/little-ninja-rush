#ifndef ___CLASSES_POPKILL_H_
#define ___CLASSES_POPKILL_H_
//
//  PopKill.h
//  NinjiaRush
//
//  Created by 马 颂文 on 12-11-30.
//  Copyright (c) 2012年 TrinGame. All rights reserved.
//

#include "GameTool.h"

class PopKill:public GameObject
{
  public:
    static PopKill* popKill(bool steak, int num, cocos2d::Point pos);
    CREATE_FUNC(PopKill);

    virtual void onCreate();
    virtual void onUpdate(float delta);
    virtual void onDestroy();
cocos2d::Label *label;

    void onCompleteAction();
};
#endif
