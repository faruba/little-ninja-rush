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
cocos2d::Sprite *mRode;
    CEClipedNode *mBoard;
    
    float mTimer;
    bool mFlag;
    
    //loaded variable
cocos2d::CCNode *mObjDaily;
cocos2d::CCNode *mObjWeekly;
cocos2d::CCNode *mObjMonthly;
cocos2d::CCMenu *mMenu;
cocos2d::CCLabelTTF *mBest;
cocos2d::CCMenuItemImage *mSfx;
cocos2d::CCMenuItemImage *mMusic;
cocos2d::CCMenuItemImage *mQuit;
cocos2d::CCMenuItemImage *mResume;
cocos2d::CCMenuItemImage *mRetry;
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
cocos2d::CCNode *mClassic;
cocos2d::CCNode *mArcade;
    
    //-- Arcade --
cocos2d::CCLabelBMFont *mGoldScore;
cocos2d::CCLabelBMFont *mSilverScore;
cocos2d::CCLabelBMFont *mBronzeScore;
cocos2d::CCLabelBMFont *mGoldPrize;
cocos2d::CCLabelBMFont *mSilverPrize;
cocos2d::CCLabelBMFont *mBronzePrize;
cocos2d::Sprite *mGoldCoin;
cocos2d::Sprite *mSilverCoin;
cocos2d::Sprite *mBronzeCoin;

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

    virtual bool onAssignCCBMemberVariable(cocos2d::Ref*, const char*, cocos2d::CCNode*);
    virtual SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::Ref *, const char*);
    virtual cocos2d::extension::Control::Handler onResolveCCBCCControlSelector(cocos2d::Ref *, const char*);
};
#endif
