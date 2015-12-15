#ifndef ___CLASSES_FOOTPRINT_H_
#define ___CLASSES_FOOTPRINT_H_
//
//  FootPrint.h
//  NinjiaRush
//
//  Created by 马 颂文 on 12-11-23.
//  Copyright (c) 2012年 TrinGame. All rights reserved.
//

#include "GameObject.h"

class FootPrint:public GameObject
{
  public:
    static FootPrint* footprint();
    CREATE_FUNC(FootPrint);
    virtual bool init(){return true;};

    virtual void onCreate();
    virtual void onUpdate(float delta);
    virtual void onDestroy();

    //a helper function to go foot print
    static void goFootPrint (float *pv, CCPoint pos);

};
#endif
