//
//  ClassicContinue.m
//  NinjiaRush
//
//  Created by 马 颂文 on 12-12-4.
//  Copyright (c) 2012年 TrinGame. All rights reserved.
//

#include "ClassicContinue.h"
#include "GamePlay.h"
#include "GameRecord.h"
#include "GameOver.h"
#include "ObjectiveComplete.h"
#include "UniversalFit.h"
//#include "ABSystem.h"

class ControlLayer : 
  public CCLayer,
  public CCBMemberVariableAssigner,
  public CCBSelectorResolver
{
  public:
    CREATE_FUNC(ControlLayer);

    virtual bool onAssignCCBMemberVariable(cocos2d::CCObject*, const char*, cocos2d::CCNode*);
    virtual SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject *, const char*);
    virtual SEL_CCControlHandler onResolveCCBCCControlSelector(cocos2d::CCObject *, const char*);

    virtual bool onTouchBegan(Touch * touch, Event * event);
    virtual void onTouchMoved(Touch * touch, Event * event);
    virtual void onTouchEnded(Touch * touch, Event * event);
    
    virtual bool init();
    void updateButton();
    void tickNumber(int n);

    CCLabelBMFont *mNumber;
    CCSprite *mBtnCancel;
    CCSprite *mBtnCancel2;
    CCSprite *mBtnRevive;
    CCSprite *mBtnRevive2;
    CCLabelBMFont *mCoins;
    CCSprite *mBtnBuyBasic;
    CCSprite *mBtnBuyBasic2;
    CCSprite *mBtnBuyStandard;
    CCSprite *mBtnBuyStandard2;
    CCSprite *mBtnBuyDeluxe;
    CCSprite *mBtnBuyDeluxe2;

    ClassicContinue *master;
    int mClicked;
};

bool ControlLayer::init() 
{
    if( CCLayer::init() )
    {
      CCNodeLoaderLibrary *pNodeLib = cocos2d::CCNodeLoaderLibrary::sharedCCNodeLoaderLibrary();
      CCBReader *pReader = new CCBReader(pNodeLib, this, this);
      CCNode *node = pReader->readNodeGraphFromFile("ui-continue.ccbi", this);
      pReader->release();
      node->setPosition(cocos2d::ccp(UniversalFit::sharedUniversalFit()->baseLeft, 0));
      this->addChild(node);

      auto listener = EventListenerTouchOneByOne::create();
      listener->onTouchBegan = cocos2d::CC_CALLBACK_2(ControlLayer::onTouchBegan, this);
      listener->onTouchEnded = cocos2d::CC_CALLBACK_2(ControlLayer::onTouchEnded, this);
      listener->onTouchMoved = cocos2d::CC_CALLBACK_2(ControlLayer::onTouchMoved, this);
      listener->onTouchCancelled = cocos2d::CC_CALLBACK_2(ControlLayer::onTouchEnded, this);
      _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

      mClicked = -1;

      this->setTouchEnabled(true);
      return true;
    }
    return false;
}

void ControlLayer::tickNumber(int n) 
{
    mNumber->setString(cocos2d::CCString::createWithFormat("%d", n)->getCString());
    CCScaleTo *st1 = cocos2d::CCScaleTo::create(0.2f, 1.5f);
    CCScaleTo *st2 = cocos2d::CCScaleTo::create(0.4f, 1);
    CCSequence *seq = cocos2d::CCSequence::create(st1, st2, NULL);
    mNumber->runAction(seq);
    
    SimpleAudioEngine::sharedEngine()->playEffect(cocos2d::CCFileUtils::sharedFileUtils()->fullPathForFilename("sound/feverbgmend.mp3").c_str());
}

void ControlLayer::updateButton() 
{
    if( GameRecord::sharedGameRecord()->item_credit > 0 )
    {
        mBtnRevive->setVisible(true);
        mBtnRevive2->setVisible(false);
        
        mBtnBuyBasic->setVisible(false);
        mBtnBuyStandard->setVisible(false);
        mBtnBuyDeluxe->setVisible(false);
        
        mCoins->setString(cocos2d::CCString::createWithFormat("%d", GameRecord::sharedGameRecord()->item_credit)->getCString());
    }
    else
    {
        mBtnRevive->setVisible(false);
        
        mBtnBuyBasic->setVisible(true);
        mBtnBuyBasic2->setVisible(false);
        mBtnBuyStandard->setVisible(true);
        mBtnBuyStandard2->setVisible(false);
        mBtnBuyDeluxe->setVisible(true);
        mBtnBuyDeluxe2->setVisible(false);
    }
}

