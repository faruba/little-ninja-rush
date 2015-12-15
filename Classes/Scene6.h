#ifndef ___CLASSES_SCENE6_H_
#define ___CLASSES_SCENE6_H_
#include "GameTool.h"

class Scene6:
  public CCObject,
  public Scene
{
  public:
    virtual bool init(){return true;};
    CREATE_FUNC(Scene6);
    virtual void loadScene();
    virtual void unloadScene();
    virtual void createScene(CCNode * bg, CCNode * fbg, CCNode * fg, RingTile * tiles); 
    virtual void update(float delta);
    virtual void release(){};

    RingTile *mTiles;
    CCNode *mFront;
    CCNode *mBg;
    
    CCSprite *mFronts[4];
    CCSprite *mMountains[4];
    float mMountainLen;
    CCSprite *mSmallTrees[4];
    float mSmallDistance;
    CCSprite *mSmoke1[3];
    CCSprite *mSmokedObjs[14];
    float mSmokedDistance;
    CCSprite *mSmoke2[3];
    CCSprite *mFrontObjs[14];
    float mFrontDistance;
    CCSprite *mBgs1[5];
};
#endif
