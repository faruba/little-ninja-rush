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
    virtual void createScene(cocos2d::Node * bg, Node * fbg, Node * fg, RingTile * tiles); 
    virtual void update(float delta);
    virtual void release(){};

    RingTile *mTiles;
cocos2d::Node *mFront;
cocos2d::Node *mBg;
cocos2d::Sprite *mFronts[4];
cocos2d::Sprite *mMountains[4];
    float mMountainLen;
cocos2d::Sprite *mSmallTrees[4];
    float mSmallDistance;
cocos2d::Sprite *mSmoke1[3];
cocos2d::Sprite *mSmokedObjs[14];
    float mSmokedDistance;
cocos2d::Sprite *mSmoke2[3];
cocos2d::Sprite *mFrontObjs[14];
    float mFrontDistance;
cocos2d::Sprite *mBgs1[5];
};
#endif
