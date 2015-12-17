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
    virtual void createScene(cocos2d::Node * bg, Node * fbg, Node * fg, RingTile * tiles); 
    virtual void update(float delta);
    virtual void release(){};

    RingTile *mTiles;
cocos2d::Node *mFront;
cocos2d::Node *mBg;
    int mTileState;
    int mTileCount;
cocos2d::Sprite *mFronts[S1_FONTCOUNT];
};
#endif
