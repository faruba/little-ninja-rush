//
//  StaticParticle.m
//  NinjiaRush
//
//  Created by 马 颂文 on 12-7-11.
//  Copyright (c) 2012年 TrinGame. All rights reserved.
//

#include "StaticParticle.h"
#include "GamePlay.h"



StaticParticle* StaticParticle::particleSakura(cocos2d::Point pos) 
{
	StaticParticle *pat = StaticParticle::create();
	pat->mSprite = cocos2d::Sprite::createWithSpriteFrameName("skblade6.png");
	pat->mSprite->setPosition(pos);
	pat->mSprite->retain();
	pat->mDir = ccpForAngle(CC_DEGREES_TO_RADIANS(180.0f*CCRANDOM_0_1()));
	pat->mSpeed = 100.0f + 50.0f*CCRANDOM_0_1();
	pat->mLife = 2.0f + 2.0f*CCRANDOM_0_1();
	pat->mRotate = 60;
	pat->mFade = true;
	pat->mFlipX = 1 + CCRANDOM_0_1();
	pat->mGravityY = -100;
	pat->mGravityX = -200;
	pat->mStick = false;
	return pat;
}

StaticParticle* StaticParticle::particleIce(cocos2d::Point pos, bool up) 
{
	StaticParticle *pat = StaticParticle::create();
	pat->mSprite = cocos2d::Sprite::createWithSpriteFrameName("snow.png");
	pat->mSprite->setPosition(pos);
	pat->mSprite->retain();
	if( up )
	{
		pat->mDir = ccpForAngle(CC_DEGREES_TO_RADIANS(180.0f*CCRANDOM_0_1()));
	}
	else {
		pat->mDir = ccpForAngle(CC_DEGREES_TO_RADIANS(360.0f*CCRANDOM_0_1()));
	}
	pat->mSpeed = 100.0f + 50.0f*CCRANDOM_0_1();
	pat->mLife = 1.0f + 1.0f*CCRANDOM_0_1();
	pat->mRotate = 180;
	pat->mFade = true;
	pat->mFlipX = 0;
	pat->mGravityY = -100;
	pat->mGravityX = -200;
	pat->mStick = false;
	return pat;
}

StaticParticle* StaticParticle::particleShadow(cocos2d::Point pos) 
{
	StaticParticle *pat = StaticParticle::create();
	pat->mSprite = cocos2d::Sprite::createWithSpriteFrameName("ayblade7.png");
	pat->mSprite->retain();
	pat->mDir = ccpForAngle(CC_DEGREES_TO_RADIANS(180.0f*CCRANDOM_0_1()));
	cocos2d::Point npos = ccpAdd(pos, ccpMult(pat->mDir, 20));
	pat->mSprite->setPosition(npos);
	pat->mSpeed = 100.0f + 50.0f*CCRANDOM_0_1();
	pat->mLife = 0.5f;
	pat->mRotate = 800;
	pat->mFade = true;
	pat->mFlipX = 0;
	pat->mGravityY = 0;
	pat->mGravityX = 0;
	pat->mStick = false;
	pat->mShurink = true;
	return pat;
}

StaticParticle* StaticParticle::particleIceAir(cocos2d::Point pos, bool up) 
{
	StaticParticle *pat = StaticParticle::create();
	pat->mSprite = cocos2d::Sprite::createWithSpriteFrameName("bblade7.png");
	pat->mSprite->retain();
	if( up )
	{
		pat->mDir = ccpForAngle(CC_DEGREES_TO_RADIANS(180.0f*CCRANDOM_0_1()));
	}
	else {
		pat->mDir = ccpForAngle(CC_DEGREES_TO_RADIANS(360.0f*CCRANDOM_0_1()));
	}
	//cocos2d::Point npos = ccpAdd(pos, ccpMult(pat->mDir, 20));
	pat->mSprite->setPosition(pos);
	pat->mSpeed = 70.0f + 50.0f*CCRANDOM_0_1();
	pat->mLife = 0.5f+0.5f*CCRANDOM_0_1();
	pat->mRotate = 200;
	pat->mFade = true;
	pat->mFlipX = 0;
	pat->mGravityY = 0;
	pat->mGravityX = 0;
	pat->mStick = false;
	pat->mShurink = true;
	return pat;
}

