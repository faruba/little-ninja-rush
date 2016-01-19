#ifndef ___CLASSES_SAMURI_H_
#define ___CLASSES_SAMURI_H_
#include "GameTool.h"
#include "Role.h"

class Samuri:
  public Role
{
  public:
    CREATE_FUNC(Samuri);

    virtual void onCreate();
    virtual void onUpdate(float delta);
    virtual void onDestroy();
    virtual const char* animationSetName() { return "samurai"; }

    virtual bool deliverHit(int type, cocos2d::Point dir);
    virtual cocos2d::Point center();
    virtual bool supportAimAid();

    GTAnimatedSprite *mHint;
    //Sprite* mMark;
    float   mTimer;
    bool    mFlag;
};
#endif
