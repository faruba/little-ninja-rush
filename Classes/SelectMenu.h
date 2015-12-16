#ifndef _SELECTMENU_H_
#define _SELECTMENU_H_

#include "GameTool.h"
#include "GameConstants.h"
#include "cocos2d.h"
#include "cocos-ext.h"
#include "GTAnimatedSprite.h"
#include "cocosbuilder/CocosBuilder.h"
;

class SelectMenu :
  public cocos2d::CCLayer, 
  public cocosbuilder::CCBMemberVariableAssigner,
  public cocosbuilder::CCBSelectorResolver
{
public:
CCB_STATIC_NEW_AUTORELEASE_OBJECT_WITH_INIT_METHOD(SelectMenu, create);
  virtual bool onAssignCCBMemberVariable(cocos2d::Ref*, const char*, cocos2d::CCNode*);
  virtual SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::Ref *, const char*);
  virtual cocos2d::extension::Control::Handler onResolveCCBCCControlSelector(cocos2d::Ref * , const char* );

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
  void setSceneOutro(cocos2d::CCScene* newscene);
  void doneOutro();
private:
  GTAnimatedSprite *mPreview;
  GTAnimatedSprite *mStart;
  float mStartTimer;
  cocos2d::Point mShadowDir;

  float mSlideAngle;
  float mSlideBegin;
  float mSlideBeginX;
cocos2d::CCSprite *mProtraits[GAME_CHARCOUNT];
  float   mCurrAngle;
  int mTargetRole;
  int mCurrRole;
  bool mSliding;
  float mSlideTimer;

  //--- auto assign ---
cocos2d::CCSprite *mShadow;
cocos2d::CCNode *mHero;
cocos2d::CCSprite *mDesc;
cocos2d::CCNode *mHearts;
cocos2d::CCNode *mDarts;
cocos2d::CCMenuItemImage *mBtnStart;
cocos2d::CCMenuItemImage *mBtnDart;
cocos2d::CCMenuItemImage *mBtnBlade;
cocos2d::CCMenuItemImage *mBtnSpecial;
cocos2d::CCSprite *mEquipDart;
cocos2d::CCSprite *mEquipBlade;
cocos2d::CCSprite *mEquipSpecial;
cocos2d::CCNode *mStartPos;
cocos2d::CCSprite *mLeftMark;
cocos2d::CCSprite *mRightMark;
cocos2d::CCLabelBMFont *mMoney;
cocos2d::CCSprite *mCoinMark;
cocos2d::CCMenuItemImage *mClickMe;
cocos2d::CCSprite *mHeroDesc;
cocos2d::CCSprite *mTitle;
cocos2d::CCSprite *mParam;
cocos2d::CCSprite *mBouns;

  //intro&outro
cocos2d::CCNode *mSceneIntro;
cocos2d::CCScene *mNewScene;
  bool mIntroFlag;

  bool mButtoned;
};

class SelectMenuLayerLoader : public cocosbuilder::NodeLoader {
  public:
CCB_STATIC_NEW_AUTORELEASE_OBJECT_METHOD(SelectMenuLayerLoader, loader);

  protected:
CCB_VIRTUAL_NEW_AUTORELEASE_CREATECCNODE_METHOD(SelectMenu);
};
#endif
