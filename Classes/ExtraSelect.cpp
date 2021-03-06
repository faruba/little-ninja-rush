#include "ExtraSelect.h"
#include "TitleMenu.h"
#include "GameConstants.h"
#include "ExtraMenu.h"
#include "TipsMenu.h"
#include "UniversalFit.h"
#include "ABScrollContent.h"



bool ExtraSelect::init() 
{
	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = CC_CALLBACK_2(ExtraSelect::onTouchBegan, this);
	listener->onTouchEnded = CC_CALLBACK_2(ExtraSelect::onTouchEnded, this);
	listener->onTouchMoved = CC_CALLBACK_2(ExtraSelect::onTouchMoved, this);
	listener->onTouchCancelled = CC_CALLBACK_2(ExtraSelect::onTouchEnded, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

	this->setTouchEnabled(true);
	return true;
}

void ExtraSelect::onEnter() 
{
	mUISwapper.onEnter();
	cocos2d::Node * node = createUIByCCBI("menu-extramenu", "ExtraMenu", ExtraSelectLayerLoader::loader(), this);
	if(node != NULL) {
		this->addChild(node);
	}

	//    if( UniversalFit::sharedUniversalFit()->shouldUsingSinaWeibo() )
	//    {
	//        mTwitterBanner->setDisplayFrame(GameTool::getSpriteFrameByName("exb10.png"));
	//    }

	mUISwapper.setSceneIntro(this);
	cocos2d::Layer::onEnter();
}

void ExtraSelect::onExit() 
{
	cocos2d::Layer::onExit();
}

void ExtraSelect::onBack(cocos2d::Ref* )
{
	if(mUISwapper.isDone())
	{
		GameTool::PlaySound("menu-change.mp3");
		mUISwapper.setSceneOutro(GameTool::scene<TitleMenu>(), this);
	}
}

void ExtraSelect::onAchievement() 
{
	if(mUISwapper.isDone())
	{
		GameTool::PlaySound("menu-change.mp3");
		mUISwapper.setSceneOutro(ExtraMenu::scene(0), this);
	}
}

void ExtraSelect::onCredits() 
{
	if(mUISwapper.isDone())
	{
		GameTool::PlaySound("menu-change.mp3");
		mUISwapper.setSceneOutro(ExtraMenu::scene(2), this);
	}
}

void ExtraSelect::onStatics() 
{
	if(mUISwapper.isDone())
	{
		GameTool::PlaySound("menu-change.mp3");
		mUISwapper.setSceneOutro(ExtraMenu::scene(1), this);
	}
}

void ExtraSelect::onTips() 
{
	if(mUISwapper.isDone())
	{
		GameTool::PlaySound("menu-change.mp3");
		mUISwapper.setSceneOutro(GameTool::scene<TipsMenu>(), this);
	}
}

void ExtraSelect::onFacebook() 
{
}

void ExtraSelect::onTwitter() 
{
	/*
		 if( UniversalFit::sharedUniversalFit()->shouldUsingSinaWeibo() )
		 {
		 , //weibo.com/u/2810380794"));
		 }
		 else
		 {
		 , //twitter.com/TrinGame"));
		 }
		 */
}

void ExtraSelect::resetButtons() 
{
	mAchievement->setVisible(false);
	mStatistics->setVisible(false);
	mTips->setVisible(false);
	mCredits->setVisible(false);
	//    mFacebook->setVisible(false);
	//    mTwitter->setVisible(false);
	mSelect = NULL;
}

cocos2d::Sprite* ExtraSelect::checkButton(cocos2d::Point pos) 
{
	if( mAchievement->getParent()->boundingBox().containsPoint(pos) )
	{
		return mAchievement;
	}
	if( mStatistics->getParent()->boundingBox().containsPoint(pos) )
	{
		return mStatistics;
	}
	if( mTips->getParent()->boundingBox().containsPoint(pos) )
	{
		return mTips;
	}
	if( mCredits->getParent()->boundingBox().containsPoint(pos) )
	{
		return mCredits;
	}
	//    if( mFacebook->getParent()->boundingBox().containsPoint(pos) )
	//    {
	//        return mFacebook;
	//    }
	//    if( mTwitter->getParent()->boundingBox().containsPoint(pos) )
	//    {
	//        return mTwitter;
	//    }
	return NULL;
}

bool ExtraSelect::onTouchBegan(Touch * touch, Event * event) 
{
	LNR_GET_TOUCH_POS;

	this->resetButtons();
	mSelect = this->checkButton(pos);
	if( mSelect != NULL )
	{
		mSelect->setVisible(true);
		return true;
	}
	else {
		return false;
	}
}

void ExtraSelect::onTouchMoved(Touch * touch, Event * event) 
{
	if( mSelect != NULL )
	{
		LNR_GET_TOUCH_POS;

		if( mSelect != this->checkButton(pos) )
		{
			mSelect->setVisible(false);
		}
		else {
			mSelect->setVisible(true);
		}
	}
}

void ExtraSelect::onTouchEnded(Touch * touch, Event * event) 
{
	if( mSelect != NULL )
	{
		LNR_GET_TOUCH_POS;

		if( mSelect == this->checkButton(pos) )
		{
			if( mSelect == mAchievement )
			{
				this->onAchievement();
			}
			if( mSelect == mStatistics )
			{
				this->onStatics();
			}
			if( mSelect == mTips )
			{
				this->onTips();
			}
			if( mSelect == mCredits )
			{
				this->onCredits();
			}
			//            if( mSelect == mFacebook )
			//            {
			//                this->onFacebook();
			//            }
			//            if( mSelect == mTwitter )
			//            {
			//                this->onTwitter();
			//            }
		}
		this->resetButtons();
	}
}

SEL_MenuHandler ExtraSelect::onResolveCCBCCMenuItemSelector(cocos2d::Ref * pTarget, const char* pSelectorName) {
	CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "onBack", ExtraSelect::onBack)
		return NULL;
}

cocos2d::extension::Control::Handler   ExtraSelect::onResolveCCBCCControlSelector(cocos2d::Ref * pTarget, const char* pSelectorName) {
	return NULL;
}

bool ExtraSelect::onAssignCCBMemberVariable(cocos2d::Ref* pTarget, const char* pMemberVariableName, Node* pNode) {
	//  CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "mMask", LayerColor*, mMask) 
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "mAchievement", Sprite*, mAchievement)
		CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "mStatistics", Sprite*, mStatistics)
		CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "mTips", Sprite*, mTips)
		CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "mCredits", Sprite*, mCredits)
		//  CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "mFacebook", Sprite*, mFacebook)
		//  CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "mTwitterBanner", Sprite*, mTwitterBanner)
		//  CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "mTwitter", Sprite*, mTwitter)
		return false;
}
