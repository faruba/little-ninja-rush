#ifndef ___CLASSES_GAMESCENE_H_
#define ___CLASSES_GAMESCENE_H_
#include "GameTool.h"
#include "Scene.h"
#define SCENE_COUNT (7)

class GameScene:public GameObject{
  public:
    virtual bool init(){return true;};
    CREATE_FUNC(GameScene);
    virtual void onCreate();
    virtual void onUpdate(float delta);
    virtual void onDestroy();
    static GameScene* scene(cocos2d::Node* parent);
    static void initSceneSequence();
    static int  getCurrentSceneId();
    static void nextScene();
cocos2d::Node* mParent;
cocos2d::Node* mSceneBgNode;
cocos2d::Node *mSceneFbNode;
cocos2d::Node* mSceneFgNode;
    RingTile* mTiles;
    int     mCurrScene;
    SceneInterface* mScene;

    //int mStepSound;


    void setScene(int sid);
    int currentScene();

    float offset();


};
#endif
