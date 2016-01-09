//
//  FeverBrust.m
//  NinjiaRush
//
//  Created by 马 颂文 on 12-7-31.
//  Copyright (c) 2012年 TrinGame. All rights reserved.
//

#include "FeverBrust.h"
#include "GamePlay.h"
#include "GameConstants.h"

#define FEVER_TIME (0.5f)



FeverBrust* FeverBrust::feverbrust(int eid, bool loop) 
{
	FeverBrust *ret = FeverBrust::create();
	ret->mEffectId = eid;
	ret->mLoop = loop;
	return ret;
}

void FeverBrust::onCreate() 
{
	GamePlay *play = GamePlay::sharedGamePlay();
	mSprite = GTAnimatedSprite::spriteWithGTAnimation(GTAnimation::loadedAnimationSet("effect"));
	mSprite->playGTAnimation(mEffectId, mLoop);
	mSprite->setPosition(play->mainrole->center());
	play->addChild(mSprite, LAYER_MAINROLE-1);
	mSprite->setScale(0.01);
	play->scheduleVibrate(0.5f);

	mTimer = 0;
}

void FeverBrust::onUpdate(float delta) 
{
	GamePlay *play = GamePlay::sharedGamePlay();
	mTimer += delta;
	if( mTimer < 1 )
	{
		bool playend = mSprite->updateGTAnimation(delta);
		float k = mTimer/FEVER_TIME;
		float r = 1.0f - k;
		if( playend )
		{
			r = 0;
		}
		mSprite->setScale(8.0f*k);
		mSprite->setOpacity(255*r);
	}
	else {
		play->manager->removeGameObject(this);
	}
}

void FeverBrust::onDestroy() 
{
	GamePlay *play = GamePlay::sharedGamePlay();
	play->removeChild(mSprite, false);
}


