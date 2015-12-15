#ifndef ___CLASSES_LOADING_H_
#define ___CLASSES_LOADING_H_
#include "cocos2d.h"
#include "GTLoadList.h"
#include "cocos-ext.h"

USING_NS_CC_EXT;

class Loading:public CCLayer
{
  public:
    virtual bool init();  

    CCB_STATIC_NEW_AUTORELEASE_OBJECT_WITH_INIT_METHOD(Loading, create);
    static CCScene *loadTo(CCScene* dst, GTLoadList* lst, GTLoadList * ull, bool tip);

    virtual bool onTouchBegan(Touch * touch, Event * event);
    virtual void onTouchEnded(Touch * touch, Event * event);

    virtual void onEnter();
    virtual void update(float);

    CCScene *mDst;
    CCScene* mScene;
    GTLoadList* mLoadlist;
    GTLoadList* mUnloadlist;
    bool unload;
    bool mTip;
    bool mFinish;

    CCSprite *mLoading;

    void prepareLoading();
    void finishLoading();
};
#endif
