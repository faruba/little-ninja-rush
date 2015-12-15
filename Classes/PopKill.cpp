//
//  PopKill.m
//  NinjiaRush
//
//  Created by 马 颂文 on 12-11-30.
//  Copyright (c) 2012年 TrinGame. All rights reserved.
//

#include "PopKill.h"
#include "GamePlay.h"

PopKill* PopKill::popKill(bool steak, int num, cocos2d::Point pos) 
{
    PopKill *pk = PopKill::create();
    if( steak )
    {
        pk->label = cocos2d::CCLabelBMFont::create(cocos2d::CCString::createWithFormat("%dSTK!", num)->getCString(), "popkill.fnt");
        pk->label->setColor(ccc3(250, 41, 41));
    }
    else
    {
        pk->label = cocos2d::CCLabelBMFont::create(cocos2d::CCString::createWithFormat("+%d", num)->getCString(), "popkill.fnt");
        pk->label->setColor(ccc3(255, 204, 0));
    }
    pk->label->setPosition(pos);
    pk->label->setRotation(-30 + 60*CCRANDOM_0_1());
    return pk;
}

void PopKill::onCreate() 
{
    GamePlay::sharedGamePlay()->addChild(label, LAYER_MAINROLE+1);
    //set animation
    //label->setScale(0);
cocos2d::CCScaleTo *st1 = cocos2d::CCScaleTo::create(0.2f, 1.2f);
cocos2d::CCScaleTo *st2 = cocos2d::CCScaleTo::create(0.1f, 0.9f);
cocos2d::CCScaleTo *st3 = cocos2d::CCScaleTo::create(0.05f, 1.1f);
cocos2d::CCScaleTo *st4 = cocos2d::CCScaleTo::create(0.05f, 1);
cocos2d::CCDelayTime *dt = cocos2d::CCDelayTime::create(0.6f);
cocos2d::CCScaleTo *st5 = cocos2d::CCScaleTo::create(0.1f, 0);
cocos2d::CCCallFunc *cf = cocos2d::CCCallFunc::create(this, callfunc_selector(PopKill::onCompleteAction));
cocos2d::CCSequence *seq = cocos2d::CCSequence::create(st1, st2, st3, st4, dt, st5, cf, NULL);
    label->runAction(seq);
}

void PopKill::onUpdate(float) {}
void PopKill::onDestroy() 
{
    GamePlay::sharedGamePlay()->removeChild(label, true);
}

void PopKill::onCompleteAction() 
{
    GamePlay::sharedGamePlay()->manager->removeGameObject(this);
}
