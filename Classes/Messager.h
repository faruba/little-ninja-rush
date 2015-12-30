#ifndef ___CLASSES_MESSAGER_H_
#define ___CLASSES_MESSAGER_H_
#include "GameTool.h"
#include "Role.h"

class Messager:
  public Role
{
public:
    CREATE_FUNC(Messager);

    virtual void onCreate();
    virtual void onUpdate(float delta);
    virtual void onDestroy();
    virtual const char* animationSetName() { return "messager"; }

    virtual bool deliverHit(int type, cocos2d::Point dir);
    virtual cocos2d::Point center();
    virtual bool supportAimAid();
    int mState;
    float mAwake;
    float mFalldown;
    bool mFlagFalldown;
    float mTimer;
    float mSpeed;
    
    float mStepSnow;
    bool mFlag;


};
#endif
