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
    virtual void createScene(cocos2d::Node * bg, Node * fbg, Node * fg, RingTile * tiles); 
    virtual void update(float delta);
    virtual void release(){};

    RingTile *mTiles;
cocos2d::Node *mFront;
cocos2d::Node *mBg;
    int mTileCount;
    int mBlock;
cocos2d::Sprite *mFronts[8];
cocos2d::Sprite *mClouds[2]; 
cocos2d::Sprite *mFarHouse[9];
cocos2d::Sprite *mHills[3];

};
#endif