StaticParticle* StaticParticle::particleIcePiece(cocos2d::Point pos, cocos2d::Point dir) 
{
	cocos2d::Point odir = ccpMult(dir, -1);
	float angle = CC_RADIANS_TO_DEGREES(ccpToAngle(odir));
	StaticParticle* pat = StaticParticle::create();
	pat->mSprite = cocos2d::Sprite::createWithSpriteFrameName("bblade6.png");
	pat->mSprite->setPosition(pos);
	pat->mSprite->retain();
	pat->mDir = ccpForAngle(CC_DEGREES_TO_RADIANS(angle -30 + CCRANDOM_0_1()*60));
	pat->mSpeed = 100.0f + 50.0f*CCRANDOM_0_1();
	pat->mLife = 1.0f*CCRANDOM_0_1();
	pat->mRotate = 180;
	pat->mFade = true;
	pat->mFlipX = 0;
	pat->mGravityY = 0;
	pat->mGravityX = 0;
	pat->mStick = false;
	pat->mShurink = false;
	return  pat;
}

StaticParticle* StaticParticle::particleDash(cocos2d::Point pos) 
{
	StaticParticle *pat = StaticParticle::create();
	pat->mSprite = cocos2d::Sprite::createWithSpriteFrameName("dashline.png");
	pat->mSprite->retain();
	pat->mDir = Vec2(0, 0);
	cocos2d::Point npos = pos;
	npos.y += -20 + 40*CCRANDOM_0_1();
	pat->mSprite->setPosition(npos);
	pat->mSpeed = 0;
	pat->mLife = 0.2f + 0.3f*CCRANDOM_0_1();
	pat->mRotate = 0;
	pat->mFade = true;
	pat->mFlipX = 0;
	pat->mGravityY = 0;
	pat->mGravityX = 0;
	pat->mStick = false;
	pat->mShurink = false;
	return pat;
}

