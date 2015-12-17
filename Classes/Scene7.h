#ifndef ___CLASSES_SCENE7_H_
#define ___CLASSES_SCENE7_H_
#include "GameTool.h"

#include "Scene.h"

class Scene7: public SceneInterface{
  public:
    virtual bool init(){return true;};
    CREATE_FUNC(Scene7);
    virtual void loadScene();
    virtual void unloadScene();
    virtual void createScene(cocos2d::Node * bg, Node * fbg, Node * fg, RingTile * tiles); 
    virtual void update(float delta);
    virtual void release(){};

    RingTile *mTiles;
cocos2d::Node *mFront;
cocos2d::Node *mFBg;
cocos2d::Node *mBg;
cocos2d::Node *mWhiteTrees;
cocos2d::Sprite *mSnowSlopes[3];
cocos2d::Node *mWhiteTrees2;
cocos2d::Node *mGreenTrees2;
cocos2d::Sprite *mHouse;
cocos2d::Node *mGreenTrees;
cocos2d::Sprite *mGrass[8];
cocos2d::Node *mDecos;
cocos2d::Node *mFronts;
    
    float mWhiteLen;
    float mWhiteLen2;
    float mGreenLen2;
    float mGreenLen;
    float mDecosLen;
    float mFrontLen;
    
    //snow
cocos2d::Node *mSakuraNode;
cocos2d::CCArray *mSakura;
    float mWind;
    float mWindx;
};

class Snow : 
  public Ref
{
  public:
    virtual bool init(){return true;};
    CREATE_FUNC(Snow);
    bool update(float delta, float wind);

    static Snow* snow(cocos2d::Node* parent);
cocos2d::Node *mParent;
cocos2d::Sprite *mSprite;
    float mWindX;
    float mSpeed;
    float mSpeedY;
};
#endif
