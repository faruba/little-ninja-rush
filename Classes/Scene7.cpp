//
//  Scene7.m
//  NinjiaRush
//
//  Created by 马 颂文 on 12-11-22.
//  Copyright (c) 2012年 TrinGame. All rights reserved.
//

#include "Scene7.h"
#include "GameConstants.h"
#include "UniversalFit.h"
#include "GamePlay.h"

//--------
Snow* Snow::snow(cocos2d::CCNode* parent) 
{
    Snow *ret = Snow::create();
    ret->mSprite = cocos2d::CCSprite::createWithSpriteFrameName("scene-snow.png");
    ret->mSprite->setOpacity(200);
    ret->mSprite->setPosition(cocos2d::ccp((SCREEN_WIDTH+300)*CCRANDOM_0_1(), SCREEN_HEIGHT+50 ));
    ret->mParent = parent;
    ret->mWindX = CCRANDOM_0_1();
    ret->mSpeed = CCRANDOM_0_1()*50 - 80;
    ret->mSpeedY = 40 + 30*CCRANDOM_0_1();
    parent->addChild(ret->mSprite);
    return ret;
}
bool Snow::update(float delta, float wind) 
{
    cocos2d::Point np = mSprite->getPosition();
    np.y -= delta*mSpeedY;
    mSpeed += delta*wind;
    np.x += delta*mSpeed;
    mSprite->setPosition(np);
    if( np.y < -10 || np.x < -10 )
    {
        mParent->removeChild(mSprite, false);
        return true;
    }
    return false;
}

//--------------



void Scene7::loadScene() 
{
cocos2d::CCSpriteFrameCache *cache = cocos2d::CCSpriteFrameCache::sharedSpriteFrameCache();
    cache->addSpriteFramesWithFile("scene13.plist");
    
    //创建foot print batch
    GamePlay *play = GamePlay::sharedGamePlay();
    if( play->footprints != NULL )
    {
        play->removeChild(play->footprints);
    }
    play->footprints = cocos2d::CCSpriteBatchNode::create("scene13.png");
    play->addChild(play->footprints, LAYER_ROLE-1);
}

void Scene7::unloadScene() 
{
cocos2d::CCSpriteFrameCache *cache = cocos2d::CCSpriteFrameCache::sharedSpriteFrameCache();
    cache->removeSpriteFramesFromFile("scene13.plist");
    unloadTextureFromeSpriteFrameFile("scene13.plist");
    
    mSakura->release();
    mSakura = NULL;
}

