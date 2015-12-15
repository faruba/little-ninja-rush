#ifndef ___CLASSES_GTANIMATEDSPRITE_H_
#define ___CLASSES_GTANIMATEDSPRITE_H_
#include "cocos2d.h"
#include "GTAnimation.h"
#include "cocos-ext.h"
#include "cocosbuilder/CocosBuilder.h"

USING_NS_CC;
USING_NS_CC_EXT;

class GTAnimatedSprite : 
  public CCSprite 
{
  public:
    virtual bool init(){return CCSprite::init();};  
    virtual bool initWithTexture(CCTexture2D * texture, CCRect rect);
    CCB_STATIC_NEW_AUTORELEASE_OBJECT_WITH_INIT_METHOD(GTAnimatedSprite, create);

    static GTAnimatedSprite* spriteWithGTAnimation(GTAnimation * animation);
    void playGTAnimation(int AnimationId, bool LoopPlay);
    void resetGTAnimation();//to play the animation from begining
    int animationId();
    int frameId();
    bool updateGTAnimation(float delta);
    float playBackTime();
    bool looping();

  private:
    GTAnimation *mAnimation;
    int         mAnimationId;
    int         mFrameId;
    bool        mLoopPlay;
    float       mTimer;
};
#endif
