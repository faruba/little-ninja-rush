#ifndef ___CLASSES_GAMEINTERFACE_H_
#define ___CLASSES_GAMEINTERFACE_H_
#include "GameTool.h"
#include "GameConstants.h"
//#include "CCBReader.h"
#include "cocosbuilder/CocosBuilder.h"


#define MAXHPMETER (6)
class GamePlay;

;
USING_NS_CC_EXT;
using namespace cocosbuilder;

class GameInterface :
public GameObject,
public CCBMemberVariableAssigner,
public CCBSelectorResolver
{
  public:
    bool init(){return true;};
    CREATE_FUNC(GameInterface);
    static GameInterface* interface(cocos2d::CCNode* parent);
    virtual void onCreate();
    virtual void onUpdate(float delta);
    virtual void onDestroy();
    
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject*, const char*, cocos2d::CCNode*);
    virtual SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject *, const char*);
    virtual Control::Handler onResolveCCBCCControlSelector(cocos2d::CCObject *, const char*);

    void onUpdateClassic(float delta, GamePlay* play);
    void onUpdateArcade(float delta, GamePlay* play);
    void toggleShowItemButtons(bool flag);
    void rebuildDarts(bool unlimit);

    CCNode *mParent;
    //-------- new ui ------------
    CCSprite *mUIBg;
    CCSprite *mSpell;
    CCSprite *mSpellName;
    CCNode *mSpFgNode;
    CCSprite *mSpBg;
    CCSprite *mSpBgTop;
    CCSprite *mSpellGrid;
    int       mSpCache;

    //distance
    CCLabelBMFont *mDistanceNumber;
    CCLabelBMFont *mDistanceM;
    int       mDistanceCache;

    //coins
    CCLabelBMFont *mCoinNumber;
    CCSprite *mCoinMark;
    int       mCoinCache;

    //hp
    int mHPCache;
    CCSprite *mHPMeters[MAXHPMETER];

    //darts
    CCNode *mDartMeter;
    int mDartCache;
    float mDartDis;
    bool mUnlimtedDarts;
    float mUnlimtedDartsTimer;

    CCNode *mHitNode;
    CCNode *mFeverCoins;
    CCLabelBMFont *mFeverFont;

    //score * arcade
    CCLabelBMFont *mScoreNumber;
    int mScoreCache;

    //speed * boost
    CCNode *mSpeedBoost;
    CCSprite *mSpeedMask;
    CCLayerColor *mSpeedBg;
    CCLayerColor *mSpeedRed;
    CCLayerColor *mSpeedNormal;
    CCLayerColor *mSpeedFever;
    int mSpeedState;//0-normal 1-red flash 2-fever flash
    CCAction *mSpeedAction;

    //arcade * timer
    int mTimerCache;
    CCLabelBMFont *mTimerLabel;
    CCSprite *mTimerMark;

    //arcade * cup
    int mCupCache;
    CCSprite *mCup;

    //arcade x
    CCLabelBMFont *mArcadeX;

    CCMenu *mMenu;
    CCMenuItemSprite *mFly;
    CCMenuItemSprite *mSP;
    CCSprite *mFlyBg;
    CCSprite *mSPBg;
    CCLabelBMFont *mFlyNumber;
    CCLabelBMFont *mSPNumber;
    //----- sub node ---------
    CCSprite *mHits;
    CCLabelBMFont *mHitCount;
    CCSprite *mSpFg;
    CCSprite *mSpFgTop;

    bool mFlyUsed;
    bool mPotionUsed;


    void onFly();

    void onSP();

    void popFeverCoins(int coins);

    void popHits(int number);

    void updateHits(float time);

    void setArcadeX(int x);

    void fadeHits();

    void toggleUnlimtedDarts(bool flag);
};
#endif
