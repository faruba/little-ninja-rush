#ifndef ___CLASSES_TRACEDART_H_
#define ___CLASSES_TRACEDART_H_
//
//  TraceDart.h
//  NinjiaRush
//
//  Created by 马 颂文 on 12-6-25.
//  Copyright (c) 2012年 TrinGame. All rights reserved.
//

#include "GameObject.h"
#include "GameTool.h"

class TraceDart:
  public GameObject
{
  public:
    static TraceDart* dart(cocos2d::Point pos, cocos2d::Point dir);

    CREATE_FUNC(TraceDart);

    virtual void onCreate();
    virtual void onUpdate(float delta);
    virtual void onDestroy();
cocos2d::Sprite *mSprite;
    cocos2d::Point mPos;
    cocos2d::Point mDir;
    float   mTraceTimer;

    GOHandler mTrace;
    bool mPaused;
};
#endif
