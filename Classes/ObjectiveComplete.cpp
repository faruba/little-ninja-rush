//
//  ObjectiveComplete.m
//  NinjiaRush
//
//  Created by 马 颂文 on 12-8-2.
//  Copyright (c) 2012年 TrinGame. All rights reserved.
//

#include "ObjectiveComplete.h"
#include "GameTool.h"
#include "GamePlay.h"
#include "GameOver.h"
#include "GameRecord.h"
#include "AnimatedParticle.h"
#include "UniversalFit.h"
#include "GameConstants.h"

#define STATE_INIT (0)
#define STATE_MOVE (1)
#define STATE_CROWN (2)
#define STATE_BOUNS (3)
#define STATE_FLIP (4)
#define STATE_DELAY (5)
#define STATE_OUT (6)

#define MOVESPEED (2000)
#define FLIPSPEED (200)

int gCrownBouns[] = {50, 100, 200};



ObjectiveComplete* ObjectiveComplete::commitObjective() 
{
    return ObjectiveComplete::create();
}

void ObjectiveComplete::onCreate() 
{
    GamePlay *play = GamePlay::sharedGamePlay();
    play->scheduleMask(ccc3(0, 0, 0), 128, 0);
    CCNodeLoaderLibrary *pNodeLib = cocos2d::CCNodeLoaderLibrary::sharedCCNodeLoaderLibrary();
    CCBReader *pReader = new CCBReader(pNodeLib, this, this);
    mNode = pReader->readNodeGraphFromFile("ui-scroll", this);
    pReader->release();
    mNode->setPosition(cocos2d::ccp(UniversalFit::sharedUniversalFit()->baseLeft, 0));
    play->addChild(mNode, LAYER_MASK+1);
    CCRect clip = UniversalFit::sharedUniversalFit()->transformRect(cocos2d::CCRectMake(0, 142, UniversalFit::sharedUniversalFit()->playSize.width, 36));
    mObjRect = CEClipedNode::create();
    mObjRect->setClipRect(&clip);
    mObjRect->setPosition(cocos2d::ccp(12, 12));
    mBanner->addChild(mObjRect);
    
    mState = 0;
    mCurrIndex = 0;
    mSoundTimer = 0;
}

