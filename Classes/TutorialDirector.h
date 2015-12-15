#ifndef ___CLASSES_TUTORIALDIRECTOR_H_
#define ___CLASSES_TUTORIALDIRECTOR_H_
#include "GameTool.h"
#include "TutorialPuppet.h"

// 1. slide up to fire
// 2. tilt to move
// 3. slide right to slash
// 4. get coins and powerful items
// 5. slide down to release special skill

class TutorialDirector:
  public GameObject
{
  public:
    static TutorialDirector* tutorial();

    CREATE_FUNC(TutorialDirector);

    virtual void onCreate();
    virtual void onUpdate(float delta);
    virtual void onDestroy();

    void onSkip();
    void playSlideUp();
    void stopSlide();
    void playSlideForward();
    void playSlideDown();
    void playShake();
    void stopShake();
    void tutorialOver();

    int mState;
    TutorialPuppet *mDummyEnemy;
    float mTimer;
    float mPosx;
    int mCount;
    TutorialPuppet *mDummies[3];
cocos2d::CCLabelTTF *mTeachText;

    int cacheDartId;
    int cacheBladeId;
    int cacheSpellId;
cocos2d::CCLayerColor *mLayer;
cocos2d::CCNode *mAnimateNode;
    GTAnimatedSprite *mAnimateSprite;
cocos2d::CCMenu *mMenu;


    void begin();
    void doneMove();
    void doneKilled();
    void doneCleanup();
    void doneMove2();
    void doneFire();
    void waitBlade();
    void doneBlade();
    void doneEvade();
    void doneKilled2();
    void doneCleanup2();
    void doneCharge();
    void doneTutorial();

    void skip();

};
#endif
