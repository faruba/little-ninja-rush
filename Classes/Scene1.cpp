#include "Scene1.h"
#include "GameConstants.h"
#include "GameTool.h"
#include "GTLoadList.h"
#include "UniversalFit.h"


void Scene1::loadScene() 
{
cocos2d::SpriteFrameCache *cache = cocos2d::SpriteFrameCache::sharedSpriteFrameCache();
    cache->addSpriteFramesWithFile("scene1.plist");
    cache->addSpriteFramesWithFile("scene2.plist");
}

void Scene1::unloadScene() 
{
cocos2d::SpriteFrameCache *cache = cocos2d::SpriteFrameCache::sharedSpriteFrameCache();
    cache->removeSpriteFramesFromFile("scene1.plist");
    cache->removeSpriteFramesFromFile("scene2.plist");
    unloadTextureFromeSpriteFrameFile("scene1.plist");
    unloadTextureFromeSpriteFrameFile("scene2.plist");
}

void Scene1::createScene(cocos2d::CCNode * bg, CCNode * fbg, CCNode * fg, RingTile * tiles) 
{
    mTiles = tiles;
    
    mBg = bg;
    mFront = fg;
cocos2d::LayerColor *sbg = cocos2d::LayerColor::create(Color4B(16, 23, 39, 255));
    mBg->addChild(sbg, 0);
cocos2d::Sprite *StaBg = cocos2d::Sprite::createWithSpriteFrameName("s1_sbg.png");
    StaBg->setAnchorPoint(cocos2d::ccp(0, 1));
    StaBg->setPosition(cocos2d::ccp(0, SCREEN_HEIGHT));
    mBg->addChild(StaBg, 0);
cocos2d::Sprite *DynBg0 = cocos2d::Sprite::createWithSpriteFrameName("s1_dbg.png");
    DynBg0->setAnchorPoint(cocos2d::ccp(0, 0));
    DynBg0->setPosition(cocos2d::ccp(0, 80));
    mBg->addChild(DynBg0, 1, 0);
cocos2d::Sprite *DynBg1 = cocos2d::Sprite::createWithSpriteFrameName("s1_dbg.png");
    DynBg1->setAnchorPoint(cocos2d::ccp(0, 0));
    DynBg1->setPosition(cocos2d::ccp(DynBg0->getContentSize().width, 80));
    mBg->addChild(DynBg1, 1, 1);
cocos2d::Sprite *DynBg2 = cocos2d::Sprite::createWithSpriteFrameName("s1_dbg.png");
    DynBg2->setAnchorPoint(cocos2d::ccp(0, 0));
    DynBg2->setPosition(cocos2d::ccp(DynBg0->getContentSize().width*2, 80));
    mBg->addChild(DynBg2, 1, 2);
    //----------
    int btiles[] = {2, 3, 2, 3, 2, 3, 2, 3, 2, 3, 2, 3, 2, 3, 2, 3, 2, 3, 2, 3, 2, 3, 2, 3};
    mTiles->initTiles(btiles, 24);
    mTileState = 0;
    mTileCount = 8;
    mTiles->setPosition(cocos2d::ccp(0, 0));
    
    //增加前景
    for( int i=0; i<S1_FONTCOUNT; ++i)
    {
        mFronts[i] = cocos2d::Sprite::createWithSpriteFrameName("cj1_qj.png");
        mFronts[i]->setAnchorPoint(cocos2d::ccp(0, 0));
        mFronts[i]->setPosition(cocos2d::ccp(mFronts[i]->getContentSize().width*i*2, -10));
        mFront->addChild(mFronts[i]);
    }
}

void Scene1::update(float delta) 
{
    //run scene
    {
cocos2d::CCNode *DynBg0 = (mBg->getChildByTag(0));
cocos2d::CCNode *DynBg1 = (mBg->getChildByTag(1));
cocos2d::CCNode *DynBg2 = (mBg->getChildByTag(2));
        int woff = mTiles->offset()/2;
        int dr = DynBg0->getContentSize().width;
        int doff = woff%dr;
        DynBg0->setPosition(cocos2d::ccp(-doff, 80));
        DynBg1->setPosition(cocos2d::ccp(-doff+DynBg0->getContentSize().width, 80));
        DynBg2->setPosition(cocos2d::ccp(-doff+DynBg0->getContentSize().width*2, 80));
    }
    //滚动前景
    {
        for(int i=0; i<2; ++i)
        {
            float offset = delta*2;
            cocos2d::Point np = mFronts[i]->getPosition();
            np.x -= offset;
            if( np.x < -100 )
            {
                np.x = UniversalFit::sharedUniversalFit()->playSize.width + 100 + 500*CCRANDOM_0_1();
            }
            mFronts[i]->setPosition(np);
        }
    }
    
    switch (mTileState) {
        case 0://黑
        {
            if( mTiles->freedCount() >= 2 )
            {
                int chg = randomInt(100);
                int rat = mTileCount*25;
                if(chg < rat)
                {//change
                    int data[] = {4,5};
                    mTiles->pushTiles(data, 2);
                    mTileCount = 0;
                    mTileState = 1;
                }
                else {//keep
                    int data[] = {2,3};
                    mTiles->pushTiles(data, 2);
                    mTileCount++;
                }
            }
        }
            break;
        case 1://楼
        {
            if( mTiles->freedCount() >= 2 )
            {
                int chg = randomInt(100);
                int rat = mTileCount*5;
                if(chg < rat)
                {//change
                    if( chg%2 == 0 )
                    {
                        int data[] = {0,1};
                        mTiles->pushTiles(data, 2);
                        mTileState = 0;
                        
                    }
                    else {
                        int data[] = {10,11};
                        mTiles->pushTiles(data, 2);
                        mTileState = 2;
                    }
                    mTileCount = 0;
                }
                else {//keep
                    if( chg%2 == 0 )
                    {
                        int data[] = {6,7};
                        mTiles->pushTiles(data, 2);
                    }
                    else {
                        int data[] = {8,9};
                        mTiles->pushTiles(data, 2);
                    }
                    mTileCount++;
                }
            }
            
        }
            break;
        case 2://空
        {
            if( mTiles->freedCount() >= 2 )
            {
                int chg = randomInt(100);
                int rat = mTileCount*25;
                if(chg < rat)
                {//change
                    int data[] = {14,15};
                    mTiles->pushTiles(data, 2);
                    mTileCount = 0;
                    mTileState = 1;
                }
                else {//keep
                    int data[] = {12,13};
                    mTiles->pushTiles(data, 2);
                    mTileCount++;
                }
            }
            
        }
            break;
    }
}


