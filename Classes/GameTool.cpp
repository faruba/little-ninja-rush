#include "GameTool.h"
#include "GameConstants.h"
#include "SimpleAudioEngine.h"
#include "CCNumber.h"
#include "GameRecord.h"

void GameTool::UpdateObjectives(cocos2d::Label* labels[3],cocos2d::Sprite* icons[3],cocos2d::Node* crowns[3]){
	Tasks *task = GameRecord::sharedGameRecord()->task;
  ObjectiveManager *managers[] = { &task->dailyObjective, &task->weeklyObjective, &task->monthlyObjective };
  for (int i = 0; i < 3; i++) {
    if (managers[i]->hasObjective()) {
      const Achievement &info = *(managers[i]->info());

      labels[i]->setString(Tasks::stringForObjective(info.desc, info.achieveCode,  info.achieveNumber, managers[i]->currentObjective.count)->getCString());
      labels[i]->setColor(Color3B(255, 255, 255));

      icons[i]->setVisible(true);
      cocos2d::Sprite *icon = cocos2d::Sprite::createWithSpriteFrameName(info.icon);
      icon->setPosition(cocos2d::Vec2(icons[i]->getContentSize().width/2, icons[i]->getContentSize().height/2));
      icons[i]->addChild(icon);

      if (managers[i]->isCompleted()) {
        labels[i]->setColor(Color3B(128, 128, 128));
      }
    } else {
      labels[i]->setString("已完成！");
      labels[i]->setColor(Color3B(128, 128, 128));
    }

		for(int ii=0; ii<managers[i]->currentObjective.index; ++ii)
		{
			cocos2d::Sprite *crown = cocos2d::Sprite::createWithSpriteFrameName(cocos2d::CCString::createWithFormat("crown%d.png", ii)->getCString());
			crown->setPosition(cocos2d::Vec2(32-16*ii, 0));
			crowns[ii]->addChild(crown);
		}
  }
}
// ----------
// UISwapper
// ----------
void UISwapper::initiateDoors(cocos2d::Node* target) {
    mDoornode = cocos2d::Node::create();
    target->addChild(mDoornode, 999);
    mLeftDoor = cocos2d::Sprite::create("door.png");
    mLeftDoor->setAnchorPoint(cocos2d::Vec2(1, 0));
    mDoornode->addChild(mLeftDoor, 0, 0);

    mRightDoor = cocos2d::Sprite::create("door.png");
    mRightDoor->setScaleX(-1);
    mRightDoor->setAnchorPoint(cocos2d::Vec2(1, 0));
    mDoornode->addChild(mRightDoor, 0, 1);
}

void UISwapper::doSceneOutro(cocos2d::Node* target) {
  if ( mDoornode == NULL ) {
    initiateDoors(target);
  }

  mLeftDoor->setVisible(true);
  mLeftDoor->setPosition(cocos2d::Vec2(0, 0));
  mRightDoor->setVisible(true);
  mRightDoor->setPosition(cocos2d::Vec2(SCREEN_WIDTH, 0));

  mLeftDoor->runAction(cocos2d::Sequence::create(
        cocos2d::MoveBy::create(SCENEOUTRO_TIME, Vec2(SCREEN_WIDTH/2, 0)),
        cocos2d::DelayTime::create(SCENEOUTRO_DELAY),
        cocos2d::CallFunc::create([this]() { this->doneOutro(); }),
        nullptr
        ));

  mRightDoor->runAction(cocos2d::Sequence::create(
        cocos2d::MoveBy::create(SCENEOUTRO_TIME, Vec2(-SCREEN_WIDTH/2, 0)),
        cocos2d::DelayTime::create(SCENEOUTRO_DELAY),
        nullptr
        ));

  GameTool::PlaySound("close.mp3");
}

