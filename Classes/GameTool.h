#ifndef ___CLASSES_GAMETOOL_H_
#define ___CLASSES_GAMETOOL_H_
#include "CEClipedNode.h"

#include "RingTile.h"
#include "GTAnimatedSprite.h"
#include "GTGhostShadow.h"
#include "GTAnimation.h"
#include "GTAnimatedEffect.h"
#include "GameObject.h"
#include "GameObjectManager.h"

#include "UniversalFit.h"

#include "SimpleAudioEngine.h"
#include <time.h>
#include <cocos2d.h>
#include "cocos-ext.h"
#include <time.h>


class GameTool {
  public:
    static int PlaySound(const char* filename) {
      std::string path = "sound/";
      return CocosDenshion::SimpleAudioEngine::getInstance()->playEffect((path+filename).c_str());
    }

    static void StopSound(const int id) {
      CocosDenshion::SimpleAudioEngine::getInstance()->stopEffect(id);
    }

    static void PlayBackgroundMusic(const char* filename) {
      std::string path = "sound/";
      CocosDenshion::SimpleAudioEngine *instance = CocosDenshion::SimpleAudioEngine::getInstance();
      if ( !instance->isBackgroundMusicPlaying() ) {
        instance->playBackgroundMusic((path+filename).c_str(), true);
      }
    }

    static void StopBackgroundMusic() {
      CocosDenshion::SimpleAudioEngine::getInstance()->stopBackgroundMusic();
    }

    static SpriteFrame* getSpriteFrameByName(const std::string&name) { return cocos2d::SpriteFrameCache::getInstance()->getSpriteFrameByName(name); }

    template<typename LayerType>
      static cocos2d::Scene* scene(LayerType** pLayer = NULL) {
        cocos2d::Scene *ret = cocos2d::Scene::create();
        LayerType *tm = LayerType::create();
        if(pLayer != NULL){
          *pLayer = tm;
        }
        ret->setScale(UniversalFit::sharedUniversalFit()->scaleFactor);
        ret->setAnchorPoint(cocos2d::Vec2(0,0));
        //ret->setPosition(UniversalFit::sharedUniversalFit()->sceneOffset);
        tm->setPosition(UniversalFit::sharedUniversalFit()->sceneOffset);
        CEClipedNode *clip = CEClipedNode::create();
        clip->setClipRect(&(UniversalFit::sharedUniversalFit()->clipRect));
        clip->addChild(tm);
        ret->addChild(clip);
        return ret;
      }

    template<typename DataType>
    static void addMemberWithFormatToDocument(rapidjson::Document &document, std::string head, int id, DataType data) {
      const std::string key = head+std::to_string(id);
      rapidjson::Value index(key.c_str(), key.size(), document.GetAllocator());
      document.AddMember(index, data, document.GetAllocator());
    }

    template<typename DataType>
    static void addMemberWithFormatToDocument(rapidjson::Document &document, std::string head, const char id[], DataType data) {
      const std::string key = head+std::to_string(*id);
      rapidjson::Value index(key.c_str(), key.size(), document.GetAllocator());
      document.AddMember(index, data, document.GetAllocator());
    }

    static void addMemberWithFormatToDocument(rapidjson::Document &document, std::string head, int id, std::string data) {
        const std::string key = head+std::to_string(id);
        rapidjson::Value index(key.c_str(), key.size(), document.GetAllocator());
        rapidjson::Value value(data.c_str(), data.size(), document.GetAllocator());
        document.AddMember(index, value, document.GetAllocator());
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
    pos = cocos2d::Director::getInstance()->convertToGL(pos); \
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

int gtReadInt(cocos2d::CCDictionary *dic, std::string key, int def=0);

float gtReadFloat(cocos2d::CCDictionary *dic, const char *key, float def=0.0f);

double gtReadDouble(cocos2d::CCDictionary *dic, const char *key, double def=0.0f);

std::string gtReadString(cocos2d::CCDictionary *dic, const char *key, std::string def);

void unloadTextureFromeSpriteFrameFile(const char *plist);

cocos2d::Sequence *createScaleSequence(float fDuration[], float fScale[], int count);
cocos2d::Node *createUIByCCBI(const char* szCCBI, const char *pClassName, cocosbuilder::NodeLoader *pNodeLoader, Ref *target);

#endif
