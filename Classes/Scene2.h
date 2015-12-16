#ifndef ___CLASSES_SCENE2_H_
#define ___CLASSES_SCENE2_H_

#include "GameTool.h"
#include "Scene.h"

class Scene2: public SceneInterface
{
  public:
    virtual bool init(){return true;};
    CREATE_FUNC(Scene2);
    virtual void loadScene();
    virtual void unloadScene();
    virtual void createScene(cocos2d::CCNode * bg, CCNode * fbg, CCNode * fg, RingTile * tiles); 
    virtual void update(float delta);
    virtual void release(){};

    RingTile *mTiles;
cocos2d::CCNode *mFront;
cocos2d::CCNode *mBg;
    int mTileCount;
cocos2d::Sprite *mLight;
cocos2d::Sprite *mDynBg[12];
cocos2d::Sprite *mUpbg[4];

};
#endif
