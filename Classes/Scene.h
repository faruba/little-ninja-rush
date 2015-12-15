#ifndef ___CLASSES_SCENE_H_
#define ___CLASSES_SCENE_H_
//
//  Scene.h
//  NinjiaRush
//
//  Created by 马 颂文 on 12-4-29.
//  Copyright (c) 2012年 TrinGame. All rights reserved.
//

#include <Foundation/Foundation.h>
#include "cocos2d.h"
#include "RingTile.h"

class SceneInterface: cocos2d::Object {
    virtual void loadScene() = 0;
    virtual void unloadScene() = 0;

    virtual void createScene(CCNode* bg, CCNode* fbg, CCNode* fg, RingTile* tiles) = 0;
    virtual void update(float delta) = 0;
};
#endif
