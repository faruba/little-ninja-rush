#include "GameObjectManager.h"
#include "GameTool.h"

OperationCache* OperationCache::cacheAddObject(GameObject* ObjId) 
{
  OperationCache* ret = OperationCache::create();
  ret->opt = 0;
  ret->opn = ObjId;
  ret->opn->retain();
  return ret;
}
OperationCache* OperationCache::cacheRemoveObject(GameObject* ObjId) 
{
  OperationCache* ret = OperationCache::create();
  ret->opt = 1;
  ret->opr = ObjId->handler();
  return ret;
}
OperationCache* OperationCache::cacheClear() 
{
  OperationCache* ret = OperationCache::create();
  ret->opt = 2;
  ret->opn = NULL;
  return ret;
}

bool GameObjectManager::init() 
{
  mList = CCArray::create();
  mList->retain();
  mCache = CCArray::create();
  mCache->retain();
  mFree = CCArray::create();
  mFree->retain();
  mCaching = false;
  mAddCount = 0;
  
  return true;
}

GameObject* GameObjectManager::addGameObject(GameObject* Obj) 
{
  if( mCaching )
  {
    OperationCache *oc = OperationCache::cacheAddObject(Obj);
    mCache->addObject(oc);
  }
  else {
    int index = -1;
    if( mFree->count() > 0 )
    {
      index = ((CCInteger*)(mFree->objectAtIndex(0)))->getValue();
      mFree->removeObjectAtIndex(0);
      mList->replaceObjectAtIndex(index, Obj);
    }
    if( index < 0 )
    {
      index = mList->count();
      mList->addObject(Obj);
    }
    Obj->setHandler(index, mAddCount);
    Obj->onCreate();
    mAddCount++;
  }
  return Obj;
}

void GameObjectManager::removeGameObject(GameObject* ObjId) 
{
  if( mCaching )
  {
    OperationCache *oc = OperationCache::cacheRemoveObject(ObjId);
    mCache->addObject(oc);
  }
  else {
    unsigned int index = mList->indexOfObject(ObjId);
    if( index != CC_INVALID_INDEX )
    {
      ObjId->onDestroy();
      mList->replaceObjectAtIndex(index, GameObject::dummyObject());
      CCInteger *num = CCInteger::create(index);
      mFree->addObject(num);
    }
  }
}

void GameObjectManager::clear() 
{
  if( mCaching )
  {
    OperationCache *oc = OperationCache::cacheClear();
    mCache->addObject(oc);
  }
  else {
    while(mList->count() > 0)
    {
      GameObject* go = (GameObject*)(mList->objectAtIndex(mList->count()-1));
      go->onDestroy();
      mList->removeLastObject();
    }
    mFree->removeAllObjects();
  }
}

void GameObjectManager::cacheOperation() 
{
  if(mCaching == false)
  {
    mCaching = true;
  }
}

void GameObjectManager::flushOperation() 
{
  if( mCaching != false )
  {
    while(mCache->count() > 0)
    {
      OperationCache *oc = (OperationCache *)(mCache->objectAtIndex(0));
      switch(oc->opt)
      {
        case 0://add
          {
            int index = -1;
            if( mFree->count() > 0 )
            {
              index = ((CCInteger*)mFree->objectAtIndex(0))->getValue();
              mFree->removeObjectAtIndex(0);
              mList->replaceObjectAtIndex(index, oc->opn);
            }
            if( index < 0 )
            {
              index = mList->count();
              mList->addObject(oc->opn);
            }
            oc->opn->setHandler(index, mAddCount);
            oc->opn->onCreate();
            oc->opn->release();
            mAddCount++;
          }
          break;
        case 1://remove
          {
            GameObject *todel = this->gameObjectWithHandler(oc->opr);
            if( todel != NULL )
            {
              unsigned int index = oc->opr.index;
              if( index != CC_INVALID_INDEX )
              {
                todel->onDestroy();
                mList->replaceObjectAtIndex(index, GameObject::dummyObject());
                CCInteger *num = CCInteger::create(index);
                mFree->addObject(num);
              }
            }
          }
          break;
        case 2://clear
          {
            while(mList->count() > 0)
            {
              GameObject* go = (GameObject*)(mList->objectAtIndex(mList->count()-1));
              go->onDestroy();
              mList->removeLastObject();
            }
            mFree->removeAllObjects();
          }
          break;
      }
      mCache->removeObjectAtIndex(0);
    }
    mCaching = false;
  }
}

int GameObjectManager::objectCount() 
{
  return mList->count();
}

void GameObjectManager::update(float delta) 
{
  this->cacheOperation();
  CCObject* node = NULL;
  CCARRAY_FOREACH(mList, node)
  {
    GameObject* go = (GameObject*)node;
    go->onUpdate(delta);
  }
  this->flushOperation();
}

GameObject* GameObjectManager::gameObjectWithHandler(GOHandler hd) 
{
  if( hd.index >= 0 && hd.uid >= 0 && hd.index < mList->count() )
  {
    GameObject *ret = (GameObject *)(mList->objectAtIndex(hd.index));
    if( ret->handler().uid == hd.uid )
    {
      return ret;
    }
  }
  return NULL;
}

