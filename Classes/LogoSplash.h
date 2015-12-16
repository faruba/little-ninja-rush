#ifndef ___CLASSES_LOGOSPLASH_H_
#define ___CLASSES_LOGOSPLASH_H_
#include "cocos2d.h"

class LogoSplash : public cocos2d::Layer
{
public:
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();  

    // there's no 'id' in cpp, so we recommand to return the exactly class pointer
    static cocos2d::Scene* scene();
    
    // implement the "static node()" method manually
    CREATE_FUNC(LogoSplash);

    virtual void onEnter();
    virtual void update(float);

private:
    void load();
    void done();

private:
    cocos2d::Sprite *mLogo;
    float mTimer;
    int mLoadFlag;
    int mLoadFix;
    int mSoundId;
};
#endif
