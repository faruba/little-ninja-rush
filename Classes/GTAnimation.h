#ifndef ___CLASSES_GTANIMATION_H_
#define ___CLASSES_GTANIMATION_H_
#ifndef _GTANIMATION_H_
#define _GTANIMATION_H_

#include "cocos2d.h"
#include "extensions/cocos-ext.h"
#include "cocosbuilder/CocosBuilder.h"

;

/* 此类用于生成GTAnimatedSprite对象 */
typedef struct{
    int framecount;
    float playbacktime;
    CCSpriteFrame **frames;
}GTInternal_AnimationData;

class GTAnimation : public CCObject 
{
  public:
    CCB_STATIC_NEW_AUTORELEASE_OBJECT_WITH_INIT_METHOD(GTAnimation, create);
    virtual bool init();
    /* 管理装载在内存里的动画对象 */
    static GTAnimation* loadedAnimationSet(const char* Name);
    static void unloadAnimationSet(const char* Name);
    void unloadAllAnimationSet();

    //Building Methods
    static GTAnimation* startAnimationSet(int AnimationCount);
    bool startAnimation(int FrameCount, float Time);//start editing a new animation
    bool addFrame(const char* FrameName);//add frame to current animation
    bool endAnimation();//end of the editing of animation
    bool endAnimationSet();//check if the animation is complete

    //Accessing Methods
    int animationCount();
    int animationFrameCount(int AnimationId);
    float animationPlaybackTime(int AnimationId);
    CCSpriteFrame* spriteFrame(int AnimationId, int FrameId);

    static void loadAnimationSet(GTAnimation * Anim, const char * Name); 
  private:
    int mAnimationCount;
    GTInternal_AnimationData *mpAnimationData;

    //edit state
    int  mCurrentAnimationId;
    int  mCurrentFrameId;
};



#endif
#endif
