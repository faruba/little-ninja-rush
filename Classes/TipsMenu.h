#ifndef ___CLASSES_TIPSMENU_H_
#define ___CLASSES_TIPSMENU_H_
#include "GameTool.h"

class TipsMenu:
  public cocos2d::CCLayer, 
  public cocos2d::extension::CCBMemberVariableAssigner,
  public cocos2d::extension::CCBSelectorResolver
{
  public:
    virtual bool init();  

    LNR_SCENE_METHOD(TipsMenu);

    CCB_STATIC_NEW_AUTORELEASE_OBJECT_WITH_INIT_METHOD(TipsMenu, create);
    
    bool onAssignCCBMemberVariable(cocos2d::CCObject*, const char*, cocos2d::CCNode*);
    SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject *, const char*);
    SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject *, const char*);

    virtual void onEnter();
    //virtual void update(float);
    virtual void onExit();

private:
    void updateTip(int index);
    void setSceneIntro();
    void setSceneOutro(cocos2d::CCScene* newscene);
    void doneOutro();
    
    CCNode *mNode;
    CCLabelTTF *mCount;
    CCLabelTTF *mTip;
    int mIndex;

    CCMenuItemImage *mLeft;
    CCMenuItemImage *mRight;

    //intro&outro
    CCNode *mSceneIntro;
    CCScene *mNewScene;
    bool mIntroFlag;


    void onBack();
    void onLeft();
    void onRight();

};
#endif
