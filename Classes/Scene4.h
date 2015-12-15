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
cocos2d::CCNode *mFront;
cocos2d::CCNode *mBg;
    int mTileCount;
    int mTileState;
cocos2d::CCSprite *mFronts[6];
cocos2d::CCSprite *mBgs[6];
cocos2d::CCSprite *mClouds[2];
};
#endif