void ObjectiveComplete::onUpdate(float delta) 
{
    GamePlay *play = GamePlay::sharedGamePlay();
    //jump coins
    if( mCurrBounsCoins != mTargetBounsCoins )
    {
        mSoundTimer += delta;
        if( mSoundTimer > 0.1 )
        {
            SimpleAudioEngine::sharedEngine()->playEffect(cocos2d::CCFileUtils::sharedFileUtils()->fullPathForFilename("sound/getcoin.mp3").c_str());
            mSoundTimer = 0;
        }
        int dst = mTargetBounsCoins - mCurrBounsCoins;
        int speed = 200 + dst/3;
        int ds = speed*delta;
        if( dst <= ds )
        {
            mCurrBounsCoins = mTargetBounsCoins;
        }
        else {
            mCurrBounsCoins += ds;
        }
        mBounsCoins->setString(cocos2d::CCString::createWithFormat("%04d", mCurrBounsCoins)->getCString());
    }
    if( mCurrBounsScrolls != mTargetBounsScrolls )
    {
        mScroll->setVisible(true);
        mBounsScroll->setString(cocos2d::CCString::createWithFormat("x%d", mTargetBounsScrolls)->getCString());
        mCurrBounsScrolls = mTargetBounsScrolls;
    }
    
    switch (mState) {
        case STATE_INIT:
        {
            bool foundcomplete = false;
            for(int i=mCurrIndex; i<3; ++i)
            {
                this->retriveObjectiveInfo(i, &mCurrObj, &mCurrAch);
                if( mCurrAch != NULL && mCurrObj->count >= mCurrAch->achieveNumber )
                {
                    foundcomplete = true;
                    mCurrIndex = i;
                    
                    break;
                }
            }
            if( foundcomplete )
            {
                //--- to move ---
                mState = STATE_MOVE;
                //add objective detail
                CCSpriteFrameCache *cache = cocos2d::CCSpriteFrameCache::sharedSpriteFrameCache();
                mObjType->setDisplayFrame(cache->spriteFrameByName(cocos2d::CCString::createWithFormat("objc-%d.png", mCurrIndex)->getCString()));
                mObjRect->setPosition(cocos2d::ccp(12, 12));
                CCNode *currobj = this->genObjectiveInfo(mCurrAch, mCurrIndex);
                currobj->setPosition(cocos2d::ccp(18, 18));
                mObjRect->addChild(currobj);
                cocos2d::Point np = mBanner->getPosition();
                np.x = -240;
                mBanner->setPosition(np);
                //add crowns
                for(int i=0; i<mCurrObj->index; ++i)
                {
                    CCSprite *crown = cocos2d::CCSprite::createWithSpriteFrameName(cocos2d::CCString::createWithFormat("crown%d.png", i)->getCString());
                    crown->setPosition(cocos2d::ccp(-20*i, 0));
                    mCrowns->addChild(crown);
                }
                mStateTimer = 0;
                SimpleAudioEngine::sharedEngine()->playEffect(cocos2d::CCFileUtils::sharedFileUtils()->fullPathForFilename("sound/flip.mp3").c_str());
            }
            else {
                //Finish of ObjectiveComplete
                play->manager->removeGameObject(this);
            }
        }
            break;
        case STATE_MOVE:
        {
            mStateTimer += delta;
            float dst = 240 - mBanner->getPosition().x;
            float stp = delta*MOVESPEED;
            cocos2d::Point np = mBanner->getPosition();
            if( dst <= stp )
            {
                np.x = 240;
                mBanner->setPosition(np);
                
                //--- to crown ---
                mState = STATE_CROWN;
                mStateTimer = 0;
            }
            else {
                np.x += stp;
                mBanner->setPosition(np);
            }
        }
            break;
        case STATE_CROWN:
        {
            mStateTimer += delta;
            if( mStateTimer >= 2 )
            {
                SimpleAudioEngine::sharedEngine()->playEffect(cocos2d::CCFileUtils::sharedFileUtils()->fullPathForFilename("sound/getcrown.mp3").c_str());
                CCString *crownname = cocos2d::CCString::createWithFormat("crown%d.png", mCurrObj->index);
                CCSprite *crown = cocos2d::CCSprite::createWithSpriteFrameName(crownname->getCString());
                crown->setPosition(cocos2d::ccp(-20*mCurrObj->index, 0));
                mCrowns->addChild(crown);
                for(int i=0; i<10; ++i)
                {
                    play->manager->addGameObject(AnimatedParticle::particleCoinUI(crown->getPosition(), i, mCrowns));
                }
                
                //--- to bouns ---
                mState = STATE_BOUNS;
                mStateTimer = 0;
                mStateFlag = false;
            }
        }
            break;
        case STATE_BOUNS:
        {
            mStateTimer += delta;
            if( !mStateFlag && mStateTimer > 1 )
            {
                int bounscoins = mCurrAch->prizeCount;
                bounscoins += mCurrObj->index * gCrownBouns[mCurrIndex];
                mTargetBounsCoins += bounscoins;
                if( mCurrIndex == 1 && mCurrObj->index == 2 )
                {
                    mTargetBounsScrolls++;
                    GameRecord::sharedGameRecord()->collection->magic_piece++;
                }
                if( mCurrIndex == 2 && mCurrObj->index > 0 )
                {
                    mTargetBounsScrolls+=mCurrObj->index;
                    GameRecord::sharedGameRecord()->collection->magic_piece+=mCurrObj->index;
                }
                
                mStateFlag = true;
            }
            if( mStateTimer > 3 )
            {
                //--- to flip ---
                mState = STATE_FLIP;
                mStateTimer = 0;
                if( mCurrObj->index < 2 )
                {
                    switch (mCurrIndex) {
                        case 0:
                        {
                            GameRecord::sharedGameRecord()->task->refreshDailyObjective();
                            mCurrObj = GameRecord::sharedGameRecord()->task->dailyObjective;
                            mCurrAch = Tasks::dailyObjectiveWithUiid(mCurrObj->uiid);
                        }
                            break;
                        case 1:
                        {
                            GameRecord::sharedGameRecord()->task->refreshWeeklyObjective();
                            mCurrObj = GameRecord::sharedGameRecord()->task->weeklyObjective;
                            mCurrAch = Tasks::weeklyObjectiveWithUiid(mCurrObj->uiid);
                        }
                            break;
                        case 2:
                        {
                            GameRecord::sharedGameRecord()->task->refreshMonthlyObjective();
                            mCurrObj = GameRecord::sharedGameRecord()->task->monthlyObjective;
                            mCurrAch = Tasks::monthlyObjectiveWithUiid(mCurrObj->uiid);
                        }
                            break;
                    }
                    CCNode *newobj = this->genObjectiveInfo(mCurrAch, mCurrIndex);
                    newobj->setPosition(cocos2d::ccp(18, 54));
                    mObjRect->addChild(newobj);
                }
                else {
                    mCurrAch = NULL;
                    CCNode *newobj = this->genObjectiveInfo(NULL, mCurrIndex);
                    newobj->setPosition(cocos2d::ccp(18, 54));
                    mObjRect->addChild(newobj);
                    //mark completed
                    switch (mCurrIndex) {
                        case 0:
                        {
                            GameRecord::sharedGameRecord()->task->dailyObjective->index = 3;
                            GameRecord::sharedGameRecord()->task->dailyObjective->uiid = -1;
                        }
                            break;
                        case 1:
                        {
                            GameRecord::sharedGameRecord()->task->weeklyObjective->index = 3;
                            GameRecord::sharedGameRecord()->task->weeklyObjective->uiid = -1;
                        }
                            break;
                        case 2:
                        {
                            GameRecord::sharedGameRecord()->task->monthlyObjective->index = 3;
                            GameRecord::sharedGameRecord()->task->monthlyObjective->uiid = -1;
                        }
                            break;
                    }
                }
            }
        }
            break;
        case STATE_FLIP:
        {
            float dst = -24 - mObjRect->getPosition().y;
            float stp = delta*FLIPSPEED;
            cocos2d::Point np = mObjRect->getPosition();
            if( fabsf(dst) <= stp )
            {
                np.y = -24;
                mObjRect->setPosition(np);
                
                //--- to delay ---
                mState = STATE_DELAY;
                mStateTimer = 0;
            }
            else {
                np.y -= stp;
                mObjRect->setPosition(np);
            }
        }
            break;
        case STATE_DELAY:
        {
            mStateTimer += delta;
            if( mStateTimer >= 2 )
            {
                mState = STATE_OUT;
                SimpleAudioEngine::sharedEngine()->playEffect(cocos2d::CCFileUtils::sharedFileUtils()->fullPathForFilename("sound/flip.mp3").c_str());
            }
        }
            break;
        case STATE_OUT:
        {
            float dst = 720 - mBanner->getPosition().x;
            float stp = delta*MOVESPEED;
            cocos2d::Point np = mBanner->getPosition();
            if( dst <= stp )
            {
                np.x = -240;
                mBanner->setPosition(np);
                
                //--- to crown ---
                mObjRect->removeAllChildrenWithCleanup(true);
                mCrowns->removeAllChildrenWithCleanup(true);
                mState = STATE_INIT;
                mCurrIndex++;
            }
            else {
                np.x += stp;
                mBanner->setPosition(np);
            }
        }
            break;
    }
}

