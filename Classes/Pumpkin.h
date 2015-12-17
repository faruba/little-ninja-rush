#ifndef ___CLASSES_PUMPKIN_H_
#define ___CLASSES_PUMPKIN_H_

#include "GameTool.h"
#include "Role.h"

class Pumpkin:
  public Role
{
  public:
    static Pumpkin* role(cocos2d::Node* parent);

    CREATE_FUNC(Pumpkin);

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
    int mState;//0-floating 1-escape 2-dying
    float mTimer;
    int mHitCount;
    float mHurtTimer;

    cocos2d::Point mPumpkinSpeed;
    cocos2d::Point mTargetPos;
    cocos2d::Point mTargetSpeed;

    float mFlySound;

    int mFacing;
    int mCoinsCounter;


};
#endif
