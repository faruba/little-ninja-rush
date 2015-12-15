#ifndef ___CLASSES_MIDDLENINJA_H_
#define ___CLASSES_MIDDLENINJA_H_
#include "GameTool.h"
#include "Role.h"

class MiddleNinja:
  public Role
{
  public:
    CREATE_FUNC(MiddleNinja);
    static MiddleNinja* role(CCNode* parent);

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
    int    mDartCount;
    CCPoint  mTargetPos;
    bool   mFlag;
    float  mSpeed;

    float mStepSnow;


};
#endif
