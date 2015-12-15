#ifndef ___CLASSES_ARCHER_H_
#define ___CLASSES_ARCHER_H_
#include "GameTool.h"
#include "Darts.h"

class Archer:
  public Darts
{
  public:
    static Archer* role(cocos2d::CCNode* parent);
    CREATE_FUNC(Archer);

    virtual void onCreate();
    virtual void onUpdate(float delta);
    virtual void onDestroy();
    virtual bool isEnemy();
    virtual cocos2d::Point position();
    virtual void onHitback(cocos2d::Point origin);
    virtual void setPosition(cocos2d::Point pos);
    void toggleVisible(bool flag);
cocos2d::CCNode *mParent;
    cocos2d::Point mTarget;
    GTAnimatedSprite *mArrow;
    int mState;
    float mTimer;
    float mLockTimer;
    cocos2d::Point mFrom;
    cocos2d::Point mMidPoint;
    bool mAvaiable;


};
#endif
