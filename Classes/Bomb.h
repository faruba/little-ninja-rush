#ifndef ___CLASSES_BOMB_H_
#define ___CLASSES_BOMB_H_
//
//  Bomb.h
//  NinjiaRush
//
//  Created by 马 颂文 on 12-5-17.
//  Copyright (c) 2012年 TrinGame. All rights reserved.
//

#include "cocos2d.h"
#include "GameTool.h"
#include "Darts.h"

class Bomb:
  public Darts
{
  public:
    CREATE_FUNC(Bomb);
    static Bomb* bomb(cocos2d::Point pos, cocos2d::Point dir, CCNode* parent);

    virtual void onCreate();
    virtual void onUpdate(float delta);
    virtual void onDestroy();
    virtual bool isEnemy();
    virtual cocos2d::Point position();
    virtual void onHitback(cocos2d::Point origin);

    CCNode *mParent;

    GTAnimatedSprite *mSprite;
    cocos2d::Point mPosition;
    cocos2d::Point mDirection;
    float mTimer;

    bool mPaused;
};
#endif
