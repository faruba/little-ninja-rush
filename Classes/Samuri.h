#ifndef ___CLASSES_SAMURI_H_
#define ___CLASSES_SAMURI_H_
#include "GameTool.h"
#include "Role.h"

class Samuri:
  public Role
{
  public:
    static Samuri* role(CCNode* parent);

    CREATE_FUNC(Samuri);

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
    GTAnimatedSprite* mSprite;
    GTAnimatedSprite *mHint;
    //CCSprite* mMark;
    int     mState;
    float   mTimer;
    bool    mFlag;
};
#endif
