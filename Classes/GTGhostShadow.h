#ifndef ___CLASSES_GTGHOSTSHADOW_H_
#define ___CLASSES_GTGHOSTSHADOW_H_
#include "cocos2d.h"
#include "GTAnimatedSprite.h"

USING_NS_CC;

class GTGhostShadow:public CCNode
{
  public:
    virtual bool init();
    CREATE_FUNC(GTGhostShadow);
    static GTGhostShadow* shadow(GTAnimatedSprite* copy, float interval, int number);

    GTAnimatedSprite *mCopy;
    float mInterval;
    int mNumber;

    CCSprite **mShadows;
    int mIndex;
    int mCount;
    float mTimer;
    float mMove;

    bool mToggle;

    GLubyte opacity;


    void syncShadow(float delta);

    void toggleShadow(bool flag);

    void setMoveSpeed(float move);

};
#endif
