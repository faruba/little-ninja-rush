#ifndef ___CLASSES_SPEEDLINE_H_
#define ___CLASSES_SPEEDLINE_H_
#include "GameTool.h"

class SpeedLine:public GameObject
{
  public:
    CREATE_FUNC(SpeedLine);
    virtual bool init(){return true;};

    virtual void onCreate();
    virtual void onUpdate(float delta);
    virtual void onDestroy();

    static SpeedLine* line();

    CCSprite *mSprite;
    float     mSpeed;


};
#endif
