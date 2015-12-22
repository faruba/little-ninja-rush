//
//  CommitArcade.m
//  NinjiaRush
//
//  Created by 马 颂文 on 12-11-29.
//  Copyright (c) 2012年 TrinGame. All rights reserved.
//

#include "CommitArcade.h"
#include "GamePlay.h"
#include "UniversalFit.h"
#include "GameRecord.h"
#include "GameData.h"
//#include "GameCenterController.h"
#include "GameOver.h"

#define COMMIT_ARCRADE_INCREASE_SPEED (2000.0f)



CommitArcade* CommitArcade::commit() 
{
    return CommitArcade::create();
}

void CommitArcade::onCreate() 
{
    //hot load
cocos2d::SpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("ui-scroll.plist");
    
    GamePlay *play = GamePlay::sharedGamePlay();
    play->scheduleMask(Color3B(0, 0, 0), 128, 0);
    NodeLoaderLibrary *pNodeLib = NodeLoaderLibrary::sharedNodeLoaderLibrary();
CCBReader *pReader = new CCBReader(pNodeLib, this, this);
    mNode = pReader->readNodeGraphFromFile("ui-arcadeover", this);
    pReader->release();
    mNode->setPosition(cocos2d::Vec2(UniversalFit::sharedUniversalFit()->baseLeft, 0));
    play->addChild(mNode, LAYER_MASK+1);
    
    // TODO
    //mFlash.blendFunc =  (ccBlendFunc) { GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA };//fix flash bug
cocos2d::CCString *rolefile = cocos2d::CCString::createWithFormat("sc_role%d0.png", GameRecord::sharedGameRecord()->curr_char);
    mRole->setDisplayFrame(cocos2d::SpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(rolefile->getCString()));
    mLight->setVisible(false);
    mCup->setVisible(false);
    mScore->setString(cocos2d::CCString::createWithFormat("%d", play->arcade->score)->getCString());
    mBouns->setString("0");
    mGoldenTrophy->setString(cocos2d::CCString::createWithFormat("%d", GameRecord::sharedGameRecord()->golden_trophy)->getCString());
    mSilverTrophy->setString(cocos2d::CCString::createWithFormat("%d", GameRecord::sharedGameRecord()->silver_trophy)->getCString());
    mBronzeTrophy->setString(cocos2d::CCString::createWithFormat("%d", GameRecord::sharedGameRecord()->bronze_trophy)->getCString());
    
    mCurrentBouns = 0;
    mCurrentScore = play->arcade->score;
    mTimer = 0;
    mState = 0;
}

void CommitArcade::onDestroy() 
{
    GamePlay *play = GamePlay::sharedGamePlay();
    play->unscheduleMask();
    play->removeChild(mNode, true);
    
    //hot release
cocos2d::SpriteFrameCache::sharedSpriteFrameCache()->removeSpriteFramesFromFile("ui-scroll.plist");
    unloadTextureFromeSpriteFrameFile("ui-scroll.plist");
    
    play->manager->addGameObject(GameOver::gameOver(play));
}

