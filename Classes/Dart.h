#ifndef ___CLASSES_DART_H_
#define ___CLASSES_DART_H_
#include "GameTool.h"
#include "Darts.h"

class Dart:
  public Darts
{
  public:
    CREATE_FUNC(Dart);
    static Dart* dart(cocos2d::CCString* shap, cocos2d::Point pos, cocos2d::Point dir, int typ, CCNode* parent);

    virtual bool isEnemy();
    virtual cocos2d::Point position();
    virtual void onHitback(cocos2d::Point origin);
    virtual void onCreate();
    virtual void onUpdate(float delta);
    virtual void onDestroy();

    CCNode* mParent;
    CCSprite* mSprite;
    CCSprite* mTail;
    GTAnimatedSprite *mSTail;
    CCString *mShap;
    bool mPaused;
    bool mIsEnemy;
    int mHitEffect;
    int mHitEffect2;

    int mParticle;
    float mParticleInterval;

    bool mRemoved;
    float mTimer;

    GOHandler mTrace;

    int type;
    cocos2d::Point direction;
    cocos2d::Point pos;
    float speed;
    bool blocked;


    void traceRole(GOHandler target);//跟踪角色
    void addTail();
    void addSTail(cocos2d::CCString* ani, int aid);
};
#endif
