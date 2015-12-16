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
    static GameOver* gameOver(cocos2d::CCNode* parent);
    virtual bool init(){return true;};
    CREATE_FUNC(GameOver);

    virtual bool onAssignCCBMemberVariable(cocos2d::Ref*, const char*, cocos2d::CCNode*);
    virtual SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::Ref *, const char*);
    virtual cocos2d::extension::Control::Handler onResolveCCBCCControlSelector(cocos2d::Ref *, const char*);

    void updateClassic();
    void updateArcade();
    virtual void onCreate();
    virtual void onUpdate(float delta);
    virtual void onDestroy();

    bool haveFacebookCoins();
    bool haveTwitterCoins();
cocos2d::CCNode *mParent;
cocos2d::CCNode *mNode;
    int mCost;
    int mScore;
    bool mPostingFacebook;
    bool mPostingTwitter;

    //-- auto assign --
cocos2d::CCLabelBMFont *mDistance;
cocos2d::CCLabelBMFont *mCoins;
cocos2d::CCMenuItemImage *mChange;
cocos2d::CCLabelTTF *mDailyObjective;
cocos2d::CCLabelTTF *mWeeklyObjective;
cocos2d::CCLabelTTF *mMonthlyObjective;
cocos2d::CCNode *mDailyCrown;
cocos2d::CCNode *mWeeklyCrown;
cocos2d::CCNode *mMonthlyCrown;
cocos2d::Sprite *mDailyIcon;
cocos2d::Sprite *mWeeklyIcon;
cocos2d::Sprite *mMonthlyIcon;
cocos2d::Sprite *mLeadSp0;
cocos2d::Sprite *mLeadSp1;
cocos2d::Sprite *mLeadSp2;
cocos2d::CCLabelBMFont *mLeadLb0;
cocos2d::CCLabelBMFont *mLeadLb1;
cocos2d::CCLabelBMFont *mLeadLb2;
cocos2d::CCMenuItemImage *mPlayAgain;
cocos2d::CCMenuItemImage *mTwitter;
cocos2d::CCMenuItemImage *mFacebook;
cocos2d::Sprite *mFacebookAction;
cocos2d::Sprite *mTwitterAction;
cocos2d::Sprite *mFacebookCoins;
cocos2d::Sprite *mTwitterCoins;
cocos2d::CCNode *mClassic;
cocos2d::CCNode *mArcade;

    // -- Arcade --
cocos2d::CCLabelBMFont *mArcadeScore;
cocos2d::Sprite *mGoldCoin;
cocos2d::Sprite *mSilverCoin;
cocos2d::Sprite *mBronzeCoin;
cocos2d::CCLabelBMFont *mGoldScore;
cocos2d::CCLabelBMFont *mSilverScore;
cocos2d::CCLabelBMFont *mBronzeScore;
cocos2d::CCLabelBMFont *mGoldPrize;
cocos2d::CCLabelBMFont *mSilverPrize;
cocos2d::CCLabelBMFont *mBronzePrize;
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
