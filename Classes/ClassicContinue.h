#ifndef ___CLASSES_CLASSICCONTINUE_H_
#define ___CLASSES_CLASSICCONTINUE_H_
//
//  ClassicContinue.h
//  NinjiaRush
//
//  Created by 马 颂文 on 12-12-4.
//  Copyright (c) 2012年 TrinGame. All rights reserved.
//

#include "GameTool.h"
//#include "IAPHelper.h"

class ControlLayer;

class ClassicContinue:
  public GameObject
{
  public:
    static ClassicContinue* classicContinue();
    CREATE_FUNC(ClassicContinue);

    virtual void onCreate();
    virtual void onUpdate(float delta);
    virtual void onDestroy();

    ControlLayer *mLayer;
    int mState;//0-counting 1-pause
    float mTimer;
    int mCounter;

    void onButton(int bid);
    void onCancel();
    void onUseCredit();
    void onBuyCredit1();
    void onBuyCredit2();
    void onBuyCredit3();

    void purchaseVerified(int pid);
    void purchaseCancelled(int pid);
    void purchaseFailed(int pid);

};
#endif
