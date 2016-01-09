#include "GTAnimation.h"
#include <string>
#include "AnimationRes.h"

static CCDictionary* gAniSetDic = NULL;

//in-memory animation object management
void GTAnimation::loadAnimationSet(GTAnimation * Anim, const char * Name) 
{
	if( gAniSetDic == NULL )
	{
		gAniSetDic = cocos2d::CCDictionary::create();
		gAniSetDic->retain();
	}
	gAniSetDic->setObject(Anim, std::string(Name));
}

GTAnimation* GTAnimation::loadedAnimationSet(const char * Name) 
{
	GTAnimation* pAnimation = NULL;

	if(gAniSetDic != NULL)
		pAnimation = (GTAnimation*)gAniSetDic->objectForKey(std::string(Name));

	return pAnimation;
}

void GTAnimation::unloadAnimationSet(const char * Name) 
{
	if( gAniSetDic != NULL )
	{
		gAniSetDic->removeObjectForKey(std::string(Name));
	}
}

void GTAnimation::unloadAllAnimationSet() 
{
	if(gAniSetDic != NULL)
	{
		gAniSetDic->removeAllObjects();
		gAniSetDic->release();
		gAniSetDic = NULL;
	}
}

GTAnimation* GTAnimation::startAnimationSet(int AnimationCount) 
{
	GTAnimation* ret = GTAnimation::create();

	ret->mAnimationCount = AnimationCount;
	ret->mpAnimationData = new GTInternal_AnimationData[AnimationCount];
	for(int i=0; i<AnimationCount; ++i)
	{
		ret->mpAnimationData[i].framecount = -1;
		ret->mpAnimationData[i].frames = NULL;
		ret->mpAnimationData[i].playbacktime = 0;
	}

	return ret;
}

// buidling metholds

bool GTAnimation::init() 
{
	mAnimationCount = 0;
	mpAnimationData = NULL;
	mCurrentAnimationId = -1;
	mCurrentFrameId = -1;
	return true;
}

bool GTAnimation::startAnimation(int FrameCount, float Time) 
{
	if(mCurrentAnimationId < mAnimationCount-1)
	{
		mCurrentAnimationId++;
		mpAnimationData[mCurrentAnimationId].framecount = FrameCount;
		mpAnimationData[mCurrentAnimationId].playbacktime = Time;
		mpAnimationData[mCurrentAnimationId].frames = new SpriteFrame*[FrameCount];
		for(int i=0; i<FrameCount; ++i)
		{
			mpAnimationData[mCurrentAnimationId].frames[i] = NULL;
		}
		mCurrentFrameId = 0;
		return true;
	}
	return false;
}

bool GTAnimation::addFrame(const char * FrameName) 
{
	if(mCurrentAnimationId < mAnimationCount && mCurrentFrameId < mpAnimationData[mCurrentAnimationId].framecount)
	{
		mpAnimationData[mCurrentAnimationId].frames[mCurrentFrameId] = GameTool::getSpriteFrameByName(FrameName);
		if (mpAnimationData[mCurrentAnimationId].frames[mCurrentFrameId])
		{
			mpAnimationData[mCurrentAnimationId].frames[mCurrentFrameId]->retain();
			mCurrentFrameId++;
		}
		return true;
	}
	return false;
}

bool GTAnimation::endAnimation() 
{
	if(mCurrentAnimationId < mAnimationCount && mCurrentFrameId == mpAnimationData[mCurrentAnimationId].framecount)
	{
		mCurrentFrameId = -1;
		return true;
	}
	return false;
}

bool GTAnimation::endAnimationSet() 
{
	if(mCurrentAnimationId == mAnimationCount)
	{
		return true;
	}
	return false;
}

// access methods

int GTAnimation::animationCount() 
{
	return mAnimationCount;
}

int GTAnimation::animationFrameCount(int AnimationId) 
{
	if(AnimationId >=0 && AnimationId<mAnimationCount)
	{
		return mpAnimationData[AnimationId].framecount;
	}
	return -1;
}

float GTAnimation::animationPlaybackTime(int AnimationId) 
{
	if(AnimationId >=0 && AnimationId<mAnimationCount)
	{
		return mpAnimationData[AnimationId].playbacktime;
	}
	return -1;
}

cocos2d::SpriteFrame* GTAnimation::spriteFrame(int AnimationId, int FrameId) 
{
	if(AnimationId >=0 && AnimationId<mAnimationCount)
	{
		if(FrameId >=0 && FrameId<mpAnimationData[AnimationId].framecount)
		{
			return mpAnimationData[AnimationId].frames[FrameId];
		}
	}
	return NULL;
}
/* TODO:释放处理
	 void GTAnimation::dealloc() 
	 {
	 if(mpAnimationData != NULL)
	 {
	 for(int i=0; i<mAnimationCount; ++i)
	 {
	 for(int j=0; j<mpAnimationData[i].framecount; ++j)
	 {
	 if( mpAnimationData[i].frames[j] != NULL )
	 {
	 mpAnimationData[i].frames[j]->release();
	 mpAnimationData[i].frames[j] = NULL;
	 }
	 }
	 free(mpAnimationData[i].frames);
	 }
	 free(mpAnimationData);
	 }

	 super->dealloc();
	 }

*/