void CommitArcade::onUpdate(float delta) 
{
    switch (mState) {
        case 0:
        {//wait
            mTimer += delta;
            if( mTimer > 2 )
            {
                mState = 1;
                mSoundTimer = 0;
                GamePlay *play = GamePlay::sharedGamePlay();
                mTargetBouns = play->arcade->score * GameData::roleArcadeBouns(GameRecord::sharedGameRecord()->curr_char);
                mTargetScore = play->arcade->score + mTargetBouns;
                mAdder = 0;
            }
        }
            break;
        case 1:
        {
            mSoundTimer += delta;
            if( mSoundTimer > 0.1f )
            {
                GameTool::PlaySound("sound/getcoin.mp3");
                mSoundTimer = 0;
            }
            int ds = mTargetBouns - mCurrentBouns;
            mAdder += COMMIT_ARCRADE_INCREASE_SPEED*delta;
            if( ds <= mAdder )
            {
                mCurrentBouns = mTargetBouns;
                mCurrentScore = mTargetScore;
                
                mTimer = 0;
                GamePlay *play = GamePlay::sharedGamePlay();
                play->arcade->score = mCurrentScore;
                //update cup
                {
                    int ccup = 0;
                    if( GameRecord::sharedGameRecord()->task->goldPrize->prize > 0 &&
                       mCurrentScore >= GameRecord::sharedGameRecord()->task->goldPrize->score )
                    {
                        ccup = 3;
                        mCup->setDisplayFrame(cocos2d::SpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("tp-gold2.png"));
                        //take the cup
                        play->coins = GameRecord::sharedGameRecord()->task->goldPrize->prize;
                        GameRecord::sharedGameRecord()->task->goldPrize->prize = -1;
                        GameRecord::sharedGameRecord()->golden_trophy++;
                        // TODO
                        //GameCenterController->sharedGameCenterController()->submitArcadeGoldenTrophy(GameRecord::sharedGameRecord()->golden_trophy);
                        
                        //achievement
                        GameRecord::sharedGameRecord()->task->dispatchTask(ACH_ARCADEGOLD, 1);
                        GameRecord::sharedGameRecord()->golden_steak++;
                        GameRecord::sharedGameRecord()->task->dispatchTask(ACH_GOLDSTEAK, GameRecord::sharedGameRecord()->golden_steak);
                        GameRecord::sharedGameRecord()->task->dispatchTask(ACH_ARCADETROPHY, 1);
                    }
                    else if( GameRecord::sharedGameRecord()->task->silverPrize->prize > 0 &&
                            mCurrentScore >= GameRecord::sharedGameRecord()->task->silverPrize->score )
                    {
                        ccup = 2;
                        mCup->setDisplayFrame(cocos2d::SpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("tp-silver2.png"));
                        //take the cup
                        play->coins = GameRecord::sharedGameRecord()->task->silverPrize->prize;
                        GameRecord::sharedGameRecord()->task->silverPrize->prize = -1;
                        GameRecord::sharedGameRecord()->silver_trophy++;
                        //TODO
                        //GameCenterController->sharedGameCenterController()->submitArcadeSilverTrophy(GameRecord::sharedGameRecord()->silver_trophy);
                        GameRecord::sharedGameRecord()->task->dispatchTask(ACH_ARCADETROPHY, 1);
                    }
                    else if( GameRecord::sharedGameRecord()->task->bronzePrize->prize > 0 &&
                            mCurrentScore >= GameRecord::sharedGameRecord()->task->bronzePrize->score )
                    {
                        ccup = 1;
                        mCup->setDisplayFrame(cocos2d::SpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("tp-bronze2.png"));
                        //take the cup
                        play->coins = GameRecord::sharedGameRecord()->task->bronzePrize->prize;
                        GameRecord::sharedGameRecord()->task->bronzePrize->prize = -1;
                        GameRecord::sharedGameRecord()->bronze_trophy++;
                        //TODO
                        //GameCenterController->sharedGameCenterController()->submitArcadeBronzeTrophy(GameRecord::sharedGameRecord()->bronze_trophy);
                        GameRecord::sharedGameRecord()->task->dispatchTask(ACH_ARCADETROPHY, 1);
                    }
                    play->arcade->cup = ccup;
                }
                if( play->arcade->cup > 0 )
                {
                    mState = 4;
                }
                else
                {
                    mState = 3;
                    mTimer = 0;
                }
            }
            else
            {
                int add = mAdder;
                mAdder -= add;
                mCurrentBouns += add;
                mCurrentScore += add;
            }
            mScore->setString(cocos2d::CCString::createWithFormat("%d", mCurrentScore)->getCString());
            mBouns->setString(cocos2d::CCString::createWithFormat("%d", mCurrentBouns)->getCString());
        }
            break;
        case 2:
        {//pop the cup
            mTimer += delta;
            if( mTimer > 4 )
            {
                mCup->setVisible(false);
                mState = 3;
                mTimer = 0;
            }
        }
            break;
        case 3:
        {//wait some moment
            mTimer += delta;
            if( mTimer > 3 )
            {//commit over
                GamePlay::sharedGamePlay()->manager->removeGameObject(this);
            }
        }
            break;
        case 4:
        {//wait pop
            mTimer += delta;
            if( mTimer > 1 )
            {
                GamePlay *play = GamePlay::sharedGamePlay();
                mCoins->setString(cocos2d::CCString::createWithFormat("%d", play->coins)->getCString());
                mState = 2;
                mTimer = 0;
                mCup->setScale(0);
                mCup->setVisible(true);
cocos2d::CCScaleTo *cst = cocos2d::CCScaleTo::create(0.2f, 1.5f);
cocos2d::CCScaleTo *cst2 = cocos2d::CCScaleTo::create(0.1f, 1);
cocos2d::CCDelayTime *dt = cocos2d::CCDelayTime::create(2);
cocos2d::CCScaleTo *cst3 = cocos2d::CCScaleTo::create(1, 0);
cocos2d::CCSequence *cseq = cocos2d::CCSequence::create(cst, cst2, dt, cst3, NULL);
                mCup->runAction(cseq);
                
                mLight->setScale(0);
                mLight->setVisible(true);
                mLight->setOpacity(0);
cocos2d::CCFadeIn *lfi = cocos2d::CCFadeIn::create(0.2f);
cocos2d::CCDelayTime *ldy = cocos2d::CCDelayTime::create(2);
cocos2d::CCFadeOut *lfo = cocos2d::CCFadeOut::create(2);
cocos2d::CCSequence *lfs = cocos2d::CCSequence::create(lfi, ldy, lfo, NULL);
                mLight->runAction(lfs);
cocos2d::CCScaleTo *lst = cocos2d::CCScaleTo::create(1, 3);
                mLight->runAction(lst);
cocos2d::CCRotateBy *lrt = cocos2d::CCRotateBy::create(6, 1000);
                mLight->runAction(lrt);
                
                mFlash->setVisible(true);
cocos2d::CCFadeOut *fo1 = cocos2d::CCFadeOut::create(2);
                mFlash->runAction(fo1);
                
                GameTool::PlaySound("sound/openscroll.mp3");
                
                //update trophy count
                mGoldenTrophy->setString(cocos2d::CCString::createWithFormat("%d", GameRecord::sharedGameRecord()->golden_trophy)->getCString());
                mSilverTrophy->setString(cocos2d::CCString::createWithFormat("%d", GameRecord::sharedGameRecord()->silver_trophy)->getCString());
                mBronzeTrophy->setString(cocos2d::CCString::createWithFormat("%d", GameRecord::sharedGameRecord()->bronze_trophy)->getCString());
            }
        }
            break;
    }
}


SEL_MenuHandler CommitArcade::onResolveCCBCCMenuItemSelector(cocos2d::Ref * pTarget, const char* pSelectorName)
{
    //  CCLog(pSelectorName);
  return NULL;
}

cocos2d::extension::Control::Handler  CommitArcade::onResolveCCBCCControlSelector(cocos2d::Ref * pTarget, const char* pSelectorName) {
cocos2d::CCLog("Control");
  return NULL;
}
bool CommitArcade::onAssignCCBMemberVariable(cocos2d::Ref* pTarget, const char* pMemberVariableName, Node* pNode)
{
CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "mRole", Sprite *, mRole)
CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "mLight", Sprite *, mLight)
CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "mCup", Sprite *, mCup)
CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "mScore", Label *, mScore)
CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "mBouns", Label *, mBouns)
CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "mGoldenTrophy", Label *, mGoldenTrophy)
CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "mSilverTrophy", Label *, mSilverTrophy)
CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "mBronzeTrophy", Label *, mBronzeTrophy)
CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "mCoins", Label *, mCoins)
CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "mFlash", LayerColor *, mFlash)

  //  CCLog(pMemberVariableName);

  return false;
}
