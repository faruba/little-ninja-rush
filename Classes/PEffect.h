#ifndef ___CLASSES_PEFFECT_H_
#define ___CLASSES_PEFFECT_H_
//
//  PEffect.h
//  NinjiaRush
//
//  Created by 马 颂文 on 12-5-9.
//  Copyright (c) 2012年 TrinGame. All rights reserved.
//

#include "GameTool.h"

class Wood:
  public GameObject
{
  public:
    static Wood* wood(CCPoint pos, CCPoint dir, CCNode* parent, GOHandler target, CCPoint anchor);

    CREATE_FUNC(Wood);

    virtual void onCreate();
    virtual void onUpdate(float delta);
    virtual void onDestroy();

    CCNode *mParent;
    CCPoint mPos;
    CCPoint mDir;
    CCPoint mAnchor;
    CCSprite *mSprite;
    float mTimer;

    GOHandler mTarget;
    bool mFlag;


};
#endif
