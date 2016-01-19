#ifndef ___CLASSES_GAMEINTERFACE_H_
#define ___CLASSES_GAMEINTERFACE_H_
#include "GameTool.h"
#include "GameConstants.h"
//#include "CCBReader.h"
#include "cocosbuilder/CocosBuilder.h"


#define MAXHPMETER (20)
class GamePlay;

;
USING_NS_CC_EXT;
using namespace cocosbuilder;

class GameInterface :
	public GameObject,
	public CCBMemberVariableAssigner,
	public CCBSelectorResolver
{
	public:
		bool init(){return true;};
		CREATE_FUNC(GameInterface);
		static GameInterface* interface(cocos2d::Node* parent);
		virtual void onCreate();
		virtual void onUpdate(float delta);
		virtual void onDestroy();

		virtual bool onAssignCCBMemberVariable(cocos2d::Ref*, const char*, cocos2d::Node*);
		virtual SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::Ref *, const char*);
		virtual Control::Handler onResolveCCBCCControlSelector(cocos2d::Ref *, const char*);

		void onUpdateClassic(float delta, GamePlay* play);
		void onUpdateArcade(float delta, GamePlay* play);
		void toggleShowItemButtons(bool flag);
		void rebuildDarts(bool unlimit);
		cocos2d::Node *mParent;
		//-------- new ui ------------
		cocos2d::Sprite *mUIBg;
		cocos2d::Sprite *mSpell;
		cocos2d::Sprite *mSpellName;
		cocos2d::Node *mSpFgNode;
		cocos2d::Sprite *mSpBg;
		cocos2d::Sprite *mSpBgTop;
		cocos2d::Sprite *mSpellGrid;
		int       mSpCache;

		//distance
		cocos2d::Label *mDistanceNumber;
		cocos2d::Label *mDistanceM;
		int       mDistanceCache;

		//coins
		cocos2d::Label *mCoinNumber;
		cocos2d::Sprite *mCoinMark;
		int       mCoinCache;

		//hp
		int mHPCache;
		cocos2d::Sprite *mHPMeters[MAXHPMETER];

		//darts
		cocos2d::Node *mDartMeter;
		int mDartCache;
		float mDartDis;
		bool mUnlimtedDarts;
		float mUnlimtedDartsTimer;
		cocos2d::Node *mHitNode;
		cocos2d::Node *mFeverCoins;
		cocos2d::Label *mFeverFont;

		//score * arcade
		cocos2d::Label *mScoreNumber;
		int mScoreCache;

		//speed * boost
		cocos2d::Node *mSpeedBoost;
		cocos2d::Sprite *mSpeedMask;
		cocos2d::LayerColor *mSpeedBg;
		cocos2d::LayerColor *mSpeedRed;
		cocos2d::LayerColor *mSpeedNormal;
		cocos2d::LayerColor *mSpeedFever;
		int mSpeedState;//0-normal 1-red flash 2-fever flash
		cocos2d::Action *mSpeedAction;

		//arcade * timer
		int mTimerCache;
		cocos2d::Label *mTimerLabel;
		cocos2d::Sprite *mTimerMark;

		//arcade * cup
		int mCupCache;
		cocos2d::Sprite *mCup;

		//arcade x
		cocos2d::Label *mArcadeX;
		cocos2d::Menu *mMenu;
		cocos2d::MenuItemSprite *mFly;
		cocos2d::MenuItemSprite *mSP;
		cocos2d::Sprite *mFlyBg;
		cocos2d::Sprite *mSPBg;
		cocos2d::Label *mFlyNumber;
		cocos2d::Label *mSPNumber;
		//----- sub node ---------
		cocos2d::Sprite *mHits;
		cocos2d::Label *mHitCount;
		cocos2d::Sprite *mSpFg;
		cocos2d::Sprite *mSpFgTop;

		bool mFlyUsed;
		bool mPotionUsed;


		void onFly(cocos2d::Ref*);

		void onSP(cocos2d::Ref*);

		void popFeverCoins(int coins);

		void popHits(int number);

		void updateHits(float time);

		void setArcadeX(int x);

		void fadeHits();

		void toggleUnlimtedDarts(bool flag);
};
#endif
