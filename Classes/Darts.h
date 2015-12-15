#ifndef ___CLASSES_DARTS_H_
#define ___CLASSES_DARTS_H_
#include "GameObject.h"
class Darts:
  public GameObject
{
  public:
    virtual bool isEnemy() = 0;
    virtual CCPoint position() = 0;
    virtual void onHitback(CCPoint origin) = 0;
};
#endif
