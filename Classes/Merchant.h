#ifndef ___CLASSES_MERCHANT_H_
#define ___CLASSES_MERCHANT_H_
#include "GameTool.h"
#include "Role.h"

class Merchant:
  public Role
{
  public:
    static Merchant* role(cocos2d::CCNode* parent);

    CREATE_FUNC(Merchant);

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
cocos2d::CCNode *mParent;
    GTAnimatedSprite *mSprite;
    bool mFlag;

    float mStepTimer;

};
#endif
