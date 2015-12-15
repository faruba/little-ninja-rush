#ifndef ___CLASSES_SCENE7_H_
#define ___CLASSES_SCENE7_H_
#include "GameTool.h"

class Scene7:
  public CCObject,
  public Scene
{
  public:
    virtual bool init(){return true;};
    CREATE_FUNC(Scene7);
    virtual void loadScene();
    virtual void unloadScene();
    virtual void createScene(CCNode * bg, CCNode * fbg, CCNode * fg, RingTile * tiles); 
    virtual void update(float delta);
    virtual void release(){};

    RingTile *mTiles;
    CCNode *mFront;
    CCNode *mFBg;
    CCNode *mBg;
    
    CCNode *mWhiteTrees;
    CCSprite *mSnowSlopes[3];
    CCNode *mWhiteTrees2;
    CCNode *mGreenTrees2;
    CCSprite *mHouse;
    CCNode *mGreenTrees;
    CCSprite *mGrass[8];
    CCNode *mDecos;
    CCNode *mFronts;
    
    float mWhiteLen;
    float mWhiteLen2;
    float mGreenLen2;
    float mGreenLen;
    float mDecosLen;
    float mFrontLen;
    
    //snow
    CCNode *mSakuraNode;
    CCArray *mSakura;
    float mWind;
    float mWindx;
};

class Snow : 
  public CCObject
{
  public:
    virtual bool init(){return true;};
    CREATE_FUNC(Snow);
    bool update(float delta, float wind);

    static Snow* snow(CCNode* parent);
    CCNode *mParent;
    CCSprite *mSprite;
    float mWindX;
    float mSpeed;
    float mSpeedY;
};
#endif
