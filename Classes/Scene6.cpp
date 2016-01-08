//
//  Scene5.m
//  NinjiaRush
//
//  Created by 马 颂文 on 12-7-25.
//  Copyright (c) 2012年 TrinGame. All rights reserved.
//

#include "Scene6.h"
#include "GameConstants.h"
#include "GTLoadList.h"
#include "UniversalFit.h"



void Scene6::loadScene() 
{
cocos2d::SpriteFrameCache *cache = cocos2d::SpriteFrameCache::sharedSpriteFrameCache();
    cache->addSpriteFramesWithFile("scene11.plist");
    cache->addSpriteFramesWithFile("scene12.plist");
}

void Scene6::unloadScene() 
{
cocos2d::SpriteFrameCache *cache = cocos2d::SpriteFrameCache::sharedSpriteFrameCache();
    cache->removeSpriteFramesFromFile("scene11.plist");
    cache->removeSpriteFramesFromFile("scene12.plist");
    unloadTextureFromeSpriteFrameFile("scene11.plist");
    unloadTextureFromeSpriteFrameFile("scene12.plist");
}

void Scene6::createScene(cocos2d::Node * bg, Node * fbg, Node * fg, RingTile * tiles) 
{
    mTiles = tiles;
    mBg = bg;
    mFront = fg;
    
    int btiles[] = {75, 76, 77, 78, 79, 80, 81, 82, 75, 76, 77, 78, 79, 80, 81, 82, 75, 76, 77, 78, 79, 80, 81, 82};
    mTiles->initTiles(btiles, 24);
    mTiles->setPosition(cocos2d::Vec2(0, 0));
    
    //增加前景
    for( int i=0; i<4; ++i)
    {
        mFronts[i] = cocos2d::Sprite::createWithSpriteFrameName("hw-qj.png");
        mFronts[i]->setAnchorPoint(cocos2d::Vec2(0, 0));
        mFronts[i]->setPosition(cocos2d::Vec2(mFronts[i]->getContentSize().width*i, 0));
        mFront->addChild(mFronts[i]);
    }
    //增加静态背景
    {
cocos2d::Sprite *sbg = cocos2d::Sprite::createWithSpriteFrameName("hw-bg.png");
        sbg->setAnchorPoint(cocos2d::Vec2(0, 0));
        sbg->setPosition(cocos2d::Vec2(0, 0));
        sbg->setScaleX(UniversalFit::sharedUniversalFit()->playSize.width/30);
        mBg->addChild(sbg, 0);
cocos2d::Sprite *moon = cocos2d::Sprite::createWithSpriteFrameName("hw-bg-moon.png");
        moon->setPosition(cocos2d::Vec2(324, 301));
        mBg->addChild(moon, 1);
cocos2d::Sprite *star1 = cocos2d::Sprite::createWithSpriteFrameName("hw-bg-star1.png");
        star1->setPosition(cocos2d::Vec2(110, 284));
        mBg->addChild(star1, 1);
cocos2d::Sprite *star2 = cocos2d::Sprite::createWithSpriteFrameName("hw-bg-star2.png");
        star2->setPosition(cocos2d::Vec2(78, 305));
        mBg->addChild(star2, 1);
cocos2d::Sprite *star3 = cocos2d::Sprite::createWithSpriteFrameName("hw-bg-star1.png");
        star3->setPosition(cocos2d::Vec2(221, 278));
        mBg->addChild(star3, 1);
cocos2d::Sprite *star4 = cocos2d::Sprite::createWithSpriteFrameName("hw-bg-star2.png");
        star4->setPosition(cocos2d::Vec2(386, 284));
        mBg->addChild(star4, 1);
    }
    
    //增加山的背景
    {
        mMountains[0] = cocos2d::Sprite::createWithSpriteFrameName("hw-hj5-1.png");
        mMountains[0]->setAnchorPoint(cocos2d::Vec2(0, 0));
        mMountains[0]->setPosition(cocos2d::Vec2(0, 43));
        mBg->addChild(mMountains[0], 2);
        mMountains[1] = cocos2d::Sprite::createWithSpriteFrameName("hw-hj5-2.png");
        mMountains[1]->setAnchorPoint(cocos2d::Vec2(0, 0));
        mMountains[1]->setPosition(cocos2d::Vec2(mMountains[0]->getContentSize().width, 43));
        mBg->addChild(mMountains[1], 2);
        mMountains[2] = cocos2d::Sprite::createWithSpriteFrameName("hw-hj5-1.png");
        mMountains[2]->setAnchorPoint(cocos2d::Vec2(0, 0));
        mMountains[2]->setPosition(cocos2d::Vec2(mMountains[0]->getContentSize().width + mMountains[1]->getContentSize().width, 43));
        mBg->addChild(mMountains[2], 2);
        mMountains[3] = cocos2d::Sprite::createWithSpriteFrameName("hw-hj5-2.png");
        mMountains[3]->setAnchorPoint(cocos2d::Vec2(0, 0));
        mMountains[3]->setPosition(cocos2d::Vec2(mMountains[0]->getContentSize().width + mMountains[1]->getContentSize().width + mMountains[2]->getContentSize().width, 43));
        mBg->addChild(mMountains[3], 2);
        mMountainLen = mMountains[0]->getContentSize().width + mMountains[1]->getContentSize().width + mMountains[2]->getContentSize().width + mMountains[3]->getContentSize().width;
    }
    
    //增加矮树
    {
        mSmallDistance = 0;
        for(int i=0; i<4; ++i)
        {
            mSmallTrees[i] = cocos2d::Sprite::createWithSpriteFrameName(std::string_format("hw-hj4-%d.png", randomInt(6)+1).c_str());
            mSmallTrees[i]->setAnchorPoint(cocos2d::Vec2(0.5f, 0));
            mSmallTrees[i]->setPosition(cocos2d::Vec2(mSmallDistance, 58));
            mSmallDistance += 90 + 110*CCRANDOM_0_1();
            mBg->addChild(mSmallTrees[i], 3);
        }
    }
    
    //SMOKE 1
    {
        for( int i=0; i<3; ++i)
        {
            mSmoke1[i] = cocos2d::Sprite::createWithSpriteFrameName("hw-hj3-1.png");
            mSmoke1[i]->setAnchorPoint(cocos2d::Vec2(0, 0));
            mSmoke1[i]->setPosition(cocos2d::Vec2(mSmoke1[i]->getContentSize().width*i, 60));
            mBg->addChild(mSmoke1[i], 4);
        }
    }
    
    //SMOKED OBJS
    {
        mSmokedDistance = 0;
        int seq[] =   { 1, 11,  2,  3,  4, 12,  5,  6, 13,  7,  8, 14,  9, 10};
        float gap[] = {30,125, 40, 25, 30,139, 22, 40,112, 45, 32,131, 55, 32};
        for(int i=0; i<14; ++i)
        {
            mSmokedObjs[i] = cocos2d::Sprite::createWithSpriteFrameName(std::string_format("hw-smk-%d.png", seq[i]).c_str());
            mSmokedObjs[i]->setAnchorPoint(cocos2d::Vec2(0, 0));
            mSmokedObjs[i]->setPosition(cocos2d::Vec2(mSmokedDistance, 70));
            mSmokedDistance += gap[i];
            mBg->addChild(mSmokedObjs[i], 6);
        }
    }
    
    //SMOKE 2
    {
        for( int i=0; i<3; ++i)
        {
            mSmoke2[i] = cocos2d::Sprite::createWithSpriteFrameName("hw-hj2-1.png");
            mSmoke2[i]->setAnchorPoint(cocos2d::Vec2(0, 0));
            mSmoke2[i]->setPosition(cocos2d::Vec2(mSmoke1[i]->getContentSize().width*i, 53));
            mBg->addChild(mSmoke2[i], 7);
        }
    }
    
    //FRONT OBJS
    {
        mFrontDistance = 0;
        int seq[] = {8, 13, 7, 3, 4, 14, 6, 5, 11, 2, 1, 12, 9, 10};
        float gap[] = {30,135, 40, 25, 30,129, 22, 40,142, 45, 32,121, 55, 32};
        for(int i=0; i<14; ++i)
        {
            mFrontObjs[i] = cocos2d::Sprite::createWithSpriteFrameName(std::string_format("hw-smk-%d.png", seq[i]).c_str());
            mFrontObjs[i]->setAnchorPoint(cocos2d::Vec2(0, 0));
            mFrontObjs[i]->setPosition(cocos2d::Vec2(mFrontDistance, 66));
            mFrontDistance += gap[i];
            mBg->addChild(mFrontObjs[i], 8);
        }
    }
    
    //增加背景0
    for( int i=0; i<5; ++i)
    {
        mBgs1[i] = cocos2d::Sprite::createWithSpriteFrameName("hw-hj.png");
        mBgs1[i]->setAnchorPoint(cocos2d::Vec2(0, 0));
        mBgs1[i]->setPosition(cocos2d::Vec2(mBgs1[i]->getContentSize().width*i, 57));
        mBg->addChild(mBgs1[i], 10);
    }
}

