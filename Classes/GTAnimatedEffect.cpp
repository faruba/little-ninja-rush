#include "GTAnimatedEffect.h"

GTAnimatedEffect* GTAnimatedEffect::create(GTAnimation* animation, int animationid, bool loop)
{
  GTAnimatedEffect *ret = new GTAnimatedEffect;
  ret->initWithSpriteFrame(animation->spriteFrame(animationid, 0));
  ret->mAnimation = animation;
  animation->retain();
  ret->playGTAnimation(animationid, loop);
  ret->scheduleUpdate();
  ret->mFrameId = 0;

  return ret;
}

void GTAnimatedEffect::playGTAnimation(int AnimationId, bool LoopPlay) 
{
  if( AnimationId < mAnimation->animationCount() )
  {
    mAnimationId = AnimationId;
    mLoopPlay = LoopPlay;
    mTimer = 0;
    this->setDisplayFrame(mAnimation->spriteFrame(AnimationId, 0));
  }
  else {
cocos2d::CCLog("GameWarning: GTAnimatedEffect::playGTAnimation > Invaild animation id(%d).", AnimationId);
  }
}

void GTAnimatedEffect::resetGTAnimation() 
{
  mTimer = 0;
  this->setDisplayFrame(mAnimation->spriteFrame(mAnimationId, 0));
}

int GTAnimatedEffect::animationId() 
{
  return mAnimationId;
}

int GTAnimatedEffect::frameId() 
{
  return mFrameId;
}

void GTAnimatedEffect::update(float delta) 
{
  if(updateGTAnimation(delta))
  {
    getParent()->removeChild(this, true);
    //CCLog("parent->removeChild(this, true);");
  }
}

bool GTAnimatedEffect::updateGTAnimation(float delta) 
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

float GTAnimatedEffect::playBackTime() 
{
  return mAnimation->animationPlaybackTime(mAnimationId);
}
