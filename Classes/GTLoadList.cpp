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

LoadItem::LoadItem(const char * param, loadfunc func)
{
  loadmethod = func;
  if( param != NULL )
  {
    stringparam = std::string(param);
  }
}
void LoadItem::execute(bool isload) 
{
  loadmethod( stringparam, isload);
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
  while ( mLoadIndex < mList.size() && cost < mMaxInterval ) {
    //CFAbsoluteTime bl = CFAbsoluteTimeGetCurrent();
    timeval bl;
    gettimeofday(&bl, NULL);
    
    LoadItem &item = mList[mLoadIndex];
    item.execute(true);
    
    //CFAbsoluteTime al = CFAbsoluteTimeGetCurrent();
    timeval al;
    gettimeofday(&al, NULL);
    
    //cost += al-bl;
    cost += gettimeofday(&bl, &al)/1000.0;
    mLoadIndex++;
  }
  return mList.size() - mLoadIndex;
}

void GTLoadList::unloadAll() 
{  
  for(LoadItem& li: mList)
  {
    li.execute(false);
  }
}

void GTLoadList::loadAll() 
{
  for(LoadItem& li: mList)
  {
    li.execute(true);
  }
}

int GTLoadList::length() 
{
  return mList.size();
}

void GTLoadList::addLoadFuncWithName(const char * name, loadfunc func)
{
  if( mLoadIndex < 0 )
  {
    LoadItem it(name, func);
    mList.push_back(it);
  } 
}
void GTLoadList::addSpriteSheet(const char * name) 
{
  addLoadFuncWithName(name, GTLoadSpriteSheet);
}

void GTLoadList::addMusic(const char * name) 
{
  addLoadFuncWithName(name, GTLoadMusic);
}

void GTLoadList::addSfx(const char * name) 
{
  addLoadFuncWithName(name, GTLoadSfx);
}

void GTLoadList::addLoadFunc(loadfunc func) 
{
  addLoadFuncWithName("",func);
}
