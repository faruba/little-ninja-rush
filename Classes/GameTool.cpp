#include "GameTool.h"
#include "GameConstants.h"
#include "SimpleAudioEngine.h"
#include "CCNumber.h"

;

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

bool exCollisionWithCircles(cocos2d::Point op, float ox, float oy, float r, cocos2d::Point p, float pr)
{
  cocos2d::Point cc = Vec2(op.x+ox, op.y+oy);
  float disq = ccpLengthSQ(ccpSub(p, cc));
  if( disq < (r+pr)*(r+pr) ) {
    return true;
  }
  else {
    return false;
  }
}

int gtReadInt(cocos2d::CCDictionary *dic, const char *key, int def)
{
cocos2d::Ref *obj = dic->objectForKey(key);
  if( obj != NULL ) {
    return ((CCNumber*)obj)->getInt();
  }
  else {
    return def;
  }
}

float gtReadFloat(cocos2d::CCDictionary *dic, const char *key, float def)
{
cocos2d::Ref* obj = dic->objectForKey(key);
  if( obj != NULL )
  {
    return ((CCNumber*)obj)->getFloat();
  }
  else {
    return def;
  }
}

double gtReadDouble(cocos2d::CCDictionary *dic, const char *key, double def)
{
cocos2d::Ref* obj = dic->objectForKey(key);
    if( obj != NULL )
    {
        return ((CCNumber*)obj)->getFloat();
    }
    else {
        return def;
    }
}

cocos2d::CCString* gtReadString(cocos2d::CCDictionary *dic, const char *key, CCString *def)
{
cocos2d::Ref* obj = dic->objectForKey(key);
    if( obj != NULL )
    {
        return (cocos2d::CCString*)obj;
    }
    else{
        return def;
    }
}

void unloadTextureFromeSpriteFrameFile(const char *plist)
{
    const char *path = cocos2d::CCFileUtils::sharedFileUtils()->fullPathForFilename(plist).c_str();
  //const char *path = cocos2d::CCFileUtils::sharedFileUtils()->fullPathFromRelativePath(plist);
cocos2d::CCDictionary *dict = cocos2d::CCDictionary::createWithContentsOfFile(path);
cocos2d::CCDictionary *meta = (cocos2d::CCDictionary*)dict->objectForKey("metadata");
cocos2d::CCString *texname = (cocos2d::CCString*)meta->objectForKey("textureFileName");
cocos2d::CCTextureCache::sharedTextureCache()->removeTextureForKey(texname->getCString());
}

//UIImage* makeScreenshot()
//{
//    int tx = cocos2d::CCDirector->sharedDirector().winSize.width;
//    int ty = cocos2d::CCDirector->sharedDirector().winSize.height;
//    CCRenderTexture *renderer	= cocos2d::CCRenderTexture->renderTextureWithWidth(tx, ty);
//    renderer->begin();
//    CCDirector->sharedDirector().runningScene->visit();
//    renderer->end();
//    
//    //return renderer->getUIImageFromBuffer();
//    return renderer->getUIImage();//iphone 5
//}

cocos2d::Node *createUIByCCBI(const char* szCCBI, const char *pClassName, cocosbuilder::NodeLoader *pCCNodeLoader, Ref *target)
{
  /* Create an autorelease CCNodeLoaderLibrary. */
  cocosbuilder::NodeLoaderLibrary * ccnll = cocosbuilder::NodeLoaderLibrary::newDefaultNodeLoaderLibrary();
  ccnll->registerNodeLoader(pClassName, pCCNodeLoader);
  /* Create an autorelease CCBReader. */
  cocosbuilder::CCBReader * ccbReader = new cocosbuilder::CCBReader(ccnll);

  /* Read a ccbi file. */
  cocos2d::Node * node = ccbReader->readNodeGraphFromFile(szCCBI, target);
  ccbReader->release();

  return node;
}

