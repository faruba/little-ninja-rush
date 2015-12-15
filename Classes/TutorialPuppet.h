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

    virtual bool collisionWithCircle(cocos2d::Point cc, float rad);
    virtual bool deliverHit(int type, cocos2d::Point dir);
    virtual cocos2d::Point position();
    virtual void setPosition(cocos2d::Point pos);
    virtual cocos2d::Point center();
    virtual bool supportAimAid();
    virtual void toggleVisible(bool flag);

    GTAnimatedSprite *mSprite;
    //AI PARAM
    int mState;//0-idle 1-move to 2-fire 3-waitkilled 4-dead
    cocos2d::Point mTarget;
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
    void setPos(cocos2d::Point pos);
    void moveTo(cocos2d::Point pos, GameObject* tar, SEL_CallFunc sel);
    void fireAt(cocos2d::Point pos, GameObject* tar, SEL_CallFunc sel);
    void waitKilled(GameObject* tar, SEL_CallFunc sel);
    void waitCleanup(GameObject* tar, SEL_CallFunc sel);
};
#endif
