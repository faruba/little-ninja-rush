#include "Loading.h"
#include "GameConstants.h"
#include "GameData.h"
#include "UniversalFit.h"
#include "CEClipedNode.h"

cocos2d::Scene *Loading::loadTo(cocos2d::Scene* dst, GTLoadList* lst, GTLoadList * ull, bool tip)
{
	Loading *ll = Loading::create();
	ll->mTip = tip;
	ll->mFinish = false;
  ll->mClicked = false;
	ll->mScene = cocos2d::Scene::create();
	ll->mDst = dst;
	ll->mDst->retain();
	ll->mLoadlist = lst;
	if( ull != NULL )
	{
		ll->mUnloadlist = ull;
	}

	ll->mScene->setScale(UniversalFit::sharedUniversalFit()->scaleFactor);
	ll->mScene->setAnchorPoint(cocos2d::Vec2(0, 0));
	ll->mScene->setPosition(UniversalFit::sharedUniversalFit()->sceneOffset);

	CEClipedNode *clip = CEClipedNode::create();
	clip->setClipRect(&(UniversalFit::sharedUniversalFit()->clipRect));
	ll->mScene->addChild(clip);
	clip->addChild(ll);

	ll->prepareLoading();
	return ll->mScene;
}

bool Loading::init() 
{
	if (!Layer::init())
		return false;

	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan = CC_CALLBACK_2(Loading::onTouchBegan, this);
	listener->onTouchEnded = CC_CALLBACK_2(Loading::onTouchEnded, this);
	listener->onTouchCancelled = CC_CALLBACK_2(Loading::onTouchEnded, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

	this->setTouchEnabled(true);
	this->scheduleUpdate();
	return true;
}

void Loading::prepareLoading() 
{
	mLoadlist->reset(1.0f/5);
	unload = false;
}

void Loading::update(float delta) 
{
	if( !mFinish )
	{
		if( !unload )
		{
			if( mUnloadlist != NULL )
			{
				mUnloadlist->unloadAll();
				mUnloadlist = NULL;
			}
			unload = true;
		}
		int left = mLoadlist->loadSome();
		if( left == 0 )
		{
			mLoadlist = NULL;
			mFinish = true;
			if( !mTip )
			{
				this->finishLoading();
			}
			else {
				removeChild(mLoading, true);
				mLoading = NULL;
				mLoading = cocos2d::Sprite::create("tts.png");
				mLoading->setPosition(cocos2d::Vec2( 370, 30 ));
				cocos2d::CCBlink *bk = cocos2d::CCBlink::create(5, 5);
				cocos2d::RepeatForever *rf = cocos2d::RepeatForever::create(bk);
				mLoading->runAction(rf);
				this->addChild(mLoading);
			}
		}
	}
}

void Loading::onEnter() 
{
	cocos2d::Node *sceneintro = cocos2d::Node::create();
	this->addChild(sceneintro);
	cocos2d::Sprite *left = cocos2d::Sprite::create("door.png");
	left->setAnchorPoint(cocos2d::Vec2(1, 0));
	left->setPosition(cocos2d::Vec2(SCREEN_WIDTH/2, 0));
	sceneintro->addChild(left, 0, 0);
	cocos2d::Sprite *right = cocos2d::Sprite::create("door.png");
	right->setScaleX(-1);
	right->setAnchorPoint(cocos2d::Vec2(1, 0));
	right->setPosition(cocos2d::Vec2(SCREEN_WIDTH/2, 0));
	sceneintro->addChild(right, 0, 1);

	mLoading = cocos2d::Sprite::create("loading.png");
	mLoading->setPosition(cocos2d::Vec2( 400, 30 ));
	this->addChild(mLoading);

	if( mTip )
	{
		//TODO:
		//std::string tip = GameData::randomTip();
		//cocos2d::Size size = cocos2d::SizeMake(335, 50);
		//cocos2d::Size actualSize = tip->sizeWithFont(UIFont->fontWithName(TIP_FONTNAME, 18), size, UILineBreakModeWordWrap);
		//cocos2d::Label *tips = cocos2d::Label::create(tip.c_str(), TIP_FONTNAME, 18, actualSize, kCCTextAlignmentCenter);
		//tips->setAnchorPoint(cocos2d::Vec2(0, 0.5f));
		//tips->setPosition(cocos2d::Vec2(90, 38));
		//cocos2d::Sprite *bound = cocos2d::Sprite::create("tipsbg.png");
		//bound->setPosition(cocos2d::Vec2(240, 265));

		//addChild(bound);
		//bound->addChild(tips);
	}
	cocos2d::Layer::onEnter();
}

bool Loading::onTouchBegan(Touch * touch, Event * event) {
  return mFinish && !mClicked;
}

void Loading::onTouchEnded(Touch * touch, Event * event) {
	if ( mFinish && !mClicked) {
		this->finishLoading();
    mClicked = true;
	}
}

void Loading::finishLoading() {
	cocos2d::CCDirector::getInstance()->replaceScene(mDst);
}
