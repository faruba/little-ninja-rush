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
    virtual void createScene(cocos2d::CCNode * bg, CCNode * fbg, CCNode * fg, RingTile * tiles); 
    virtual void update(float delta);
    virtual void release(){};

    RingTile *mTiles;
cocos2d::CCNode *mFront;
cocos2d::CCNode *mFBg;
cocos2d::CCNode *mBg;
cocos2d::CCNode *mWhiteTrees;
cocos2d::CCSprite *mSnowSlopes[3];
cocos2d::CCNode *mWhiteTrees2;
cocos2d::CCNode *mGreenTrees2;
cocos2d::CCSprite *mHouse;
cocos2d::CCNode *mGreenTrees;
cocos2d::CCSprite *mGrass[8];
cocos2d::CCNode *mDecos;
cocos2d::CCNode *mFronts;
    
    float mWhiteLen;
    float mWhiteLen2;
    float mGreenLen2;
    float mGreenLen;
    float mDecosLen;
    float mFrontLen;
    
    //snow
cocos2d::CCNode *mSakuraNode;
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

    static Snow* snow(cocos2d::CCNode* parent);
cocos2d::CCNode *mParent;
cocos2d::CCSprite *mSprite;
    float mWindX;
    float mSpeed;
    float mSpeedY;
};
#endif
