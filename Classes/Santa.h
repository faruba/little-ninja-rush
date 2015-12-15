#ifndef ___CLASSES_SANTA_H_
#define ___CLASSES_SANTA_H_
#include "GameTool.h"
#include "Role.h"

class Santa:
  public Role
{
  public:
    static Santa* role(CCNode* parent);

    CREATE_FUNC(Santa);

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
