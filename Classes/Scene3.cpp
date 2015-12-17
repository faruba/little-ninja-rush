#include "Scene3.h"
#include "GameConstants.h"
#include "GamePlay.h"
#include "GTLoadList.h"
#include "UniversalFit.h"



void Scene3::loadScene() 
{
cocos2d::SpriteFrameCache *cache = cocos2d::SpriteFrameCache::sharedSpriteFrameCache();
    cache->addSpriteFramesWithFile("scene5.plist");
    cache->addSpriteFramesWithFile("scene6.plist");
}

void Scene3::unloadScene() 
{
cocos2d::SpriteFrameCache *cache = cocos2d::SpriteFrameCache::sharedSpriteFrameCache();
    cache->removeSpriteFramesFromFile("scene5.plist");
    cache->removeSpriteFramesFromFile("scene6.plist");
    unloadTextureFromeSpriteFrameFile("scene5.plist");
    unloadTextureFromeSpriteFrameFile("scene6.plist");
}

void Scene3::createScene(cocos2d::Node * bg, Node * fbg, Node * fg, RingTile * tiles) 
{
    mTiles = tiles;
    mBg = bg;
    mFront = fg;
    mTileCount = 0;
    mBlock = -1;
    
    int btiles[] = {27, 28, 27, 28, 29, 30, 33, 34, 35, 36, 27, 28, 27, 28, 27, 28, 29, 30, 33, 34, 35, 36, 27, 28};
    mTiles->initTiles(btiles, 24);
    mTileCount = 0;
    mTiles->setPosition(cocos2d::Vec2(0, 0));
    
    //增加背景
cocos2d::Sprite *sbg = cocos2d::Sprite::createWithSpriteFrameName("s3_bg.png");
    sbg->setAnchorPoint(cocos2d::Vec2(0, 1));
    sbg->setPosition(cocos2d::Vec2(0, SCREEN_HEIGHT));
    bg->addChild(sbg);
    //extra bg
cocos2d::Sprite *exsbg = cocos2d::Sprite::createWithSpriteFrameName("cj3_bg2.png");
    exsbg->setAnchorPoint(cocos2d::Vec2(0, 1));
    exsbg->setPosition(cocos2d::Vec2(sbg->getContentSize().width, SCREEN_HEIGHT));
    //calc scale
    exsbg->setScaleX((UniversalFit::sharedUniversalFit()->screenSize.width - SCREEN_WIDTH)/10);
    bg->addChild(exsbg);
    
    mFarHouse[0] = cocos2d::Sprite::createWithSpriteFrameName("cj3_hj2a.png");
    mFarHouse[0]->setAnchorPoint(cocos2d::Vec2(0, 0));
    mFarHouse[0]->setPosition(cocos2d::Vec2(0, 152));
    mBg->addChild(mFarHouse[0]);
    mFarHouse[1] = cocos2d::Sprite::createWithSpriteFrameName("cj3_hj2b.png");
    mFarHouse[1]->setAnchorPoint(cocos2d::Vec2(0, 0));
    mFarHouse[1]->setPosition(cocos2d::Vec2(mFarHouse[0]->getContentSize().width, 152));
    mBg->addChild(mFarHouse[1]);
    mFarHouse[2] = cocos2d::Sprite::createWithSpriteFrameName("cj3_hj2a.png");
    mFarHouse[2]->setAnchorPoint(cocos2d::Vec2(0, 0));
    mFarHouse[2]->setPosition(cocos2d::Vec2(mFarHouse[0]->getContentSize().width*2, 152));
    mBg->addChild(mFarHouse[2]);
    mFarHouse[3] = cocos2d::Sprite::createWithSpriteFrameName("cj3_hj2b.png");
    mFarHouse[3]->setAnchorPoint(cocos2d::Vec2(0, 0));
    mFarHouse[3]->setPosition(cocos2d::Vec2(mFarHouse[0]->getContentSize().width*3, 152));
    mBg->addChild(mFarHouse[3]);
    mFarHouse[4] = cocos2d::Sprite::createWithSpriteFrameName("cj3_hj2a.png");
    mFarHouse[4]->setAnchorPoint(cocos2d::Vec2(0, 0));
    mFarHouse[4]->setPosition(cocos2d::Vec2(mFarHouse[0]->getContentSize().width*4, 152));
    mBg->addChild(mFarHouse[4]);
    mFarHouse[5] = cocos2d::Sprite::createWithSpriteFrameName("cj3_hj2b.png");
    mFarHouse[5]->setAnchorPoint(cocos2d::Vec2(0, 0));
    mFarHouse[5]->setPosition(cocos2d::Vec2(mFarHouse[0]->getContentSize().width*5, 152));
    mBg->addChild(mFarHouse[5]);
    
    mFarHouse[6] = cocos2d::Sprite::createWithSpriteFrameName("cj3_hj1.png");
    mFarHouse[6]->setAnchorPoint(cocos2d::Vec2(0, 0));
    mFarHouse[6]->setPosition(cocos2d::Vec2(0, 152));
    mBg->addChild(mFarHouse[6]);
    mFarHouse[7] = cocos2d::Sprite::createWithSpriteFrameName("cj3_hj1.png");
    mFarHouse[7]->setAnchorPoint(cocos2d::Vec2(0, 0));
    mFarHouse[7]->setPosition(cocos2d::Vec2(mFarHouse[6]->getContentSize().width, 152));
    mBg->addChild(mFarHouse[7]);
    mFarHouse[8] = cocos2d::Sprite::createWithSpriteFrameName("cj3_hj1.png");
    mFarHouse[8]->setAnchorPoint(cocos2d::Vec2(0, 0));
    mFarHouse[8]->setPosition(cocos2d::Vec2(mFarHouse[6]->getContentSize().width*2, 152));
    mBg->addChild(mFarHouse[8]);
    
    //增加远山
    for(int i=0; i<3; ++i)
    {
cocos2d::CCString *sp = cocos2d::CCString::createWithFormat("cj3_mountain%d.png", i+1);
        mHills[i] = cocos2d::Sprite::createWithSpriteFrameName(sp->getCString());
        mHills[i]->setAnchorPoint(cocos2d::Vec2(0, 0));
        mHills[i]->setPosition(cocos2d::Vec2( 100 + 225*i, 262));
        mBg->addChild(mHills[i]);
    }
    
    //增加前景
    for( int i=0; i<8; ++i)
    {
        mFronts[i] = cocos2d::Sprite::createWithSpriteFrameName("s3_front.png");
        mFronts[i]->setAnchorPoint(cocos2d::Vec2(0, 0));
        mFronts[i]->setPosition(cocos2d::Vec2(mFronts[i]->getContentSize().width*i, 0));
        mFront->addChild(mFronts[i]);
    }
    
    //增加云彩
    for( int i=0; i<2; ++i)
    {
        mClouds[i] = cocos2d::Sprite::createWithSpriteFrameName("s3_cloud.png");
        mClouds[i]->setPosition(cocos2d::Vec2( 300+200*i, 280+20*i));
        mBg->addChild(mClouds[i]);
    }
}

