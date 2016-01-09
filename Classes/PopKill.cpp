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
		pk->label = cocos2d::Label::createWithBMFont("popkill.fnt", std::string_format("%dSTK!", num).c_str());
		pk->label->setColor(Color3B(250, 41, 41));
	}
	else
	{
		pk->label = cocos2d::Label::createWithBMFont("popkill.fnt", std::string_format("+%d", num).c_str());
		pk->label->setColor(Color3B(255, 204, 0));
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
	cocos2d::ScaleTo *st1 = cocos2d::ScaleTo::create(0.2f, 1.2f);
	cocos2d::ScaleTo *st2 = cocos2d::ScaleTo::create(0.1f, 0.9f);
	cocos2d::ScaleTo *st3 = cocos2d::ScaleTo::create(0.05f, 1.1f);
	cocos2d::ScaleTo *st4 = cocos2d::ScaleTo::create(0.05f, 1);
	cocos2d::DelayTime *dt = cocos2d::DelayTime::create(0.6f);
	cocos2d::ScaleTo *st5 = cocos2d::ScaleTo::create(0.1f, 0);
	cocos2d::CCCallFunc *cf = cocos2d::CCCallFunc::create(this, callfunc_selector(PopKill::onCompleteAction));
	cocos2d::Sequence *seq = cocos2d::Sequence::create(st1, st2, st3, st4, dt, st5, cf, NULL);
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
