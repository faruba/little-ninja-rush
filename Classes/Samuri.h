#ifndef ___CLASSES_SAMURI_H_
#define ___CLASSES_SAMURI_H_
#include "GameTool.h"
#include "Role.h"

class Samuri:
  public Role
{
  public:
    static Samuri* role(cocos2d::CCNode* parent);

    CREATE_FUNC(Samuri);

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
    
    CCNode* mParent;
    GTAnimatedSprite* mSprite;
    GTAnimatedSprite *mHint;
    //CCSprite* mMark;
    int     mState;
    float   mTimer;
    bool    mFlag;
};
#endif
