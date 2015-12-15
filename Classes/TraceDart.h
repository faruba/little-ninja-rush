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
    static TraceDart* dart(CCPoint pos, CCPoint dir);

    CREATE_FUNC(TraceDart);

    virtual void onCreate();
    virtual void onUpdate(float delta);
    virtual void onDestroy();

    CCSprite *mSprite;
    CCPoint mPos;
    CCPoint mDir;
    float   mTraceTimer;

    GOHandler mTrace;
    bool mPaused;
};
#endif
