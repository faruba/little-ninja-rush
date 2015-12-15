#ifndef ___CLASSES_MERCHANTPANEL_H_
#define ___CLASSES_MERCHANTPANEL_H_
//
//  MerchantPanel.h
//  NinjiaRush
//
//  Created by 马 颂文 on 12-7-10.
//  Copyright (c) 2012年 TrinGame. All rights reserved.
//

#include "GameTool.h"

class MerchantPanel:
  public GameObject
{
  public:
    static MerchantPanel* merchantPanel();

    CREATE_FUNC(MerchantPanel);

    virtual void onCreate();
    virtual void onUpdate(float delta);
    virtual void onDestroy();

    void updateBuy1();
    void updateBuy2();
    void updateBuy3();

    void gainShurikenPiece();
    void gainKatanaPiece();
    void gainSpecialPiece();
    CCNode *mMerchantbox;

    int mIC1;
    int mIC2;
    int mIC3;

    //auto assign
    CCMenuItemImage *mBuyItem1;
    CCMenuItemImage *mBuyItem2;
    CCMenuItemImage *mBuyItem3;
    CCSprite *mBody;
    CCSprite *mHead;
    CCLabelBMFont *mItemCount1;
    CCLabelBMFont *mItemCount2;
    CCLabelBMFont *mItemCount3;
    CCLabelBMFont *mCoins;
    CCSprite *mTalkbox;
    CCSprite *mTalkText;
    CCSprite *mItem1;
    CCSprite *mItem2;
    CCSprite *mItem3;
    CCSprite *mItem4;


    void onMerchantClose();
    void onMerchantBuy1();
    void onMerchantBuy2();
    void onMerchantBuy3();
    void onMerchantBuy4();

};
#endif
