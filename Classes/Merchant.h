#ifndef ___CLASSES_MERCHANT_H_
#define ___CLASSES_MERCHANT_H_
#include "GameTool.h"
#include "Role.h"

class Merchant:
  public Role
{
  public:
    CREATE_FUNC(Merchant);

    virtual void onCreate();
    virtual void onUpdate(float delta);
    virtual void onDestroy();

    virtual const char* animationSetName() { return "merchant"; }
    virtual bool deliverHit(int type, cocos2d::Point dir);
    virtual cocos2d::Point center();
    virtual bool supportAimAid();

    bool mFlag;

    float mStepTimer;

};
#endif
