#ifndef ___CLASSES_RESPAWN_H_
#define ___CLASSES_RESPAWN_H_
//*** Classic ***
#define MAXPIPLECOUNT (9)
#define MERCHANT_WAIT (7)
#include "GameTool.h"

class Respawn:
  public Ref
{
  public:
    virtual bool init(){return true;};

    CREATE_FUNC(Respawn);

    int   mReady;
    float mPT[MAXPIPLECOUNT];
    int   mLevel;
    int   mPiple;

    float mMessagerPos[2];
    float mMerchant;
    bool  mFlagMerchant;
    float mMerchantTimer;

    float mFestivalEnemy;
    int mFestivalRole;

    bool mHalloween;
    bool mChristmas;
    int  mMode;


    void setPipleLine(int pc);

    void setMode(int mod);

    void setLevel(int lv);

    void updateClassic(float delta);

    void updateArcade(float delta);

    bool judge(int tid);

    void gen(int tid);

};
#endif
