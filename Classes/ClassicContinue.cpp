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
  public Layer,
  public CCBMemberVariableAssigner,
  public CCBSelectorResolver
{
  public:
    CREATE_FUNC(ControlLayer);

    virtual bool onAssignCCBMemberVariable(cocos2d::Ref*, const char*, cocos2d::Node*);
    virtual SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::Ref *, const char*);
    virtual cocos2d::extension::Control::Handler onResolveCCBCCControlSelector(cocos2d::Ref * , const char* );

    virtual bool onTouchBegan(Touch * touch, Event * event);
    virtual void onTouchMoved(Touch * touch, Event * event);
    virtual void onTouchEnded(Touch * touch, Event * event);
    
    virtual bool init();
    void updateButton();
    void tickNumber(int n);
cocos2d::CCLabelBMFont *mNumber;
cocos2d::Sprite *mBtnCancel;
cocos2d::Sprite *mBtnCancel2;
cocos2d::Sprite *mBtnRevive;
cocos2d::Sprite *mBtnRevive2;
cocos2d::CCLabelBMFont *mCoins;
cocos2d::Sprite *mBtnBuyBasic;
cocos2d::Sprite *mBtnBuyBasic2;
cocos2d::Sprite *mBtnBuyStandard;
cocos2d::Sprite *mBtnBuyStandard2;
cocos2d::Sprite *mBtnBuyDeluxe;
cocos2d::Sprite *mBtnBuyDeluxe2;

    ClassicContinue *master;
    int mClicked;
};

bool ControlLayer::init() 
{
    if( Layer::init() )
    {
NodeLoaderLibrary *pNodeLib = NodeLoaderLibrary::sharedNodeLoaderLibrary();
cocosbuilder::CCBReader *pReader = new CCBReader(pNodeLib, this, this);
cocos2d::Node *node = pReader->readNodeGraphFromFile("ui-continue.ccbi", this);
      pReader->release();
      node->setPosition(cocos2d::Vec2(UniversalFit::sharedUniversalFit()->baseLeft, 0));
      this->addChild(node);

      auto listener = EventListenerTouchOneByOne::create();
      listener->onTouchBegan = CC_CALLBACK_2(ControlLayer::onTouchBegan, this);
      listener->onTouchEnded = CC_CALLBACK_2(ControlLayer::onTouchEnded, this);
      listener->onTouchMoved = CC_CALLBACK_2(ControlLayer::onTouchMoved, this);
      listener->onTouchCancelled = CC_CALLBACK_2(ControlLayer::onTouchEnded, this);
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
cocos2d::CCScaleTo *st1 = cocos2d::CCScaleTo::create(0.2f, 1.5f);
cocos2d::CCScaleTo *st2 = cocos2d::CCScaleTo::create(0.4f, 1);
cocos2d::CCSequence *seq = cocos2d::CCSequence::create(st1, st2, NULL);
    mNumber->runAction(seq);
    
    GameTool::PlaySound("sound/feverbgmend.mp3");
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

SEL_MenuHandler ControlLayer::onResolveCCBCCMenuItemSelector(cocos2d::Ref * pTarget, const char* pSelectorName)
{
  //  CCLog(pSelectorName);
  return NULL;
}

cocos2d::extension::Control::Handler   ControlLayer::onResolveCCBCCControlSelector(cocos2d::Ref * pTarget, const char* pSelectorName)
{
cocos2d::CCLog("Control");
  return NULL;
}
bool ControlLayer::onAssignCCBMemberVariable(cocos2d::Ref* pTarget, const char* pMemberVariableName, Node* pNode)
{
CCB_MEMBERVARIABLEASSIGNER_GLUE(this,"mNumber", CCLabelBMFont *, mNumber)
CCB_MEMBERVARIABLEASSIGNER_GLUE(this,"mBtnCancel", Sprite *, mBtnCancel)
CCB_MEMBERVARIABLEASSIGNER_GLUE(this,"mBtnCancel2", Sprite *, mBtnCancel2)
CCB_MEMBERVARIABLEASSIGNER_GLUE(this,"mBtnRevive", Sprite *, mBtnRevive)
CCB_MEMBERVARIABLEASSIGNER_GLUE(this,"mBtnRevive2", Sprite *, mBtnRevive2)
CCB_MEMBERVARIABLEASSIGNER_GLUE(this,"mCoins", CCLabelBMFont *, mCoins)
CCB_MEMBERVARIABLEASSIGNER_GLUE(this,"mBtnBuyBasic", Sprite *, mBtnBuyBasic)
CCB_MEMBERVARIABLEASSIGNER_GLUE(this,"mBtnBuyBasic2", Sprite *, mBtnBuyBasic2)
CCB_MEMBERVARIABLEASSIGNER_GLUE(this,"mBtnBuyStandard", Sprite *, mBtnBuyStandard)
CCB_MEMBERVARIABLEASSIGNER_GLUE(this,"mBtnBuyStandard2", Sprite *, mBtnBuyStandard2)
CCB_MEMBERVARIABLEASSIGNER_GLUE(this,"mBtnBuyDeluxe", Sprite *, mBtnBuyDeluxe)
CCB_MEMBERVARIABLEASSIGNER_GLUE(this,"mBtnBuyDeluxe2", Sprite *, mBtnBuyDeluxe2)
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
        GameTool::PlaySound("sound/charge.mp3");
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
    GameTool::PlaySound("sound/click.mp3");
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
