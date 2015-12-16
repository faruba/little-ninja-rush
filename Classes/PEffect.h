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
    static Wood* wood(cocos2d::Point pos, cocos2d::Point dir, CCNode* parent, GOHandler target, cocos2d::Point anchor);

    CREATE_FUNC(Wood);

    virtual void onCreate();
    virtual void onUpdate(float delta);
    virtual void onDestroy();
cocos2d::CCNode *mParent;
    cocos2d::Point mPos;
    cocos2d::Point mDir;
    cocos2d::Point mAnchor;
cocos2d::Sprite *mSprite;
    float mTimer;

    GOHandler mTarget;
    bool mFlag;


};
#endif
