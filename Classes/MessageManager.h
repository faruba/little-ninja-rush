#ifndef ___CLASSES_MESSAGEMANAGER_H_
#define ___CLASSES_MESSAGEMANAGER_H_
#include "GameTool.h"

class Msg:public Ref
{
  public:
    static Msg* msg(cocos2d::CCNode* tar, SEL_CallFuncND sel, Ref* obj);
    CREATE_FUNC(Msg);
    virtual bool init(){return true;};
cocos2d::CCNode* target;
    SEL_CallFuncND sel;
cocos2d::Ref* obj;
};

class MessageManager:public GameObject{
  public:
    CREATE_FUNC(MessageManager);
    virtual bool init();
    static MessageManager* sharedMessageManager();

    virtual void onCreate();
    virtual void onUpdate(float delta);
    virtual void onDestroy();
//cocos2d::CCLock *mLock;
cocos2d::CCArray *mMsg;


    void clearMsg();
    void pushMsg(cocos2d::CCNode* target, SEL_CallFuncND sel, Ref* obj);

};
#endif
