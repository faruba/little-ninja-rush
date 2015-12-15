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
    static HighNinja* role(CCNode* parent);

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
    float  mTargetPos;
    bool   mFlag;
    float  mSpeed;
    int   mSpell;
    CCPoint mPoint;

    float mStepSnow;

    GOHandler mWoodLink;
};
#endif
