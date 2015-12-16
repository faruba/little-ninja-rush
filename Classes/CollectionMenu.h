#ifndef ___CLASSES_COLLECTIONMENU_H_
#define ___CLASSES_COLLECTIONMENU_H_
#include "cocos2d.h"
#include "cocos-ext.h"
#include "ABScrollContent.h"

;
USING_NS_CC_EXT;

class CollectionMenu : 
  public CCLayer, 
  public cocosbuilder::CCBMemberVariableAssigner,
  public cocosbuilder::CCBSelectorResolver
{
  public:
CCB_STATIC_NEW_AUTORELEASE_OBJECT_WITH_INIT_METHOD(CollectionMenu, create);
    virtual bool onAssignCCBMemberVariable(cocos2d::Ref*, const char*, cocos2d::CCNode*);
    virtual SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::Ref *, const char*);
    virtual cocos2d::extension::Control::Handler onResolveCCBCCControlSelector(cocos2d::Ref * , const char* );

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
cocos2d::CCSprite *mEquipedMark;
cocos2d::CCSprite *mCurrentMark;
    //CCNode *mItemIcons;
    ABScrollContent *mScroll;

    bool mTouchEnabled;

    bool mPostingFacebook;
    bool mPostingTwitter;
    //-- auto assign --
cocos2d::CCSprite *mShadow;
cocos2d::CCSprite *mItemTitle;
cocos2d::CCLabelTTF *mItemDesc;
cocos2d::CCLabelBMFont *mScrollCount;
cocos2d::CCMenuItemImage *mCharacter1;
cocos2d::CCMenuItemImage *mCharacter2;
cocos2d::CCMenuItemImage *mCharacter3;
cocos2d::CCMenuItemImage *mCharacter4;
cocos2d::CCMenuItemImage *mShuriken;
cocos2d::CCMenuItemImage *mKatana;
cocos2d::CCMenuItemImage *mSpecial;
cocos2d::CCMenuItemImage *mPowerup;
cocos2d::CCMenuItemImage *mUse;
cocos2d::CCMenu *mMenu;
    //CCMenu *mItems;
cocos2d::CCNode *mPowerUp;
cocos2d::CCNode *mLifeGuage;
cocos2d::CCNode *mDartGuage;
cocos2d::CCSprite *mLifeMask;
cocos2d::CCSprite *mDartMask;
cocos2d::CCLabelBMFont *mLifeCount;
cocos2d::CCLabelBMFont *mDartCount;
cocos2d::CCLayerColor *mMask;
cocos2d::CCMenu *mPowerupMenu;
cocos2d::CCMenuItemImage *mFacebook;
cocos2d::CCMenuItemImage *mTwitter;
cocos2d::CCSprite *mShare;
cocos2d::CCSprite *mFacebookAction;
cocos2d::CCSprite *mTwitterAction;
cocos2d::CCNode *mScrollPoint;

    //intro&outro
cocos2d::CCNode *mSceneIntro;
cocos2d::CCScene *mNewScene;
    bool mIntroFlag;

    bool mModal;
cocos2d::Ref *mModalTarget;
    SEL_CallFunc mModalSel;

    void setModal(const char* title, const char* desc, Ref* target, SEL_CallFunc sel);
    void onModalConfirm();
    void onModalCancel();


    // 0-mainmenu 1-selectmenu
    static void setNavBack(int nid);
    void markUsing(int i);
    void markCurrent(int i);
cocos2d::CCMenuItemImage* character(int rid);
    void setTypeButton(int typ, bool mod);

    //bid = 0 :no blink 1:blink hp 2:blink dart
    void updateCharacterInfo(int rid, int bid);

    void doneFacebook(cocos2d::CCInteger* res);
    void doneTwitter(cocos2d::CCInteger* res);

    void toggleShare(bool flag);
    void onItemCallback(int i);

};
class CollectionMenuLayerLoader : public cocosbuilder::NodeLoader {
  public:
CCB_STATIC_NEW_AUTORELEASE_OBJECT_METHOD(CollectionMenuLayerLoader, loader);

  protected:
CCB_VIRTUAL_NEW_AUTORELEASE_CREATECCNODE_METHOD(CollectionMenu);
};
#endif