StaticParticle* StaticParticle::particleDart(cocos2d::Point pos, cocos2d::Point dir, int typ) 
{
	StaticParticle *pat = NULL;
	cocos2d::Point odir = ccpMult(dir, -1);
	float angle = CC_RADIANS_TO_DEGREES(ccpToAngle(odir));
	switch (typ) {
		case 1://火镖
			{
				pat = StaticParticle::create();
				pat->mSprite = cocos2d::Sprite::createWithSpriteFrameName("fbh3.png");
				pat->mSprite->setPosition(pos);
				pat->mSprite->retain();
				pat->mDir = ccpForAngle(CC_DEGREES_TO_RADIANS(angle -30 + CCRANDOM_0_1()*60));
				pat->mSpeed = 100.0f + 50.0f*CCRANDOM_0_1();
				pat->mLife = 1.0f*CCRANDOM_0_1();
				pat->mRotate = 180;
				pat->mFade = true;
				pat->mFlipX = 0;
				pat->mGravityY = 0;
				pat->mGravityX = 0;
				pat->mStick = false;
				pat->mShurink = false;
			}
			break;
		case 2://冰镖
			{
				pat = StaticParticle::create();
				pat->mSprite = cocos2d::Sprite::createWithSpriteFrameName("snow.png");
				pat->mSprite->setPosition(pos);
				pat->mSprite->retain();
				pat->mDir = ccpForAngle(CC_DEGREES_TO_RADIANS(angle -30 + CCRANDOM_0_1()*60));
				pat->mSpeed = 100.0f + 50.0f*CCRANDOM_0_1();
				pat->mLife = 1.0f*CCRANDOM_0_1();
				pat->mRotate = 180;
				pat->mFade = true;
				pat->mFlipX = 0;
				pat->mGravityY = 0;
				pat->mGravityX = 0;
				pat->mStick = false;
				pat->mShurink = false;
			}
			break;
		case 3://樱花镖
			{
				pat = StaticParticle::create();
				pat->mSprite = cocos2d::Sprite::createWithSpriteFrameName("skblade6.png");
				pat->mSprite->setPosition(pos);
				pat->mSprite->retain();
				pat->mDir = ccpForAngle(CC_DEGREES_TO_RADIANS(angle -30 + CCRANDOM_0_1()*60));
				pat->mSpeed = 100.0f + 50.0f*CCRANDOM_0_1();
				pat->mLife = 1.0f*CCRANDOM_0_1();
				pat->mRotate = 180;
				pat->mFade = true;
				pat->mFlipX = 0.2f;
				pat->mGravityY = 0;
				pat->mGravityX = 0;
				pat->mStick = false;
				pat->mShurink = false;
			}
			break;
		case 4://真空镖
			{
				pat = StaticParticle::create();
				pat->mSprite = cocos2d::Sprite::createWithSpriteFrameName("fbzk_tail2.png");
				pat->mSprite->setPosition(pos);
				pat->mSprite->retain();
				pat->mDir = ccpForAngle(CC_DEGREES_TO_RADIANS(angle -30 + CCRANDOM_0_1()*60));
				pat->mSpeed = 100.0f + 50.0f*CCRANDOM_0_1();
				pat->mLife = 1.0f*CCRANDOM_0_1();
				pat->mRotate = 180;
				pat->mFade = true;
				pat->mFlipX = 0;
				pat->mGravityY = 0;
				pat->mGravityX = 0;
				pat->mStick = false;
				pat->mShurink = false;
			}
			break;
		case 5://暗影镖
			{
				pat = StaticParticle::create();
				pat->mSprite = cocos2d::Sprite::createWithSpriteFrameName("ayblade7.png");
				pat->mSprite->setPosition(pos);
				pat->mSprite->retain();
				pat->mDir = ccpForAngle(CC_DEGREES_TO_RADIANS(angle -30 + CCRANDOM_0_1()*60));
				pat->mSpeed = 100.0f + 50.0f*CCRANDOM_0_1();
				pat->mLife = 0.5f*CCRANDOM_0_1();
				pat->mRotate = 800;
				pat->mFade = true;
				pat->mFlipX = 0;
				pat->mGravityY = 0;
				pat->mGravityX = 0;
				pat->mStick = false;
				pat->mShurink = true;
			}
			break;
		case 6://追踪镖
			{
				pat = StaticParticle::create();
				pat->mSprite = cocos2d::Sprite::createWithSpriteFrameName("fbbf_tail.png");
				pat->mSprite->setPosition(pos);
				pat->mSprite->retain();
				pat->mDir = Vec2(0, 0);
				pat->mSpeed = 0;
				pat->mLife = 0.5f;
				pat->mRotate = 200;
				pat->mFade = true;
				pat->mFlipX = 0;
				pat->mGravityY = 0;
				pat->mGravityX = 0;
				pat->mStick = false;
				pat->mShurink = true;
			}
			break;
	}
	return pat;
}

StaticParticle* StaticParticle::particleAddHP(cocos2d::Point pos) 
{
	StaticParticle *pat = StaticParticle::create();
	pat->mSprite = cocos2d::Sprite::createWithSpriteFrameName("bxtx.png");
	//position diter
	cocos2d::Point diter = Vec2( -20 + 40*CCRANDOM_0_1(), -20 + 40*CCRANDOM_0_1() );
	pat->mSprite->setPosition(ccpAdd(pos, diter));
	pat->mSprite->retain();
	pat->mDir = ccpForAngle(CC_DEGREES_TO_RADIANS(90));
	pat->mSpeed = 50.0f + 50.0f*CCRANDOM_0_1();
	pat->mLife = 1.0f + 1.0f*CCRANDOM_0_1();
	pat->mRotate = 0;
	pat->mFade = true;
	pat->mFlipX = 0;
	pat->mGravityY = 0;
	pat->mGravityX = 0;
	pat->mStick = false;
	return pat;
}

