#include "LogoSplash.h"
#include "SimpleAudioEngine.h"
#include "GameConstants.h"
#include "TitleMenu.h"
#include "PublicLoad.h"
//#include "IAPHelper.h"
#include "GameRecord.h"
#include "GameData.h"
#include "UniversalFit.h"
//#include "ABDelivery.h"

#include "JsonWrapper.h"
#include "rapidjson/rapidjson.h"
#include "rapidjson/document.h"
#include "rapidjson/prettywriter.h"
#include "rapidjson/filestream.h"


using namespace cocos2d;
using namespace CocosDenshion;

CCScene* LogoSplash::scene()
{
    // 'scene' is an autorelease object
    CCScene *scene = CCScene::create();
    
    // 'layer' is an autorelease object
    LogoSplash *layer = LogoSplash::create();

    // add layer as a child to scene
    scene->addChild(layer);
    layer->setAnchorPoint(ccp(0, 0));

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool LogoSplash::init()
{
    if ( !CCLayer::init() )
    {
        return false;
    }

    scheduleUpdate();
    
    return true;
}

void LogoSplash::onEnter()
{
  CCLayerColor *bg = CCLayerColor::create(ccc4(255, 255, 255, 255));

  addChild(bg);

  mLogo = CCSprite::create("logo.png");
  mLogo->setOpacity(0);
  mLogo->setPosition(UniversalFit::sharedUniversalFit()->centralPoint);

  bg->addChild(mLogo);
  mSoundId = SimpleAudioEngine::sharedEngine()->playEffect(CCFileUtils::sharedFileUtils()->fullPathForFilename("sound/logo.mp3").c_str());

  mTimer = 0;
  mLoadFlag = 0;
  mLoadFix = 0;

  // TODO:
  //trigger check delivery
  //ABDelivery->sharedDelivery()->triggerCheckDelivery();
  CCLayer::onEnter();
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

//static const char* filenames[] =
//{
//    "data/achievements.json",
//    "data/dailyobj.json",
//    "data/katana.json",
//    "data/monthlyobj.json",
//    "data/shuriken.json",
//    "data/special.json",
//    "data/statistics.json",
//    "data/tips.json",
//    "data/tipschs.json",
//    "data/weeklyobj.json",
//};

void LogoSplash::load()
{
    CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("items.plist");

//    //test json reader -->
//    CCLog("TESTING JSON READER");
//    for(int i=0; i<10; ++i)
//    {
//        const char *filename = filenames[i];
//        CCString *data = CCString::createWithContentsOfFile(CCFileUtils::sharedFileUtils()->fullPathForFilename(filename).c_str());
//        if( data != NULL )
//        {
//            CCArray *parsed = (CCArray*)JsonWrapper::parseJson(data);
//            if( parsed != NULL )
//            {
//                CCLog("OK...%s", filename);
//            }
//            else
//            {
//                CCLog("TEST FAILED: failed to parse file (%s)", filename);
//            }
//        }
//        else
//        {
//            CCLog("TEST FAILED: failed reading file (%s)", filename);
//        }
//    }
//    CCLog("END TEST JSON READER");
//    //test json reader <--
    
    Tasks::loadObjectivesFromFile();
    Tasks::loadAchievementsFromFile();
    Tasks::loadStatisticsFromFile();
    GameData::loadShurikens();
    GameData::loadKatanas();
    GameData::loadSpecials();
    GameData::loadTips();
    // IAPHelper->sharedIAPHelper()->initIAP(GameRecord::sharedGameRecord());
    PublicLoad::commonLoadingList()->loadAll();
    PublicLoad::menuLoadingList()->loadAll();
}

void LogoSplash::done()
{
  SimpleAudioEngine::sharedEngine()->stopEffect(mSoundId);
  CCDirector::sharedDirector()->replaceScene(TitleMenu::scene());
} 

