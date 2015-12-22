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
cocos2d::Node *mMerchantbox;

    int mIC1;
    int mIC2;
    int mIC3;

    //auto assign
cocos2d::MenuItemImage *mBuyItem1;
cocos2d::MenuItemImage *mBuyItem2;
cocos2d::MenuItemImage *mBuyItem3;
cocos2d::Sprite *mBody;
cocos2d::Sprite *mHead;
cocos2d::Label *mItemCount1;
cocos2d::Label *mItemCount2;
cocos2d::Label *mItemCount3;
cocos2d::Label *mCoins;
cocos2d::Sprite *mTalkbox;
cocos2d::Sprite *mTalkText;
cocos2d::Sprite *mItem1;
cocos2d::Sprite *mItem2;
cocos2d::Sprite *mItem3;
cocos2d::Sprite *mItem4;


    void onMerchantClose();
    void onMerchantBuy1();
    void onMerchantBuy2();
    void onMerchantBuy3();
    void onMerchantBuy4();

};
#endif
