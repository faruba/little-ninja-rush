//
//  ReflectDart.m
//  NinjiaRush
//
//  Created by 马 颂文 on 12-6-25.
//  Copyright (c) 2012年 TrinGame. All rights reserved.
//

#include "ReflectDart.h"
#include "GamePlay.h"
#include "UniversalFit.h"



ReflectDart* ReflectDart::dart(cocos2d::Point pos, cocos2d::Point dir) 
{
	ReflectDart *ret = ReflectDart::create();
	ret->mPos = pos;
	ret->mDir = dir;
	return ret;
}

void ReflectDart::onCreate() 
{
	mSprite = cocos2d::Sprite::createWithSpriteFrameName("tjshuriken.png");
	mSprite->setAnchorPoint(cocos2d::Vec2(0.5f, 0.5f));
	mSprite->setPosition(mPos);
	cocos2d::RotateBy* rb = cocos2d::RotateBy::create(0.5f, 720);
	cocos2d::RepeatForever* rf = cocos2d::RepeatForever::create(rb);
	mSprite->runAction(rf);
	GamePlay::sharedGamePlay()->addChild(mSprite, LAYER_MAINROLE+1);

	mReflectCounter = 9;
	mPaused = false;
}

void ReflectDart::onUpdate(float delta) 
{
	GamePlay* play = GamePlay::sharedGamePlay();
	bool ref = false;//是否播放反弹声
	//暂停和恢复动作
	if( play->paused && !mPaused )
	{
		mPaused = true;
		mSprite->pauseSchedulerAndActions();
	}
	if( mPaused && !play->paused )
	{
		mPaused = false;
		mSprite->resumeSchedulerAndActions();
	}
	//反弹
	if( mReflectCounter > 0 )
	{
		if( mSprite->getPosition().x < 0 && mDir.x < 0 )
		{
			mDir.x *= -1;
			ref = true;
			mReflectCounter--;
		}
		if( mSprite->getPosition().x > UniversalFit::sharedUniversalFit()->playSize.width && mDir.x > 0 )
		{
			mDir.x *= -1;
			ref = true;
			mReflectCounter--;
		}
		if( mSprite->getPosition().y < 0 && mDir.y < 0 )
		{
			mDir.y *= -1;
			ref = true;
			mReflectCounter--;
		}
		if( mSprite->getPosition().y > SCREEN_HEIGHT && mDir.y > 0 )
		{
			mDir.y *= -1;
			ref = true;
			mReflectCounter--;
		}
	}
	//飞行
	cocos2d::Point ds = ccpMult(mDir, 400.0f*delta);
	cocos2d::Point np = ccpAdd(ds, mSprite->getPosition());
	mSprite->setPosition(np);
	//伤害判定
	cocos2d::Ref* node = NULL;
	CCARRAY_FOREACH(play->enemies, node)
	{
		Role * em = (Role*)node;
		if( em->collisionWithCircle(mSprite->getPosition(), 20) )//杀伤半径
		{
			bool hit = em->deliverHit(HIT_MAGIC, mDir);
			if( hit )
			{
				GameTool::PlaySound("hit.mp3");
				GTAnimatedEffect *hiteff = GTAnimatedEffect::create(GTAnimation::loadedAnimationSet("effect"), 1, false);
				hiteff->setAnchorPoint(cocos2d::Vec2(0.5f, 0.5f));
				hiteff->setPosition(em->center());
				//hiteff->setRotation(90 - CC_RADIANS_TO_DEGREES( ccpToAngle(direction) ));
				hiteff->setRotation(60 - CC_RADIANS_TO_DEGREES( ccpToAngle(mDir) ) + 60*CCRANDOM_0_1());
				play->addChild(hiteff, LAYER_MAINROLE+1);

				GTAnimatedEffect *hiteff2 = GTAnimatedEffect::create(GTAnimation::loadedAnimationSet("effect"), 2, false);
				hiteff2->setAnchorPoint(cocos2d::Vec2(0.5f, 0.5f));
				hiteff2->setPosition(em->center());
				play->addChild(hiteff2, LAYER_ROLE);
			}
			break;
		}
	}
	//弹镖
	node = NULL;
	CCARRAY_FOREACH(play->darts, node)
	{
		Darts *dr = (Darts*)node;
		if( dr->isEnemy() && exCollisionWithCircles(mSprite->getPosition(), 0, 0, 30, dr->position(), 7) && dr->position().y > mSprite->getPosition().y )
		{
			dr->onHitback(mSprite->getPosition());
			ref = true;
		}
	}
	if( ref )
	{
		GameTool::PlaySound("reflect.mp3");
	}

	//越界判定
	if( mReflectCounter <= 0 )
	{
		if( mSprite->getPosition().x < -100 || mSprite->getPosition().x > UniversalFit::sharedUniversalFit()->playSize.width+100
				|| mSprite->getPosition().y < -100 || mSprite->getPosition().y > SCREEN_HEIGHT+100 )
		{
			play->manager->removeGameObject(this);
		}
	}
}

void ReflectDart::onDestroy() 
{
	GamePlay::sharedGamePlay()->removeChild(mSprite, true);
}


