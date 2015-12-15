#ifndef ___CLASSES_COINSMENU_H_
#define ___CLASSES_COINSMENU_H_

#include "GameTool.h"
#include "cocos2d.h"
#include "cocos-ext.h"
#include "FoldItem.h"
//#include "IAPHelper.h"

USING_NS_CC;
USING_NS_CC_EXT;

class CoinsMenu:
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

    LNR_SCENE_METHOD(CoinsMenu);
    CCB_STATIC_NEW_AUTORELEASE_OBJECT_WITH_INIT_METHOD(CoinsMenu, create);
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject*, const char*, cocos2d::CCNode*);
    virtual SEL_MenuHandler onResolveCCBCCMenuItemSelector(CCObject *, const char*);
    virtual SEL_CCControlHandler onResolveCCBCCControlSelector(CCObject *, const char*);

    virtual bool onTouchBegan(Touch * touch, Event * event);
    virtual void onTouchMoved(Touch * touch, Event * event);
    virtual void onTouchEnded(Touch * touch, Event * event);

    CCNode *mNode;
    CEClipedNode *mClipedList;
    CCNode *mItemList;
    float mOffset;
    int mCurrUnFold;
    CCPoint mTouchBegin;
    //scrolls
    CCSprite *mScrollHead;
    CCSprite *mScrollBody;
    CCSprite *mScrollFoot;

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
    CCLayerColor *mMask;
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


    void activate(int cid);
    virtual void purchase(int cid);

    //scroll control
    void updateScorll();

    void addItem(int iid);

    //-- auto callback
    void onBack();

    //intro&outro
    void setSceneIntro();
    void setSceneOutro(CCScene* newscene);
    void doneOutro();

};
class CoinsMenuLayerLoader : public cocos2d::extension::CCLayerLoader {
  public:
    CCB_STATIC_NEW_AUTORELEASE_OBJECT_METHOD(CoinsMenuLayerLoader, loader);

  protected:
    CCB_VIRTUAL_NEW_AUTORELEASE_CREATECCNODE_METHOD(CoinsMenu);
};
#endif
