#ifndef ___CLASSES_SCENE5_H_
#define ___CLASSES_SCENE5_H_
#include "GameTool.h"
#include "Scene.h"

class Scene5: public SceneInterface
{
  public:
    virtual bool init(){return true;};
    CREATE_FUNC(Scene5);
    virtual void loadScene();
    virtual void unloadScene();
    virtual void createScene(cocos2d::CCNode * bg, CCNode * fbg, CCNode * fg, RingTile * tiles); 
    virtual void update(float delta);
    virtual void release(){};

    RingTile *mTiles;
cocos2d::CCNode *mFront;
cocos2d::CCNode *mBg;
cocos2d::CCSprite *mFronts[4];
cocos2d::CCSprite *mBgs1[2];
cocos2d::CCSprite *mBgs2[3];
cocos2d::CCSprite *mBgs3[3];

};
#endif
