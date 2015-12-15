#ifndef ___CLASSES_SCENE1_H_
#define ___CLASSES_SCENE1_H_
#include "GameTool.h"

#define S1_FONTCOUNT (2)

class Scene1:
  public CCObject,
  public Scene
{
  public:
    virtual bool init(){return true;};
    CREATE_FUNC(Scene1);
    virtual void loadScene();
    virtual void unloadScene();
    virtual void createScene(CCNode * bg, CCNode * fbg, CCNode * fg, RingTile * tiles); 
    virtual void update(float delta);
    virtual void release(){};

    RingTile *mTiles;
    CCNode *mFront;
    CCNode *mBg;
    int mTileState;
    int mTileCount;

    CCSprite *mFronts[S1_FONTCOUNT];
};
#endif
