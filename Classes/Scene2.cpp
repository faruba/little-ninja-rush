#include "Scene2.h"
#include "GamePlay.h"
#include "GTLoadList.h"
#include "UniversalFit.h"



void Scene2::loadScene() 
{
    CCSpriteFrameCache *cache = CCSpriteFrameCache::sharedSpriteFrameCache();
    cache->addSpriteFramesWithFile("scene3.plist");
    cache->addSpriteFramesWithFile("scene4.plist");
}

void Scene2::unloadScene() 
{
    CCSpriteFrameCache *cache = CCSpriteFrameCache::sharedSpriteFrameCache();
    cache->removeSpriteFramesFromFile("scene3.plist");
    cache->removeSpriteFramesFromFile("scene4.plist");
    unloadTextureFromeSpriteFrameFile("scene3.plist");
    unloadTextureFromeSpriteFrameFile("scene4.plist");
}

void Scene2::createScene(CCNode * bg, CCNode * fbg, CCNode * fg, RingTile * tiles) 
{
    mTiles = tiles;
    
    mBg = bg;
    mFront = fg;
    
    CCLayerColor *sbg = CCLayerColor::create(ccc4(135, 206, 233, 255));
    mBg->addChild(sbg, 0);
    CCSprite *sun = CCSprite::createWithSpriteFrameName("s2_sun.png");
    sun->setPosition(ccp(433, 300));
    mBg->addChild(sun, 1);
    //1
    mDynBg[0] = CCSprite::createWithSpriteFrameName("s2_bg3.png");
    mDynBg[0]->setAnchorPoint(ccp(0, 0));
    mDynBg[0]->setPosition(ccp(0, 110));
    mBg->addChild(mDynBg[0], 2);
    mDynBg[1] = CCSprite::createWithSpriteFrameName("s2_bg3.png");
    mDynBg[1]->setAnchorPoint(ccp(0, 0));
    mDynBg[1]->setPosition(ccp(mDynBg[0]->getContentSize().width, 110));
    mBg->addChild(mDynBg[1], 2);
    mDynBg[2] = CCSprite::createWithSpriteFrameName("s2_bg3.png");
    mDynBg[2]->setAnchorPoint(ccp(0, 0));
    mDynBg[2]->setPosition(ccp(mDynBg[0]->getContentSize().width*2, 110));
    mBg->addChild(mDynBg[2], 2);
    //2
    mDynBg[3] = CCSprite::createWithSpriteFrameName("s2_bg2.png");
    mDynBg[3]->setAnchorPoint(ccp(0, 0));
    mDynBg[3]->setPosition(ccp(0, 90));
    mBg->addChild(mDynBg[3], 3);
    mDynBg[4] = CCSprite::createWithSpriteFrameName("s2_bg2.png");
    mDynBg[4]->setAnchorPoint(ccp(0, 0));
    mDynBg[4]->setPosition(ccp(mDynBg[3]->getContentSize().width, 90));
    mBg->addChild(mDynBg[4], 3);
    mDynBg[5] = CCSprite::createWithSpriteFrameName("s2_bg2.png");
    mDynBg[5]->setAnchorPoint(ccp(0, 0));
    mDynBg[5]->setPosition(ccp(mDynBg[3]->getContentSize().width*2, 90));
    mBg->addChild(mDynBg[5], 3);
    //3
    mDynBg[6] = CCSprite::createWithSpriteFrameName("s2_bg1.png");
    mDynBg[6]->setAnchorPoint(ccp(0, 0));
    mDynBg[6]->setPosition(ccp(0, 80));
    mBg->addChild(mDynBg[6], 4);
    mDynBg[7] = CCSprite::createWithSpriteFrameName("s2_bg1.png");
    mDynBg[7]->setAnchorPoint(ccp(0, 0));
    mDynBg[7]->setPosition(ccp(mDynBg[6]->getContentSize().width, 80));
    mBg->addChild(mDynBg[7], 4);
    mDynBg[8] = CCSprite::createWithSpriteFrameName("s2_bg1.png");
    mDynBg[8]->setAnchorPoint(ccp(0, 0));
    mDynBg[8]->setPosition(ccp(mDynBg[6]->getContentSize().width*2, 80));
    mBg->addChild(mDynBg[8], 4);
    //4
    mDynBg[9] = CCSprite::createWithSpriteFrameName("s2_front.png");
    mDynBg[9]->setAnchorPoint(ccp(0, 0));
    mDynBg[9]->setPosition(ccp(0, 0));
    mFront->addChild(mDynBg[9], 0);
    mDynBg[10] = CCSprite::createWithSpriteFrameName("s2_front.png");
    mDynBg[10]->setAnchorPoint(ccp(0, 0));
    mDynBg[10]->setPosition(ccp(mDynBg[9]->getContentSize().width, 0));
    mFront->addChild(mDynBg[10], 0);
    mDynBg[11] = CCSprite::createWithSpriteFrameName("s2_front.png");
    mDynBg[11]->setAnchorPoint(ccp(0, 0));
    mDynBg[11]->setPosition(ccp(mDynBg[9]->getContentSize().width*2, 0));
    mFront->addChild(mDynBg[11], 0);
    
    int btiles[] = {16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26};
    mTiles->initTiles(btiles, 22);
    mTileCount = 0;
    mTiles->setPosition(ccp(0, 0));
    
    //增加光照
    mLight = CCSprite::createWithSpriteFrameName("s2_light.png");
    mLight->setAnchorPoint(ccp(0.5f, 1));
    mLight->setRotation(30);
    mLight->setPosition(ccp(3*UniversalFit::sharedUniversalFit()->playSize.width, 215));
    mBg->addChild(mLight, 4);
    
    //增加树上背景
    for( int i=0; i<4; i++)
    {
        mUpbg[i] = CCSprite::createWithSpriteFrameName("s2_ubg.png");
        mUpbg[i]->setAnchorPoint(ccp(0, 0));
        mUpbg[i]->setPosition(ccp(mUpbg[i]->getContentSize().width*i, 242));
        mBg->addChild(mUpbg[i], 4);
    }
}

