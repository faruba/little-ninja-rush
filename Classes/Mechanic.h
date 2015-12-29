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

    virtual bool collisionWithCircle(cocos2d::Point cc, float rad);
    virtual bool deliverHit(int type, cocos2d::Point dir);
    virtual cocos2d::Point position();
    virtual void setPosition(cocos2d::Point pos);
    virtual cocos2d::Point center();
    virtual bool supportAimAid();
    virtual void toggleVisible(bool flag);

    GTAnimatedSprite *mSprite;
    int    mState;
    float  mTimer;
    float  mSpeed;
    bool   mFlag;
    int    mCount;
};
#endif
