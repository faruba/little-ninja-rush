#ifndef ___CLASSES_LOADING_H_
#define ___CLASSES_LOADING_H_
#include "cocos2d.h"
#include "GTLoadList.h"
#include "cocos-ext.h"

USING_NS_CC_EXT;

class Loading:public Layer
{
  public:
    virtual bool init();  
CCB_STATIC_NEW_AUTORELEASE_OBJECT_WITH_INIT_METHOD(Loading, create);
    static cocos2d::Scene *loadTo(cocos2d::Scene* dst, GTLoadList* lst, GTLoadList * ull, bool tip);

    virtual bool onTouchBegan(Touch * touch, Event * event);
    virtual void onTouchEnded(Touch * touch, Event * event);

    virtual void onEnter();
    virtual void update(float);
cocos2d::Scene *mDst;
cocos2d::Scene* mScene;
    GTLoadList* mLoadlist;
    GTLoadList* mUnloadlist;
    bool unload;
    bool mTip;
    bool mFinish;
    bool mClicked = false;
cocos2d::Sprite *mLoading;

    void prepareLoading();
    void finishLoading();
};
#endif
