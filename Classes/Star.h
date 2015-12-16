#ifndef ___CLASSES_STAR_H_
#define ___CLASSES_STAR_H_
//
//  Star.h
//  NinjiaRush
//
//  Created by 马 颂文 on 12-6-26.
//  Copyright (c) 2012年 TrinGame. All rights reserved.
//

#include "GameObject.h"

class Star:
  public GameObject
{
  public:
    static Star* star(cocos2d::Point pos, cocos2d::Point dir);

    CREATE_FUNC(Star);

    virtual void onCreate();
    virtual void onUpdate(float delta);
    virtual void onDestroy();
cocos2d::Sprite *mSprite;
    cocos2d::Point mPos;
    cocos2d::Point mDir;


};
#endif
