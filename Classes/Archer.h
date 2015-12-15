#ifndef ___CLASSES_ARCHER_H_
#define ___CLASSES_ARCHER_H_
#include "GameTool.h"
#include "Darts.h"

class Archer:
  public Darts
{
  public:
    static Archer* role(CCNode* parent);
    CREATE_FUNC(Archer);

    virtual void onCreate();
    virtual void onUpdate(float delta);
    virtual void onDestroy();
    virtual bool isEnemy();
    virtual CCPoint position();
    virtual void onHitback(CCPoint origin);
    virtual void setPosition(CCPoint pos);
    void toggleVisible(bool flag);

    CCNode *mParent;
    CCPoint mTarget;
    GTAnimatedSprite *mArrow;
    int mState;
    float mTimer;
    float mLockTimer;
    CCPoint mFrom;
    CCPoint mMidPoint;
    bool mAvaiable;


};
#endif
