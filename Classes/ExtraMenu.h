#ifndef ___CLASSES_EXTRAMENU_H_
#define ___CLASSES_EXTRAMENU_H_

#include "GameTool.h"
#include "FoldItem.h"

class ExtraMenu:
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

    LNR_SCENE_METHOD(ExtraMenu);
    static CCScene* scene(int mode);

    CCB_STATIC_NEW_AUTORELEASE_OBJECT_WITH_INIT_METHOD(ExtraMenu, create);
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject*, const char*, cocos2d::CCNode*);
    virtual SEL_MenuHandler onResolveCCBCCMenuItemSelector(CCObject *, const char*);
    virtual SEL_CCControlHandler onResolveCCBCCControlSelector(CCObject *, const char*);

    virtual bool onTouchBegan(Touch * touch, Event * event);
    virtual void onTouchMoved(Touch * touch, Event * event);
    virtual void onTouchEnded(Touch * touch, Event * event);

    int mMode;//0-achievement 1-statistics 2-about
    CCNode *mNode;
    CEClipedNode *mClipedList;
    CCNode *mItemList;
    float mOffset;
    int mCurrUnFold;
    CCPoint mTouchBegin;
    //scrolls
    CCSprite *mScrollBody;

    int mCurrState;//0-achievements 1-statistics 2-about us

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
    CCSprite *mBanner;

    //intro&outro
    CCNode *mSceneIntro;
    CCScene *mNewScene;
    bool mIntroFlag;

    void activate(int cid);

    //scroll control
    void updateScorll();

    void loadAchievements();
    void loadStatistics();
    void loadAboutUs();

    void onTwitter();
    void onFacebook();
    void onMail();
    void onWeibo();

    //-- auto callback
    void onBack();

    //intro&outro
    void setSceneIntro();
    void setSceneOutro(CCScene* newscene);
    void doneOutro();

    virtual void purchase(int cid) {};
};

class ExtraMenuLayerLoader : public cocos2d::extension::CCLayerLoader {
  public:
    CCB_STATIC_NEW_AUTORELEASE_OBJECT_METHOD(ExtraMenuLayerLoader, loader);

  protected:
    CCB_VIRTUAL_NEW_AUTORELEASE_CREATECCNODE_METHOD(ExtraMenu);
};
#endif
