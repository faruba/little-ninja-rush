#include "ABScrollContent.h"
#include "UniversalFit.h"

bool ABScrollContent::init() 
{
  CCLog("ABScrollContent::init");
  mFlySpeed = 0.0f;
  mClipNode = CEClipedNode::create();
  //by default don't clip anything
  mClipNode->setClipRect(new CCRect(0, 0, 1024, 768));
  addChild(mClipNode);
  contentNode = cocos2d::CCNode::create();
  contentNode->setPosition(cocos2d::ccp(0, clipRect.size.height));
  mClipNode->addChild(contentNode);

  auto listener = EventListenerTouchOneByOne::create();
  listener->onTouchBegan = cocos2d::CC_CALLBACK_2(ABScrollContent::onTouchBegan, this);
  listener->onTouchEnded = cocos2d::CC_CALLBACK_2(ABScrollContent::onTouchEnded, this);
  listener->onTouchMoved = cocos2d::CC_CALLBACK_2(ABScrollContent::onTouchMoved, this);
  listener->onTouchCancelled = cocos2d::CC_CALLBACK_2(ABScrollContent::onTouchEnded, this);
  _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

  setTouchEnabled(true);
  scheduleUpdate();
  return true;
}

void ABScrollContent::resetContentPosition() 
{
  contentNode->setPosition(cocos2d::ccp(0, clipRect.size.height));
}

void ABScrollContent::addContent(cocos2d::CCNode * content) 
{
  contentNode->addChild(content);
}

void ABScrollContent::removeContent(cocos2d::CCNode * content) 
{
  contentNode->removeChild(content);
}

void ABScrollContent::removeAllContent() 
{
  contentNode->removeAllChildrenWithCleanup(true);
}

void ABScrollContent::setClipRect(cocos2d::CCRect rect) 
{
  clipRect = rect;
  mClipNode->setClipRect(new CCRect(UniversalFit::sharedUniversalFit()->transformRect(clipRect)));

  contentNode->setPosition(cocos2d::ccp(0, clipRect.size.height));
}

void ABScrollContent::update(float delta) 
{
  if( mFly )
  {
    float yy = contentNode->getPosition().y - clipRect.size.height;
    float minY = 0;
    float maxY = getContentSize().height-clipRect.size.height;
    if( maxY < 0 )
    {
      maxY = 0;
    }

    //降速
    float fade = delta*3000;
    if( mFlySpeed > 0 )
    {
      mFlySpeed -= fade;
      if( mFlySpeed < 0 )
      {
        mFlySpeed = 0;
      }
    }
    if( mFlySpeed < 0 )
    {
      mFlySpeed += fade;
      if( mFlySpeed > 0 )
      {
        mFlySpeed = 0;
      }
    }
    //速度修正
    if( yy < minY )
    {
      float k = minY - yy;
      float a = delta*k*100;
      mFlySpeed += a;
      float ms = 50+k*20;
      if( mFlySpeed > ms )
      {
        mFlySpeed = ms;
      }
    }
    if( yy > maxY )
    {
      float k = yy - maxY;
      float a = delta*k*100;
      mFlySpeed -= a;
      float ms = -50-k*20;
      if( mFlySpeed < ms )
      {
        mFlySpeed = ms;
      }
    }

    //运动
    float dy = mFlySpeed*delta;
    cocos2d::Point np = contentNode->getPosition();
    np.y += dy;
    contentNode->setPosition(np);

    float chy = np.y - clipRect.size.height;

    CCLog("Fly, %f, %f %f %f", mFlySpeed, chy, minY, maxY );

    if( fabsf(mFlySpeed)<1000 && chy >= minY && chy <= maxY )
    {
      mFly = false;
    }

    //this->updateScorll();
  }
}

bool ABScrollContent::onTouchBegan(Touch * touch, Event * event) 
{
  cocos2d::Point pos = touch->getLocationInView();
  pos = cocos2d::CCDirector::sharedDirector()->convertToGL(pos);
  pos = UniversalFit::sharedUniversalFit()->restorePoint(pos);

  mTouchBegin = pos;
  if( clipRect.containsPoint(pos) )
  {
    mBeginPressY = pos.y;
    mBeginNodeY = contentNode->getPosition().y;
    mFly = false;
    mLastY = -20000;
    return true;
  }
  return false;
}

void ABScrollContent::onTouchMoved(Touch * touch, Event * event) 
{
  cocos2d::Point pos = touch->getLocationInView();
  pos = cocos2d::CCDirector::sharedDirector()->convertToGL(pos);
  pos = UniversalFit::sharedUniversalFit()->restorePoint(pos);

  float dy = pos.y - mBeginPressY;
  float y = mBeginNodeY + dy;
  cocos2d::Point np = contentNode->getPosition();
  np.y = y;
  contentNode->setPosition(np);
  //CFAbsoluteTime time = CFAbsoluteTimeGetCurrent();
    struct cc_timeval time;
    CCTime::gettimeofdayCocos2d(&time, NULL);
  if( mLastY > -10000 )
  {
    float ds = np.y - mLastY;
    //float dt = time - mLastTime;
      float dt = cocos2d::CCTime::timersubCocos2d(&mLastTime, &time)/1000.0;
    mFlySpeed = ds/dt;
  }
  mLastY = np.y;
  mLastTime = time;
  //this->updateScorll();
}

void ABScrollContent::onTouchEnded(Touch * touch, Event * event) 
{
  mFly = true;
  cocos2d::Point pos = touch->getLocationInView();
  pos = cocos2d::CCDirector::sharedDirector()->convertToGL(pos);
  pos = UniversalFit::sharedUniversalFit()->restorePoint(pos);

  if( ccpLengthSQ(ccpSub(pos, mTouchBegin)) < 10*10 &&
      clipRect.containsPoint(pos) )//only available in achievement state
  {
    cocos2d::Point offset = contentNode->convertToWorldSpaceAR(cocos2d::ccp(0, 0));
    offset = UniversalFit::sharedUniversalFit()->restorePoint(offset);
    cocos2d::Point fixed = ccpSub(pos, offset);
    fixed.y *= -1;//reverse the y coordinate
    clickPoint = fixed;
    if( clickTarget != NULL )
    {
      CCCallFunc *callSelectorAction = cocos2d::CCCallFunc::create(clickTarget, clickMethod);
      this->runAction(callSelectorAction);
    }
  }
}

