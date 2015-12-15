#ifndef ___CLASSES_TITLEMENU_H_
#define ___CLASSES_TITLEMENU_H_

#include "cocos2d.h"
#include "cocos-ext.h"
#include "GameTool.h"
#include "cocosbuilder/CocosBuilder.h"

class GTAnimatedSprite;
class PopObj;
class PopOption;

#define RUN (3)

;
USING_NS_CC_EXT;

class TitleMenu : 
  public cocos2d::CCLayer, 
  public cocosbuilder::CCBMemberVariableAssigner,
  public cocosbuilder::CCBSelectorResolver
{
public:
  virtual bool init();  

  LNR_SCENE_METHOD(TitleMenu);
CCB_STATIC_NEW_AUTORELEASE_OBJECT_WITH_INIT_METHOD(TitleMenu, create);
  virtual bool onAssignCCBMemberVariable(cocos2d::CCObject*, const char*, cocos2d::CCNode*);
  virtual SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject *, const char*);
  virtual cocos2d::extension::Control::Handler onResolveCCBCCControlSelector(cocos2d::Ref * , const char* );

  virtual void onEnter();
  virtual void update(float);
  virtual void onExit();

//hammer android:不应该是private
//private:
  void onPlayClassic();
  void onPlayArcade();
  void onCollections();
  void onStore();
  void onFacebook();
  void onTwitter();
  void onLeaderboard();
  void onAchievement();
  void onShareCode();
  void onExtra();
  void onShowObjectives();
  void onOption();
  void onMoreFun();
  void showObjs();
  void hideObjs();
  void showOpt(int typ);
  void hideOpt();
    
    //android back key support
    virtual void onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event);
    
  // intro&outro
  void setSceneIntro();
  void setSceneOutro(cocos2d::CCScene* newscene);
  void doneOutro();

private:
cocos2d::CCSprite *mStar1;
cocos2d::CCSprite *mStar2;
cocos2d::CCSprite *mStar3;
cocos2d::CCSprite *mStar4;
cocos2d::CCSprite *mStar5;
cocos2d::CCSprite *mCloud1;
cocos2d::CCSprite *mCloud2;
cocos2d::CCMenu *mMainButton;
cocos2d::CCMenu *mMiniButton;
cocos2d::CCSprite *mLogo;
cocos2d::CCNode *mSakuraNode;
cocos2d::CCNode *mSakuraNode2;
cocos2d::CCNode *mEnemies;
cocos2d::CCLayerColor *mMask;
cocos2d::CCArray *mSakura;
  float mWind;
  float mWindx;
cocos2d::CCSprite *mNew;
cocos2d::CCSprite *mCNew;
cocos2d::CCMenuItemImage *mTwitter;
cocos2d::CCMenuItemImage *mOption;

  GTAnimatedSprite *mAni[RUN];
  float mSpeed[RUN];

  float mStarTimers[5];

  PopObj *mPop;
  PopOption *mOpt;

  //intro&outro
cocos2d::CCNode *mSceneIntro;
cocos2d::CCScene *mNewScene;
  bool mIntroFlag;
};

class TitleMenuLayerLoader : public cocosbuilder::NodeLoader {
  public:
    CCB_STATIC_NEW_AUTORELEASE_OBJECT_METHOD(TitleMenuLayerLoader, loader);

  protected:
    CCB_VIRTUAL_NEW_AUTORELEASE_CREATECCNODE_METHOD(TitleMenu);
};

class Sakura : public CCObject
{
public:
  static Sakura* create(cocos2d::CCNode *parent);

  virtual bool init();  

  bool Update(float, float);
cocos2d::CCNode *mParent;
cocos2d::CCSprite *mSprite;
  //float mFlipX;
  float mWindX;
  float mSpeed;
  float mSpeedY;
};

#endif
