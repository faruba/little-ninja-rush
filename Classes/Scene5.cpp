//
//  Scene5.m
//  NinjiaRush
//
//  Created by 马 颂文 on 12-7-25.
//  Copyright (c) 2012年 TrinGame. All rights reserved.
//

#include "Scene5.h"
#include "GameConstants.h"
#include "GTLoadList.h"



void Scene5::loadScene() 
{
cocos2d::SpriteFrameCache *cache = cocos2d::SpriteFrameCache::sharedSpriteFrameCache();
    cache->addSpriteFramesWithFile("scene9.plist");
    cache->addSpriteFramesWithFile("scene10.plist");
}

void Scene5::unloadScene() 
{
cocos2d::SpriteFrameCache *cache = cocos2d::SpriteFrameCache::sharedSpriteFrameCache();
    cache->removeSpriteFramesFromFile("scene9.plist");
    cache->removeSpriteFramesFromFile("scene10.plist");
    unloadTextureFromeSpriteFrameFile("scene9.plist");
    unloadTextureFromeSpriteFrameFile("scene10.plist");
}

void Scene5::createScene(cocos2d::Node * bg, Node * fbg, Node * fg, RingTile * tiles) 
{
    mTiles = tiles;
    mBg = bg;
    mFront = fg;
    
    int btiles[] = {63, 64, 65, 66, 67, 68, 69, 70, 63, 64, 65, 66, 63, 64, 65, 66, 67, 68, 69, 70, 63, 64, 65, 66};
    mTiles->initTiles(btiles, 24);
    mTiles->setPosition(cocos2d::Vec2(0, 0));
    
    //增加前景
    for( int i=0; i<4; ++i)
    {
        mFronts[i] = cocos2d::Sprite::createWithSpriteFrameName("cj5_qj.png");
        mFronts[i]->setAnchorPoint(cocos2d::Vec2(0, 0));
        mFronts[i]->setPosition(cocos2d::Vec2(mFronts[i]->getContentSize().width*i, 0));
        mFront->addChild(mFronts[i]);
    }
    
    //增加背景0
    for( int i=0; i<3; ++i)
    {
        mBgs3[i] = cocos2d::Sprite::createWithSpriteFrameName("cj5_hj3.png");
        mBgs3[i]->setAnchorPoint(cocos2d::Vec2(0, 1));
        mBgs3[i]->setPosition(cocos2d::Vec2(mBgs3[i]->getContentSize().width*i, SCREEN_HEIGHT));
        mBg->addChild(mBgs3[i]);
    }
    
    //增加背景1
    for( int i=0; i<2; ++i)
    {
        mBgs1[i] = cocos2d::Sprite::createWithSpriteFrameName("cj5_hj2.png");
        mBgs1[i]->setAnchorPoint(cocos2d::Vec2(0, 1));
        mBgs1[i]->setPosition(cocos2d::Vec2(mBgs1[i]->getContentSize().width*i, SCREEN_HEIGHT));
        mBg->addChild(mBgs1[i]);
    }
    
    //增加背景2
    for( int i=0; i<3; ++i)
    {
        mBgs2[i] = cocos2d::Sprite::createWithSpriteFrameName("cj5_hj1.png");
        mBgs2[i]->setAnchorPoint(cocos2d::Vec2(0, 1));
        mBgs2[i]->setPosition(cocos2d::Vec2(mBgs2[i]->getContentSize().width*i, SCREEN_HEIGHT));
        mBg->addChild(mBgs2[i]);
    }
}

void Scene5::update(float delta) 
{
    //滚动前景
    {
        int woff = mTiles->offset()*1.6f;
        int dr = mFronts[0]->getContentSize().width;
        int doff = woff%dr;
        for( int i=0; i<4; ++i)
        {
            mFronts[i]->setPosition(cocos2d::Vec2(-doff+dr*i, 0));
        }
    }
    //滚动背景0
    {
        int woff = mTiles->offset()*0.3f;
        int dr = mBgs3[0]->getContentSize().width;
        int doff = woff%dr;
        for( int i=0; i<3; ++i)
        {
            mBgs3[i]->setPosition(cocos2d::Vec2(-doff+dr*i, SCREEN_HEIGHT));
        }
    }
    //滚动背景1
    {
        int woff = mTiles->offset()*0.5f;
        int dr = mBgs1[0]->getContentSize().width;
        int doff = woff%dr;
        for( int i=0; i<2; ++i)
        {
            mBgs1[i]->setPosition(cocos2d::Vec2(-doff+dr*i, SCREEN_HEIGHT));
        }
    }
    //滚动背景2
    {
        int woff = mTiles->offset()*0.7f;
        int dr = mBgs2[0]->getContentSize().width;
        int doff = woff%dr;
        for( int i=0; i<3; ++i)
        {
            mBgs2[i]->setPosition(cocos2d::Vec2(-doff+dr*i, SCREEN_HEIGHT));
        }
    }

    if( mTiles->freedCount() >= 4 )
    {
        int val = randomInt(100);
        if( val < 45 )
        {
            int block[] = {63, 64, 65, 66};
            mTiles->pushTiles(block, 4);
        }
        else if( val < 90 )
        {
            int block[] = {67, 68, 69, 70};
            mTiles->pushTiles(block, 4);
        }
        else {
            int block[] = {71, 72, 73, 74};
            mTiles->pushTiles(block, 4);
        }
    }
}


