#include "GTAnimatedSprite.h"

GTAnimatedSprite* GTAnimatedSprite::spriteWithGTAnimation(GTAnimation * animation)
{
    GTAnimatedSprite *ret = GTAnimatedSprite::create();
    ret->initWithSpriteFrame(animation->spriteFrame(0,0));
    ret->mAnimation = animation;
    
    return ret;
}

bool GTAnimatedSprite::initWithTexture(cocos2d::Texture2D * texture, cocos2d::Rect rect)
{
    if(cocos2d::Sprite::initWithTexture(texture, rect))
    {
        mAnimation = NULL;
        mAnimationId = 0;
        mFrameId = 0;

        return true;
    }
    return false;
}

void GTAnimatedSprite::playGTAnimation(int AnimationId, bool LoopPlay)
{
    if( AnimationId < mAnimation->animationCount() )
    {
        mAnimationId = AnimationId;
        mLoopPlay = LoopPlay;
        mTimer = 0;
        this->setDisplayFrame(mAnimation->spriteFrame(AnimationId, 0));
    }
    else {
cocos2d::CCLog("GameWarning: GTAnimatedSprite::playGTAnimation > Invaild animation id(%d).", AnimationId);
    }
}

void GTAnimatedSprite::resetGTAnimation()
{
    mTimer = 0;
    this->setDisplayFrame(mAnimation->spriteFrame(mAnimationId, 0));
}

int GTAnimatedSprite::animationId()
{
    return mAnimationId;
}

int GTAnimatedSprite::frameId()
{
    return mFrameId;
}

bool GTAnimatedSprite::updateGTAnimation(float delta)
{
    bool ret = false;
    mTimer += delta;
    float pb = mAnimation->animationPlaybackTime(mAnimationId);
    while( mLoopPlay && mTimer>=pb )
    {
        mTimer -= pb;
    }
    int tf = mAnimation->animationFrameCount(mAnimationId);
    int nf = tf*(mTimer/pb);
    if(nf >= tf)
    {
        nf = tf-1;
        ret = true;
    }
    if(nf != mFrameId)
    {
        mFrameId = nf;
        this->setDisplayFrame(mAnimation->spriteFrame(mAnimationId, nf));
    }
    return ret;
}

float GTAnimatedSprite::playBackTime()
{
    return mAnimation->animationPlaybackTime(mAnimationId);
}

bool GTAnimatedSprite::looping()
{
    return mLoopPlay;
}

