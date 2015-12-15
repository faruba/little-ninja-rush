#ifndef ___CLASSES_SCENE6_H_
#define ___CLASSES_SCENE6_H_
#include "GameTool.h"

#include "Scene.h"

class Scene6: public SceneInterface
{
  public:
    virtual bool init(){return true;};
    CREATE_FUNC(Scene6);
    virtual void loadScene();
    virtual void unloadScene();
    virtual void createScene(cocos2d::CCNode * bg, CCNode * fbg, CCNode * fg, RingTile * tiles); 
    virtual void update(float delta);
    virtual void release(){};

    RingTile *mTiles;
cocos2d::CCNode *mFront;
cocos2d::CCNode *mBg;
cocos2d::CCSprite *mFronts[4];
cocos2d::CCSprite *mMountains[4];
    float mMountainLen;
cocos2d::CCSprite *mSmallTrees[4];
    float mSmallDistance;
cocos2d::CCSprite *mSmoke1[3];
cocos2d::CCSprite *mSmokedObjs[14];
    float mSmokedDistance;
cocos2d::CCSprite *mSmoke2[3];
cocos2d::CCSprite *mFrontObjs[14];
    float mFrontDistance;
cocos2d::CCSprite *mBgs1[5];
};
#endif
