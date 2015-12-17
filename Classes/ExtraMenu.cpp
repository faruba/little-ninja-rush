#include "ExtraMenu.h"
#include "ExtraSelect.h"
#include "GameConstants.h"
#include "Tasks.h"
#include "GameRecord.h"
#include "UniversalFit.h"
#include "PublicLoad.h"

#define PADDING (6)

cocos2d::Scene* ExtraMenu::scene(int mode) 
{
cocos2d::Scene *ret = cocos2d::Scene::create();

  ExtraMenu *tm = ExtraMenu::create();
  tm->mMode = mode;

  //universal
  ret->setScale(UniversalFit::sharedUniversalFit()->scaleFactor);
  ret->setAnchorPoint(cocos2d::Vec2(0, 0));
  ret->setPosition(UniversalFit::sharedUniversalFit()->sceneOffset);

  CEClipedNode *clip = CEClipedNode::create();
  clip->setClipRect(&(UniversalFit::sharedUniversalFit()->clipRect));

  clip->addChild(tm);

  ret->addChild(clip);
  return ret;
}

bool ExtraMenu::init() 
{
  this->setTouchEnabled(true);
  this->scheduleUpdate();
  return true;
}

void ExtraMenu::onEnter() 
{
    PublicLoad::menuExtra()->loadAll();
cocos2d::Node * node = createUIByCCBI("menu-extra", "ExtraMenu", ExtraMenuLayerLoader::loader(), this);
  if(node != NULL) {
    this->addChild(node);
  }

  mClipedList = CEClipedNode::create();
  mClipedList->setClipRect(new CCRect((UniversalFit::sharedUniversalFit()->transformRect(cocos2d::CCRectMake(12, 12, 455, 264)))));
  mList->addChild(mClipedList);
  mItemList = cocos2d::Node::create();
  mClipedList->addChild(mItemList);
  mFly = false;

  mScrollBody = cocos2d::Sprite::createWithSpriteFrameName("sp_scroll2.png");
  mScrollBody->setAnchorPoint(cocos2d::Vec2(0, 1));
  mClipedList->addChild(mScrollBody);

  mSceneIntro = NULL;
  mIntroFlag = false;

  //--------------
  mOffset = 0;
  mItemList->removeAllChildrenWithCleanup(true);
  mCurrUnFold = -1;

  mCurrState = -1;

  switch (mMode) {
    case 0:
      {
        mBanner->setDisplayFrame(cocos2d::SpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("exb6.png"));
        this->loadAchievements();
      }
      break;
    case 1:
      {
        mBanner->setDisplayFrame(cocos2d::SpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("exb7.png"));
        this->loadStatistics();
      }
      break;
    case 2:
      {
        mBanner->setDisplayFrame(cocos2d::SpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("exb5.png"));
        this->loadAboutUs();
      }
      break;
  }

  //update scroll
  this->updateScorll();

  this->setSceneIntro();
cocos2d::Layer::onEnter();
}

void ExtraMenu::activate(int cid) 
{
//  FoldItem *newitem = (FoldItem*)(mItemList->getChildByTag(cid));
//  bool unfold = false;
//  if( newitem->isFoldable() )
//  {
//    SimpleAudioEngine::sharedEngine()->playEffect(cocos2d::CCFileUtils::sharedFileUtils()->fullPathForFilename("sound/ui-cancel.mp3").c_str());
//    if( cid == mCurrUnFold )
//    {
//      //fold
//      FoldItem *item = (FoldItem*)(mItemList->getChildByTag(cid));
//      item->toggleFold(true);
//      mCurrUnFold = -1;
//    }
//    else {
//      //unfold
//      if( mCurrUnFold >= 0 )
//      {
//        FoldItem *item = (FoldItem*)(mItemList->getChildByTag(mCurrUnFold));
//        item->toggleFold(true);
//        mCurrUnFold = -1;
//      }
//      newitem->toggleFold(false);
//      mCurrUnFold = cid;
//      unfold = true;
//    }
//    //rearrange items
//    mOffset = 0;
//cocos2d::Ref* node = NULL;
//CCARRAY_FOREACH(mItemList->getChildren(), node)
//    {
//      FoldItem *it = (FoldItem*)node;
//      it->setPosition(cocos2d::Vec2(0, mOffset));
//      mOffset -= it->getContentSize().height + PADDING;
//    }
//    if( unfold )
//    {
//      float upbound = -newitem->getPosition().y - mItemList->getPosition().y;
//      if( upbound < 0 )
//      {
//        cocos2d::Point np = mItemList->getPosition();
//        np.y = -newitem->getPosition().y;
//        mItemList->setPosition(np);
//      }
//      float downbound = upbound + newitem->getContentSize().height;
//      if( downbound > 264 )
//      {
//        cocos2d::Point np = mItemList->getPosition();
//        np.y = -newitem->getPosition().y + newitem->getContentSize().height - 264;
//        mItemList->setPosition(np);
//      }
//    }
//  }
}

