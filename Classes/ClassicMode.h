#ifndef ___CLASSES_CLASSICMODE_H_
#define ___CLASSES_CLASSICMODE_H_
#include "Respawn.h"
#include "GameObject.h"
#include "GameTool.h"

class ClassicMode:
  public GameObject
{
  public:
    CREATE_FUNC(ClassicMode);

    virtual bool init() {return true;};
    virtual void onCreate();
    virtual void onUpdate(float);
    virtual void onDestroy();


    Respawn *mRespawn;
    float mSpeedlineTimer;
    bool mUsedMagicCredit;

    static ClassicMode* mode();

    void goOver();

};
#endif
