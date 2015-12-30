#ifndef ___CLASSES_MIDDLENINJA_H_
#define ___CLASSES_MIDDLENINJA_H_
#include "GameTool.h"
#include "Role.h"

class MiddleNinja:
  public Role
{
  public:
    CREATE_FUNC(MiddleNinja);

    virtual void onCreate();
    virtual void onUpdate(float delta);
    virtual void onDestroy();

    virtual const char* animationSetName() { return "mninja"; }

    virtual bool deliverHit(int type, cocos2d::Point dir);
    virtual cocos2d::Point center();
    virtual bool supportAimAid();
    int    mState;
    float  mTimer;
    int    mDartCount;
    cocos2d::Point  mTargetPos;
    bool   mFlag;
    float  mSpeed;

    float mStepSnow;


};
#endif