void doSceneIntro(cocos2d::CCNode *&mSceneIntro, CCNode *target)
{
cocos2d::Sprite *left, *right; 
  if( mSceneIntro == NULL )
  {
    mSceneIntro = cocos2d::CCNode::create();
    target->addChild(mSceneIntro, 99);
    left = cocos2d::Sprite::create("door.png");
    left->setAnchorPoint(cocos2d::Vec2(1, 0));
    left->setPosition(cocos2d::Vec2(0, 0));
    mSceneIntro->addChild(left, 0, 0);
    right = cocos2d::Sprite::create("door.png");
    right->setScaleX(-1);
    right->setAnchorPoint(cocos2d::Vec2(1, 0));
    right->setPosition(cocos2d::Vec2(SCREEN_WIDTH, 0));
    mSceneIntro->addChild(right, 0, 1);
  }
  else
  {
    left = (cocos2d::Sprite*)mSceneIntro->getChildByTag(0);
    right = (cocos2d::Sprite*)mSceneIntro->getChildByTag(1);
  }
  left->setVisible(true);
  left->setPosition(cocos2d::Vec2(SCREEN_WIDTH/2, 0));
  right->setVisible(true);
  right->setPosition(cocos2d::Vec2(SCREEN_WIDTH/2, 0));
cocos2d::CCDelayTime *dt1 = cocos2d::CCDelayTime::create(SCENEINTRO_DELAY);
cocos2d::CCMoveBy *mb1 = cocos2d::CCMoveBy::create(SCENEINTRO_TIME,Vec2(-SCREEN_WIDTH/2, 0));
cocos2d::CCSequence *sq1 = (cocos2d::CCSequence*)CCSequence::create(dt1, mb1, NULL);
  left->runAction(sq1);
cocos2d::CCDelayTime *dt2 = cocos2d::CCDelayTime::create(SCENEINTRO_DELAY);
cocos2d::CCMoveBy *mb2 = cocos2d::CCMoveBy::create(SCENEINTRO_TIME,Vec2(SCREEN_WIDTH/2, 0));
cocos2d::CCSequence *sq2 = (cocos2d::CCSequence*)CCSequence::create(dt2, mb2, NULL);
  right->runAction(sq2);

  SimpleAudioEngine::sharedEngine()->playEffect(cocos2d::CCFileUtils::sharedFileUtils()->fullPathForFilename("sound/open.mp3").c_str());
}
cocos2d::Scene* doSceneOutro(cocos2d::Scene* mNewScene, CCNode *&mSceneIntro, SEL_CallFunc callBack, CCNode *target)
{
  if( mSceneIntro == NULL )
  {
    mSceneIntro = cocos2d::CCNode::create();
    target->addChild(mSceneIntro, 99);
cocos2d::Sprite *left = cocos2d::Sprite::create("door.png");
    left->setAnchorPoint(cocos2d::Vec2(1, 0));
    mSceneIntro->addChild(left, 0, 0);
cocos2d::Sprite *right = cocos2d::Sprite::create("door.png");
    right->setScaleX(-1);
    right->setAnchorPoint(cocos2d::Vec2(1, 0));
    mSceneIntro->addChild(right, 0, 1);
  }
cocos2d::Sprite *left = (cocos2d::Sprite*)mSceneIntro->getChildByTag(0);
  left->setVisible(true);
  left->setPosition(cocos2d::Vec2(0, 0));
cocos2d::Sprite *right = (cocos2d::Sprite*)mSceneIntro->getChildByTag(1);
  right->setVisible(true);
  right->setPosition(cocos2d::Vec2(SCREEN_WIDTH, 0));
cocos2d::CCMoveBy *mb1 = cocos2d::CCMoveBy::create(SCENEOUTRO_TIME, Vec2(SCREEN_WIDTH/2, 0));
cocos2d::CCDelayTime *dt1 = cocos2d::CCDelayTime::create(SCENEOUTRO_DELAY);
cocos2d::CCCallFunc *ca1 = cocos2d::CCCallFunc::create(target, callBack);
cocos2d::CCSequence *sq1 = (cocos2d::CCSequence*)CCSequence::create(mb1, dt1, ca1, NULL);
  left->runAction(sq1);
cocos2d::CCMoveBy *mb2 = cocos2d::CCMoveBy::create(SCENEOUTRO_TIME, Vec2(-SCREEN_WIDTH/2, 0));
cocos2d::CCDelayTime *dt2 = cocos2d::CCDelayTime::create(SCENEOUTRO_DELAY);
cocos2d::CCSequence *sq2 = (cocos2d::CCSequence*)CCSequence::create(mb2, dt2, NULL);
  right->runAction(sq2);

  SimpleAudioEngine::sharedEngine()->playEffect(cocos2d::CCFileUtils::sharedFileUtils()->fullPathForFilename("sound/close.mp3").c_str());

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
