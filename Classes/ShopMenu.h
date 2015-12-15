#ifndef ___CLASSES_SHOPMENU_H_
#define ___CLASSES_SHOPMENU_H_

#include "GameTool.h"
#include "cocos2d.h"
#include "cocos-ext.h"
#include "FoldItem.h"
//#include "IAPHelper.h"

;
USING_NS_CC_EXT;

class ShopMenu:
  public cocos2d::CCLayer, 
  public cocos2d::extension::CCBMemberVariableAssigner,
  public cocos2d::extension::CCBSelectorResolver,
  public FoldItemController
{
public:
    virtual bool init();  
    virtual void onEnter();
    virtual void update(float);
    virtual void onExit();

    LNR_SCENE_METHOD(ShopMenu);
    CCB_STATIC_NEW_AUTORELEASE_OBJECT_WITH_INIT_METHOD(ShopMenu, create);
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject*, const char*, cocos2d::CCNode*);
    virtual SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject *, const char*);
    virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject *, const char*);

    virtual bool onTouchBegan(Touch * touch, Event * event);
    virtual void onTouchMoved(Touch * touch, Event * event);
    virtual void onTouchEnded(Touch * touch, Event * event);

    CCNode *mNode;
    CEClipedNode *mClipedList;
    CCNode *mItemList;
    float mOffset;
    int mCurrUnFold;
    cocos2d::Point mTouchBegin;
    //scrolls
    CCSprite *mScrollBody;

    CCLayerColor *mMask;

    float mBeginPressY;
    float mBeginNodeY;
    bool mFly;
    float mLastY;
    float mFlySpeed;
    //CFAbsoluteTime mLastTime;
    struct cc_timeval mLastTime;
    //-- auto assign --
    CCLabelBMFont *mCoins;
    CCNode *mList;
    CCMenu *mMenu;

    //intro&outro
    CCNode *mSceneIntro;
    CCScene *mNewScene;
    bool mIntroFlag;

    //---modal---
    bool mIsModal;
    int mModalPurchase;
    float mModalTimer;
    void setModal(int pid); 
    void modalOver();
    void cancelModal();
    void purchaseVerified(int pid);
    void purchaseFailed(int pid);
    void purchaseCancelled(int pid);


    //0-main 1-select
    static void setNavBack(int sid);

    void activate(int cid);
    virtual void purchase(int cid);

    void addCategory(int cid);
    void addCharacter(int cid);
    void addUtility(int cid);
    void addSupply(int cid, int index);

    //scroll control
    void updateScorll();

    //-- auto callback
    void onBack();
    void onMoreCoins();

    //intro&outro
    void setSceneIntro();
    void setSceneOutro(cocos2d::CCScene* newscene);
    void doneOutro();

};

class ShopMenuLayerLoader : public cocos2d::extension::CCLayerLoader {
  public:
    CCB_STATIC_NEW_AUTORELEASE_OBJECT_METHOD(ShopMenuLayerLoader, loader);

  protected:
    CCB_VIRTUAL_NEW_AUTORELEASE_CREATECCNODE_METHOD(ShopMenu);
};

#endif
