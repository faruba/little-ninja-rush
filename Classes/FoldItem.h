#ifndef ___FOLD_ITEM_H_
#define ___FOLD_ITEM_H_
#include "cocos2d.h"
#include "cocos-ext.h"
#include "cocosbuilder/CocosBuilder.h"

;
USING_NS_CC_EXT;

class FoldItemController
{
	public:
		virtual void purchase(int cid) = 0;
};

class FoldItem : public cocos2d::Node
{
	public:
		CCB_STATIC_NEW_AUTORELEASE_OBJECT_METHOD(FoldItem, create);

		//if title < 0 presents category
		static FoldItem* foldItem(int title, int cost, bool iap, int icon, const char* detail, int btntyp, FoldItemController* controller);
		static FoldItem* foldItem(const char* title, const char* icon, const char* detail, int achieve, int counter, FoldItemController* controller);

		void purchase(Ref* ref);
		void toggleFold(bool flag);
		bool isFoldable();
		void updateInfo(int icon, int cost, const char* detail, int btntyp);
		void togglePurchaseButton(bool flag);

	private:
		int mTitle;
		int mIconId;
		std::string mDetail;
		int mBtnType;
		bool mIsIAP;
		int mCostNumber;
		//--------------------
		cocos2d::Sprite *mIcon;
		cocos2d::Menu *mMenu;
		cocos2d::Sprite *mBgButton;
		cocos2d::MenuItemImage *mPurchase;
		cocos2d::Node *mCost;
		cocos2d::Sprite *mTitleSp;
		cocos2d::Label *mDetailSp;

		FoldItemController *mController;
		bool mIsAchievement;
};
#endif
