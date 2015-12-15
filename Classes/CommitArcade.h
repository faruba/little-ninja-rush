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
    virtual SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject *, const char*);
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
cocos2d::CCNode *mNode;
    // --- auto assign ---
cocos2d::CCSprite *mRole;
cocos2d::CCSprite *mLight;
cocos2d::CCSprite *mCup;
cocos2d::CCLabelBMFont *mScore;
cocos2d::CCLabelBMFont *mBouns;
cocos2d::CCLabelBMFont *mGoldenTrophy;
cocos2d::CCLabelBMFont *mSilverTrophy;
cocos2d::CCLabelBMFont *mBronzeTrophy;
cocos2d::CCLabelBMFont *mCoins;
cocos2d::CCLayerColor *mFlash;


};
#endif
