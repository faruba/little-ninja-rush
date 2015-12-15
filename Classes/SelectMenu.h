#ifndef _SELECTMENU_H_
#define _SELECTMENU_H_

#include "GameTool.h"
#include "GameConstants.h"
#include "cocos2d.h"
#include "cocos-ext.h"
#include "GTAnimatedSprite.h"

USING_NS_CC;

class SelectMenu :
  public cocos2d::CCLayer, 
  public cocos2d::extension::CCBMemberVariableAssigner,
  public cocos2d::extension::CCBSelectorResolver
{
public:
  CCB_STATIC_NEW_AUTORELEASE_OBJECT_WITH_INIT_METHOD(SelectMenu, create);
  virtual bool onAssignCCBMemberVariable(cocos2d::CCObject*, const char*, cocos2d::CCNode*);
  virtual SEL_MenuHandler onResolveCCBCCMenuItemSelector(CCObject *, const char*);
  virtual SEL_CCControlHandler onResolveCCBCCControlSelector(CCObject *, const char*);

  virtual bool onTouchBegan(Touch * touch, Event * event);
  virtual void onTouchMoved(Touch * touch, Event * event);
  virtual void onTouchEnded(Touch * touch, Event * event);

  LNR_SCENE_METHOD(SelectMenu);
  virtual bool init();
  virtual void onEnter();
  virtual void update(float delta);

  void onStart();
  void onChangeDart();
  void onChangeBlade();
  void onChangeSpecial();
  void onBack();
  void onLeftRole();
  void onRightRole();
  void onClickMe();

  void updateAngle();
  void updateCharacterInfo(int rid);

  //intro&outro
  void setSceneIntro();
  void setSceneOutro(CCScene* newscene);
  void doneOutro();
private:
  GTAnimatedSprite *mPreview;
  GTAnimatedSprite *mStart;
  float mStartTimer;
  CCPoint mShadowDir;

  float mSlideAngle;
  float mSlideBegin;
  float mSlideBeginX;
  CCSprite *mProtraits[GAME_CHARCOUNT];
  float   mCurrAngle;
  int mTargetRole;
  int mCurrRole;
  bool mSliding;
  float mSlideTimer;

  //--- auto assign ---
  CCSprite *mShadow;
  CCNode *mHero;
  CCSprite *mDesc;
  CCNode *mHearts;
  CCNode *mDarts;
  CCMenuItemImage *mBtnStart;
  CCMenuItemImage *mBtnDart;
  CCMenuItemImage *mBtnBlade;
  CCMenuItemImage *mBtnSpecial;
  CCSprite *mEquipDart;
  CCSprite *mEquipBlade;
  CCSprite *mEquipSpecial;
  CCNode *mStartPos;
  CCSprite *mLeftMark;
  CCSprite *mRightMark;
  CCLabelBMFont *mMoney;
  CCSprite *mCoinMark;
  CCMenuItemImage *mClickMe;
  CCSprite *mHeroDesc;
  CCSprite *mTitle;
  CCSprite *mParam;
  CCSprite *mBouns;

  //intro&outro
  CCNode *mSceneIntro;
  CCScene *mNewScene;
  bool mIntroFlag;

  bool mButtoned;
};

class SelectMenuLayerLoader : public cocos2d::extension::CCLayerLoader {
  public:
    CCB_STATIC_NEW_AUTORELEASE_OBJECT_METHOD(SelectMenuLayerLoader, loader);

  protected:
    CCB_VIRTUAL_NEW_AUTORELEASE_CREATECCNODE_METHOD(SelectMenu);
};
#endif
