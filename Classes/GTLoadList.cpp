#include "GameTool.h"
#include "GTLoadList.h"

void GTLoadSpriteSheet(const std::string param, bool isload)
{
    cocos2d::SpriteFrameCache *cache = cocos2d::SpriteFrameCache::getInstance();
    if( isload )
    {
        cocos2d::log("load spritesheet (%s)", param.c_str());
      cache->addSpriteFramesWithFile(param);
    }
    else {
        cocos2d::log("unload spritesheet (%s)", param.c_str());
        cache->removeSpriteFramesFromFile(param);
        //unloadTextureFromeSpriteFrameFile(param);
    }
}

void GTLoadMusic(const std::string param, bool isload)
{
    SimpleAudioEngine *engine = SimpleAudioEngine::getInstance();
    if( isload )
    {
        engine->preloadBackgroundMusic(param.c_str());
    }
}

void GTLoadSfx(const std::string param, bool isload)
{
    SimpleAudioEngine *engine = SimpleAudioEngine::getInstance();
    if( isload )
    {
        engine->preloadEffect(param.c_str());
    }
    else {
        engine->unloadEffect(param.c_str());
    }
}

LoadItem* LoadItem::itemWithInfo(const char * param, loadfunc func) 
{
    LoadItem* ret = LoadItem::create();
    ret->loadmethod = func;
    if( param != NULL )
    {
      ret->stringparam = std::string(param);
    }
    return ret;
}
void LoadItem::execute(bool isload) 
{
    loadmethod( stringparam, isload);
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
