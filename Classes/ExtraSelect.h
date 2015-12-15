#ifndef ___CLASSES_EXTRASELECT_H_
#define ___CLASSES_EXTRASELECT_H_

#include "GameTool.h"

class ExtraSelect:
  public cocos2d::CCLayer, 
  public cocos2d::extension::CCBMemberVariableAssigner,
  public cocos2d::extension::CCBSelectorResolver
{
  public:
    virtual bool init();  

    LNR_SCENE_METHOD(ExtraSelect);

    CCB_STATIC_NEW_AUTORELEASE_OBJECT_WITH_INIT_METHOD(ExtraSelect, create);
    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject*, const char*, cocos2d::CCNode*);
    virtual SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject *, const char*);
    virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject *, const char*);

    virtual void onEnter();
    virtual void onExit();

    virtual bool onTouchBegan(Touch * touch, Event * event);
    virtual void onTouchMoved(Touch * touch, Event * event);
    virtual void onTouchEnded(Touch * touch, Event * event);

    void onAchievement();
    void onCredits();
    void onStatics();
    void onTips();
    void onFacebook();
    void onTwitter();
    void resetButtons();

    CCSprite* checkButton(cocos2d::Point pos);



    CCNode *mNode;

    CCSprite *mSelect;
    //--autoassign--
    CCSprite *mAchievement;
    CCSprite *mStatistics;
    CCSprite *mTips;
    CCSprite *mCredits;
//    CCSprite *mFacebook;
//    CCSprite *mTwitter;
//    CCSprite *mTwitterBanner;

    //intro&outro
    CCNode *mSceneIntro;
    CCScene *mNewScene;
    bool mIntroFlag;
    // intro&outro
    void setSceneIntro();
    void setSceneOutro(cocos2d::CCScene* newscene);
    void doneOutro();

    void onBack();

};

class ExtraSelectLayerLoader : public cocos2d::extension::CCLayerLoader {
  public:
    CCB_STATIC_NEW_AUTORELEASE_OBJECT_METHOD(ExtraSelectLayerLoader, loader);

  protected:
    CCB_VIRTUAL_NEW_AUTORELEASE_CREATECCNODE_METHOD(ExtraSelect);
};
#endif
