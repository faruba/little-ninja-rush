//
//  Scene4.m
//  NinjiaRush
//
//  Created by 马 颂文 on 12-7-20.
//  Copyright (c) 2012年 TrinGame. All rights reserved.
//

#include "Scene4.h"
#include "GameConstants.h"
#include "GTLoadList.h"
#include "UniversalFit.h"



void Scene4::loadScene() 
{
    CCSpriteFrameCache *cache = cocos2d::CCSpriteFrameCache::sharedSpriteFrameCache();
    cache->addSpriteFramesWithFile("scene7.plist");
    cache->addSpriteFramesWithFile("scene8.plist");
}

void Scene4::unloadScene() 
{
    CCSpriteFrameCache *cache = cocos2d::CCSpriteFrameCache::sharedSpriteFrameCache();
    cache->removeSpriteFramesFromFile("scene7.plist");
    cache->removeSpriteFramesFromFile("scene8.plist");
    unloadTextureFromeSpriteFrameFile("scene7.plist");
    unloadTextureFromeSpriteFrameFile("scene8.plist");
}

void Scene4::createScene(cocos2d::CCNode * bg, CCNode * fbg, CCNode * fg, RingTile * tiles) 
{
    mTiles = tiles;
    mBg = bg;
    mFront = fg;
    
    CCLayerColor *sbg = cocos2d::CCLayerColor::create(ccc4(201, 239, 251, 255));
    mBg->addChild(sbg);
    
    CCSprite *sky = cocos2d::CCSprite::createWithSpriteFrameName("cj4_bg.png");
    sky->setAnchorPoint(cocos2d::ccp(0, 1));
    sky->setPosition(cocos2d::ccp(0, SCREEN_HEIGHT));
    mBg->addChild(sky);
    
    int btiles[] = {47, 48, 49, 50, 47, 48, 49, 50, 47, 48, 49, 50, 47, 48, 49, 50, 47, 48, 49, 50, 47, 48, 49, 50};
    mTiles->initTiles(btiles, 24);
    mTiles->setPosition(cocos2d::ccp(0, 0));
    
    //增加前景
    for( int i=0; i<6; ++i)
    {
        mFronts[i] = cocos2d::CCSprite::createWithSpriteFrameName("cj4_qj.png");
        mFronts[i]->setAnchorPoint(cocos2d::ccp(0, 0));
        mFronts[i]->setPosition(cocos2d::ccp(mFronts[i]->getContentSize().width*i, 0));
        mFront->addChild(mFronts[i]);
    }
    
    //增加云彩
    for( int i=0; i<2; ++i)
    {
        mClouds[i] = cocos2d::CCSprite::createWithSpriteFrameName("cj4_cloud.png");
        mClouds[i]->setPosition(cocos2d::ccp( 300+200*i, 280+20*i));
        mBg->addChild(mClouds[i]);
    }
    
    //增加背景
    for( int i=0; i<6; ++i)
    {
        mBgs[i] = cocos2d::CCSprite::createWithSpriteFrameName("cj4_hj.png");
        mBgs[i]->setAnchorPoint(cocos2d::ccp(0, 1));
        mBgs[i]->setPosition(cocos2d::ccp(mBgs[i]->getContentSize().width*i, SCREEN_HEIGHT - 20));
        mBg->addChild(mBgs[i]);
    }
    
    mTileState = 0;
    mTileCount = 0;
}

void Scene4::update(float delta) 
{
    //滚动前景
    {
        int woff = mTiles->offset()*1.6f;
        int dr = mFronts[0]->getContentSize().width;
        int doff = woff%dr;
        for( int i=0; i<6; ++i)
        {
            mFronts[i]->setPosition(cocos2d::ccp(-doff+dr*i, 0));
        }
    }
    //滚动背景
    {
        int woff = mTiles->offset()*0.7f;
        int dr = mBgs[0]->getContentSize().width;
        int doff = woff%dr;
        for( int i=0; i<6; ++i)
        {
            mBgs[i]->setPosition(cocos2d::ccp(-doff+dr*i, SCREEN_HEIGHT - 20));
        }
    }
    
    //滚动云彩
    for(int i=0; i<2; ++i)
    {
        float offset = delta/(20+i);
        cocos2d::Point np = mClouds[i]->getPosition();
        np.x -= offset;
        if( np.x < -100 )
        {
            np.x = UniversalFit::sharedUniversalFit()->playSize.width + 100;
        }
        mClouds[i]->setPosition(np);
    }
    
    if( mTiles->freedCount() >= 4 )
    {
        if( mTileState == 0 )
        {
            if( randomInt(20) < mTileCount )
            {
                int block[] = {51, 52, 53, 54};
                mTiles->pushTiles(block, 4);
                mTileState = 1;
                mTileCount = 0;
            }
            else {
                int block[] = {47, 48, 49, 50};
                mTiles->pushTiles(block, 4);
                mTileCount++;
            }
        }
        else {
            if( randomInt(20) < mTileCount )
            {
                int block[] = {59, 60, 61, 62};
                mTiles->pushTiles(block, 4);
                mTileState = 0;
                mTileCount = 0;
            }
            else {
                int block[] = {55, 56, 57, 58};
                mTiles->pushTiles(block, 4);
                mTileCount++;
            }

        }
    }
}


