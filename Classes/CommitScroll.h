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
#include "GameRecord.h"
#include "GamePlay.h"

class CommitScroll:
	public GameObject,
	public cocosbuilder::CCBMemberVariableAssigner,
	public cocosbuilder::CCBSelectorResolver
{
	public:
		CREATE_FUNC(CommitScroll);
		static CommitScroll* commitScorll();

		virtual bool onAssignCCBMemberVariable(cocos2d::Ref*, const char*, cocos2d::Node*);
		virtual SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::Ref *, const char*);
		virtual cocos2d::extension::Control::Handler onResolveCCBCCControlSelector(cocos2d::Ref * , const char* );

		virtual void onCreate();
		virtual void onUpdate(float delta);
		virtual void onDestroy();

		bool mOver;
		float mOverTimer;
		cocos2d::Node *mNode;
		GTAnimatedSprite *mEffect;
		int mState;//0-waitting 1-bloom 2-fade
		float mTimer;
		bool mReviveFlag;

		int mPrize;
		//-- auto assign --
		cocos2d::Sprite *mArrow;
		cocos2d::Sprite *mScroll;
		cocos2d::Sprite *mTitle;
		cocos2d::Label *mScrollCount;
		cocos2d::Node *mRotateNode;
		cocos2d::Sprite *mLightBack;
		cocos2d::Node *mRotateACW;
		cocos2d::Node *mRotateCW;
		cocos2d::LayerColor *mFlash;
		cocos2d::Label *mItemName;


		void onCashIn(cocos2d::Ref*);

		void onScroll(cocos2d::Ref*);

		bool isPrizeAvailable(int pid);

		int lottery();
		void implementPrize();
};

template<typename Type>
void drawLottoryBetween (int start, int end, std::vector<Type>& vector) {
	float sum = 0;
	for (auto &item : vector) {
		sum += GameRecord::sharedGameRecord()->collection->isItemCompleted(item.uiid) ? 0 : (float)item.wght;
	}

	float roller = 0;
	float res = 1.0f*randomInt(100000)/100000.0f;

	for (auto &item : vector) {
		if (GameRecord::sharedGameRecord()->collection->isItemCompleted(item.uiid)) continue;
		roller += ((float)item.wght)/sum;

		if ( res < roller ) {
			int uiid = item.uiid;
			GameRecord::sharedGameRecord()->collection->gainItemPiece(uiid);
			if (GameRecord::sharedGameRecord()->collection->isItemCompleted(uiid)) {
				GamePlay::sharedGamePlay()->pushNotification(item.name, item.icon, 4);
			}
			return;
		}
	}
}
#endif
