#ifndef ___CLASSES_OBJECTIVECOMPLETE_H_
#define ___CLASSES_OBJECTIVECOMPLETE_H_
//
//  ObjectiveComplete.h
//  NinjiaRush
//
//  Created by 马 颂文 on 12-8-2.
//  Copyright (c) 2012年 TrinGame. All rights reserved.
//

#include "GameObject.h"
#include "GameTool.h"
#include "Tasks.h"

class ObjectiveComplete:
  public GameObject,
  public cocosbuilder::CCBMemberVariableAssigner,
  public cocosbuilder::CCBSelectorResolver
{
  public:
    static ObjectiveComplete* commitObjective();
    CREATE_FUNC(ObjectiveComplete);

    virtual bool onAssignCCBMemberVariable(cocos2d::Ref*, const char*, cocos2d::CCNode*);
    virtual SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::Ref *, const char*);
    virtual cocos2d::extension::Control::Handler onResolveCCBCCControlSelector(cocos2d::Ref * , const char* );

    virtual void onCreate();
    virtual void onUpdate(float delta);
    virtual void onDestroy();

    int mCurrIndex;
    int mState;
    float mStateTimer;
    bool mStateFlag;

    float mSoundTimer;

    int mTargetBounsCoins;
    int mTargetBounsScrolls;

    int mCurrBounsCoins;
    int mCurrBounsScrolls;

    Objective *mCurrObj;
    Achievement *mCurrAch;
cocos2d::CCNode *mNode;
    CEClipedNode *mObjRect;
    //--- auto assign ---
cocos2d::CCSprite *mBanner;
cocos2d::CCSprite *mObjType;
cocos2d::CCNode *mCrowns;
cocos2d::CCLabelBMFont *mBounsCoins;
cocos2d::CCSprite *mScroll;
cocos2d::CCLabelBMFont *mBounsScroll;


    void retriveObjectiveInfo(int index, Objective** obj, Achievement** ach);

    //typ = 0 日任务 1 周任务 2 月任务
cocos2d::CCNode* genObjectiveInfo(Achievement* ach, int typ);

};
#endif
