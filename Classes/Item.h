#ifndef ___CLASSES_ITEM_H_
#define ___CLASSES_ITEM_H_
//
//  Item.h
//  NinjiaRush
//
//  Created by 马 颂文 on 12-5-4.
//  Copyright (c) 2012年 TrinGame. All rights reserved.
//

#include "GameTool.h"

class Item:public GameObject{
  public:
    CREATE_FUNC(Item);
    virtual bool init(){return true;};
    static Item* item(int tid, CCPoint pos, CCNode* parent, bool rock);
    static void triggerItem(int degree, CCPoint pos);

    virtual void onCreate();
    virtual void onUpdate(float delta);
    virtual void onDestroy();

    CCNode* mParent;
    CCSprite* mSprite;
    CCPoint mPos;
    int     mTid;
    float   mTimer;

    float   mSpeed;
    bool    mPaused;
    float   mSpeedX;



    static bool isItemAccept(int item);

};
#endif
