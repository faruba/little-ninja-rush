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
cocos2d::CCNode *mParent;
    //-------- new ui ------------
cocos2d::CCSprite *mUIBg;
cocos2d::CCSprite *mSpell;
cocos2d::CCSprite *mSpellName;
cocos2d::CCNode *mSpFgNode;
cocos2d::CCSprite *mSpBg;
cocos2d::CCSprite *mSpBgTop;
cocos2d::CCSprite *mSpellGrid;
    int       mSpCache;

    //distance
cocos2d::CCLabelBMFont *mDistanceNumber;
cocos2d::CCLabelBMFont *mDistanceM;
    int       mDistanceCache;

    //coins
cocos2d::CCLabelBMFont *mCoinNumber;
cocos2d::CCSprite *mCoinMark;
    int       mCoinCache;

    //hp
    int mHPCache;
cocos2d::CCSprite *mHPMeters[MAXHPMETER];

    //darts
cocos2d::CCNode *mDartMeter;
    int mDartCache;
    float mDartDis;
    bool mUnlimtedDarts;
    float mUnlimtedDartsTimer;
cocos2d::CCNode *mHitNode;
cocos2d::CCNode *mFeverCoins;
cocos2d::CCLabelBMFont *mFeverFont;

    //score * arcade
cocos2d::CCLabelBMFont *mScoreNumber;
    int mScoreCache;

    //speed * boost
cocos2d::CCNode *mSpeedBoost;
cocos2d::CCSprite *mSpeedMask;
cocos2d::CCLayerColor *mSpeedBg;
cocos2d::CCLayerColor *mSpeedRed;
cocos2d::CCLayerColor *mSpeedNormal;
cocos2d::CCLayerColor *mSpeedFever;
    int mSpeedState;//0-normal 1-red flash 2-fever flash
cocos2d::CCAction *mSpeedAction;

    //arcade * timer
    int mTimerCache;
cocos2d::CCLabelBMFont *mTimerLabel;
cocos2d::CCSprite *mTimerMark;

    //arcade * cup
    int mCupCache;
cocos2d::CCSprite *mCup;

    //arcade x
cocos2d::CCLabelBMFont *mArcadeX;
cocos2d::CCMenu *mMenu;
cocos2d::CCMenuItemSprite *mFly;
cocos2d::CCMenuItemSprite *mSP;
cocos2d::CCSprite *mFlyBg;
cocos2d::CCSprite *mSPBg;
cocos2d::CCLabelBMFont *mFlyNumber;
cocos2d::CCLabelBMFont *mSPNumber;
    //----- sub node ---------
cocos2d::CCSprite *mHits;
cocos2d::CCLabelBMFont *mHitCount;
cocos2d::CCSprite *mSpFg;
cocos2d::CCSprite *mSpFgTop;

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
