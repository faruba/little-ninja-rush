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

@protocol Scene <CCObject>

void loadScene();
void unloadScene();

void createScene(CCNode* bg, CCNode* fbg, CCNode* fg, RingTile* tiles);
void update(float delta);

};
#endif
