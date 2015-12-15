#ifndef ___CLASSES_SCENE4_H_
#define ___CLASSES_SCENE4_H_
#include "GameTool.h"

#include "Scene.h"

class Scene4: public SceneInterface {
  public:
    virtual bool init(){return true;};
    CREATE_FUNC(Scene4);
    virtual void loadScene();
    virtual void unloadScene();
    virtual void createScene(cocos2d::CCNode * bg, CCNode * fbg, CCNode * fg, RingTile * tiles); 
    virtual void update(float delta);
    virtual void release(){};

    RingTile *mTiles;
    CCNode *mFront;
    CCNode *mBg;
    int mTileCount;
    int mTileState;
    
    CCSprite *mFronts[6];
    CCSprite *mBgs[6];
    CCSprite *mClouds[2];
};
#endif
