#ifndef ___CLASSES_TIPSMENU_H_
#define ___CLASSES_TIPSMENU_H_
#include "GameTool.h"
#include "cocosbuilder/CocosBuilder.h"
class TipsMenu:
  public cocos2d::Layer, 
  public cocosbuilder::CCBMemberVariableAssigner,
  public cocosbuilder::CCBSelectorResolver
{
  public:
    virtual bool init();

    CCB_STATIC_NEW_AUTORELEASE_OBJECT_WITH_INIT_METHOD(TipsMenu, create);

    bool onAssignCCBMemberVariable(cocos2d::Ref*, const char*, cocos2d::Node*);
    SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::Ref *, const char*);
    cocos2d::extension::Control::Handler onResolveCCBCCControlSelector(cocos2d::Ref * , const char* );

    virtual void onEnter();
    //virtual void update(float);
    virtual void onExit();

  private:
    void updateTip(int index);
    cocos2d::Node *mNode;
    cocos2d::Label *mCount;
    cocos2d::Label *mTip;
    int mIndex;
    cocos2d::MenuItemImage *mLeft;
    cocos2d::MenuItemImage *mRight;

    void onBack(cocos2d::Ref*);
    void onLeft(cocos2d::Ref*);
    void onRight(cocos2d::Ref*);

  private:
    UISwapper mUISwapper;
};
#endif
