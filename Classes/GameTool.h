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
USING_NS_CC;
using namespace CocosDenshion;

#define MAX2(a,b) ((a)>(b))?(b):(a)

#ifdef LINUX
typedef float CFAbsoluteTime;
typedef float CFTimeInterval;
#define NSNotFound UINT_MAX
CFAbsoluteTime CFAbsoluteTimeGetCurrent();
#endif

#define LNR_SCENE_METHOD(T) static CCScene* scene() { \
    CCScene *ret = CCScene::create(); \
    T *tm = T::create(); \
    ret->setScale(UniversalFit::sharedUniversalFit()->scaleFactor); \
    ret->setAnchorPoint(ccp(0, 0)); \
    ret->setPosition(UniversalFit::sharedUniversalFit()->sceneOffset); \
    CEClipedNode *clip = CEClipedNode::create(); \
    clip->setClipRect(&(UniversalFit::sharedUniversalFit()->clipRect)); \
    clip->addChild(tm); \
    ret->addChild(clip); \
    return ret; \
}

#define LNR_GET_TOUCH_POS \
    CCPoint pos = touch->getLocationInView(); \
    pos = CCDirector::sharedDirector()->convertToGL(pos); \
    pos = UniversalFit::sharedUniversalFit()->restorePoint(pos);

class Scene
{
  public:
   virtual void loadScene() = 0;
   virtual void unloadScene() = 0;

   virtual void createScene(CCNode* bg, CCNode* fbg, CCNode* fg, RingTile* tiles) = 0;
   virtual void update(float delta) = 0;
   virtual void release() = 0;
};

void initRandom(int seed);

int randomInt(int max);

bool exCollisionWithCircles(CCPoint op, float ox, float oy, float r, CCPoint p, float pr);

int gtReadInt(CCDictionary *dic, const char *key, int def=0);

float gtReadFloat(CCDictionary *dic, const char *key, float def=0.0f);

double gtReadDouble(CCDictionary *dic, const char *key, double def=0.0f);

CCString* gtReadString(CCDictionary *dic, const char *key, CCString *def=NULL);

void unloadTextureFromeSpriteFrameFile(const char *plist);

//UIImage* makeScreenshot();
CCSequence *createScaleSequence(float fDuration[], float fScale[], int count);
CCNode *createUIByCCBI(const char* szCCBI, const char *pClassName, cocosbuilder::NodeLoader *pCCNodeLoader, CCObject *target);
void doSceneIntro(CCNode *&mSceneIntro, CCNode *target);
CCScene* doSceneOutro(CCScene* mNewScene, CCNode *&mSceneIntro, SEL_CallFunc callBack, CCNode *target);

#endif