void Scene6::update(float delta) 
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
    //滚动远山
    {
        float woff = delta*0.1f;
        for( int i=0; i<4; ++i)
        {
            cocos2d::Point pos = mMountains[i]->getPosition();
            pos.x -= woff;
            if( pos.x + mMountains[i]->getContentSize().width < 0 )
            {
                pos.x += mMountainLen;
            }
            mMountains[i]->setPosition(pos);
        }
    }
    //刷矮树
    {
        float woff = delta*0.2f;
        mSmallDistance -= woff;
        for(int i=0; i<4; ++i)
        {
            cocos2d::Point np = mSmallTrees[i]->getPosition();
            np.x -= woff;
            if( np.x + mSmallTrees[i]->getContentSize().width < 0 )
            {
                mSmallTrees[i]->setDisplayFrame(GameTool::getSpriteFrameByName(std::string_format("hw-hj4-%d.png", randomInt(6)+1).c_str()));
                np.x = mSmallDistance;
                if( np.x < UniversalFit::sharedUniversalFit()->playSize.width + 50 )
                {
                    np.x = UniversalFit::sharedUniversalFit()->playSize.width + 50;
                }
                mSmallTrees[i]->setPosition(np);
                mSmallDistance += 90 + 110*CCRANDOM_0_1();
            }
            else
            {
                mSmallTrees[i]->setPosition(np);
            }
        }
    }
    //SMOKE1
    {
        int woff = mTiles->offset()*0.3f;
        int dr = mSmoke1[0]->getContentSize().width;
        int doff = woff%dr;
        for( int i=0; i<3; ++i)
        {
            cocos2d::Point np = mSmoke1[i]->getPosition();
            np.x = -doff+dr*i;
            mSmoke1[i]->setPosition(np);
        }
    }
    //SOMOKED OBJS
    {
        float woff = delta*0.4f;
        for(int i=0; i<14; ++i)
        {
            cocos2d::Point np = mSmokedObjs[i]->getPosition();
            np.x -= woff;
            if( np.x + mSmokedObjs[i]->getContentSize().width < 0 )
            {
                np.x += mSmokedDistance;
            }
            mSmokedObjs[i]->setPosition(np);
        }
    }
    //SMOKE2
    {
        int woff = mTiles->offset()*0.5f;
        int dr = mSmoke2[0]->getContentSize().width;
        int doff = woff%dr;
        for( int i=0; i<3; ++i)
        {
            cocos2d::Point np = mSmoke2[i]->getPosition();
            np.x = -doff+dr*i;
            mSmoke2[i]->setPosition(np);
        }
    }
    //FRONT OBJS
    {
        float woff = delta*0.6f;
        for(int i=0; i<14; ++i)
        {
            cocos2d::Point np = mFrontObjs[i]->getPosition();
            np.x -= woff;
            if( np.x + mFrontObjs[i]->getContentSize().width < 0 )
            {
                np.x += mFrontDistance;
            }
            mFrontObjs[i]->setPosition(np);
        }
    }
    //滚动背景1
    {
        int woff = mTiles->offset()*0.8f;
        int dr = mBgs1[0]->getContentSize().width;
        int doff = woff%dr;
        for( int i=0; i<5; ++i)
        {
            cocos2d::Point np = mBgs1[i]->getPosition();
            np.x = -doff+dr*i;
            mBgs1[i]->setPosition(np);
        }
    }

    if( mTiles->freedCount() >= 8 )
    {
        int block[] = {75, 76, 77, 78, 79, 80, 81, 82};
        mTiles->pushTiles(block, 8);
    }
}