bool ControlLayer::onTouchBegan(Touch * touch, Event * event) 
{
    LNR_GET_TOUCH_POS;
    
    if( mBtnCancel->boundingBox().containsPoint(pos) )
    {
        mBtnCancel2->setVisible(true);
        mClicked = 0;
        return true;
    }
    if( mBtnRevive->isVisible() && mBtnRevive->boundingBox().containsPoint(pos) )
    {
        mBtnRevive2->setVisible(true);
        mClicked = 1;
        return true;
    }
    if( mBtnBuyBasic->isVisible() && mBtnBuyBasic->boundingBox().containsPoint(pos) )
    {
        mBtnBuyBasic2->setVisible(true);
        mClicked = 2;
        return true;
    }
    if( mBtnBuyStandard->isVisible() && mBtnBuyStandard->boundingBox().containsPoint(pos) )
    {
        mBtnBuyStandard2->setVisible(true);
        mClicked = 3;
        return true;
    }
    if( mBtnBuyDeluxe->isVisible() && mBtnBuyDeluxe->boundingBox().containsPoint(pos) )
    {
        mBtnBuyDeluxe2->setVisible(true);
        mClicked = 4;
        return true;
    }
    return false;
}

void ControlLayer::onTouchMoved(Touch * touch, Event * event) 
{
    LNR_GET_TOUCH_POS;
    
    switch (mClicked) {
        case 0:
        {
            if( mBtnCancel->boundingBox().containsPoint(pos) )
            {
                mBtnCancel2->setVisible(true);
            }
            else
            {
                mBtnCancel2->setVisible(false);
            }
        }
            break;
        case 1:
        {
            if( mBtnRevive->boundingBox().containsPoint(pos) )
            {
                mBtnRevive2->setVisible(true);
            }
            else
            {
                mBtnRevive2->setVisible(false);
            }
        }
            break;
        case 2:
        {
            if( mBtnBuyBasic->boundingBox().containsPoint(pos) )
            {
                mBtnBuyBasic2->setVisible(true);
            }
            else
            {
                mBtnBuyBasic2->setVisible(false);
            }
        }
            break;
        case 3:
        {
            if( mBtnBuyStandard->boundingBox().containsPoint(pos) )
            {
                mBtnBuyStandard2->setVisible(true);
            }
            else
            {
                mBtnBuyStandard2->setVisible(false);
            }
        }
            break;
        case 4:
        {
            if( mBtnBuyDeluxe->boundingBox().containsPoint(pos) )
            {
                mBtnBuyDeluxe2->setVisible(true);
            }
            else
            {
                mBtnBuyDeluxe2->setVisible(false);
            }
        }
            break;
    }
}

void ControlLayer::onTouchEnded(Touch * touch, Event * event) 
{
  LNR_GET_TOUCH_POS;
    
    switch (mClicked) {
        case 0:
        {
            if( mBtnCancel->boundingBox().containsPoint(pos) )
            {
                master->onButton(mClicked);
            }
            mBtnCancel2->setVisible(false);
        }
            break;
        case 1:
        {
            if( mBtnRevive->boundingBox().containsPoint(pos) )
            {
                master->onButton(mClicked);
            }
            mBtnRevive2->setVisible(false);
        }
            break;
        case 2:
        {
            if( mBtnBuyBasic->boundingBox().containsPoint(pos) )
            {
                master->onButton(mClicked);
            }
            mBtnBuyBasic2->setVisible(false);
        }
            break;
        case 3:
        {
            if( mBtnBuyStandard->boundingBox().containsPoint(pos) )
            {
                master->onButton(mClicked);
            }
            mBtnBuyStandard2->setVisible(false);
        }
            break;
        case 4:
        {
            if( mBtnBuyDeluxe->boundingBox().containsPoint(pos) )
            {
                master->onButton(mClicked);
            }
            mBtnBuyDeluxe2->setVisible(false);
        }
            break;
    }
    mClicked = -1;
}

SEL_MenuHandler ControlLayer::onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char* pSelectorName)
{
  //  CCLog(pSelectorName);
  return NULL;
}

SEL_CCControlHandler ControlLayer::onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char* pSelectorName)
{
  CCLog("Control");
  return NULL;
}
bool ControlLayer::onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, CCNode* pNode)
{
  CCB_MEMBERVARIABLEASSIGNER_GLUE(this,"mNumber", CCLabelBMFont *, mNumber)
  CCB_MEMBERVARIABLEASSIGNER_GLUE(this,"mBtnCancel", CCSprite *, mBtnCancel)
  CCB_MEMBERVARIABLEASSIGNER_GLUE(this,"mBtnCancel2", CCSprite *, mBtnCancel2)
  CCB_MEMBERVARIABLEASSIGNER_GLUE(this,"mBtnRevive", CCSprite *, mBtnRevive)
  CCB_MEMBERVARIABLEASSIGNER_GLUE(this,"mBtnRevive2", CCSprite *, mBtnRevive2)
  CCB_MEMBERVARIABLEASSIGNER_GLUE(this,"mCoins", CCLabelBMFont *, mCoins)
  CCB_MEMBERVARIABLEASSIGNER_GLUE(this,"mBtnBuyBasic", CCSprite *, mBtnBuyBasic)
  CCB_MEMBERVARIABLEASSIGNER_GLUE(this,"mBtnBuyBasic2", CCSprite *, mBtnBuyBasic2)
  CCB_MEMBERVARIABLEASSIGNER_GLUE(this,"mBtnBuyStandard", CCSprite *, mBtnBuyStandard)
  CCB_MEMBERVARIABLEASSIGNER_GLUE(this,"mBtnBuyStandard2", CCSprite *, mBtnBuyStandard2)
  CCB_MEMBERVARIABLEASSIGNER_GLUE(this,"mBtnBuyDeluxe", CCSprite *, mBtnBuyDeluxe)
  CCB_MEMBERVARIABLEASSIGNER_GLUE(this,"mBtnBuyDeluxe2", CCSprite *, mBtnBuyDeluxe2)
  //  CCLog(pMemberVariableName);

  return false;
}

