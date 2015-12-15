#ifndef ___CLASSES_MESSAGER_H_
#define ___CLASSES_MESSAGER_H_
#include "GameTool.h"
#include "Role.h"

class Messager:
  public Role
{
public:
static Messager* role(CCNode* parent);

    CREATE_FUNC(Messager);

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

    CCNode* mParent;
    GTAnimatedSprite *mSprite;
    int mState;
    float mAwake;
    float mFalldown;
    bool mFlagFalldown;
    float mTimer;
    float mSpeed;
    
    float mStepSnow;
    bool mFlag;


};
#endif
