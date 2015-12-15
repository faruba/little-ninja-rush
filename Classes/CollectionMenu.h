#ifndef ___CLASSES_COLLECTIONMENU_H_
#define ___CLASSES_COLLECTIONMENU_H_
#include "cocos2d.h"
#include "cocos-ext.h"
#include "ABScrollContent.h"

;
USING_NS_CC_EXT;

class CollectionMenu : 
  public CCLayer, 
  public extension::CCBMemberVariableAssigner,
  public extension::CCBSelectorResolver
{
  public:
    CCB_STATIC_NEW_AUTORELEASE_OBJECT_WITH_INIT_METHOD(CollectionMenu, create);
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject*, const char*, cocos2d::CCNode*);
    virtual SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject *, const char*);
    virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject *, const char*);

    static CCScene* scene();
    virtual bool init(){return true;};
    virtual void onEnter();
    virtual void onExit();
    virtual void update(float delta);

    void onUseLife();
    void onUseDart();
    void updateShurikens();
    void updateKatanas();
    void updateSpecials();
    void updatePowerups();
    void updateUsing();
    void updateItemInfo();
    void updatePowerUpButton();

    //-- auto callback --
    void onBack();
    void onUse();
    void onCharacter(int nRole);
    void onCharacter1(){onCharacter(0);};
    void onCharacter2(){onCharacter(1);};
    void onCharacter3(){onCharacter(2);};
    void onCharacter4(){onCharacter(3);};
    void onItem(int nItem);
    void onShuriken(){onItem(0);};
    void onKatana(){onItem(1);};
    void onSpecial(){onItem(2);};
    void onPowerup();

    void onFacebook();
    void onTwitter();

    void onSelectLife();
    void onSelectDart();

    void clickMethod();

    //intro&outro
    void setSceneIntro();
    void setSceneOutro(cocos2d::CCScene* newscene);
    void doneOutro();


    cocos2d::Point mShadowDir;
    int mCurrType;
    int mEquipedItem;
    int mCurrItem;
    int mItemCount;
    CCSprite *mEquipedMark;
    CCSprite *mCurrentMark;
    //CCNode *mItemIcons;
    ABScrollContent *mScroll;

    bool mTouchEnabled;

    bool mPostingFacebook;
    bool mPostingTwitter;
    //-- auto assign --
    CCSprite *mShadow;
    CCSprite *mItemTitle;
    CCLabelTTF *mItemDesc;
    CCLabelBMFont *mScrollCount;
    CCMenuItemImage *mCharacter1;
    CCMenuItemImage *mCharacter2;
    CCMenuItemImage *mCharacter3;
    CCMenuItemImage *mCharacter4;
    CCMenuItemImage *mShuriken;
    CCMenuItemImage *mKatana;
    CCMenuItemImage *mSpecial;
    CCMenuItemImage *mPowerup;
    CCMenuItemImage *mUse;
    CCMenu *mMenu;
    //CCMenu *mItems;
    CCNode *mPowerUp;
    CCNode *mLifeGuage;
    CCNode *mDartGuage;
    CCSprite *mLifeMask;
    CCSprite *mDartMask;
    CCLabelBMFont *mLifeCount;
    CCLabelBMFont *mDartCount;
    CCLayerColor *mMask;
    CCMenu *mPowerupMenu;
    CCMenuItemImage *mFacebook;
    CCMenuItemImage *mTwitter;
    CCSprite *mShare;
    CCSprite *mFacebookAction;
    CCSprite *mTwitterAction;
    CCNode *mScrollPoint;

    //intro&outro
    CCNode *mSceneIntro;
    CCScene *mNewScene;
    bool mIntroFlag;

    bool mModal;
    CCObject *mModalTarget;
    SEL_CallFunc mModalSel;

    void setModal(const char* title, const char* desc, CCObject* target, SEL_CallFunc sel);
    void onModalConfirm();
    void onModalCancel();


    // 0-mainmenu 1-selectmenu
    static void setNavBack(int nid);
    void markUsing(int i);
    void markCurrent(int i);

    CCMenuItemImage* character(int rid);
    void setTypeButton(int typ, bool mod);

    //bid = 0 :no blink 1:blink hp 2:blink dart
    void updateCharacterInfo(int rid, int bid);

    void doneFacebook(cocos2d::CCInteger* res);
    void doneTwitter(cocos2d::CCInteger* res);

    void toggleShare(bool flag);
    void onItemCallback(int i);

};
class CollectionMenuLayerLoader : public cocos2d::extension::CCLayerLoader {
  public:
    CCB_STATIC_NEW_AUTORELEASE_OBJECT_METHOD(CollectionMenuLayerLoader, loader);

  protected:
    CCB_VIRTUAL_NEW_AUTORELEASE_CREATECCNODE_METHOD(CollectionMenu);
};
#endif
