#ifndef ___CLASSES_COMMITSCROLL_H_
#define ___CLASSES_COMMITSCROLL_H_
//
//  CommitScroll.h
//  NinjiaRush
//
//  Created by 马 颂文 on 12-6-19.
//  Copyright (c) 2012年 TrinGame. All rights reserved.
//

#include "GameObject.h"
#include "GameTool.h"

class CommitScroll:
  public GameObject,
  public CCBMemberVariableAssigner,
  public CCBSelectorResolver
{
  public:
    CREATE_FUNC(CommitScroll);
    static CommitScroll* commitScorll();

    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject*, const char*, cocos2d::CCNode*);
    virtual SEL_MenuHandler onResolveCCBCCMenuItemSelector(CCObject *, const char*);
    virtual SEL_CCControlHandler onResolveCCBCCControlSelector(CCObject *, const char*);

    virtual void onCreate();
    virtual void onUpdate(float delta);
    virtual void onDestroy();

    bool mOver;
    float mOverTimer;
    CCNode *mNode;
    GTAnimatedSprite *mEffect;
    int mState;//0-waitting 1-bloom 2-fade
    float mTimer;
    bool mReviveFlag;

    int mPrize;
    //-- auto assign --
    CCSprite *mArrow;
    CCSprite *mScroll;
    CCSprite *mTitle;
    CCLabelBMFont *mScrollCount;
    CCNode *mRotateNode;
    CCSprite *mLightBack;
    CCNode *mRotateACW;
    CCNode *mRotateCW;
    CCLayerColor *mFlash;
    CCLabelTTF *mItemName;


    void onCashIn();

    void onScroll();

    bool isPrizeAvailable(int pid);

    int lottery();
    void implementPrize();
};
#endif
