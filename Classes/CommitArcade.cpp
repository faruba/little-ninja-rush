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
	std::string rolefile = std::string_format("sc_role%d0.png", GameRecord::sharedGameRecord()->curr_char);
	mRole->setDisplayFrame(GameTool::getSpriteFrameByName(rolefile.c_str()));
	mLight->setVisible(false);
	mCup->setVisible(false);
	mScore->setString(std::string_format("%d", play->arcade->score).c_str());
	mBouns->setString("0");
	mGoldenTrophy->setString(std::string_format("%d", GameRecord::sharedGameRecord()->golden_trophy).c_str());
	mSilverTrophy->setString(std::string_format("%d", GameRecord::sharedGameRecord()->silver_trophy).c_str());
	mBronzeTrophy->setString(std::string_format("%d", GameRecord::sharedGameRecord()->bronze_trophy).c_str());

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
					GameTool::PlaySound("getcoin.mp3");
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
						if( GameRecord::sharedGameRecord()->task->goldPrize.prize > 0 &&
								mCurrentScore >= GameRecord::sharedGameRecord()->task->goldPrize.score )
						{
							ccup = 3;
							mCup->setDisplayFrame(GameTool::getSpriteFrameByName("tp-gold2.png"));
							//take the cup
							play->coins = GameRecord::sharedGameRecord()->task->goldPrize.prize;
							GameRecord::sharedGameRecord()->task->goldPrize.prize = -1;
							GameRecord::sharedGameRecord()->golden_trophy++;
							// TODO
							//GameCenterController->sharedGameCenterController()->submitArcadeGoldenTrophy(GameRecord::sharedGameRecord()->golden_trophy);

							//achievement
							GameRecord::sharedGameRecord()->task->dispatchTask(ACH_ARCADEGOLD, 1);
							GameRecord::sharedGameRecord()->golden_steak++;
							GameRecord::sharedGameRecord()->task->dispatchTask(ACH_GOLDSTEAK, GameRecord::sharedGameRecord()->golden_steak);
							GameRecord::sharedGameRecord()->task->dispatchTask(ACH_ARCADETROPHY, 1);
						}
						else if( GameRecord::sharedGameRecord()->task->silverPrize.prize > 0 &&
								mCurrentScore >= GameRecord::sharedGameRecord()->task->silverPrize.score )
						{
							ccup = 2;
							mCup->setDisplayFrame(GameTool::getSpriteFrameByName("tp-silver2.png"));
							//take the cup
							play->coins = GameRecord::sharedGameRecord()->task->silverPrize.prize;
							GameRecord::sharedGameRecord()->task->silverPrize.prize = -1;
							GameRecord::sharedGameRecord()->silver_trophy++;
							//TODO
							//GameCenterController->sharedGameCenterController()->submitArcadeSilverTrophy(GameRecord::sharedGameRecord()->silver_trophy);
							GameRecord::sharedGameRecord()->task->dispatchTask(ACH_ARCADETROPHY, 1);
						}
						else if( GameRecord::sharedGameRecord()->task->bronzePrize.prize > 0 &&
								mCurrentScore >= GameRecord::sharedGameRecord()->task->bronzePrize.score )
						{
							ccup = 1;
							mCup->setDisplayFrame(GameTool::getSpriteFrameByName("tp-bronze2.png"));
							//take the cup
							play->coins = GameRecord::sharedGameRecord()->task->bronzePrize.prize;
							GameRecord::sharedGameRecord()->task->bronzePrize.prize = -1;
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
				mScore->setString(std::string_format("%d", mCurrentScore).c_str());
				mBouns->setString(std::string_format("%d", mCurrentBouns).c_str());
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
					mCoins->setString(std::string_format("%d", play->coins).c_str());
					mState = 2;
					mTimer = 0;
					mCup->setScale(0);
					mCup->setVisible(true);
					cocos2d::ScaleTo *cst = cocos2d::ScaleTo::create(0.2f, 1.5f);
					cocos2d::ScaleTo *cst2 = cocos2d::ScaleTo::create(0.1f, 1);
					cocos2d::DelayTime *dt = cocos2d::DelayTime::create(2);
					cocos2d::ScaleTo *cst3 = cocos2d::ScaleTo::create(1, 0);
					cocos2d::Sequence *cseq = cocos2d::Sequence::create(cst, cst2, dt, cst3, NULL);
					mCup->runAction(cseq);

					mLight->setScale(0);
					mLight->setVisible(true);
					mLight->setOpacity(0);
					cocos2d::FadeIn *lfi = cocos2d::FadeIn::create(0.2f);
					cocos2d::DelayTime *ldy = cocos2d::DelayTime::create(2);
					cocos2d::FadeOut *lfo = cocos2d::FadeOut::create(2);
					cocos2d::Sequence *lfs = cocos2d::Sequence::create(lfi, ldy, lfo, NULL);
					mLight->runAction(lfs);
					cocos2d::ScaleTo *lst = cocos2d::ScaleTo::create(1, 3);
					mLight->runAction(lst);
					cocos2d::RotateBy *lrt = cocos2d::RotateBy::create(6, 1000);
					mLight->runAction(lrt);

					mFlash->setVisible(true);
					cocos2d::FadeOut *fo1 = cocos2d::FadeOut::create(2);
					mFlash->runAction(fo1);

					GameTool::PlaySound("openscroll.mp3");

					//update trophy count
					mGoldenTrophy->setString(std::string_format("%d", GameRecord::sharedGameRecord()->golden_trophy).c_str());
					mSilverTrophy->setString(std::string_format("%d", GameRecord::sharedGameRecord()->silver_trophy).c_str());
					mBronzeTrophy->setString(std::string_format("%d", GameRecord::sharedGameRecord()->bronze_trophy).c_str());
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
