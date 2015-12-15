#ifndef ___CLASSES_SCENE3_H_
#define ___CLASSES_SCENE3_H_
#include "GameTool.h"
#include "Scene.h"

class Scene3: public SceneInterface
{
  public:
    virtual bool init(){return true;};
    CREATE_FUNC(Scene3);
    virtual void loadScene();
    virtual void unloadScene();
    virtual void createScene(cocos2d::CCNode * bg, CCNode * fbg, CCNode * fg, RingTile * tiles); 
    virtual void update(float delta);
    virtual void release(){};

    RingTile *mTiles;
    CCNode *mFront;
    CCNode *mBg;
    int mTileCount;
    int mBlock;
    
    CCSprite *mFronts[8];
    CCSprite *mClouds[2]; 
    CCSprite *mFarHouse[9];
    CCSprite *mHills[3];

};
#endif
