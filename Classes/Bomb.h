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
    static Bomb* bomb(CCPoint pos, CCPoint dir, CCNode* parent);

    virtual void onCreate();
    virtual void onUpdate(float delta);
    virtual void onDestroy();
    virtual bool isEnemy();
    virtual CCPoint position();
    virtual void onHitback(CCPoint origin);

    CCNode *mParent;

    GTAnimatedSprite *mSprite;
    CCPoint mPosition;
    CCPoint mDirection;
    float mTimer;

    bool mPaused;
};
#endif
