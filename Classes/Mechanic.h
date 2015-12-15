#ifndef ___CLASSES_MECHANIC_H_
#define ___CLASSES_MECHANIC_H_
#include "GameTool.h"
#include "Role.h"

class Mechanic:
  public Role
{
  public:
    static Mechanic* role();

    CREATE_FUNC(Mechanic);

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

    GTAnimatedSprite *mSprite;
    int    mState;
    float  mTimer;
    float  mSpeed;
    bool   mFlag;
    int    mCount;
};
#endif