StaticParticle* StaticParticle::particleStepDust(cocos2d::Point pos) 
{
	StaticParticle *pat = StaticParticle::create();
	pat->mSprite = cocos2d::Sprite::createWithSpriteFrameName("stepdust.png");
	cocos2d::Point diter = Vec2( -20+20*CCRANDOM_0_1(), 6*CCRANDOM_0_1() );
	pat->mSprite->setPosition(ccpAdd(pos, diter));
	pat->mSprite->retain();
	pat->mDir = ccpForAngle(CC_DEGREES_TO_RADIANS(100 + 60*CCRANDOM_0_1()));
	pat->mSpeed = 50 + 50*CCRANDOM_0_1();
	pat->mLife = 0.15f + 0.15f*CCRANDOM_0_1();
	pat->mRotate = 300;
	pat->mShurink = true;
	pat->mFade = true;
	pat->mFlipX = 0;
	pat->mGravityY = -100-200*CCRANDOM_0_1();
	pat->mGravityX = 0;
	pat->mStick = true;
	return pat;
}

StaticParticle* StaticParticle::particlePumpkinTail(cocos2d::Point pos, int typ) 
{
	StaticParticle *pat = StaticParticle::create();
	pat->mSprite = cocos2d::Sprite::createWithSpriteFrameName("hw-ngh-tx.png");
	pat->mSprite->retain();
	pat->mDir = ccpForAngle(CC_DEGREES_TO_RADIANS(360.0f*CCRANDOM_0_1()));
	cocos2d::Point npos = ccpAdd(pos, ccpMult(pat->mDir, 10));
	pat->mSprite->setPosition(npos);
	pat->mSpeed = 50.0f + 50.0f*CCRANDOM_0_1();
	pat->mLife = 0.5f;
	pat->mRotate = 800;
	pat->mFade = true;
	pat->mFlipX = 0;
	pat->mGravityY = 0;
	pat->mGravityX = 0;
	pat->mStick = false;
	pat->mShurink = true;
	return pat;
}

void StaticParticle::onCreate() 
{
	GamePlay *play = GamePlay::sharedGamePlay();
	play->addChild(mSprite, LAYER_ROLE+1);

	mTimer = 0;
}

void StaticParticle::onUpdate(float delta) 
{
	GamePlay *play = GamePlay::sharedGamePlay();
	mTimer += delta;
	if( mTimer > mLife )
	{
		play->manager->removeGameObject(this);
	}
	float k = mTimer/mLife;
	float r = 1.0f - k;
	//move
	float step = mSpeed*delta;
	cocos2d::Point np = ccpAdd(mSprite->getPosition(), ccpMult(mDir, step));
	//gravity
	if( mGravityY != 0 )
	{
		float gs = mTimer*mGravityY;
		float gd = gs*delta;
		np.y += gd;
	}
	if( mGravityX != 0 )
	{
		float gs = mTimer*mGravityX;
		float gd = gs*delta;
		np.x += gd;
	}
	//stick
	if( mStick )
	{
		float dx = play->runspeed*delta;
		np.x -= dx;
	}
	mSprite->setPosition(np);
	//rotate
	if( mRotate != 0 )
	{
		mSprite->setRotation(mSprite->getRotation() + mRotate*delta);
	}
	//fade
	if( mFade )
	{
		mSprite->setOpacity(255*r);
	}
	//shurink
	if( mShurink )
	{
		mSprite->setScale(r);
	}
	//flip
	if( mFlipX != 0 )
	{
		int n = mTimer/mFlipX;
		float a = mTimer-mFlipX*n;
		float b = a - mFlipX/2;
		if( a < mFlipX/2 )
		{
			b = mFlipX/2 - a;
		}
		mSprite->setScaleX(b/(mFlipX/2)*2-1);
	}
}

void StaticParticle::onDestroy() 
{
	GamePlay *play = GamePlay::sharedGamePlay();
	mSprite->release();
	play->removeChild(mSprite, true);
}


