#include "Scene1.h"
#include "GameConstants.h"
#include "GameTool.h"
#include "GTLoadList.h"
#include "UniversalFit.h"


void Scene1::loadScene() 
{
    CCSpriteFrameCache *cache = CCSpriteFrameCache::sharedSpriteFrameCache();
    cache->addSpriteFramesWithFile("scene1.plist");
    cache->addSpriteFramesWithFile("scene2.plist");
}

void Scene1::unloadScene() 
{
    CCSpriteFrameCache *cache = CCSpriteFrameCache::sharedSpriteFrameCache();
    cache->removeSpriteFramesFromFile("scene1.plist");
    cache->removeSpriteFramesFromFile("scene2.plist");
    unloadTextureFromeSpriteFrameFile("scene1.plist");
    unloadTextureFromeSpriteFrameFile("scene2.plist");
}

void Scene1::createScene(CCNode * bg, CCNode * fbg, CCNode * fg, RingTile * tiles) 
{
    mTiles = tiles;
    
    mBg = bg;
    mFront = fg;
    
    CCLayerColor *sbg = CCLayerColor::create(ccc4(16, 23, 39, 255));
    mBg->addChild(sbg, 0);
    
    CCSprite *StaBg = CCSprite::createWithSpriteFrameName("s1_sbg.png");
    StaBg->setAnchorPoint(ccp(0, 1));
    StaBg->setPosition(ccp(0, SCREEN_HEIGHT));
    mBg->addChild(StaBg, 0);
    
    CCSprite *DynBg0 = CCSprite::createWithSpriteFrameName("s1_dbg.png");
    DynBg0->setAnchorPoint(ccp(0, 0));
    DynBg0->setPosition(ccp(0, 80));
    mBg->addChild(DynBg0, 1, 0);
    
    CCSprite *DynBg1 = CCSprite::createWithSpriteFrameName("s1_dbg.png");
    DynBg1->setAnchorPoint(ccp(0, 0));
    DynBg1->setPosition(ccp(DynBg0->getContentSize().width, 80));
    mBg->addChild(DynBg1, 1, 1);
    
    CCSprite *DynBg2 = CCSprite::createWithSpriteFrameName("s1_dbg.png");
    DynBg2->setAnchorPoint(ccp(0, 0));
    DynBg2->setPosition(ccp(DynBg0->getContentSize().width*2, 80));
    mBg->addChild(DynBg2, 1, 2);
    //----------
    int btiles[] = {2, 3, 2, 3, 2, 3, 2, 3, 2, 3, 2, 3, 2, 3, 2, 3, 2, 3, 2, 3, 2, 3, 2, 3};
    mTiles->initTiles(btiles, 24);
    mTileState = 0;
    mTileCount = 8;
    mTiles->setPosition(ccp(0, 0));
    
    //增加前景
    for( int i=0; i<S1_FONTCOUNT; ++i)
    {
        mFronts[i] = CCSprite::createWithSpriteFrameName("cj1_qj.png");
        mFronts[i]->setAnchorPoint(ccp(0, 0));
        mFronts[i]->setPosition(ccp(mFronts[i]->getContentSize().width*i*2, -10));
        mFront->addChild(mFronts[i]);
    }
}

void Scene1::update(float delta) 
{
    //run scene
    {
        CCNode *DynBg0 = (mBg->getChildByTag(0));
        CCNode *DynBg1 = (mBg->getChildByTag(1));
        CCNode *DynBg2 = (mBg->getChildByTag(2));
        int woff = mTiles->offset()/2;
        int dr = DynBg0->getContentSize().width;
        int doff = woff%dr;
        DynBg0->setPosition(ccp(-doff, 80));
        DynBg1->setPosition(ccp(-doff+DynBg0->getContentSize().width, 80));
        DynBg2->setPosition(ccp(-doff+DynBg0->getContentSize().width*2, 80));
    }
    //滚动前景
    {
        for(int i=0; i<2; ++i)
        {
            float offset = delta*2;
            CCPoint np = mFronts[i]->getPosition();
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


