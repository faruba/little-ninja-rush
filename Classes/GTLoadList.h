#ifndef ___CLASSES_GTLOADLIST_H_
#define ___CLASSES_GTLOADLIST_H_
#include "cocos2d.h"
#include "cocos-ext.h"
#include "GameTool.h"
#include "SimpleAudioEngine.h"

USING_NS_CC_EXT;

typedef void (*loadfunc)(const std::string param, bool isload);

void GTLoadSpriteSheet(const char* param, bool isload);

void GTLoadMusic(const char* param, bool isload);

void GTLoadSfx(const char* param, bool isload);

class LoadItem
{
public:
  LoadItem(const char* param, loadfunc func);
  std::string stringparam;
  loadfunc  loadmethod;
  
  void execute(bool isload);
};

class GTLoadList
{
public:
  GTLoadList(){
    mLoadIndex = -1;
  }
  std::vector<LoadItem> mList;
  int mLoadIndex;
  //CFTimeInterval mMaxInterval;
  float mMaxInterval;
  
  GTLoadList* list();
  
  void addSpriteSheet(const char* name);
  void addLoadFunc(loadfunc func);
  void addMusic(const char* name);
  void addSfx(const char* name);
  
  void reset(float maxInterval);
  int loadSome();
  int length();
  void unloadAll();
  
  void loadAll();
private:
  void addLoadFuncWithName(const char * name, loadfunc func);
  
};
#endif
