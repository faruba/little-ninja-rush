#include "LogoSplash.h"
#include "GameConstants.h"
#include "TitleMenu.h"
#include "PublicLoad.h"
//#include "IAPHelper.h"
#include "GameRecord.h"
#include "GameData.h"
#include "UniversalFit.h"
//#include "ABDelivery.h"

#include "AnimationRes.h"


using namespace cocos2d;

cocos2d::Scene* LogoSplash::scene() {
  // 'scene' is an autorelease object
  cocos2d::Scene *scene = cocos2d::Scene::create();

  // 'layer' is an autorelease object
  LogoSplash *layer = LogoSplash::create();

  // add layer as a child to scene
  scene->addChild(layer);
  layer->setAnchorPoint(cocos2d::Vec2(0, 0));

  // return the scene
  return scene;
}

// on "init" you need to initialize your instance
bool LogoSplash::init()
{
  if ( !Layer::init() )
  {
    return false;
  }

  scheduleUpdate();

  return true;
}

void LogoSplash::onEnter()
{
  cocos2d::LayerColor *bg = cocos2d::LayerColor::create(Color4B(255, 255, 255, 255));

  addChild(bg);

  mLogo = cocos2d::Sprite::create("logo.png");
  mLogo->setOpacity(0);
  mLogo->setPosition(UniversalFit::sharedUniversalFit()->centralPoint);

  bg->addChild(mLogo);
  mSoundId = GameTool::PlaySound("logo.mp3");

  mTimer = 0;
  mLoadFlag = 0;
  mLoadFix = 0;

  cocos2d::Layer::onEnter();
}

void LogoSplash::update(float delta)
{
  mTimer += delta;

  //超时修正
  if( mLoadFlag && !mLoadFix && mTimer > 3.0f )
  {
    mTimer = 3.0f;
    mLoadFix = 1;
  }

  float k = 0.0f;

  if( mTimer < 1 )
  {
    k = mTimer/1.0f;
  }
  else if( mTimer < 3 )
  {
    k = 1;
    if( 0 == mLoadFlag )
    {
      load();
      mLoadFlag = 1;
    }
  }
  else if( mTimer < 4 )
  {
    k = 1.0f - (mTimer-3.0f)/1.0f;
  }
  else {
    k = 0;
    done();
  }

  mLogo->setOpacity(255*k);
}

void LogoSplash::load()
{
  initRandom(0);
  cocos2d::SpriteFrameCache::getInstance()->addSpriteFramesWithFile("items.plist");

  Tasks::loadObjectivesFromFile();
  Tasks::loadAchievementsFromFile();
  Tasks::loadStatisticsFromFile();
  GameData::loadData();
  AnimationSets::loadAnimationSets ();

  // IAPHelper->sharedIAPHelper()->initIAP(GameRecord::sharedGameRecord());
  PublicLoad::commonLoadingList()->loadAll();
  PublicLoad::menuLoadingList()->loadAll();
}

void LogoSplash::done() {
  GameTool::StopSound(mSoundId);
  cocos2d::Director::getInstance()->replaceScene(GameTool::scene<TitleMenu>());
} 
