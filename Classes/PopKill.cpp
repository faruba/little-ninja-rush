//
//  PopKill.m
//  NinjiaRush
//
//  Created by 马 颂文 on 12-11-30.
//  Copyright (c) 2012年 TrinGame. All rights reserved.
//

#include "PopKill.h"
#include "GamePlay.h"

PopKill* PopKill::popKill(bool steak, int num, CCPoint pos) 
{
    PopKill *pk = PopKill::create();
    if( steak )
    {
        pk->label = CCLabelBMFont::create(CCString::createWithFormat("%dSTK!", num)->getCString(), "popkill.fnt");
        pk->label->setColor(ccc3(250, 41, 41));
    }
    else
    {
        pk->label = CCLabelBMFont::create(CCString::createWithFormat("+%d", num)->getCString(), "popkill.fnt");
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
    CCScaleTo *st1 = CCScaleTo::create(0.2f, 1.2f);
    CCScaleTo *st2 = CCScaleTo::create(0.1f, 0.9f);
    CCScaleTo *st3 = CCScaleTo::create(0.05f, 1.1f);
    CCScaleTo *st4 = CCScaleTo::create(0.05f, 1);
    CCDelayTime *dt = CCDelayTime::create(0.6f);
    CCScaleTo *st5 = CCScaleTo::create(0.1f, 0);
    CCCallFunc *cf = CCCallFunc::create(this, callfunc_selector(PopKill::onCompleteAction));
    CCSequence *seq = CCSequence::create(st1, st2, st3, st4, dt, st5, cf, NULL);
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
