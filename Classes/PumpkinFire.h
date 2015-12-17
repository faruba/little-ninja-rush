#ifndef ___CLASSES_PUMPKINFIRE_H_
#define ___CLASSES_PUMPKINFIRE_H_

#include "GameTool.h"
#include "Darts.h"
#include "Role.h"

class PumpkinFire:
  public Darts
{
  public:
    CREATE_FUNC(PumpkinFire);
    static PumpkinFire* dart(cocos2d::Point pos, cocos2d::Point dir, Node* parent);

    virtual bool isEnemy();
    virtual cocos2d::Point position();
    virtual void onHitback(cocos2d::Point origin);

    virtual void onCreate();
    virtual void onUpdate(float delta);
    virtual void onDestroy();
cocos2d::Node* mParent;
cocos2d::Sprite* mSprite;
    bool mIsEnemy;

    bool mRemoved;

    int type;
    cocos2d::Point direction;
    cocos2d::Point pos;
    float speed;


};
#endif
