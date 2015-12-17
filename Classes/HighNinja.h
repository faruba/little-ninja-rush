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
    static HighNinja* role(cocos2d::Node* parent);

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
cocos2d::Node *mParent;
    GTAnimatedSprite *mSprite;
    int    mState;
    float  mTimer;
    int    mDartCount;
    float  mTargetPos;
    bool   mFlag;
    float  mSpeed;
    int   mSpell;
    cocos2d::Point mPoint;

    float mStepSnow;

    GOHandler mWoodLink;
};
#endif
