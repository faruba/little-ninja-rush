#ifndef ___CLASSES_TUTORIALPUPPET_H_
#define ___CLASSES_TUTORIALPUPPET_H_
#include "GameTool.h"
#include "Role.h"

class TutorialPuppet:
  public Role
{
  public:
    static TutorialPuppet* puppet();

    CREATE_FUNC(TutorialPuppet);

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
    //AI PARAM
    int mState;//0-idle 1-move to 2-fire 3-waitkilled 4-dead
    CCPoint mTarget;
    GameObject* mCallback;
    SEL_CallFunc mSelector;
    float mTimer;
    bool mFlag;

    GameObject* mKillCallback;
    SEL_CallFunc mKillSelector;
    bool mKillable;

    GameObject* mCleanCallback;
    SEL_CallFunc mCleanSelector;

    //AI CONTROL
    void reset();
    void setPos(CCPoint pos);
    void moveTo(CCPoint pos, GameObject* tar, SEL_CallFunc sel);
    void fireAt(CCPoint pos, GameObject* tar, SEL_CallFunc sel);
    void waitKilled(GameObject* tar, SEL_CallFunc sel);
    void waitCleanup(GameObject* tar, SEL_CallFunc sel);
};
#endif