void UISwapper::doSceneIntro(cocos2d::Node* target) {
  if( mDoornode == NULL ) {
    initiateDoors(target);
  }

  mLeftDoor->setVisible(true);
  mLeftDoor->setPosition(cocos2d::Vec2(SCREEN_WIDTH/2, 0));
  mRightDoor->setVisible(true);
  mRightDoor->setPosition(cocos2d::Vec2(SCREEN_WIDTH/2, 0));

  mLeftDoor->runAction(cocos2d::Sequence::create(
        cocos2d::DelayTime::create(SCENEINTRO_DELAY),
        cocos2d::MoveBy::create(SCENEINTRO_TIME,Vec2(-SCREEN_WIDTH/2, 0)),
        nullptr
        ));

  mRightDoor->runAction(cocos2d::Sequence::create(
        cocos2d::DelayTime::create(SCENEINTRO_DELAY),
        cocos2d::MoveBy::create(SCENEINTRO_TIME,Vec2(SCREEN_WIDTH/2, 0)),
        nullptr
        ));

  GameTool::PlaySound("open.mp3");
}

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
  return disq < (r+pr)*(r+pr);
}

bool collide(const Circle &c1, const Circle &c2) {
  float disq = (c1.center - c2.center).getLengthSq();
  float radius_sum = c1.radius + c2.radius;
  return disq < radius_sum*radius_sum;
}

int gtReadInt(cocos2d::CCDictionary *dic, std::string key, int def)
{
  cocos2d::Ref *obj = dic->objectForKey(key.c_str());
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

std::string gtReadString(cocos2d::CCDictionary *dic, const char *key, std::string def)
{
  cocos2d::Ref* obj = dic->objectForKey(key);
  if( obj != NULL )
  {
    return std::string(((cocos2d::CCString*)obj)->getCString());
  }
  else{
    return def;
  }
}

void unloadTextureFromeSpriteFrameFile(const char *plist)
{
    const char *path = cocos2d::CCFileUtils::getInstance()->fullPathForFilename(plist).c_str();
  //const char *path = cocos2d::CCFileUtils::sharedFileUtils()->fullPathFromRelativePath(plist);
//cocos2d::CCDictionary *dict = cocos2d::CCDictionary::createWithContentsOfFile(path);
//cocos2d::CCDictionary *meta = (cocos2d::CCDictionary*)dict->objectForKey("metadata");
//cocos2d::CCString *texname = (cocos2d::CCString*)meta->objectForKey("textureFileName");
//cocos2d::CCTextureCache::sharedTextureCache()->removeTextureForKey(texname->getCString());
}

//UIImage* makeScreenshot()
//{
//    int tx = cocos2d::CCDirector->getInstance().winSize.width;
//    int ty = cocos2d::CCDirector->getInstance().winSize.height;
//    CCRenderTexture *renderer	= cocos2d::CCRenderTexture->renderTextureWithWidth(tx, ty);
//    renderer->begin();
//    CCDirector->getInstance().runningScene->visit();
//    renderer->end();
//    
//    //return renderer->getUIImageFromBuffer();
//    return renderer->getUIImage();//iphone 5
//}

cocos2d::Node *createUIByCCBI(const char* szCCBI, const char *pClassName, cocosbuilder::NodeLoader *pNodeLoader, Ref *target)
{
  /* Create an autorelease NodeLoaderLibrary. */
  cocosbuilder::NodeLoaderLibrary * ccnll = cocosbuilder::NodeLoaderLibrary::newDefaultNodeLoaderLibrary();
  ccnll->registerNodeLoader(pClassName, pNodeLoader);
  /* Create an autorelease CCBReader. */
  cocosbuilder::CCBReader * ccbReader = new cocosbuilder::CCBReader(ccnll);

  /* Read a ccbi file. */
  cocos2d::Node * node = ccbReader->readNodeGraphFromFile(szCCBI, target);
  ccbReader->release();

  return node;
}

#ifdef LINUX

//CFAbsoluteTime CFAbsoluteTimeGetCurrent()
//{
//  timespec tm;
//  //clock_gettime(CLOCK_REALTIME, &tm);//hammer marked
//  return tm.tv_nsec/10^9;
//}
#endif
