#ifndef ___CLASSES_COMMITARCADE_H_
#define ___CLASSES_COMMITARCADE_H_
//
//  CommitArcade.h
//  NinjiaRush
//
//  Created by 马 颂文 on 12-11-29.
//  Copyright (c) 2012年 TrinGame. All rights reserved.
//

#include "GameTool.h"

class CommitArcade:
  public GameObject,
  public cocosbuilder::CCBMemberVariableAssigner,
  public cocosbuilder::CCBSelectorResolver
{
  public:
    CREATE_FUNC(CommitArcade);
    static CommitArcade* commit();

    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject*, const char*, cocos2d::CCNode*);
    virtual SEL_MenuHandler onResolveCCBCCMenuItemSelector(CCObject *, const char*);
    virtual cocos2d::extension::Control::Handler onResolveCCBCCControlSelector(cocos2d::Ref *, const char*);

    virtual void onCreate();
    virtual void onUpdate(float delta);
    virtual void onDestroy();

    int mState;
    float mTimer;
    float mAdder;
    float mSoundTimer;
    int mTargetBouns;
    int mTargetScore;
    int mCurrentScore;
    int mCurrentBouns;

    CCNode *mNode;
    // --- auto assign ---
    CCSprite *mRole;
    CCSprite *mLight;
    CCSprite *mCup;
    CCLabelBMFont *mScore;
    CCLabelBMFont *mBouns;
    CCLabelBMFont *mGoldenTrophy;
    CCLabelBMFont *mSilverTrophy;
    CCLabelBMFont *mBronzeTrophy;
    CCLabelBMFont *mCoins;
    CCLayerColor *mFlash;


};
#endif
