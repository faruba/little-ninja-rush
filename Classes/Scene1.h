#ifndef ___CLASSES_SCENE1_H_
#define ___CLASSES_SCENE1_H_
#include "GameTool.h"
#include "Scene.h"

#define S1_FONTCOUNT (2)

class Scene1: public SceneInterface
{
  public:
    virtual bool init(){return true;};
    CREATE_FUNC(Scene1);
    virtual void loadScene();
    virtual void unloadScene();
    virtual void createScene(cocos2d::CCNode * bg, CCNode * fbg, CCNode * fg, RingTile * tiles); 
    virtual void update(float delta);
    virtual void release(){};

    RingTile *mTiles;
cocos2d::CCNode *mFront;
cocos2d::CCNode *mBg;
    int mTileState;
    int mTileCount;
cocos2d::CCSprite *mFronts[S1_FONTCOUNT];
};
#endif
