//
//  Starfall.m
//  NinjiaRush
//
//  Created by 马 颂文 on 12-6-26.
//  Copyright (c) 2012年 TrinGame. All rights reserved.
//

#include "Starfall.h"
#include "GamePlay.h"
#include "Star.h"
#include "GameRecord.h"
#include "UniversalFit.h"



Starfall* Starfall::starfall() 
{
	return Starfall::create();
}

void Starfall::onCreate() 
{
	mTimer = 0;
	int level = GameRecord::sharedGameRecord()->item_upgrade[6];
	mCount = 2+level;
	mWave = 0;
}

void Starfall::onUpdate(float delta) 
{
	mTimer += delta;
	float tar = mWave*3;
	if( mTimer >= tar )
	{
		GamePlay *play = GamePlay::sharedGamePlay();
		//respawn stars
		for(int i=0; i<3; ++i)
		{
			cocos2d::Point pos;
			pos.x = 50 + (UniversalFit::sharedUniversalFit()->playSize.width - 150)*CCRANDOM_0_1();
			pos.y = SCREEN_HEIGHT + 50 + 100*CCRANDOM_0_1();
			float angle = CC_DEGREES_TO_RADIANS(-80.0f + 20.0f*CCRANDOM_0_1());
			cocos2d::Point dir = ccpForAngle(angle);
			play->manager->addGameObject(Star::star(pos, dir));
		}

		mWave++;
		mCount--;
		if( mCount <= 0 )
		{
			play->manager->removeGameObject(this);
		}
	}
}

void Starfall::onDestroy() 
{

}


