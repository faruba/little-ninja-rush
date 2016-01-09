//
//  MoonWave.m
//  NinjiaRush
//
//  Created by 马 颂文 on 12-6-25.
//  Copyright (c) 2012年 TrinGame. All rights reserved.
//

#include "MoonWave.h"
#include "GamePlay.h"
#include "UniversalFit.h"



MoonWave* MoonWave::wave(cocos2d::Point pos) 
{
	MoonWave *ret = MoonWave::create();
	ret->mPos = pos;
	return ret;
}

void MoonWave::onCreate() 
{
	GamePlay *play = GamePlay::sharedGamePlay();
	mSprite = GTAnimatedSprite::spriteWithGTAnimation(GTAnimation::loadedAnimationSet("effect"));
	mSprite->playGTAnimation(26, true);
	mSprite->setAnchorPoint(cocos2d::Vec2(0, 0));
	mSprite->setPosition(mPos);
	play->addChild(mSprite, LAYER_MAINROLE+1);

	mShadow = GTGhostShadow::shadow(mSprite, 0.05f, 4);
	mShadow->opacity= 128;
	play->addChild(mShadow, LAYER_MAINROLE);
	mShadow->toggleShadow(true);

	GameTool::PlaySound("moonblade2.mp3");
}

void MoonWave::onUpdate(float delta) 
{
	GamePlay *play = GamePlay::sharedGamePlay();

	mSprite->updateGTAnimation(delta);
	mShadow->syncShadow(delta);

	//运动
	cocos2d::Point np = mSprite->getPosition();
	np.x += delta*400;
	mSprite->setPosition(np);
	//碰撞检测
	cocos2d::CCRect rect;
	rect.origin = mSprite->getPosition();
	rect.size = mSprite->getContentSize();
	bool ref = false;
	cocos2d::Ref* node = NULL;
	CCARRAY_FOREACH(play->darts, node)
	{
		Darts *dr = (Darts*)node;
		if(rect.containsPoint(dr->position())){
			play->manager->removeGameObject(dr);
			ref = true;
		}
	}
	//伤害判定
	node = NULL;
	CCARRAY_FOREACH(play->enemies, node)
	{
		Role * em = (Role*)node;
		if( rect.containsPoint(em->center()) )//杀伤半径
		{
			bool hit = em->deliverHit(HIT_MAGIC, Vec2(1, 0));
			if( hit )
			{
				GameTool::PlaySound("hit.mp3");
				GTAnimatedEffect *hiteff = GTAnimatedEffect::create(GTAnimation::loadedAnimationSet("effect"), 1, false);
				hiteff->setAnchorPoint(cocos2d::Vec2(0.5f, 0.5f));
				hiteff->setPosition(em->center());
				hiteff->setRotation(60 - CC_RADIANS_TO_DEGREES( ccpToAngle(cocos2d::Vec2(1, 0)) ) + 60*CCRANDOM_0_1());
				play->addChild(hiteff, LAYER_MAINROLE+1);

				GTAnimatedEffect *hiteff2 = GTAnimatedEffect::create(GTAnimation::loadedAnimationSet("effect"), 2, false);
				hiteff2->setAnchorPoint(cocos2d::Vec2(0.5f, 0.5f));
				hiteff2->setPosition(em->center());
				play->addChild(hiteff2, LAYER_ROLE);
			}
		}
	}
	if( ref )
	{
		GameTool::PlaySound("reflect.mp3");
	}
	//越界删除
	if( mSprite->getPosition().x > UniversalFit::sharedUniversalFit()->playSize.width+100 )
	{
		play->manager->removeGameObject(this);
	}
}

void MoonWave::onDestroy() 
{
	GamePlay::sharedGamePlay()->removeChild(mSprite, true);
	GamePlay::sharedGamePlay()->removeChild(mShadow, true);
}


