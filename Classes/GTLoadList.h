#ifndef ___CLASSES_GTLOADLIST_H_
#define ___CLASSES_GTLOADLIST_H_
#include "cocos2d.h"
#include "cocos-ext.h"
#include "GameTool.h"
#include "SimpleAudioEngine.h"

USING_NS_CC_EXT;

typedef void (*loadfunc)(const char* param, bool isload);

void GTLoadSpriteSheet(const char* param, bool isload);

void GTLoadMusic(const char* param, bool isload);

void GTLoadSfx(const char* param, bool isload);

class LoadItem:
  public CCObject
{
  public:
CCB_STATIC_NEW_AUTORELEASE_OBJECT_METHOD(LoadItem, create);
cocos2d::CCString* stringparam;
    loadfunc  loadmethod;

    static LoadItem* itemWithInfo(const char* param, loadfunc func);
    void execute(bool isload);
};

class GTLoadList:
  public CCObject
{
  public:
    virtual bool init();
CCB_STATIC_NEW_AUTORELEASE_OBJECT_WITH_INIT_METHOD(GTLoadList, create);
cocos2d::CCArray *mList;
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

};
#endif
