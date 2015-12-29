#ifndef ___CLASSES_MESSAGER_H_
#define ___CLASSES_MESSAGER_H_
#include "GameTool.h"
#include "Role.h"

class Messager:
  public Role
{
public:
    CREATE_FUNC(Messager);

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