void ExtraMenu::onExit() 
{
    PublicLoad::menuExtra()->unloadAll();
cocos2d::Layer::onExit();
}

void ExtraMenu::onBack() 
{
    if( !mIntroFlag )
    {
        SimpleAudioEngine::sharedEngine()->playEffect(cocos2d::CCFileUtils::sharedFileUtils()->fullPathForFilename("sound/menu-change.mp3").c_str());
        setSceneOutro(ExtraSelect::scene());
    }
}

void ExtraMenu::update(float delta) 
{
    if( mFly )
    {
        float yy = mItemList->getPosition().y;
        float minY = 0;
        float maxY = -mOffset-264;
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
        cocos2d::Point np = mItemList->getPosition();
        np.y += dy;
        mItemList->setPosition(np);
        
        if( fabsf(mFlySpeed)<100 && np.y >= minY && np.y <= maxY )
        {
            mFly = false;
        }
        
        this->updateScorll();
    }
}

void ExtraMenu::updateScorll() 
{
    float length = 264.0f*(264.0f/-mOffset)/100;
    float begin = -264.0f*(mItemList->getPosition().y/-mOffset);
    mScrollBody->setScaleY(length);
    mScrollBody->setPosition(cocos2d::Vec2(441, begin));
}

void ExtraMenu::loadAchievements() 
{
  mCurrState = 0;
  mItemList->removeAllChildrenWithCleanup(true);
  mOffset = 0;
cocos2d::Ref* node = NULL;
CCARRAY_FOREACH(Tasks::getAchievements(), node)
  {
    Achievement *ach = (Achievement *)node;
cocos2d::CCLog("*(%d) - %s", ach->achieveCode, ach->text->getCString());
      
    int achnum = ach->achieveNumber;
    if( ach->achieveCode > ACH_OVERLINE )
    {
      achnum = 1;
    }
    FoldItem *fi = FoldItem::foldItem(ach->name->getCString(), ach->icon->getCString(), ach->desc->getCString(), achnum, ach->achieveCount, this);
    fi->setPosition(cocos2d::Vec2(0, mOffset));
    mItemList->addChild(fi, 1, mItemList->getChildrenCount());
    mOffset -= fi->getContentSize().height + PADDING;
  }
}

void ExtraMenu::loadStatistics() 
{
    mCurrState = 1;
    mItemList->removeAllChildrenWithCleanup(true);
    mOffset = 0;
cocos2d::Ref* node = NULL;
CCARRAY_FOREACH(Tasks::getStatistics(), node)
    {
        Statistics *sta = (Statistics *)node;
        //CCLog("*(%d) - %s", sta->achieveCode, sta->name->getCString());
        
        if( sta->achieveCode != -1 )
        {//normal statistics
            //CCLabelBMFont *title = cocos2d::CCLabelBMFont::create(sta->name->getCString(), "ab34.fnt");
cocos2d::CCLabelTTF *title = cocos2d::CCLabelTTF::create(sta->name->getCString(), GFONT_NAME, GFONT_SIZE_NORMAL);
            title->setAnchorPoint(cocos2d::Vec2(0, 1));
            title->setPosition(cocos2d::Vec2(10, mOffset));
            mItemList->addChild(title, 1, mItemList->getChildrenCount());
            if( sta->achieveCode >= 0 )
            {
cocos2d::CCString *val = cocos2d::CCString::createWithFormat("%d%", sta->achieveCount, sta->psfx);
cocos2d::CCLabelBMFont *result = cocos2d::CCLabelBMFont::create(val->getCString(), "ab34.fnt");
                result->setAnchorPoint(cocos2d::Vec2(1, 1));
                result->setPosition(cocos2d::Vec2(426, mOffset));
                mItemList->addChild(result, 1, mItemList->getChildrenCount());
            }
            else if( sta->achieveCode == -2 )
            {
cocos2d::CCString *val = cocos2d::CCString::createWithFormat("%d%", GameRecord::sharedGameRecord()->combo_high, sta->psfx);
cocos2d::CCLabelBMFont *result = cocos2d::CCLabelBMFont::create(val->getCString(), "ab34.fnt");
                result->setAnchorPoint(cocos2d::Vec2(1, 1));
                result->setPosition(cocos2d::Vec2(426, mOffset));
                mItemList->addChild(result, 1, mItemList->getChildrenCount());
            }
            
            mOffset -= title->getContentSize().height + 5 + PADDING;
        }
        else {
            //category label
cocos2d::Sprite *line = cocos2d::Sprite::createWithSpriteFrameName("ex-line.png");
            line->setAnchorPoint(cocos2d::Vec2(0.5f, 1));
            line->setPosition(cocos2d::Vec2(218, mOffset - 10));
            mItemList->addChild(line, 1, mItemList->getChildrenCount());
cocos2d::Sprite *label = cocos2d::Sprite::createWithSpriteFrameName(sta->name->getCString());
            label->setAnchorPoint(cocos2d::Vec2(0.5f, 1));
            label->setPosition(cocos2d::Vec2(line->getContentSize().width/2, 20));
            line->addChild(label, 1, mItemList->getChildrenCount());
            
            mOffset -= line->getContentSize().height + PADDING + 20;
        }
    }
}

