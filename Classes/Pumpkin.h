#ifndef ___CLASSES_PUMPKIN_H_
#define ___CLASSES_PUMPKIN_H_

#include "GameTool.h"
#include "Role.h"

class Pumpkin:
  public Role
{
  public:
    static Pumpkin* role(CCNode* parent);

    CREATE_FUNC(Pumpkin);

    virtual void onCreate();
    virtual void onUpdate(float delta);
    virtual void onDestroy();

    virtual bool collisionWithCircle(CCPoint cc, float rad);
    virtual bool deliverHit(int type, CCPoint dir);
    virtual CCPoint position();
    virtual void setPosition(CCPoint pos);
    virtual CCPoint center();
    virtual bool supportAimAid();
    virtual void toggleVisible(bool flag);


    CCNode *mParent;
    GTAnimatedSprite *mSprite;
    int mState;//0-floating 1-escape 2-dying
    float mTimer;
    int mHitCount;
    float mHurtTimer;

    CCPoint mPumpkinSpeed;
    CCPoint mTargetPos;
    CCPoint mTargetSpeed;

    float mFlySound;

    int mFacing;
    int mCoinsCounter;


};
#endif