void Scene7::createScene(cocos2d::CCNode * bg, CCNode * fbg, CCNode * fg, RingTile * tiles) 
{
    mTiles = tiles;
    mFBg = fbg;
    mBg = bg;
    mFront = fg;
    
    int btiles[] = {83, 84, 85, 86, 83, 84, 85, 86, 83, 84, 85, 86, 83, 84, 85, 86, 83, 84, 85, 86, 83, 84, 85, 86};
    mTiles->initTiles(btiles, 24);
    mTiles->setPosition(cocos2d::ccp(0, 0));
    
    //sky
cocos2d::CCSprite *bgm = cocos2d::CCSprite::createWithSpriteFrameName("mc-bg.png");
    bgm->setAnchorPoint(cocos2d::ccp(0, 1));
    bgm->setPosition(cocos2d::ccp(0, SCREEN_HEIGHT));
    bgm->setScaleX(UniversalFit::sharedUniversalFit()->screenSize.width/10);
    mBg->addChild(bgm);
    
    //white trees
    mWhiteTrees = cocos2d::CCNode::create();
    mBg->addChild(mWhiteTrees);
    mWhiteLen = 0;
    for(int i=0; i<6; ++i)
    {
        mWhiteLen += 10 + 60*CCRANDOM_0_1();
        int tr = 1 + randomInt(4);
cocos2d::CCSprite *sp = cocos2d::CCSprite::createWithSpriteFrameName(cocos2d::CCString::createWithFormat("mc-tree2-%d.png", tr)->getCString());
        sp->setAnchorPoint(cocos2d::ccp(0, 0));
        sp->setPosition(cocos2d::ccp(mWhiteLen, 55));
        mWhiteTrees->addChild(sp);
        mWhiteLen += sp->getContentSize().width;
    }
    
    //snow slope
    for( int i=0; i<3; ++i)
    {
        mSnowSlopes[i] = cocos2d::CCSprite::createWithSpriteFrameName("mc-hj2.png");
        mSnowSlopes[i]->setAnchorPoint(cocos2d::ccp(0, 0));
        mSnowSlopes[i]->setPosition(cocos2d::ccp(mSnowSlopes[i]->getContentSize().width*i, 37));
        mBg->addChild(mSnowSlopes[i]);
    }
    
    //white trees
    mWhiteTrees2 = cocos2d::CCNode::create();
    mBg->addChild(mWhiteTrees2);
    mWhiteLen2 = 0;
    for(int i=0; i<6; ++i)
    {
        mWhiteLen2 += 10 + 60*CCRANDOM_0_1();
        int tr = 1 + randomInt(4);
cocos2d::CCSprite *sp = cocos2d::CCSprite::createWithSpriteFrameName(cocos2d::CCString::createWithFormat("mc-tree2-%d.png", tr)->getCString());
        sp->setAnchorPoint(cocos2d::ccp(0, 0));
        sp->setPosition(cocos2d::ccp(mWhiteLen2, 60));
        mWhiteTrees2->addChild(sp);
        mWhiteLen2 += sp->getContentSize().width;
    }
    
    //green trees
    mGreenTrees2 = cocos2d::CCNode::create();
    mBg->addChild(mGreenTrees2);
    mGreenLen2 = 0;
    for(int i=0; i<6; ++i)
    {
        mGreenLen2 += 10 + 60*CCRANDOM_0_1();
        int tr = 1 + randomInt(4);
cocos2d::CCSprite *sp = cocos2d::CCSprite::createWithSpriteFrameName(cocos2d::CCString::createWithFormat("mc-tree-%d.png", tr)->getCString());
        sp->setAnchorPoint(cocos2d::ccp(0, 0));
        sp->setPosition(cocos2d::ccp(mGreenLen2, 41));
        mGreenTrees2->addChild(sp);
        mGreenLen2 += sp->getContentSize().width;
    }
    
    //house
    mHouse = cocos2d::CCSprite::createWithSpriteFrameName("mc-hj-house.png");
    mHouse->setAnchorPoint(cocos2d::ccp(0, 0));
    mHouse->setPosition(cocos2d::ccp(2000*CCRANDOM_0_1(), 47));
    mBg->addChild(mHouse);
    
    //green trees
    mGreenTrees = cocos2d::CCNode::create();
    mBg->addChild(mGreenTrees);
    mGreenLen = 0;
    for(int i=0; i<6; ++i)
    {
        mGreenLen += 10 + 60*CCRANDOM_0_1();
        int tr = 1 + randomInt(4);
cocos2d::CCSprite *sp = cocos2d::CCSprite::createWithSpriteFrameName(cocos2d::CCString::createWithFormat("mc-tree-%d.png", tr)->getCString());
        sp->setAnchorPoint(cocos2d::ccp(0, 0));
        sp->setPosition(cocos2d::ccp(mGreenLen, 51));
        mGreenTrees->addChild(sp);
        mGreenLen += sp->getContentSize().width;
    }
    
    //grass
    for( int i=0; i<8; ++i)
    {
        mGrass[i] = cocos2d::CCSprite::createWithSpriteFrameName("mc-hj1.png");
        mGrass[i]->setAnchorPoint(cocos2d::ccp(0, 0));
        mGrass[i]->setPosition(cocos2d::ccp(mSnowSlopes[i]->getContentSize().width*i, 51));
        mBg->addChild(mGrass[i]);
    }
    
    //decorator
    mDecos = cocos2d::CCNode::create();
    mFBg->addChild(mDecos);
    for(int i=0; i<5; ++i)
    {
        mDecosLen += 10 + 40*CCRANDOM_0_1();
        if( randomInt(3) == 0 )
        {
            int sc = 1+randomInt(2);
cocos2d::CCSprite *sp = cocos2d::CCSprite::createWithSpriteFrameName(cocos2d::CCString::createWithFormat("mc-stocking%d.png", sc)->getCString());
            sp->setAnchorPoint(cocos2d::ccp(0, 1));
            sp->setPosition(cocos2d::ccp(mDecosLen, 198));
            
            mDecos->addChild(sp);
            mDecosLen += sp->getContentSize().width;
        }
        else
        {
            int sc = 1+randomInt(6);
cocos2d::CCSprite *sp = cocos2d::CCSprite::createWithSpriteFrameName(cocos2d::CCString::createWithFormat("mc-top%d.png", sc)->getCString());
            sp->setAnchorPoint(cocos2d::ccp(0, 0));
            sp->setPosition(cocos2d::ccp(mDecosLen, 248));
            
            mDecos->addChild(sp);
            mDecosLen += sp->getContentSize().width;
        }
    }
    
    //front
    mFronts = cocos2d::CCNode::create();
    mFront->addChild(mFronts);
    for(int i=0; i<5; ++i)
    {
        mFrontLen += 10 + 30*CCRANDOM_0_1();
        int tr = 1 + randomInt(3);
cocos2d::CCSprite *sp = cocos2d::CCSprite::createWithSpriteFrameName(cocos2d::CCString::createWithFormat("mc-qj%d.png", tr)->getCString());
        sp->setAnchorPoint(cocos2d::ccp(0, 0));
        sp->setPosition(cocos2d::ccp(mFrontLen, 0));
        mFronts->addChild(sp);
        mFrontLen += sp->getContentSize().width;
    }
    
    //snow
    mSakuraNode = cocos2d::CCNode::create();
    mSakuraNode->setPosition(cocos2d::ccp(0, 0));
    mFront->addChild(mSakuraNode);
    mSakura = cocos2d::CCArray::create();
    mWind = 0;
}

