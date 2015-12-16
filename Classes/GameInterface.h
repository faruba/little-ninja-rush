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
    
    virtual bool onAssignCCBMemberVariable(cocos2d::Ref*, const char*, cocos2d::CCNode*);
    virtual SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::Ref *, const char*);
    virtual Control::Handler onResolveCCBCCControlSelector(cocos2d::Ref *, const char*);

    void onUpdateClassic(float delta, GamePlay* play);
    void onUpdateArcade(float delta, GamePlay* play);
    void toggleShowItemButtons(bool flag);
    void rebuildDarts(bool unlimit);
cocos2d::CCNode *mParent;
    //-------- new ui ------------
cocos2d::Sprite *mUIBg;
cocos2d::Sprite *mSpell;
cocos2d::Sprite *mSpellName;
cocos2d::CCNode *mSpFgNode;
cocos2d::Sprite *mSpBg;
cocos2d::Sprite *mSpBgTop;
cocos2d::Sprite *mSpellGrid;
    int       mSpCache;

    //distance
cocos2d::CCLabelBMFont *mDistanceNumber;
cocos2d::CCLabelBMFont *mDistanceM;
    int       mDistanceCache;

    //coins
cocos2d::CCLabelBMFont *mCoinNumber;
cocos2d::Sprite *mCoinMark;
    int       mCoinCache;

    //hp
    int mHPCache;
cocos2d::Sprite *mHPMeters[MAXHPMETER];

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
cocos2d::Sprite *mSpeedMask;
cocos2d::LayerColor *mSpeedBg;
cocos2d::LayerColor *mSpeedRed;
cocos2d::LayerColor *mSpeedNormal;
cocos2d::LayerColor *mSpeedFever;
    int mSpeedState;//0-normal 1-red flash 2-fever flash
cocos2d::CCAction *mSpeedAction;

    //arcade * timer
    int mTimerCache;
cocos2d::CCLabelBMFont *mTimerLabel;
cocos2d::Sprite *mTimerMark;

    //arcade * cup
    int mCupCache;
cocos2d::Sprite *mCup;

    //arcade x
cocos2d::CCLabelBMFont *mArcadeX;
cocos2d::CCMenu *mMenu;
cocos2d::CCMenuItemSprite *mFly;
cocos2d::CCMenuItemSprite *mSP;
cocos2d::Sprite *mFlyBg;
cocos2d::Sprite *mSPBg;
cocos2d::CCLabelBMFont *mFlyNumber;
cocos2d::CCLabelBMFont *mSPNumber;
    //----- sub node ---------
cocos2d::Sprite *mHits;
cocos2d::CCLabelBMFont *mHitCount;
cocos2d::Sprite *mSpFg;
cocos2d::Sprite *mSpFgTop;

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
