#ifndef ___CLASSES_DARTS_H_
#define ___CLASSES_DARTS_H_
#include "GameObject.h"
class Darts:
  public GameObject
{
  public:
    virtual bool isEnemy() = 0;
    virtual cocos2d::Point position() = 0;
    virtual void onHitback(cocos2d::Point origin) = 0;
};
#endif