void ObjectiveComplete::retriveObjectiveInfo(int index, Objective ** obj, Achievement ** ach) 
{
    *obj = NULL;
    *ach = NULL;
    switch (index) {
        case 0:
        {
            *obj = GameRecord::sharedGameRecord()->task->dailyObjective;
            *ach = Tasks::dailyObjectiveWithUiid((*obj)->uiid);
        }
            break;
        case 1:
        {
            *obj = GameRecord::sharedGameRecord()->task->weeklyObjective;
            *ach = Tasks::weeklyObjectiveWithUiid((*obj)->uiid);
        }
            break;
        case 2:
        {
            *obj = GameRecord::sharedGameRecord()->task->monthlyObjective;
            *ach = Tasks::monthlyObjectiveWithUiid((*obj)->uiid);
        }
            break;
    }
}

CCNode* ObjectiveComplete::genObjectiveInfo(Achievement * ach, int typ) 
{
    if( ach == NULL )
    {
        CCNode *sp = cocos2d::CCNode::create();
        switch (typ) {
            case 0:
            {
                CCLabelTTF *desc = cocos2d::CCLabelTTF::create("日常任务\n已全部完成！", GFONT_NAME, GFONT_SIZE_NORMAL );
                desc->setColor(ccc3(128, 128, 128));
                desc->setAnchorPoint(cocos2d::ccp(0, 0.5f));
                desc->setPosition(cocos2d::ccp(54, 0));
                sp->addChild(desc);
            }
                break;
            case 1:
            {
                CCLabelTTF *desc = cocos2d::CCLabelTTF::create("周常任务\n已全部完成！", GFONT_NAME, GFONT_SIZE_NORMAL);
                desc->setColor(ccc3(128, 128, 128));
                desc->setAnchorPoint(cocos2d::ccp(0, 0.5f));
                desc->setPosition(cocos2d::ccp(54, 0));
                sp->addChild(desc);
            }
                break;
            case 2:
            {
                CCLabelTTF *desc = cocos2d::CCLabelTTF::create("月常任务\n已全部完成！", GFONT_NAME, GFONT_SIZE_NORMAL);
                desc->setColor(ccc3(128, 128, 128));
                desc->setAnchorPoint(cocos2d::ccp(0, 0.5f));
                desc->setPosition(cocos2d::ccp(54, 0));
                sp->addChild(desc);
            }
                break;
        }
        return sp;
    }
    else {
        CCSprite *spbg = NULL;
        switch (typ) {
            case 0:
            {
                spbg = cocos2d::CCSprite::createWithSpriteFrameName("ms_bg1.png");
            }
                break;
            case 1:
            {
                spbg = cocos2d::CCSprite::createWithSpriteFrameName("ms_bg3.png");
            }
                break;
            case 2:
            {
                spbg = cocos2d::CCSprite::createWithSpriteFrameName("ms_bg2.png");
            }
                break;
        }
        CCSprite *spicon = cocos2d::CCSprite::createWithSpriteFrameName(ach->icon->getCString());
        spicon->setPosition(cocos2d::ccp(spbg->getContentSize().width/2, spbg->getContentSize().height/2));
        spbg->addChild(spicon, 0, 0);
        CCLabelBMFont *desc = cocos2d::CCLabelBMFont::create(ach->desc->getCString(), "ab34.fnt");
        desc->setAnchorPoint(cocos2d::ccp(0, 0.5f));
        desc->setPosition(cocos2d::ccp(54, 12));
        spbg->addChild(desc, 0, 1);
        
        return spbg;
    }
}

