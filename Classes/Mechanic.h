#ifndef ___CLASSES_MECHANIC_H_
#define ___CLASSES_MECHANIC_H_
#include "GameTool.h"
#include "Role.h"

class Mechanic:
  public Role
{
  public:
    CREATE_FUNC(Mechanic);

    virtual void onCreate();
    virtual void onUpdate(float delta);
    virtual void onDestroy();

    virtual const char* animationSetName() { return "mechanic"; }
    virtual bool deliverHit(int type, cocos2d::Point dir);
    virtual cocos2d::Point center();
    virtual bool supportAimAid();

    GTAnimatedSprite *mSprite;
    float  mTimer;
    float  mSpeed;
    bool   mFlag;
    int    mCount;
};
#endif
