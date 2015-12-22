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
cocos2d::Node *mObjDaily;
cocos2d::Node *mObjWeekly;
cocos2d::Node *mObjMonthly;
cocos2d::CCMenu *mMenu;
cocos2d::Label *mBest;
cocos2d::MenuItemImage *mSfx;
cocos2d::MenuItemImage *mMusic;
cocos2d::MenuItemImage *mQuit;
cocos2d::MenuItemImage *mResume;
cocos2d::MenuItemImage *mRetry;
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
cocos2d::Node *mClassic;
cocos2d::Node *mArcade;
    
    //-- Arcade --
cocos2d::Label *mGoldScore;
cocos2d::Label *mSilverScore;
cocos2d::Label *mBronzeScore;
cocos2d::Label *mGoldPrize;
cocos2d::Label *mSilverPrize;
cocos2d::Label *mBronzePrize;
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

    virtual bool onAssignCCBMemberVariable(cocos2d::Ref*, const char*, cocos2d::Node*);
    virtual SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::Ref *, const char*);
    virtual cocos2d::extension::Control::Handler onResolveCCBCCControlSelector(cocos2d::Ref *, const char*);
};
#endif
