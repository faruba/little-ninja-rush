#include "MessageManager.h"

Msg* Msg::msg(CCNode* tar, SEL_CallFuncND sel, CCObject* obj) 
{
    Msg *m = Msg::create();
    m->target = tar;
    m->sel = sel;
    m->obj = obj;
    return m;
}

MessageManager* MessageManager::sharedMessageManager() 
{
    static MessageManager *ret = NULL;
    if( ret == NULL )
    {
        ret = MessageManager::create();
        ret->retain();
    }
    return ret;
}

bool MessageManager::init() 
{
  //mLock = CCLock::create();
  mMsg = CCArray::create();
  mMsg->retain();
  return true;
}

void MessageManager::clearMsg() 
{
    //mLock->lock();
    mMsg->removeAllObjects();
    //mLock->unlock();
}

void MessageManager::pushMsg(CCNode* target, SEL_CallFuncND sel, CCObject* obj) 
{
    //mLock->lock();
    if (obj)
      obj->retain();
    mMsg->addObject(Msg::msg(target, sel, obj));
    //mLock->unlock();
}

void MessageManager::onCreate() {}

void MessageManager::onUpdate(float delta) 
{
    //mLock->lock();
    while( mMsg->count() > 0 )
    {
        Msg *m = (Msg*)mMsg->objectAtIndex(0);
        //m->target->performSelector(m->sel, m->obj);
        CCCallFuncN *callSelectorAction = CCCallFuncND::create(m->target, m->sel, m->obj);
        m->target->runAction(CCSequence::create(callSelectorAction,NULL));
        mMsg->removeObjectAtIndex(0);
    }
    //mLock->unlock();
}

void MessageManager::onDestroy() 
{
    this->clearMsg();
}

