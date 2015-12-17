#ifndef ___CLASSES_SANTA_H_
#define ___CLASSES_SANTA_H_
#include "GameTool.h"
#include "Role.h"

class Santa:
  public Role
{
  public:
    static Santa* role(cocos2d::Node* parent);

    CREATE_FUNC(Santa);

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
    float  mSpeed;

    float mTargetPos;
    float mActionTimer;

    float mCoinTimer;

    float mStepSnow;
    bool mHited;
    bool mFlag;


};
#endif
