#include "GameTool.h"
#include "GameConstants.h"
#include "SimpleAudioEngine.h"
#include "CCNumber.h"

USING_NS_CC;

using namespace CocosDenshion;

void initRandom(int seed)
{
  if(seed <= 0) {
    srand((unsigned int)time(NULL));
  }
  else {
    srand(seed);
  }
}

int randomInt(int max)
{
  return rand()%max;
}

bool exCollisionWithCircles(CCPoint op, float ox, float oy, float r, CCPoint p, float pr)
{
  CCPoint cc = ccp(op.x+ox, op.y+oy);
  float disq = ccpLengthSQ(ccpSub(p, cc));
  if( disq < (r+pr)*(r+pr) ) {
    return true;
  }
  else {
    return false;
  }
}

int gtReadInt(CCDictionary *dic, const char *key, int def)
{
  CCObject *obj = dic->objectForKey(key);
  if( obj != NULL ) {
    return ((CCNumber*)obj)->getInt();
  }
  else {
    return def;
  }
}

float gtReadFloat(CCDictionary *dic, const char *key, float def)
{
  CCObject* obj = dic->objectForKey(key);
  if( obj != NULL )
  {
    return ((CCNumber*)obj)->getFloat();
  }
  else {
    return def;
  }
}

double gtReadDouble(CCDictionary *dic, const char *key, double def)
{
    CCObject* obj = dic->objectForKey(key);
    if( obj != NULL )
    {
        return ((CCNumber*)obj)->getFloat();
    }
    else {
        return def;
    }
}

CCString* gtReadString(CCDictionary *dic, const char *key, CCString *def)
{
    CCObject* obj = dic->objectForKey(key);
    if( obj != NULL )
    {
        return (CCString*)obj;
    }
    else{
        return def;
    }
}

void unloadTextureFromeSpriteFrameFile(const char *plist)
{
    const char *path = CCFileUtils::sharedFileUtils()->fullPathForFilename(plist).c_str();
  //const char *path = CCFileUtils::sharedFileUtils()->fullPathFromRelativePath(plist);
  CCDictionary *dict = CCDictionary::createWithContentsOfFile(path);
  CCDictionary *meta = (CCDictionary*)dict->objectForKey("metadata");
  CCString *texname = (CCString*)meta->objectForKey("textureFileName");
  CCTextureCache::sharedTextureCache()->removeTextureForKey(texname->getCString());
}

//UIImage* makeScreenshot()
//{
//    int tx = CCDirector->sharedDirector().winSize.width;
//    int ty = CCDirector->sharedDirector().winSize.height;
//    CCRenderTexture *renderer	= CCRenderTexture->renderTextureWithWidth(tx, ty);
//    renderer->begin();
//    CCDirector->sharedDirector().runningScene->visit();
//    renderer->end();
//    
//    //return renderer->getUIImageFromBuffer();
//    return renderer->getUIImage();//iphone 5
//}

CCSequence *createScaleSequence(float fDuration[], float fScale[], int count)
{
  CCArray *pArr = CCArray::create();
  for (int i = 0; i < count; i++) {
    pArr->addObject(CCScaleTo::create(fDuration[i],fScale[i]));

  }
  return (CCSequence*)CCSequence::create(pArr);
}
CCNode *createUIByCCBI(const char* szCCBI, const char *pClassName, CCNodeLoader *pCCNodeLoader, CCObject *target)
{
  /* Create an autorelease CCNodeLoaderLibrary. */
  CCNodeLoaderLibrary * ccnll = CCNodeLoaderLibrary::newDefaultCCNodeLoaderLibrary();
  ccnll->registerCCNodeLoader(pClassName, pCCNodeLoader);
  /* Create an autorelease CCBReader. */
  cocos2d::extension::CCBReader * ccbReader = new cocos2d::extension::CCBReader(ccnll);

  /* Read a ccbi file. */
  CCNode * node = ccbReader->readNodeGraphFromFile(szCCBI, target);
  ccbReader->release();

  return node;
}