void Scene2::update(float delta) 
{
  //增加树上背景
  int woff = mTiles->offset()/2;
  int dr = mUpbg[0]->getContentSize().width;
  int doff = woff%dr;
  for( int i=0; i<4; ++i)
  {
    mUpbg[i]->setPosition(ccp(-doff+dr*i, 242));
  }

  woff = mTiles->offset()/4;
  dr = mDynBg[0]->getContentSize().width;
  doff = woff%dr;
  mDynBg[0]->setPosition(ccp(-doff, 110));
  mDynBg[1]->setPosition(ccp(-doff+mDynBg[0]->getContentSize().width, 110));
  mDynBg[2]->setPosition(ccp(-doff+mDynBg[0]->getContentSize().width*2, 110));
  woff = mTiles->offset()/3;
  dr = mDynBg[3]->getContentSize().width;
  doff = woff%dr;
  mDynBg[3]->setPosition(ccp(-doff, 90));
  mDynBg[4]->setPosition(ccp(-doff+mDynBg[3]->getContentSize().width, 90));
  mDynBg[5]->setPosition(ccp(-doff+mDynBg[3]->getContentSize().width*2, 90));
  woff = mTiles->offset()/2;
  dr = mDynBg[4]->getContentSize().width;
  doff = woff%dr;
  mDynBg[6]->setPosition(ccp(-doff, 80));
  mDynBg[7]->setPosition(ccp(-doff+mDynBg[6]->getContentSize().width, 80));
  mDynBg[8]->setPosition(ccp(-doff+mDynBg[6]->getContentSize().width*2, 80));
  woff = mTiles->offset()*2;
  dr = mDynBg[6]->getContentSize().width;
  doff = woff%dr;
  mDynBg[9]->setPosition(ccp(-doff, 0));
  mDynBg[10]->setPosition(ccp(-doff+mDynBg[9]->getContentSize().width, 0));
  mDynBg[11]->setPosition(ccp(-doff+mDynBg[9]->getContentSize().width*2, 0));

  if( mTiles->freedCount() > 0 )
  {
    int d = 16 + mTileCount%11;
    mTileCount++;
    int data[] = {d};
    mTiles->pushTiles(data, 1);
  }
  //光照
  CCPoint np = mLight->getPosition();
  np.x -= delta/3.0f;
  if( np.x < -3*UniversalFit::sharedUniversalFit()->playSize.width )
  {
    np.x = 3*UniversalFit::sharedUniversalFit()->playSize.width;
  }
  mLight->setPosition(np);
  //重新计算光照角度
  CCPoint dis = ccpNormalize(ccpSub(ccp(443, 300), mLight->getPosition()));
  float angle = 90 - CC_RADIANS_TO_DEGREES(ccpToAngle(dis));
  mLight->setRotation(angle);
  angle = (60.0f - fabsf(angle))/60.0f;
  if( angle < 0 )
  {
    angle = 0;
  }
  mLight->setOpacity(angle*255);
}


