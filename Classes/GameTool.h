#ifndef ___CLASSES_GAMETOOL_H_
#define ___CLASSES_GAMETOOL_H_
#include "RingTile.h"
#include "GTAnimatedSprite.h"
#include "GTGhostShadow.h"
#include "GTAnimation.h"
#include "GTAnimatedEffect.h"
#include "GameObject.h"
#include "GameObjectManager.h"

#include "UniversalFit.h"
#include "CEClipedNode.h"
#include "SimpleAudioEngine.h"
#include <time.h>
#include <cocos2d.h>
#include "cocos-ext.h"
#include <time.h>


class GameTool {
  public:
    static int PlaySound(const char* path) {
      return CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(path);
    }

    static void StopSound(const int id) {
      CocosDenshion::SimpleAudioEngine::getInstance()->stopEffect(id);
    }

    static void PlayBackgroundMusic(const char* path) {
      CocosDenshion::SimpleAudioEngine *instance = CocosDenshion::SimpleAudioEngine::getInstance();
      if ( !instance->isBackgroundMusicPlaying() ) {
        instance->playBackgroundMusic(path, true);
      }
    }

    static void StopBackgroundMusic() {
      CocosDenshion::SimpleAudioEngine::getInstance()->stopBackgroundMusic();
    }

    template<typename LayerType>
      static cocos2d::Scene* scene() { 
        cocos2d::Scene *ret = cocos2d::Scene::create();
        LayerType *tm = LayerType::create();
        //ret->setScale(UniversalFit::sharedUniversalFit()->scaleFactor);
        ret->setAnchorPoint(cocos2d::Vec2(0, 0));
        //ret->setPosition(UniversalFit::sharedUniversalFit()->sceneOffset);
        //CEClipedNode *clip = CEClipedNode::create();
        //clip->setClipRect(&(UniversalFit::sharedUniversalFit()->clipRect));
        //clip->addChild(tm);
        //ret->addChild(clip);
        ret->addChild(tm);
        return ret;

      }
};

class UISwapper {
  public:
    void onEnter() {
      mIntroFlag = false;
      mDoornode = NULL;
    }

    bool isDone() {
      return !mIntroFlag;
    }

    void setSceneIntro(cocos2d::Node* target) {
      doSceneIntro(target);
    }

    void setSceneOutro(cocos2d::Scene* newscene, cocos2d::Node* target) {
      if (mIntroFlag) {
        return;
      }

      mIntroFlag = true;

      mNewScene = newscene;
      mNewScene->retain();
      doSceneOutro(target);
    }

    void doneOutro() {
      mIntroFlag = false;
      cocos2d::Director::getInstance()->replaceScene(mNewScene);
      mNewScene->release();
    }

  private:
    cocos2d::Node *mDoornode;
    cocos2d::Scene *mNewScene;
    cocos2d::Sprite *mLeftDoor;
    cocos2d::Sprite *mRightDoor;

    bool mIntroFlag;

    void initiateDoors(cocos2d::Node* target);
    void doSceneOutro(cocos2d::Node *target);
    void doSceneIntro(cocos2d::Node *target);
};




USING_NS_CC_EXT;
using namespace CocosDenshion;

#define MAX2(a,b) ((a)>(b))?(b):(a)

#ifdef LINUX
typedef float CFAbsoluteTime;
typedef float CFTimeInterval;
#define NSNotFound UINT_MAX
CFAbsoluteTime CFAbsoluteTimeGetCurrent();
#endif

#define LNR_GET_TOUCH_POS \
    cocos2d::Point pos = touch->getLocationInView(); \
    pos = cocos2d::CCDirector::sharedDirector()->convertToGL(pos); \
    pos = UniversalFit::sharedUniversalFit()->restorePoint(pos);

class Scene
{
  public:
   virtual void loadScene() = 0;
   virtual void unloadScene() = 0;

   virtual void createScene(cocos2d::Node* bg, Node* fbg, Node* fg, RingTile* tiles) = 0;
   virtual void update(float delta) = 0;
   virtual void release() = 0;
};

void initRandom(int seed);

int randomInt(int max);

bool exCollisionWithCircles(cocos2d::Point op, float ox, float oy, float r, cocos2d::Point p, float pr);

int gtReadInt(cocos2d::CCDictionary *dic, const char *key, int def=0);

float gtReadFloat(cocos2d::CCDictionary *dic, const char *key, float def=0.0f);

double gtReadDouble(cocos2d::CCDictionary *dic, const char *key, double def=0.0f);

cocos2d::CCString* gtReadString(cocos2d::CCDictionary *dic, const char *key, CCString *def=NULL);

void unloadTextureFromeSpriteFrameFile(const char *plist);

//UIImage* makeScreenshot();
cocos2d::CCSequence *createScaleSequence(float fDuration[], float fScale[], int count);
cocos2d::Node *createUIByCCBI(const char* szCCBI, const char *pClassName, cocosbuilder::NodeLoader *pNodeLoader, Ref *target);


#endif