void Scene3::update(float delta) 
{
    //滚动前景
    int woff = mTiles->offset()*1.2f;
    int dr = mFronts[0]->getContentSize().width;
    int doff = woff%dr;
    for( int i=0; i<8; ++i)
    {
        mFronts[i]->setPosition(cocos2d::Vec2(-doff+dr*i, 0));
    }
    //滚动远处的房屋
    for( int i=0; i<9; ++i)
    {
        int rate = 3;
        int dr = mFarHouse[0]->getContentSize().width;
        if( i>5 )
        {
            rate = 2;
            dr = mFarHouse[6]->getContentSize().width;
        }
        int woff = mTiles->offset()/rate;
        int doff = woff%dr;
        int k = i%3;
        cocos2d::Point np = mFarHouse[i]->getPosition();
        np.x = -doff+k*dr;
        mFarHouse[i]->setPosition(np);
    }
    //滚动远山
    for(int i=0; i<3; ++i)
    {
        float offset = delta/(30+3*i);
        cocos2d::Point np = mHills[i]->getPosition();
        np.x -= offset;
        if( np.x < -200 )
        {
            np.x = UniversalFit::sharedUniversalFit()->playSize.width+200;
        }
        mHills[i]->setPosition(np);
    }
    //滚动云彩
    for(int i=0; i<2; ++i)
    {
        float offset = delta/(20+i);
        cocos2d::Point np = mClouds[i]->getPosition();
        np.x -= offset;
        if( np.x < -100 )
        {
            np.x = UniversalFit::sharedUniversalFit()->playSize.width+100;
        }
        mClouds[i]->setPosition(np);
    }
    //刷块
    if( mBlock >= 0 )
    {
        switch (mBlock) {
            case 0://窗/墙
            {
                if( mTiles->freedCount() >= 2 )
                {
                    if( randomInt(2)==0 )
                    {
                        if( randomInt(2)==0 )
                        {
                            int bits[] = {31, 32};
                            mTiles->pushTiles(bits, 2);
                            mBlock = -1;
                        }
                        else{
                            int bits[] = {33, 34};
                            mTiles->pushTiles(bits, 2);
                            mBlock = -1;
                        }
                    }
                    else{
                        if( randomInt(3)==0 )
                        {
                            int bits[] = {29, 30};
                            mTiles->pushTiles(bits, 2);
                            mBlock = -1;
                        }
                        else {
                            int bits[] = {27, 28};
                            mTiles->pushTiles(bits, 2);
                            mBlock = -1;
                        }
                    }
                }
            }
                break;
            case 1://门
            {
                if( mTiles->freedCount() >= 6 )
                {
                    int n = randomInt(4);
                    if( n==3 && randomInt(5)==0 )
                    {
                        int bits[] = {27, 28, 41, 46, 27, 28};
                        mTiles->pushTiles(bits, 6);
                    }
                    else {
                        int bits[] = {27, 28, 35+n*2, 36+n*2, 27, 28};
                        mTiles->pushTiles(bits, 6);
                    }
                    mBlock = -1;
                }
            }
                break;
            case 2://院
            {
                if( mTiles->freedCount() >= 9 )
                {
                    int bits[] = {33, 34, 43, 44, 44, 44, 45, 33, 34};
                    mTiles->pushTiles(bits, 9);
                    mBlock = -1;
                }
            }
                break;
        }
    }
    else {
        int n = randomInt(7);
        if( n < mTileCount )
        {
            if ( randomInt(4)==0 ) {
                mBlock = 2;
            }
            else {
                mBlock = 1;
            }
            mTileCount = 0;
        }
        else {
            mBlock = 0;
            mTileCount++;
        }
    }
}


