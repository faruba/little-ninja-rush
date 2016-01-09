#ifndef ___CLASSES_COLLECTIONMENU_H_
#define ___CLASSES_COLLECTIONMENU_H_
#include "cocos2d.h"
#include "cocos-ext.h"
#include "ABScrollContent.h"
#include "GameRecord.h"
#include "GameData.h"
#include "CollectionMenuDelegate.hpp"

USING_NS_CC_EXT;
class CollectionMenu : 
	public Layer, 
	public cocosbuilder::CCBMemberVariableAssigner,
	public cocosbuilder::CCBSelectorResolver
{
	public:
		CCB_STATIC_NEW_AUTORELEASE_OBJECT_WITH_INIT_METHOD(CollectionMenu, create);
		virtual bool onAssignCCBMemberVariable(cocos2d::Ref*, const char*, cocos2d::Node*);
		virtual SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::Ref *, const char*);
		virtual cocos2d::extension::Control::Handler onResolveCCBCCControlSelector(cocos2d::Ref * , const char* );

		virtual bool init(){
			mCurrentDelegate = nullptr;
			return true;
		}
		virtual void onEnter();
		virtual void onExit();
		virtual void update(float delta);

		void updatePowerUpButton();

		//-- auto callback --
		void onBack(cocos2d::Ref*);
		void onUse(cocos2d::Ref*);
		void onCharacter(int nRole);
		void onCharacter1(cocos2d::Ref*){onCharacter(0);};
		void onCharacter2(cocos2d::Ref*){onCharacter(1);};
		void onCharacter3(cocos2d::Ref*){onCharacter(2);};
		void onCharacter4(cocos2d::Ref*){onCharacter(3);};
		void onItem(CollectionMenuDelegate *newDelegate);
		void onShuriken(cocos2d::Ref*){onItem(&mShurikenDelegate);};
		void onKatana(cocos2d::Ref*){onItem(&mKatanaDelegate);};
		void onSpecial(cocos2d::Ref*){onItem(&mSpecialDelegate);};
		void onPowerup(cocos2d::Ref*);

		void onFacebook(cocos2d::Ref*);
		void onTwitter(cocos2d::Ref*);

		void onSelectLife(cocos2d::Ref*);
		void onSelectDart(cocos2d::Ref*);

		void clickMethod();

		cocos2d::Point mShadowDir;
		int mEquipedItem;
		int mCurrItem;

		cocos2d::Sprite *mCurrentMark;
		ABScrollContent *mScroll;

		bool mTouchEnabled;

		bool mPostingFacebook;
		bool mPostingTwitter;
		//-- auto assign --
		cocos2d::Sprite *mShadow;
		cocos2d::Sprite *mItemTitle;
		cocos2d::Label *mItemDesc;
		cocos2d::Label *mScrollCount;
		cocos2d::MenuItemImage *mCharacter1;
		cocos2d::MenuItemImage *mCharacter2;
		cocos2d::MenuItemImage *mCharacter3;
		cocos2d::MenuItemImage *mCharacter4;
		cocos2d::MenuItemImage *mUse;
		cocos2d::CCMenu *mMenu;
		//CCMenu *mItems;
		cocos2d::Node *mPowerUp;
		cocos2d::Node *mLifeGuage;
		cocos2d::Node *mDartGuage;
		cocos2d::Sprite *mLifeMask;
		cocos2d::Sprite *mDartMask;
		cocos2d::Label *mLifeCount;
		cocos2d::Label *mDartCount;
		cocos2d::LayerColor *mMask;
		cocos2d::CCMenu *mPowerupMenu;
		cocos2d::MenuItemImage *mFacebook;
		cocos2d::MenuItemImage *mTwitter;
		cocos2d::Sprite *mShare;
		cocos2d::Sprite *mFacebookAction;
		cocos2d::Sprite *mTwitterAction;
		cocos2d::Node *mScrollPoint;

		bool mModal;
		cocos2d::Ref *mModalTarget;
		SEL_CallFunc mModalSel;

		void setModal(const char* title, const char* desc, Ref* target, SEL_CallFunc sel);
		void onModalConfirm(cocos2d::Ref*);
		void onModalCancel(cocos2d::Ref*);


		// 0-mainmenu 1-selectmenu
		static void setNavBack(int nid);
		cocos2d::MenuItemImage* character(int rid);

		//bid = 0 :no blink 1:blink hp 2:blink dart
		void updateCharacterInfo(int rid, int bid);

		void doneFacebook(cocos2d::CCInteger* res);
		void doneTwitter(cocos2d::CCInteger* res);

		void toggleShare(bool flag);
		void onItemCallback(int i);

	private:
		UISwapper mUISwapper;

		CollectionMenuDelegate* mCurrentDelegate;
		ShurikenCollectionDelegate mShurikenDelegate;
		KatanaCollectionDelegate mKatanaDelegate;
		SpecialCollectionDelegate mSpecialDelegate;
		PowerUpCollectionDelegate mPowerUpDelegate;
};
class CollectionMenuLayerLoader : public cocosbuilder::NodeLoader {
	public:
		CCB_STATIC_NEW_AUTORELEASE_OBJECT_METHOD(CollectionMenuLayerLoader, loader);

	protected:
		CCB_VIRTUAL_NEW_AUTORELEASE_CREATECCNODE_METHOD(CollectionMenu);
};
#endif