void ExtraMenu::loadAboutUs() 
{
    mCurrState = 2;
    mItemList->removeAllChildrenWithCleanup(true);
cocos2d::Node * node = createUIByCCBI("about", "ExtraMenu", ExtraMenuLayerLoader::loader(), this);
    if(node != NULL) {
      mItemList->addChild(node);
    }
    
    mOffset = -800;
}

void ExtraMenu::onFacebook() 
{
}

void ExtraMenu::onTwitter() 
{
}

void ExtraMenu::onMail() 
{
}

void ExtraMenu::onWeibo() 
{
}

bool ExtraMenu::onTouchBegan(Touch * touch, Event * event) 
{
  LNR_GET_TOUCH_POS;
    
    mTouchBegin = pos;
cocos2d::CCRect rect = cocos2d::CCRectMake(12, 12, 455, 264);
    if( rect.containsPoint(pos) )
    {
        mBeginPressY = pos.y;
        mBeginNodeY = mItemList->getPosition().y;
        mFly = false;
        mLastY = -20000;
        return true;
    }
    return false;
}

void ExtraMenu::onTouchMoved(Touch * touch, Event * event) 
{
//  LNR_GET_TOUCH_POS;
//    
//    float dy = pos.y - mBeginPressY;
//    float y = mBeginNodeY + dy;
//    cocos2d::Point np = mItemList->getPosition();
//    np.y = y;
//    mItemList->setPosition(np);
//    //CFAbsoluteTime time = CFAbsoluteTimeGetCurrent();
//    time_t time;
//cocos2d::CCTime::gettimeofdayCocos2d(&time, NULL);
//    if( mLastY > -10000 )
//    {
//        float ds = np.y - mLastY;
//        //float dt = time - mLastTime;
//        float dt = cocos2d::CCTime::timersubCocos2d(&mLastTime, &time)/1000.0;
//        mFlySpeed = ds/dt;
//    }
//    mLastY = np.y;
//    mLastTime = time;
//    this->updateScorll();
}

void ExtraMenu::onTouchEnded(Touch * touch, Event * event) 
{
//    mFly = true;
//  LNR_GET_TOUCH_POS;
//cocos2d::CCRect rect = cocos2d::CCRectMake(12, 12, 455, 264);
//    if( ccpLengthSQ(ccpSub(pos, mTouchBegin)) < 10*10 &&
//       rect.containsPoint(pos) && mCurrState == 0 )//only available in achievement state
//    {
//        float dy = mList->getPosition().y - pos.y;
//        float offset = 0;
//        int index = 0;
//cocos2d::Ref *node;
//CCARRAY_FOREACH(mItemList->getChildren(), node)
//        {
//cocos2d::Node *item = (cocos2d::Node*) node;
//            float upbound = offset - mItemList->getPosition().y;
//            float downbound = upbound + item->getContentSize().height + PADDING;
//            if( dy >= upbound && dy < downbound )
//            {
//                this->activate(index);
//                break;
//            }
//            offset += item->getContentSize().height + PADDING;
//            index++;
//        }
//    }
}

void ExtraMenu::setSceneIntro() 
{
  doSceneIntro(mSceneIntro, this);
}

void ExtraMenu::setSceneOutro(cocos2d::Scene* newscene) 
{
  if( mIntroFlag )
  {
    return;
  }

  mIntroFlag = true;

  mNewScene = doSceneOutro(newscene, mSceneIntro, (SEL_CallFunc)(&ExtraMenu::doneOutro), this);
}

void ExtraMenu::doneOutro() 
{
    mIntroFlag = false;
cocos2d::CCDirector::sharedDirector()->replaceScene(mNewScene);
    mNewScene->release();
}

SEL_MenuHandler ExtraMenu::onResolveCCBCCMenuItemSelector(cocos2d::Ref * pTarget, const char* pSelectorName)
{
//CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "onBack", ExtraMenu::onBack);

  //CCLog(pSelectorName);
    return NULL;
}

cocos2d::extension::Control::Handler   ExtraMenu::onResolveCCBCCControlSelector(cocos2d::Ref * pTarget, const char* pSelectorName)
{
cocos2d::CCLog("Control");
  return NULL;
}
bool ExtraMenu::onAssignCCBMemberVariable(cocos2d::Ref* pTarget, const char* pMemberVariableName, Node* pNode)
{
CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "mCoins", CCLabelBMFont *,  mCoins);
CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "mList", Node *, mList);
CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "mBanner", Sprite *, mBanner);

  //CCLog(pMemberVariableName);

  return false;
}
