#include "Loading.h"
#include "GameConstants.h"
#include "GameData.h"
#include "UniversalFit.h"
#include "CEClipedNode.h"

cocos2d::CCScene *Loading::loadTo(cocos2d::CCScene* dst, GTLoadList* lst, GTLoadList * ull, bool tip)
{
  Loading *ll = Loading::create();
  ll->mTip = tip;
  ll->mFinish = false;
  ll->mScene = cocos2d::CCScene::create();
  ll->mDst = dst;
  ll->mDst->retain();
  ll->mLoadlist = lst;
  ll->mLoadlist->retain();
  if( ull != NULL )
  {
    ll->mUnloadlist = ull;
    ll->mUnloadlist->retain();
  }

  ll->mScene->setScale(UniversalFit::sharedUniversalFit()->scaleFactor);
  ll->mScene->setAnchorPoint(cocos2d::ccp(0, 0));
  ll->mScene->setPosition(UniversalFit::sharedUniversalFit()->sceneOffset);

  CEClipedNode *clip = CEClipedNode::create();
  clip->setClipRect(&(UniversalFit::sharedUniversalFit()->clipRect));
  ll->mScene->addChild(clip);
  clip->addChild(ll);

  //ll->mScene->addChild(ll);
  ll->prepareLoading();
  return ll->mScene;
}

bool Loading::init() 
{
  if (!CCLayer::init())
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
        mUnloadlist->release();
        mUnloadlist = NULL;
      }
      unload = true;
    }
    int left = mLoadlist->loadSome();
    if( left == 0 )
    {
      mLoadlist->release();
      mLoadlist = NULL;
      mFinish = true;
      if( !mTip )
      {
        this->finishLoading();
      }
      else {
        removeChild(mLoading, true);
        mLoading = NULL;
        mLoading = cocos2d::CCSprite::create("tts.png");
        mLoading->setPosition(cocos2d::ccp( 370, 30 ));
cocos2d::CCBlink *bk = cocos2d::CCBlink::create(5, 5);
cocos2d::CCRepeatForever *rf = cocos2d::CCRepeatForever::create(bk);
        mLoading->runAction(rf);
        this->addChild(mLoading);
      }
    }
  }
}

void Loading::onEnter() 
{
cocos2d::CCNode *sceneintro = cocos2d::CCNode::create();
  this->addChild(sceneintro);
cocos2d::CCSprite *left = cocos2d::CCSprite::create("door.png");
  left->setAnchorPoint(cocos2d::ccp(1, 0));
  left->setPosition(cocos2d::ccp(SCREEN_WIDTH/2, 0));
  sceneintro->addChild(left, 0, 0);
cocos2d::CCSprite *right = cocos2d::CCSprite::create("door.png");
  right->setScaleX(-1);
  right->setAnchorPoint(cocos2d::ccp(1, 0));
  right->setPosition(cocos2d::ccp(SCREEN_WIDTH/2, 0));
  sceneintro->addChild(right, 0, 1);

  mLoading = cocos2d::CCSprite::create("loading.png");
  mLoading->setPosition(cocos2d::ccp( 400, 30 ));
  this->addChild(mLoading);

  if( mTip )
  {
    /*
cocos2d::CCString *tip = GameData::randomTip();
cocos2d::CCSize size = cocos2d::CCSizeMake(335, 50);
cocos2d::CCSize actualSize = tip->sizeWithFont(UIFont->fontWithName(TIP_FONTNAME, 18), size, UILineBreakModeWordWrap);
cocos2d::CCLabelTTF *tips = cocos2d::CCLabelTTF::create(tip->getCString(), TIP_FONTNAME, 18, actualSize, kCCTextAlignmentCenter);
    tips->setAnchorPoint(cocos2d::ccp(0, 0.5f));
    tips->setPosition(cocos2d::ccp(90, 38));
cocos2d::CCSprite *bound = cocos2d::CCSprite::create("tipsbg.png");
    bound->setPosition(cocos2d::ccp(240, 265));

    addChild(bound);
    bound->addChild(tips);
    */
  }
cocos2d::CCLayer::onEnter();
}

bool Loading::onTouchBegan(Touch * touch, Event * event) {
    if ( mFinish ) {
        return true;
    }
    return false;
}

void Loading::onTouchEnded(Touch * touch, Event * event) {
  if ( mFinish ) {
    this->finishLoading();
  }
}

void Loading::finishLoading() 
{
cocos2d::CCDirector::sharedDirector()->replaceScene(mDst);
  mDst->release();
}
