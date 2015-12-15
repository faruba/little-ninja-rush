#ifndef ___CLASSES_GTANIMATEDEFFECT_H_
#define ___CLASSES_GTANIMATEDEFFECT_H_
#include "cocos2d.h"
#include "GTAnimation.h"

class GTAnimatedEffect:public CCSprite
{
public:
  virtual void update(float);
  GTAnimation *mAnimation;
  int         mAnimationId;
  int         mFrameId;
  bool        mLoopPlay;
  float       mTimer;

  static GTAnimatedEffect* create(GTAnimation* animation, int animationid, bool loop);

  void playGTAnimation(int AnimationId, bool LoopPlay);
  void resetGTAnimation();//to play the animation from begining
  int animationId();
  int frameId();
  bool updateGTAnimation(float delta);
  float playBackTime();

};
#endif
