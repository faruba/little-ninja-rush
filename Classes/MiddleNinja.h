#ifndef ___CLASSES_MIDDLENINJA_H_
#define ___CLASSES_MIDDLENINJA_H_
#include "GameTool.h"
#include "Role.h"

class MiddleNinja:
  public Role
{
  public:
    CREATE_FUNC(MiddleNinja);
    static MiddleNinja* role(cocos2d::CCNode* parent);

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

    CCNode *mParent;
    GTAnimatedSprite *mSprite;
    int    mState;
    float  mTimer;
    int    mDartCount;
    cocos2d::Point  mTargetPos;
    bool   mFlag;
    float  mSpeed;

    float mStepSnow;


};
#endif
