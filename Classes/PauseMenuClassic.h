#ifndef ___CLASSES_PAUSEMENUCLASSIC_H_
#define ___CLASSES_PAUSEMENUCLASSIC_H_

#include "GameTool.h"
#include "GameObject.h"

class PauseMenuClassic:
  public GameObject,
  public cocosbuilder::CCBMemberVariableAssigner,
  public cocosbuilder::CCBSelectorResolver
{
private:
    CCSprite *mRode;
    CEClipedNode *mBoard;
    
    float mTimer;
    bool mFlag;
    
    //loaded variable
    CCNode *mObjDaily;
    CCNode *mObjWeekly;
    CCNode *mObjMonthly;
    CCMenu *mMenu;
    CCLabelTTF *mBest;
    CCMenuItemImage *mSfx;
    CCMenuItemImage *mMusic;
    CCMenuItemImage *mQuit;
    CCMenuItemImage *mResume;
    CCMenuItemImage *mRetry;
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
    
    CCNode *mClassic;
    CCNode *mArcade;
    
    //-- Arcade --
    CCLabelBMFont *mGoldScore;
    CCLabelBMFont *mSilverScore;
    CCLabelBMFont *mBronzeScore;
    CCLabelBMFont *mGoldPrize;
    CCLabelBMFont *mSilverPrize;
    CCLabelBMFont *mBronzePrize;
    CCSprite *mGoldCoin;
    CCSprite *mSilverCoin;
    CCSprite *mBronzeCoin;

    void updateClassic();
    void updateArcade();
    
public:
    static PauseMenuClassic* pauseMenu();

    virtual void onCreate();
    virtual void onUpdate(float delta);
    virtual void onDestroy();
    
    void toggleSfx(Ref* ref);
    void toggleMusic(Ref* ref);
    void quit(Ref* ref);
    void resume(Ref* ref);
    void retry(Ref* ref);
    void change(Ref* ref);

    CREATE_FUNC(PauseMenuClassic);
    bool init(){return true;};

    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject*, const char*, cocos2d::CCNode*);
    virtual SEL_MenuHandler onResolveCCBCCMenuItemSelector(CCObject *, const char*);
    virtual cocos2d::extension::Control::Handler onResolveCCBCCControlSelector(cocos2d::Ref *, const char*);
};
#endif