void Scene7::update(float delta) 
{/*TODO:
    if( mTiles->freedCount() >= 4 )
    {
        int block[] = {83, 84, 85, 86};
        mTiles->pushTiles(block, 4);
    }
    
    //update white trees
    {
        float ds = delta*0.2f;
        mWhiteLen -= ds;
cocos2d::Ref* node = NULL;
CCARRAY_FOREACH(mWhiteTrees->getChildren(), node)
        {
cocos2d::CCSprite *sp = (cocos2d::CCSprite *)node;
          cocos2d::Point np = sp->getPosition();
          np.x -= ds;

          if( np.x + sp->getContentSize().width < 0 )
          {
            np.x = mWhiteLen;
            if( np.x < 568 )
            {
              mWhiteLen = 568;
              np.x = 568;
            }
            mWhiteLen += sp->getContentSize().width + 10 + 60*CCRANDOM_0_1();
          }

          sp->setPosition(np);
        }
    }
    
    //roll snow slope
    {
        int woff = mTiles->offset()*0.3f;
        int dr = mSnowSlopes[0]->getContentSize().width;
        int doff = woff%dr;
        for( int i=0; i<3; ++i)
        {
            cocos2d::Point np = mSnowSlopes[i]->getPosition();
            np.x = -doff+dr*i;
            mSnowSlopes[i]->setPosition(np);
        }
    }
    
    //update white trees
    {
        float ds = delta*0.4f;
        mWhiteLen2 -= ds;
cocos2d::Ref* node = NULL;
CCARRAY_FOREACH( mWhiteTrees2->getChildren(), node)
        {
cocos2d::CCSprite *sp = (cocos2d::CCSprite*)node;
          cocos2d::Point np = sp->getPosition();
          np.x -= ds;

          if( np.x + sp->getContentSize().width < 0 )
          {
            np.x = mWhiteLen2;
            if( np.x < 568 )
            {
              mWhiteLen2 = 568;
              np.x = 568;
            }
            mWhiteLen2 += sp->getContentSize().width + 10 + 60*CCRANDOM_0_1();
          }

          sp->setPosition(np);
        }
    }
    
    //update green trees
    {
        float ds = delta*0.5f;
        mGreenLen2 -= ds;
cocos2d::Ref* node = NULL;
CCARRAY_FOREACH(mGreenTrees2->getChildren(), node)
        {
cocos2d::CCSprite *sp = (cocos2d::CCSprite*)node;
          cocos2d::Point np = sp->getPosition();
          np.x -= ds;

          if( np.x + sp->getContentSize().width < 0 )
          {
            np.x = mGreenLen2;
            if( np.x < 568 )
            {
              mGreenLen2 = 568;
              np.x = 568;
            }
            mGreenLen2 += sp->getContentSize().width + 10 + 60*CCRANDOM_0_1();
          }

          sp->setPosition(np);
        }
    }
    
    //update house
    {
        cocos2d::Point np = mHouse->getPosition();
        np.x -= delta*0.6f;
        if( np.x + mHouse->getContentSize().width < 0 )
        {
            np.x += 600 + 1000*CCRANDOM_0_1();
        }
        mHouse->setPosition(np);
    }
    
    //update green trees
    {
        float ds = delta*0.7f;
        mGreenLen -= ds;
cocos2d::Ref* node = NULL;
CCARRAY_FOREACH(mGreenTrees->getChildren(), node)
        {
cocos2d::CCSprite *sp = (cocos2d::CCSprite*)node;
          cocos2d::Point np = sp->getPosition();
          np.x -= ds;

          if( np.x + sp->getContentSize().width < 0 )
          {
            np.x = mGreenLen;
            if( np.x < 568 )
            {
              mGreenLen = 568;
              np.x = 568;
            }
            mGreenLen += sp->getContentSize().width + 10 + 60*CCRANDOM_0_1();
          }

          sp->setPosition(np);
        }
    }
    
    //roll grass
    {
        int woff = mTiles->offset()*0.9f;
        int dr = mGrass[0]->getContentSize().width;
        int doff = woff%dr;
        for( int i=0; i<8; ++i)
        {
            cocos2d::Point np = mGrass[i]->getPosition();
            np.x = -doff+dr*i;
            mGrass[i]->setPosition(np);
        }
    }
    
    //update decos
    {
        float ds = delta;
        mDecosLen -= ds;
cocos2d::Ref* node = NULL;
CCARRAY_FOREACH(mDecos->getChildren(), node)
        {
cocos2d::CCSprite *sp = (cocos2d::CCSprite*)node;
          cocos2d::Point np = sp->getPosition();
          np.x -= ds;

          if( np.x + sp->getContentSize().width < 0 )
          {
            if( randomInt(4) == 0 )
            {
              int sc = 1+randomInt(2);
cocos2d::CCSprite *nsp = cocos2d::CCSprite::createWithSpriteFrameName(cocos2d::CCString::createWithFormat("mc-stocking%d.png", sc)->getCString());
              sp->setDisplayFrame(nsp->displayFrame());
              sp->setAnchorPoint(cocos2d::ccp(0, 1));
              np.y = 198;
            }
            else
            {
              int sc = 1+randomInt(6);
cocos2d::CCSprite *nsp = cocos2d::CCSprite::createWithSpriteFrameName(cocos2d::CCString::createWithFormat("mc-top%d.png", sc)->getCString());
              sp->setDisplayFrame(nsp->displayFrame());
              sp->setAnchorPoint(cocos2d::ccp(0, 0));
              np.y = 248;
            }


            np.x = mDecosLen;
            if( np.x < 1136 )
            {
              mDecosLen = 1136;
              np.x = 1136;
            }
            mDecosLen += sp->getContentSize().width + 40 + 200*CCRANDOM_0_1();
          }

          sp->setPosition(np);
        }
    }
    
    //update fronts
    {
        float ds = delta*1.6f;
        mFrontLen -= ds;
cocos2d::Ref* node = NULL;
CCARRAY_FOREACH(mFronts->getChildren(), node)
        {
cocos2d::CCSprite *sp = (cocos2d::CCSprite*)node;
          cocos2d::Point np = sp->getPosition();
          np.x -= ds;

          if( np.x + sp->getContentSize().width < 0 )
          {
            int tr = 1 + randomInt(3);
cocos2d::CCSprite *nsp = cocos2d::CCSprite::createWithSpriteFrameName(cocos2d::CCString::createWithFormat("mc-qj%d.png", tr)->getCString());
            sp->setDisplayFrame(nsp->displayFrame());

            np.x = mFrontLen;
            if( np.x < 568 )
            {
              mFrontLen = 568;
              np.x = 568;
            }
            mFrontLen += sp->getContentSize().width + 150 + 400*CCRANDOM_0_1();
          }

          sp->setPosition(np);
        }
    }
    
    //snow
    {
        float deltatime = GamePlay::sharedGamePlay()->deltaTime;
        if( CCRANDOM_0_1() > 0.9f )
        {
            mSakura->addObject(Snow::snow(mSakuraNode));
        }
        mWindx += deltatime*3;
        mWind = 50.0f*(sinf(mWindx) - 0.5f);
        for( int i=0; i<mSakura->count(); ++i)
        {
            Snow *s = (Snow *)mSakura->objectAtIndex(i);
            if( s->update(deltatime, mWind) )
            {
                mSakura->removeObject(s);
                i--;
            }
        }
    }
  */
}


