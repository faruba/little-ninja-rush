#include "GTGhostShadow.h"

GTGhostShadow* GTGhostShadow::shadow(GTAnimatedSprite * copy, float interval, int number) 
{
	GTGhostShadow *ret = GTGhostShadow::create();
	ret->mCopy = copy;
	ret->mInterval = interval;
	ret->mNumber = number;
	//alloc sprites
	ret->mShadows = new cocos2d::Sprite*[number];
	return ret;
}

bool GTGhostShadow::init() 
{
	mIndex = 0;
	mCount = 0;
	mTimer = 0;
	opacity = 255;
	mToggle = false;
	mMove = 0;
	return true;
}

void GTGhostShadow::setMoveSpeed(float move) 
{
	mMove = move;
}

void GTGhostShadow::syncShadow(float delta) 
{
	mTimer += delta;
	while (mToggle && mTimer >= mInterval) {
		mTimer -= mInterval;
		int newid = (mIndex+mCount)%mNumber;
		if( mCount >= mNumber )
		{
			this->removeChild(mShadows[newid], false);
			mIndex = (mIndex+1)%mNumber;
		}
		else {
			mCount++;
		}
		mShadows[newid] = cocos2d::Sprite::createWithSpriteFrame(mCopy->displayFrame());
		mShadows[newid]->setAnchorPoint(mCopy->getAnchorPoint());
		mShadows[newid]->setPosition(mCopy->getPosition());
		mShadows[newid]->setScaleX(mCopy->getScaleX());
		mShadows[newid]->setOpacity(mCopy->getOpacity());
		this->addChild(mShadows[newid]);
	}
	//update alpha
	for(int i=0; i<mCount; ++i)
	{
		float p = (mInterval*(i+1)-mTimer)/(mInterval*mNumber);
		int one = (mIndex+i)%mNumber;
		if( p < 0 )
		{
			mShadows[one]->setVisible(false);
		}
		else {
			mShadows[one]->setOpacity(p*opacity);
		}
		float offset = mMove*delta;
		cocos2d::Point np = mShadows[one]->getPosition();
		np.x += offset;
		mShadows[one]->setPosition(np);
		//mShadows[one]->setDisplayFrame(mCopy->displayedFrame);//使残影与真身动作一致
	}
}

void GTGhostShadow::toggleShadow(bool flag) 
{
	if( mToggle != flag )
	{
		mToggle = flag;
		if( flag )
		{
			this->removeAllChildrenWithCleanup(false);
			mIndex = 0;
			mCount = 0;
		}
	}
}
