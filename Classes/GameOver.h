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
    static GameOver* gameOver(CCNode* parent);
    virtual bool init(){return true;};
    CREATE_FUNC(GameOver);

    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject*, const char*, cocos2d::CCNode*);
    virtual SEL_MenuHandler onResolveCCBCCMenuItemSelector(CCObject *, const char*);
    virtual cocos2d::extension::Control::Handler onResolveCCBCCControlSelector(cocos2d::Ref *, const char*);

    void updateClassic();
    void updateArcade();
    virtual void onCreate();
    virtual void onUpdate(float delta);
    virtual void onDestroy();

    bool haveFacebookCoins();
    bool haveTwitterCoins();
    CCNode *mParent;
    CCNode *mNode;
    int mCost;
    int mScore;
    bool mPostingFacebook;
    bool mPostingTwitter;

    //-- auto assign --
    CCLabelBMFont *mDistance;
    CCLabelBMFont *mCoins;
    CCMenuItemImage *mChange;
    CCLabelTTF *mDailyObjective;
    CCLabelTTF *mWeeklyObjective;
    CCLabelTTF *mMonthlyObjective;
    CCNode *mDailyCrown;
    CCNode *mWeeklyCrown;
    CCNode *mMonthlyCrown;
    CCSprite *mDailyIcon;
    CCSprite *mWeeklyIcon;
    CCSprite *mMonthlyIcon;
    CCSprite *mLeadSp0;
    CCSprite *mLeadSp1;
    CCSprite *mLeadSp2;
    CCLabelBMFont *mLeadLb0;
    CCLabelBMFont *mLeadLb1;
    CCLabelBMFont *mLeadLb2;
    CCMenuItemImage *mPlayAgain;
    CCMenuItemImage *mTwitter;
    CCMenuItemImage *mFacebook;
    CCSprite *mFacebookAction;
    CCSprite *mTwitterAction;
    CCSprite *mFacebookCoins;
    CCSprite *mTwitterCoins;

    CCNode *mClassic;
    CCNode *mArcade;

    // -- Arcade --
    CCLabelBMFont *mArcadeScore;
    CCSprite *mGoldCoin;
    CCSprite *mSilverCoin;
    CCSprite *mBronzeCoin;
    CCLabelBMFont *mGoldScore;
    CCLabelBMFont *mSilverScore;
    CCLabelBMFont *mBronzeScore;
    CCLabelBMFont *mGoldPrize;
    CCLabelBMFont *mSilverPrize;
    CCLabelBMFont *mBronzePrize;
    CCSprite *mCup;


    //-- auto callback --
    void onChange();
    void onMenu();
    void onRestart();
    void onFacebook();
    void onTwitter();

    void doneFacebook(CCInteger* res);
    void doneTwitter(CCInteger* res);

};
#endif
