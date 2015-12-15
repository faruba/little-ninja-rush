#ifndef ___CLASSES_GAMEOBJECTMANAGER_H_
#define ___CLASSES_GAMEOBJECTMANAGER_H_
#include "GameObject.h"
#include "cocos2d.h"
#include "cocos-ext.h"

;
USING_NS_CC_EXT;

class OperationCache:
  public CCObject
{
  public:
    virtual bool init() {return true;};
    
    CREATE_FUNC(OperationCache);
    //--- OPERATION CACHE ---
    static OperationCache* cacheAddObject(GameObject* ObjId);
    static OperationCache* cacheRemoveObject(GameObject* ObjId);
    static OperationCache* cacheClear();

    int opt;// 0-add 1-remove 2-clear
    GameObject* opn;
    GOHandler opr;
};

class GameObjectManager:
  public CCObject
{
  public:
    virtual bool init();
    virtual void update(float delta);

    CREATE_FUNC(GameObjectManager);

    GameObject* addGameObject(GameObject* Obj);
    GameObject* gameObjectWithHandler(GOHandler hd);
    void removeGameObject(GameObject* ObjId);
    void clear();
  private:
cocos2d::CCArray *mList;
cocos2d::CCArray *mCache;
cocos2d::CCArray *mFree;
    bool mCaching;

    int mAddCount;

    void cacheOperation();
    void flushOperation();
    int objectCount();
};
#endif
