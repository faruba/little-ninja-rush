#ifndef ___CLASSES_SANTA_H_
#define ___CLASSES_SANTA_H_
#include "GameTool.h"
#include "Role.h"

class Santa:
  public Role
{
  public:
    CREATE_FUNC(Santa);

    virtual void onCreate();
    virtual void onUpdate(float delta);
    virtual void onDestroy();

    virtual const char* animationSetName() { return "santa"; }
    virtual bool deliverHit(int type, cocos2d::Point dir);
    virtual cocos2d::Point center();
    virtual bool supportAimAid();

    float  mTimer;
    float  mSpeed;

    float mTargetPos;
    float mActionTimer;

    float mCoinTimer;

    float mStepSnow;
    bool mHited;
    bool mFlag;
};
#endif
