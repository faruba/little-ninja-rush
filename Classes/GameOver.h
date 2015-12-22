#ifndef ___CLASSES_GAMEOVER_H_
#define ___CLASSES_GAMEOVER_H_
#include "cocosbuilder/CocosBuilder.h"
#include "GameTool.h"
USING_NS_CC_EXT;

class GameOver:
  public GameObject,
  public cocosbuilder::CCBMemberVariableAssigner,
  public cocosbuilder::CCBSelectorResolver
{
  public:
    static GameOver* gameOver(cocos2d::Node* parent);
    virtual bool init(){return true;};
    CREATE_FUNC(GameOver);

    virtual bool onAssignCCBMemberVariable(cocos2d::Ref*, const char*, cocos2d::Node*);
    virtual SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::Ref *, const char*);
    virtual cocos2d::extension::Control::Handler onResolveCCBCCControlSelector(cocos2d::Ref *, const char*);

    void updateClassic();
    void updateArcade();
    virtual void onCreate();
    virtual void onUpdate(float delta);
    virtual void onDestroy();

    bool haveFacebookCoins();
    bool haveTwitterCoins();
cocos2d::Node *mParent;
cocos2d::Node *mNode;
    int mCost;
    int mScore;
    bool mPostingFacebook;
    bool mPostingTwitter;

    //-- auto assign --
cocos2d::Label *mDistance;
cocos2d::Label *mCoins;
cocos2d::MenuItemImage *mChange;
cocos2d::Label *mDailyObjective;
cocos2d::Label *mWeeklyObjective;
cocos2d::Label *mMonthlyObjective;
cocos2d::Node *mDailyCrown;
cocos2d::Node *mWeeklyCrown;
cocos2d::Node *mMonthlyCrown;
cocos2d::Sprite *mDailyIcon;
cocos2d::Sprite *mWeeklyIcon;
cocos2d::Sprite *mMonthlyIcon;
cocos2d::Sprite *mLeadSp0;
cocos2d::Sprite *mLeadSp1;
cocos2d::Sprite *mLeadSp2;
cocos2d::Label *mLeadLb0;
cocos2d::Label *mLeadLb1;
cocos2d::Label *mLeadLb2;
cocos2d::MenuItemImage *mPlayAgain;
cocos2d::MenuItemImage *mTwitter;
cocos2d::MenuItemImage *mFacebook;
cocos2d::Sprite *mFacebookAction;
cocos2d::Sprite *mTwitterAction;
cocos2d::Sprite *mFacebookCoins;
cocos2d::Sprite *mTwitterCoins;
cocos2d::Node *mClassic;
cocos2d::Node *mArcade;

    // -- Arcade --
cocos2d::Label *mArcadeScore;
cocos2d::Sprite *mGoldCoin;
cocos2d::Sprite *mSilverCoin;
cocos2d::Sprite *mBronzeCoin;
cocos2d::Label *mGoldScore;
cocos2d::Label *mSilverScore;
cocos2d::Label *mBronzeScore;
cocos2d::Label *mGoldPrize;
cocos2d::Label *mSilverPrize;
cocos2d::Label *mBronzePrize;
cocos2d::Sprite *mCup;


    //-- auto callback --
    void onChange();
    void onMenu();
    void onRestart();
    void onFacebook();
    void onTwitter();

    void doneFacebook(cocos2d::CCInteger* res);
    void doneTwitter(cocos2d::CCInteger* res);

};
#endif
