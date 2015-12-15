#include "Loading.h"
#include "GameConstants.h"
#include "GameData.h"
#include "UniversalFit.h"
#include "CEClipedNode.h"

CCScene *Loading::loadTo(CCScene* dst, GTLoadList* lst, GTLoadList * ull, bool tip)
{
  Loading *ll = Loading::create();
  ll->mTip = tip;
  ll->mFinish = false;
  ll->mScene = CCScene::create();
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
  ll->mScene->setAnchorPoint(ccp(0, 0));
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
        mLoading = CCSprite::create("tts.png");
        mLoading->setPosition(ccp( 370, 30 ));
        CCBlink *bk = CCBlink::create(5, 5);
        CCRepeatForever *rf = CCRepeatForever::create(bk);
        mLoading->runAction(rf);
        this->addChild(mLoading);
      }
    }
  }
}

void Loading::onEnter() 
{
  CCNode *sceneintro = CCNode::create();
  this->addChild(sceneintro);
  CCSprite *left = CCSprite::create("door.png");
  left->setAnchorPoint(ccp(1, 0));
  left->setPosition(ccp(SCREEN_WIDTH/2, 0));
  sceneintro->addChild(left, 0, 0);
  CCSprite *right = CCSprite::create("door.png");
  right->setScaleX(-1);
  right->setAnchorPoint(ccp(1, 0));
  right->setPosition(ccp(SCREEN_WIDTH/2, 0));
  sceneintro->addChild(right, 0, 1);

  mLoading = CCSprite::create("loading.png");
  mLoading->setPosition(ccp( 400, 30 ));
  this->addChild(mLoading);

  if( mTip )
  {
    /*
    CCString *tip = GameData::randomTip();
    CCSize size = CCSizeMake(335, 50);
    CCSize actualSize = tip->sizeWithFont(UIFont->fontWithName(TIP_FONTNAME, 18), size, UILineBreakModeWordWrap);
    CCLabelTTF *tips = CCLabelTTF::create(tip->getCString(), TIP_FONTNAME, 18, actualSize, kCCTextAlignmentCenter);
    tips->setAnchorPoint(ccp(0, 0.5f));
    tips->setPosition(ccp(90, 38));

    CCSprite *bound = CCSprite::create("tipsbg.png");
    bound->setPosition(ccp(240, 265));

    addChild(bound);
    bound->addChild(tips);
    */
  }

  CCLayer::onEnter();
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
  CCDirector::sharedDirector()->replaceScene(mDst);
  mDst->release();
}
