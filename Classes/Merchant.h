#ifndef ___CLASSES_MERCHANT_H_
#define ___CLASSES_MERCHANT_H_
#include "GameTool.h"
#include "Role.h"

class Merchant:
  public Role
{
  public:
    static Merchant* role(CCNode* parent);

    CREATE_FUNC(Merchant);

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
    bool mFlag;

    float mStepTimer;

};
#endif
