#include "GameTool.h"
#include "GTLoadList.h"

void GTLoadSpriteSheet(const char* param, bool isload)
{
cocos2d::SpriteFrameCache *cache = cocos2d::SpriteFrameCache::sharedSpriteFrameCache();
    if( isload )
    {
cocos2d::CCLog("load spritesheet (%s)", param);
        cache->addSpriteFramesWithFile(param);
    }
    else {
cocos2d::CCLog("unload spritesheet (%s)", param);
        cache->removeSpriteFramesFromFile(param);
        unloadTextureFromeSpriteFrameFile(param);
    }
}

void GTLoadMusic(const char* param, bool isload)
{
    SimpleAudioEngine *engine = SimpleAudioEngine::sharedEngine();
    if( isload )
    {
        engine->preloadBackgroundMusic(param);
    }
}

void GTLoadSfx(const char* param, bool isload)
{
    SimpleAudioEngine *engine = SimpleAudioEngine::sharedEngine();
    if( isload )
    {
        engine->preloadEffect(param);
    }
    else {
        engine->unloadEffect(param);
    }
}

LoadItem* LoadItem::itemWithInfo(const char * param, loadfunc func) 
{
    LoadItem* ret = LoadItem::create();
    ret->loadmethod = func;
    if( param != NULL )
    {
        ret->stringparam = cocos2d::CCString::create(param);
        ret->stringparam->retain();
    }
    return ret;
}
void LoadItem::execute(bool isload) 
{
  const char *szStr = NULL;
  if (stringparam)
    szStr = stringparam->getCString();
    loadmethod(szStr, isload);
}




GTLoadList* GTLoadList::list() 
{
    return GTLoadList::create();
}

bool GTLoadList::init()
{
  mList = cocos2d::CCArray::create();
  mList->retain();
  mLoadIndex = -1;
  return true;
}

void GTLoadList::reset(float maxInterval)
{
    mLoadIndex = 0;
    mMaxInterval = maxInterval;
}

int GTLoadList::loadSome() 
{
  //CFTimeInterval cost = 0;
  float cost = 0;
  while ( mLoadIndex < mList->count() && cost < mMaxInterval ) {
    //CFAbsoluteTime bl = CFAbsoluteTimeGetCurrent();
    timeval bl;
    gettimeofday(&bl, NULL);

    LoadItem *item = (LoadItem*)mList->objectAtIndex(mLoadIndex);
    item->execute(true);

    //CFAbsoluteTime al = CFAbsoluteTimeGetCurrent();
    timeval al;
    gettimeofday(&al, NULL);

    //cost += al-bl;
    cost += gettimeofday(&bl, &al)/1000.0;
    mLoadIndex++;
  }
  return mList->count() - mLoadIndex;
}

void GTLoadList::unloadAll() 
{  
cocos2d::Ref* node = NULL;
CCARRAY_FOREACH(mList, node)
  {
    LoadItem *li = (LoadItem*)node;
    li->execute(false);
  }
}

void GTLoadList::loadAll() 
{
cocos2d::Ref* node = NULL;
CCARRAY_FOREACH(mList, node)
  {
    LoadItem *li = (LoadItem*)node;
    li->execute(true);
  }
}

int GTLoadList::length() 
{
    return mList->count();
}

void GTLoadList::addSpriteSheet(const char * name) 
{
    if( mLoadIndex < 0 )
    {
      LoadItem *it = LoadItem::itemWithInfo(name, GTLoadSpriteSheet);
      mList->addObject(it);
    }
}

void GTLoadList::addMusic(const char * name) 
{
    if( mLoadIndex < 0 )
    {
        LoadItem *it = LoadItem::itemWithInfo(name, GTLoadMusic);
        mList->addObject(it);
    }
}

void GTLoadList::addSfx(const char * name) 
{
    if( mLoadIndex < 0 )
    {
        LoadItem *it = LoadItem::itemWithInfo(name, GTLoadSfx);
        mList->addObject(it);
    }
}

void GTLoadList::addLoadFunc(loadfunc func) 
{
    if( mLoadIndex < 0 )
    {
        LoadItem *it = LoadItem::itemWithInfo(NULL, func);
        mList->addObject(it);
    }
}