void ObjectiveComplete::onDestroy() 
{
    GamePlay *play = GamePlay::sharedGamePlay();
    play->unscheduleMask();
    play->coins += mTargetBounsCoins;
    play->removeChild(mNode, true);
    play->manager->addGameObject(GameOver::gameOver(play));
}

SEL_MenuHandler ObjectiveComplete::onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char* pSelectorName)
{
    //  CCLog(pSelectorName);
  return NULL;
}

SEL_CCControlHandler ObjectiveComplete::onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char* pSelectorName)
{
  CCLog("Control");
  return NULL;
}
bool ObjectiveComplete::onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, CCNode* pNode)
{
  CCB_MEMBERVARIABLEASSIGNER_GLUE(this,"mBanner", CCSprite *, mBanner)
  CCB_MEMBERVARIABLEASSIGNER_GLUE(this,"mObjType", CCSprite *, mObjType)
  CCB_MEMBERVARIABLEASSIGNER_GLUE(this,"mCrowns", CCNode *, mCrowns)
  CCB_MEMBERVARIABLEASSIGNER_GLUE(this,"mBounsCoins", CCLabelBMFont *, mBounsCoins)
  CCB_MEMBERVARIABLEASSIGNER_GLUE(this,"mScroll", CCSprite *, mScroll)
  CCB_MEMBERVARIABLEASSIGNER_GLUE(this,"mBounsScroll", CCLabelBMFont *, mBounsScroll)
  //  CCLog(pMemberVariableName);

  return false;
}
