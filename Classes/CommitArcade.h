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

		virtual bool onAssignCCBMemberVariable(cocos2d::Ref*, const char*, cocos2d::Node*);
		virtual SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::Ref *, const char*);
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
		cocos2d::Node *mNode;
		// --- auto assign ---
		cocos2d::Sprite *mRole;
		cocos2d::Sprite *mLight;
		cocos2d::Sprite *mCup;
		cocos2d::Label *mScore;
		cocos2d::Label *mBouns;
		cocos2d::Label *mGoldenTrophy;
		cocos2d::Label *mSilverTrophy;
		cocos2d::Label *mBronzeTrophy;
		cocos2d::Label *mCoins;
		cocos2d::LayerColor *mFlash;


};
#endif
