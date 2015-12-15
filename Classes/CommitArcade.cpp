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
    CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("ui-scroll.plist");
    
    GamePlay *play = GamePlay::sharedGamePlay();
    play->scheduleMask(ccc3(0, 0, 0), 128, 0);
    NodeLoaderLibrary *pNodeLib = NodeLoaderLibrary::sharedNodeLoaderLibrary();
    CCBReader *pReader = new CCBReader(pNodeLib, this, this);
    mNode = pReader->readNodeGraphFromFile("ui-arcadeover", this);
    pReader->release();
    mNode->setPosition(ccp(UniversalFit::sharedUniversalFit()->baseLeft, 0));
    play->addChild(mNode, LAYER_MASK+1);
    
    // TODO
    //mFlash.blendFunc =  (ccBlendFunc) { GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA };//fix flash bug
    
    CCString *rolefile = CCString::createWithFormat("sc_role%d0.png", GameRecord::sharedGameRecord()->curr_char);
    mRole->setDisplayFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(rolefile->getCString()));
    mLight->setVisible(false);
    mCup->setVisible(false);
    mScore->setString(CCString::createWithFormat("%d", play->arcade->score)->getCString());
    mBouns->setString("0");
    mGoldenTrophy->setString(CCString::createWithFormat("%d", GameRecord::sharedGameRecord()->golden_trophy)->getCString());
    mSilverTrophy->setString(CCString::createWithFormat("%d", GameRecord::sharedGameRecord()->silver_trophy)->getCString());
    mBronzeTrophy->setString(CCString::createWithFormat("%d", GameRecord::sharedGameRecord()->bronze_trophy)->getCString());
    
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
    CCSpriteFrameCache::sharedSpriteFrameCache()->removeSpriteFramesFromFile("ui-scroll.plist");
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
                SimpleAudioEngine::sharedEngine()->playEffect(CCFileUtils::sharedFileUtils()->fullPathForFilename("sound/getcoin.mp3").c_str());
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
                        mCup->setDisplayFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("tp-gold2.png"));
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
                        mCup->setDisplayFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("tp-silver2.png"));
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
                        mCup->setDisplayFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("tp-bronze2.png"));
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
            mScore->setString(CCString::createWithFormat("%d", mCurrentScore)->getCString());
            mBouns->setString(CCString::createWithFormat("%d", mCurrentBouns)->getCString());
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
                mCoins->setString(CCString::createWithFormat("%d", play->coins)->getCString());
                mState = 2;
                mTimer = 0;
                mCup->setScale(0);
                mCup->setVisible(true);
                
                CCScaleTo *cst = CCScaleTo::create(0.2f, 1.5f);
                CCScaleTo *cst2 = CCScaleTo::create(0.1f, 1);
                CCDelayTime *dt = CCDelayTime::create(2);
                CCScaleTo *cst3 = CCScaleTo::create(1, 0);
                CCSequence *cseq = CCSequence::create(cst, cst2, dt, cst3, NULL);
                mCup->runAction(cseq);
                
                mLight->setScale(0);
                mLight->setVisible(true);
                mLight->setOpacity(0);
                CCFadeIn *lfi = CCFadeIn::create(0.2f);
                CCDelayTime *ldy = CCDelayTime::create(2);
                CCFadeOut *lfo = CCFadeOut::create(2);
                CCSequence *lfs = CCSequence::create(lfi, ldy, lfo, NULL);
                mLight->runAction(lfs);
                CCScaleTo *lst = CCScaleTo::create(1, 3);
                mLight->runAction(lst);
                CCRotateBy *lrt = CCRotateBy::create(6, 1000);
                mLight->runAction(lrt);
                
                mFlash->setVisible(true);
                CCFadeOut *fo1 = CCFadeOut::create(2);
                mFlash->runAction(fo1);
                
                SimpleAudioEngine::sharedEngine()->playEffect(CCFileUtils::sharedFileUtils()->fullPathForFilename("sound/openscroll.mp3").c_str());
                
                //update trophy count
                mGoldenTrophy->setString(CCString::createWithFormat("%d", GameRecord::sharedGameRecord()->golden_trophy)->getCString());
                mSilverTrophy->setString(CCString::createWithFormat("%d", GameRecord::sharedGameRecord()->silver_trophy)->getCString());
                mBronzeTrophy->setString(CCString::createWithFormat("%d", GameRecord::sharedGameRecord()->bronze_trophy)->getCString());
            }
        }
            break;
    }
}


SEL_MenuHandler CommitArcade::onResolveCCBCCMenuItemSelector(CCObject * pTarget, const char* pSelectorName)
{
    //  CCLog(pSelectorName);
  return NULL;
}

cocos2d::extension::Control::Handler  CommitArcade::onResolveCCBCCControlSelector(cocos2d::Ref * pTarget, const char* pSelectorName) {
  CCLog("Control");
  return NULL;
}
bool CommitArcade::onAssignCCBMemberVariable(CCObject* pTarget, const char* pMemberVariableName, CCNode* pNode)
{
  CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "mRole", CCSprite *, mRole)
  CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "mLight", CCSprite *, mLight)
  CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "mCup", CCSprite *, mCup)
  CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "mScore", CCLabelBMFont *, mScore)
  CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "mBouns", CCLabelBMFont *, mBouns)
  CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "mGoldenTrophy", CCLabelBMFont *, mGoldenTrophy)
  CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "mSilverTrophy", CCLabelBMFont *, mSilverTrophy)
  CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "mBronzeTrophy", CCLabelBMFont *, mBronzeTrophy)
  CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "mCoins", CCLabelBMFont *, mCoins)
  CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "mFlash", CCLayerColor *, mFlash)

  //  CCLog(pMemberVariableName);

  return false;
}
