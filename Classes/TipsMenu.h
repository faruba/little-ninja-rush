#ifndef ___CLASSES_TIPSMENU_H_
#define ___CLASSES_TIPSMENU_H_
#include "GameTool.h"
#include "cocosbuilder/CocosBuilder.h"
class TipsMenu:
  public cocos2d::CCLayer, 
  public cocosbuilder::CCBMemberVariableAssigner,
  public cocosbuilder::CCBSelectorResolver
{
  public:
    virtual bool init();  

    LNR_SCENE_METHOD(TipsMenu);
CCB_STATIC_NEW_AUTORELEASE_OBJECT_WITH_INIT_METHOD(TipsMenu, create);
    
    bool onAssignCCBMemberVariable(cocos2d::Ref*, const char*, cocos2d::CCNode*);
    SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::Ref *, const char*);
    cocos2d::extension::Control::Handler onResolveCCBCCControlSelector(cocos2d::Ref * , const char* );

    virtual void onEnter();
    //virtual void update(float);
    virtual void onExit();

private:
    void updateTip(int index);
    void setSceneIntro();
    void setSceneOutro(cocos2d::CCScene* newscene);
    void doneOutro();
cocos2d::CCNode *mNode;
cocos2d::CCLabelTTF *mCount;
cocos2d::CCLabelTTF *mTip;
    int mIndex;
cocos2d::CCMenuItemImage *mLeft;
cocos2d::CCMenuItemImage *mRight;

    //intro&outro
cocos2d::CCNode *mSceneIntro;
cocos2d::CCScene *mNewScene;
    bool mIntroFlag;


    void onBack();
    void onLeft();
    void onRight();

};
#endif
