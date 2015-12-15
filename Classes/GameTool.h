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

USING_NS_CC_EXT;
;
using namespace CocosDenshion;

#define MAX2(a,b) ((a)>(b))?(b):(a)

#ifdef LINUX
typedef float CFAbsoluteTime;
typedef float CFTimeInterval;
#define NSNotFound UINT_MAX
CFAbsoluteTime CFAbsoluteTimeGetCurrent();
#endif

#define LNR_SCENE_METHOD(T) static CCScene* scene() { \
cocos2d::CCScene *ret = cocos2d::CCScene::create(); \
    T *tm = T::create(); \
    ret->setScale(UniversalFit::sharedUniversalFit()->scaleFactor); \
    ret->setAnchorPoint(cocos2d::ccp(0, 0)); \
    ret->setPosition(UniversalFit::sharedUniversalFit()->sceneOffset); \
    CEClipedNode *clip = CEClipedNode::create(); \
    clip->setClipRect(&(UniversalFit::sharedUniversalFit()->clipRect)); \
    clip->addChild(tm); \
    ret->addChild(clip); \
    return ret; \
}

#define LNR_GET_TOUCH_POS \
    cocos2d::Point pos = touch->getLocationInView(); \
    pos = cocos2d::CCDirector::sharedDirector()->convertToGL(pos); \
    pos = UniversalFit::sharedUniversalFit()->restorePoint(pos);

class Scene
{
  public:
   virtual void loadScene() = 0;
   virtual void unloadScene() = 0;

   virtual void createScene(cocos2d::CCNode* bg, CCNode* fbg, CCNode* fg, RingTile* tiles) = 0;
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
cocos2d::CCNode *createUIByCCBI(const char* szCCBI, const char *pClassName, cocosbuilder::NodeLoader *pCCNodeLoader, CCObject *target);
void doSceneIntro(cocos2d::CCNode *&mSceneIntro, CCNode *target);
cocos2d::CCScene* doSceneOutro(cocos2d::CCScene* mNewScene, CCNode *&mSceneIntro, SEL_CallFunc callBack, CCNode *target);

#endif