void doSceneIntro(CCNode *&mSceneIntro, CCNode *target)
{
  CCSprite *left, *right; 
  if( mSceneIntro == NULL )
  {
    mSceneIntro = CCNode::create();
    target->addChild(mSceneIntro, 99);
    left = CCSprite::create("door.png");
    left->setAnchorPoint(ccp(1, 0));
    left->setPosition(ccp(0, 0));
    mSceneIntro->addChild(left, 0, 0);
    right = CCSprite::create("door.png");
    right->setScaleX(-1);
    right->setAnchorPoint(ccp(1, 0));
    right->setPosition(ccp(SCREEN_WIDTH, 0));
    mSceneIntro->addChild(right, 0, 1);
  }
  else
  {
    left = (CCSprite*)mSceneIntro->getChildByTag(0);
    right = (CCSprite*)mSceneIntro->getChildByTag(1);
  }
  left->setVisible(true);
  left->setPosition(ccp(SCREEN_WIDTH/2, 0));
  right->setVisible(true);
  right->setPosition(ccp(SCREEN_WIDTH/2, 0));

  CCDelayTime *dt1 = CCDelayTime::create(SCENEINTRO_DELAY);
  CCMoveBy *mb1 = CCMoveBy::create(SCENEINTRO_TIME,ccp(-SCREEN_WIDTH/2, 0));
  CCSequence *sq1 = (CCSequence*)CCSequence::create(dt1, mb1, NULL);
  left->runAction(sq1);
  CCDelayTime *dt2 = CCDelayTime::create(SCENEINTRO_DELAY);
  CCMoveBy *mb2 = CCMoveBy::create(SCENEINTRO_TIME,ccp(SCREEN_WIDTH/2, 0));
  CCSequence *sq2 = (CCSequence*)CCSequence::create(dt2, mb2, NULL);
  right->runAction(sq2);

  SimpleAudioEngine::sharedEngine()->playEffect(CCFileUtils::sharedFileUtils()->fullPathForFilename("sound/open.mp3").c_str());
}
CCScene* doSceneOutro(CCScene* mNewScene, CCNode *&mSceneIntro, SEL_CallFunc callBack, CCNode *target)
{
  if( mSceneIntro == NULL )
  {
    mSceneIntro = CCNode::create();
    target->addChild(mSceneIntro, 99);
    CCSprite *left = CCSprite::create("door.png");
    left->setAnchorPoint(ccp(1, 0));
    mSceneIntro->addChild(left, 0, 0);
    CCSprite *right = CCSprite::create("door.png");
    right->setScaleX(-1);
    right->setAnchorPoint(ccp(1, 0));
    mSceneIntro->addChild(right, 0, 1);
  }
  CCSprite *left = (CCSprite*)mSceneIntro->getChildByTag(0);
  left->setVisible(true);
  left->setPosition(ccp(0, 0));
  CCSprite *right = (CCSprite*)mSceneIntro->getChildByTag(1);
  right->setVisible(true);
  right->setPosition(ccp(SCREEN_WIDTH, 0));

  CCMoveBy *mb1 = CCMoveBy::create(SCENEOUTRO_TIME, ccp(SCREEN_WIDTH/2, 0));
  CCDelayTime *dt1 = CCDelayTime::create(SCENEOUTRO_DELAY);
  CCCallFunc *ca1 = CCCallFunc::create(target, callBack);
  CCSequence *sq1 = (CCSequence*)CCSequence::create(mb1, dt1, ca1, NULL);
  left->runAction(sq1);

  CCMoveBy *mb2 = CCMoveBy::create(SCENEOUTRO_TIME, ccp(-SCREEN_WIDTH/2, 0));
  CCDelayTime *dt2 = CCDelayTime::create(SCENEOUTRO_DELAY);
  CCSequence *sq2 = (CCSequence*)CCSequence::create(mb2, dt2, NULL);
  right->runAction(sq2);

  SimpleAudioEngine::sharedEngine()->playEffect(CCFileUtils::sharedFileUtils()->fullPathForFilename("sound/close.mp3").c_str());

  mNewScene->retain();
  return mNewScene;
}
#ifdef LINUX

//CFAbsoluteTime CFAbsoluteTimeGetCurrent()
//{
//  timespec tm;
//  //clock_gettime(CLOCK_REALTIME, &tm);//hammer marked
//  return tm.tv_nsec/10^9;
//}
#endif
