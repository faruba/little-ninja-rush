#ifndef ___CLASSES_HIGHNINJA_H_
#define ___CLASSES_HIGHNINJA_H_
#include "GameTool.h"
#include "Role.h"

class Wood;

class HighNinja:
  public Role
{
  public:
    CREATE_FUNC(HighNinja);

    virtual void onCreate();
    virtual void onUpdate(float delta);
    virtual void onDestroy();

    virtual const char* animationSetName() { return "hninja"; }

    virtual bool deliverHit(int type, cocos2d::Point dir);
    virtual cocos2d::Point center();
    virtual bool supportAimAid();
    int   mSpell;
    cocos2d::Point mPoint;

    float mStepSnow;

    GOHandler mWoodLink;
};
#endif
