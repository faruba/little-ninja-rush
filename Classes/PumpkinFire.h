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
    static PumpkinFire* dart(CCPoint pos, CCPoint dir, CCNode* parent);

    virtual bool isEnemy();
    virtual CCPoint position();
    virtual void onHitback(CCPoint origin);

    virtual void onCreate();
    virtual void onUpdate(float delta);
    virtual void onDestroy();

    CCNode* mParent;
    CCSprite* mSprite;
    bool mIsEnemy;

    bool mRemoved;

    int type;
    CCPoint direction;
    CCPoint pos;
    float speed;


};
#endif
