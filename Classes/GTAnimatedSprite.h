#ifndef ___CLASSES_GTANIMATEDSPRITE_H_
#define ___CLASSES_GTANIMATEDSPRITE_H_
#include "cocos2d.h"
#include "GTAnimation.h"
#include "cocos-ext.h"
#include "cocosbuilder/CocosBuilder.h"

;
USING_NS_CC_EXT;

class GTAnimatedSprite : 
	public cocos2d::Sprite
{
	public:
		virtual bool init(){return cocos2d::Sprite::init();};  
		virtual bool initWithTexture(cocos2d::Texture2D * texture, cocos2d::Rect rect);
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