//------------------------------



ClassicContinue* ClassicContinue::classicContinue() 
{
    return ClassicContinue::create();
}

void ClassicContinue::onCreate() 
{
    GamePlay *play = GamePlay::sharedGamePlay();
    play->scheduleMask(ccc3(0, 0, 0), 128, 0);
    
    mLayer = ControlLayer::create();
    mLayer->master = this;
    play->addChild(mLayer, LAYER_MASK+1);
    
    mLayer->updateButton();
    
    mState = 0;
    mTimer = 0;
    mCounter = 10;
    mLayer->tickNumber(mCounter);
}

void ClassicContinue::onUpdate(float delta) 
{
    switch (mState) {
        case 0:
        {
            mTimer += delta;
            if( mTimer > 1.5f )
            {
                mCounter--;
                mTimer = 0;
                mLayer->tickNumber(mCounter);
            }
            if( mCounter < 0 )
            {
                //continue over
                this->onCancel();
                return;
            }
        }
            break;
        case 1:
        {
            //do nothing
        }
            break;
    }
}

void ClassicContinue::onDestroy() 
{
    GamePlay::sharedGamePlay()->removeChild(mLayer, true);
}

void ClassicContinue::onUseCredit() 
{
    if( GameRecord::sharedGameRecord()->item_credit > 0 )
    {
        GamePlay *play = GamePlay::sharedGamePlay();
        
        play->state = STATE_REVIVE;
        play->substate = 0;
        play->stateTimer = 0;
        play->gameOverTimer = -1;
        
        play->mainrole->HP = play->mainrole->maxHP;
        play->mainrole->SP = play->mainrole->maxSP;
        play->mainrole->dart = play->mainrole->maxDart;
        
        play->mainrole->customAction(1);
        play->count_runscene--;
        play->gameOverTimer=-1;
        play->mainrole->timedInvincible(5);
        
        GameRecord::sharedGameRecord()->item_credit--;
        GameRecord::sharedGameRecord()->checkPoint();
        
        play->unscheduleMask();
        
        play->manager->removeGameObject(this);
        SimpleAudioEngine::sharedEngine()->playEffect(cocos2d::CCFileUtils::sharedFileUtils()->fullPathForFilename("sound/charge.mp3").c_str());
    }
}

void ClassicContinue::onCancel() 
{
    GamePlay *play = GamePlay::sharedGamePlay();
    play->unscheduleMask();
    
    play->triggerDeath();
    if( GamePlay::sharedGamePlay()->completeSomeObjectives() )
    {
        play->manager->addGameObject(ObjectiveComplete::commitObjective());
    }
    else {
        play->manager->addGameObject(GameOver::gameOver(play));
    }
    
    play->manager->removeGameObject(this);
}

void ClassicContinue::onButton(int bid) 
{
    switch (bid) {
        case 0:
        {
            this->onCancel();
        }
            break;
        case 1:
        {
            this->onUseCredit();
        }
            break;
        case 2:
        {
            this->onBuyCredit1();
        }
            break;
        case 3:
        {
            this->onBuyCredit2();
        }
            break;
        case 4:
        {
            this->onBuyCredit3();
        }
            break;
    }
    SimpleAudioEngine::sharedEngine()->playEffect(cocos2d::CCFileUtils::sharedFileUtils()->fullPathForFilename("sound/click.mp3").c_str());
}

void ClassicContinue::onBuyCredit1() 
{
    //IAPHelper->sharedIAPHelper()->makePurchase(8, this);
    mState = 1;
}

void ClassicContinue::onBuyCredit2() 
{
    //IAPHelper->sharedIAPHelper()->makePurchase(9, this);
    mState = 1;
}

void ClassicContinue::onBuyCredit3() 
{
    //IAPHelper->sharedIAPHelper()->makePurchase(10, this);
    mState = 1;
}

void ClassicContinue::purchaseVerified(int pid) 
{
    GameRecord::sharedGameRecord()->purchaseVerified(pid);
    mLayer->updateButton();
    //ABSystem::popAlert("Success", "Purchase success.");
    
    mState = 0;
}

void ClassicContinue::purchaseCancelled(int pid) 
{
    //do nothing
    mState = 0;
    //ABSystem::popAlert("Failed", "Purchase cancelled.");
}

void ClassicContinue::purchaseFailed(int pid) 
{
    //do nothing
    mState = 0;
    //ABSystem::popAlert("Failed", "Purchase failed.");
}
