#ifndef ___CLASSES_EXTRASELECT_H_
#define ___CLASSES_EXTRASELECT_H_

#include "GameTool.h"
#include "cocosbuilder/CocosBuilder.h"
class ExtraSelect:
  public cocos2d::Layer, 
  public cocosbuilder::CCBMemberVariableAssigner,
  public cocosbuilder::CCBSelectorResolver
{
  public:
    virtual bool init();  

    LNR_SCENE_METHOD(ExtraSelect);
CCB_STATIC_NEW_AUTORELEASE_OBJECT_WITH_INIT_METHOD(ExtraSelect, create);
    virtual bool onAssignCCBMemberVariable(cocos2d::Ref*, const char*, cocos2d::CCNode*);
    virtual SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::Ref *, const char*);
    virtual cocos2d::extension::Control::Handler onResolveCCBCCControlSelector(cocos2d::Ref * , const char* );

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
cocos2d::Sprite* checkButton(cocos2d::Point pos);
cocos2d::CCNode *mNode;
cocos2d::Sprite *mSelect;
    //--autoassign--
cocos2d::Sprite *mAchievement;
cocos2d::Sprite *mStatistics;
cocos2d::Sprite *mTips;
cocos2d::Sprite *mCredits;
//    Sprite *mFacebook;
//    Sprite *mTwitter;
//    Sprite *mTwitterBanner;

    //intro&outro
cocos2d::CCNode *mSceneIntro;
cocos2d::Scene *mNewScene;
    bool mIntroFlag;
    // intro&outro
    void setSceneIntro();
    void setSceneOutro(cocos2d::Scene* newscene);
    void doneOutro();

    void onBack();

};

class ExtraSelectLayerLoader : public cocosbuilder::NodeLoader {
  public:
CCB_STATIC_NEW_AUTORELEASE_OBJECT_METHOD(ExtraSelectLayerLoader, loader);

  protected:
CCB_VIRTUAL_NEW_AUTORELEASE_CREATECCNODE_METHOD(ExtraSelect);
};
#endif
