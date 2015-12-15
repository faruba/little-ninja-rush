#ifndef ___CLASSES_MESSAGEMANAGER_H_
#define ___CLASSES_MESSAGEMANAGER_H_
#include "GameTool.h"

class Msg:public CCObject
{
  public:
    static Msg* msg(CCNode* tar, SEL_CallFuncND sel, CCObject* obj);
    CREATE_FUNC(Msg);
    virtual bool init(){return true;};
    
    CCNode* target;
    SEL_CallFuncND sel;
    CCObject* obj;
};

class MessageManager:public GameObject{
  public:
    CREATE_FUNC(MessageManager);
    virtual bool init();
    static MessageManager* sharedMessageManager();

    virtual void onCreate();
    virtual void onUpdate(float delta);
    virtual void onDestroy();

    CCLock *mLock;
    CCArray *mMsg;


    void clearMsg();
    void pushMsg(CCNode* target, SEL_CallFuncND sel, CCObject* obj);

};
#endif
