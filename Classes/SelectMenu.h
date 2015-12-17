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
  public cocos2d::Layer, 
  public cocosbuilder::CCBMemberVariableAssigner,
  public cocosbuilder::CCBSelectorResolver
{
public:
CCB_STATIC_NEW_AUTORELEASE_OBJECT_WITH_INIT_METHOD(SelectMenu, create);
  virtual bool onAssignCCBMemberVariable(cocos2d::Ref*, const char*, cocos2d::Node*);
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
  void setSceneOutro(cocos2d::Scene* newscene);
  void doneOutro();
private:
  GTAnimatedSprite *mPreview;
  GTAnimatedSprite *mStart;
  float mStartTimer;
  cocos2d::Point mShadowDir;

  float mSlideAngle;
  float mSlideBegin;
  float mSlideBeginX;
cocos2d::Sprite *mProtraits[GAME_CHARCOUNT];
  float   mCurrAngle;
  int mTargetRole;
  int mCurrRole;
  bool mSliding;
  float mSlideTimer;

  //--- auto assign ---
cocos2d::Sprite *mShadow;
cocos2d::Node *mHero;
cocos2d::Sprite *mDesc;
cocos2d::Node *mHearts;
cocos2d::Node *mDarts;
cocos2d::CCMenuItemImage *mBtnStart;
cocos2d::CCMenuItemImage *mBtnDart;
cocos2d::CCMenuItemImage *mBtnBlade;
cocos2d::CCMenuItemImage *mBtnSpecial;
cocos2d::Sprite *mEquipDart;
cocos2d::Sprite *mEquipBlade;
cocos2d::Sprite *mEquipSpecial;
cocos2d::Node *mStartPos;
cocos2d::Sprite *mLeftMark;
cocos2d::Sprite *mRightMark;
cocos2d::CCLabelBMFont *mMoney;
cocos2d::Sprite *mCoinMark;
cocos2d::CCMenuItemImage *mClickMe;
cocos2d::Sprite *mHeroDesc;
cocos2d::Sprite *mTitle;
cocos2d::Sprite *mParam;
cocos2d::Sprite *mBouns;

  //intro&outro
cocos2d::Node *mSceneIntro;
cocos2d::Scene *mNewScene;
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
