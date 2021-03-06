//
//  GameOver.m
//  NinjiaRush
//
//  Created by 马 颂文 on 12-6-18.
//  Copyright (c) 2012年 TrinGame. All rights reserved.
//

#include "GameOver.h"
#include "GamePlay.h"
#include "GameRecord.h"
#include "AppDelegate.h"
#include "Tasks.h"
#include "UniversalFit.h"
#include "GameData.h"
/*
#include "MidBridge.h"
#include "GameCenterController.h"
#include "ABSocial.h"
*/



GameOver* GameOver::gameOver(cocos2d::Node* parent) 
{
	GameOver *ret = GameOver::create();
	ret->mParent = parent;
	return ret;
}

void GameOver::updateClassic() 
{
	GamePlay *play = GamePlay::sharedGamePlay();

	int dm = play->distance/PLAY_DISMETER;
	mDistance->setString(std::string_format("%dm", dm).c_str());
	mCoins->setString(std::string_format("%d", play->coins).c_str());

	//get coins
	GameRecord::sharedGameRecord()->makeCoins(play->coins);
	cocos2d::CCLog("%d coins earned.", play->coins);

	//get festival pieces
	if( play->festivalPieces > 0 )
	{
		//        while (play->festivalPieces > 0) {
		//            GameRecord::sharedGameRecord()->collection->gainItemPiece(38);
		//            play->festivalPieces--;
		//        }
		//        if( GameRecord::sharedGameRecord()->collection->isItemCompleted(38) )
		//        {
		//            Special *sh = GameData::querySpecial(38);
		//            GamePlay::sharedGamePlay()->pushNotification(sh->name, sh->icon, 4);
		//        }
	}

	//save high score
	int score = play->distance/PLAY_DISMETER;
	mScore = score;

	if( score > GameRecord::sharedGameRecord()->score_high )
	{
		GameRecord::sharedGameRecord()->score_high = score;
	}
	//GameCenterController->sharedGameCenterController()->submitClassicScore(score);
	GameRecord::sharedGameRecord()->submitScore(score, GameRecord::sharedGameRecord()->lb_playername);

	GameRecord::sharedGameRecord()->checkPoint();

	//update objectives TODO:refactor this
	cocos2d::Label* labels[] = { mDailyObjective, mWeeklyObjective, mMonthlyObjective };
	cocos2d::Sprite* icons[] = { mDailyIcon, mWeeklyIcon, mMonthlyIcon };
	cocos2d::Node* crowns[] = {mDailyCrown, mWeeklyCrown, mMonthlyCrown};
	GameTool::UpdateObjectives(labels,icons,crowns);

	//---- No.1 ----
	if( GameRecord::sharedGameRecord()->lb_scores[0] > 0 )
	{
		mLeadLb0->setString(std::string_format("%dm", GameRecord::sharedGameRecord()->lb_scores[0]).c_str());
	}
	else {
		mLeadLb0->setString("-");
	}
	//---- No.2 ----
	if( GameRecord::sharedGameRecord()->lb_scores[1] > 0 )
	{
		mLeadLb1->setString(std::string_format("%dm", GameRecord::sharedGameRecord()->lb_scores[1]).c_str());
	}
	else {
		mLeadLb1->setString("-");
	}
	//---- No.3 ----
	if( GameRecord::sharedGameRecord()->lb_scores[2] > 0 )
	{
		mLeadLb2->setString(std::string_format("%dm", GameRecord::sharedGameRecord()->lb_scores[2]).c_str());
	}
	else {
		mLeadLb2->setString("-");
	}
}

void GameOver::updateArcade() 
{
	GamePlay *play = GamePlay::sharedGamePlay();
	int score = play->arcade->score;

	mArcadeScore->setString(std::string_format("%d", score).c_str());
	switch (play->arcade->cup) {
		case 0:
			{
				mCup->setVisible(false);
			}
			break;
		case 1:
			{
				mCup->setVisible(true);
				mCup->setDisplayFrame(GameTool::getSpriteFrameByName("tp-bronze.png"));
			}
			break;
		case 2:
			{
				mCup->setVisible(true);
				mCup->setDisplayFrame(GameTool::getSpriteFrameByName("tp-silver.png"));
			}
			break;
		case 3:
			{
				mCup->setVisible(true);
				mCup->setDisplayFrame(GameTool::getSpriteFrameByName("tp-gold.png"));
			}
			break;
	}

	//get coins
	GameRecord::sharedGameRecord()->makeCoins(play->coins);
	cocos2d::CCLog("%d coins earned.", play->coins);

	//save high score
	mScore = score;

	if( score > GameRecord::sharedGameRecord()->arcade_high )
	{
		GameRecord::sharedGameRecord()->arcade_high = score;
	}
	//GameCenterController->sharedGameCenterController()->submitArcadeScore(score);
	GameRecord::sharedGameRecord()->submitArcadeScore(score, GameRecord::sharedGameRecord()->lb_playername);

	GameRecord::sharedGameRecord()->checkPoint();

	//update trophies
	//gold
	{
		if( GameRecord::sharedGameRecord()->task->goldPrize.prize < 0 )
		{//completed
			mGoldCoin->setVisible(false);
			mGoldPrize->setVisible(false);
			mGoldScore->setString("已完成！");
			mGoldScore->setColor(Color3B(128, 128, 128));
		}
		else
		{//not completed
			mGoldScore->setString(std::string_format("%d", GameRecord::sharedGameRecord()->task->goldPrize.score).c_str());
			mGoldPrize->setString(std::string_format("%d", GameRecord::sharedGameRecord()->task->goldPrize.prize).c_str());
		}
	}
	//silver
	{
		if( GameRecord::sharedGameRecord()->task->silverPrize.prize < 0 )
		{//completed
			mSilverCoin->setVisible(false);
			mSilverPrize->setVisible(false);
			mSilverScore->setString("已完成！");
			mSilverScore->setColor(Color3B(128, 128, 128));
		}
		else
		{//not completed
			mSilverScore->setString(std::string_format("%d", GameRecord::sharedGameRecord()->task->silverPrize.score).c_str());
			mSilverPrize->setString(std::string_format("%d", GameRecord::sharedGameRecord()->task->silverPrize.prize).c_str());
		}
	}
	//bronze
	{
		if( GameRecord::sharedGameRecord()->task->bronzePrize.prize < 0 )
		{//completed
			mBronzeCoin->setVisible(false);
			mBronzePrize->setVisible(false);
			mBronzeScore->setString("已完成！");
			mBronzeScore->setColor(Color3B(128, 128, 128));
		}
		else
		{//not completed
			mBronzeScore->setString(std::string_format("%d", GameRecord::sharedGameRecord()->task->bronzePrize.score).c_str());
			mBronzePrize->setString(std::string_format("%d", GameRecord::sharedGameRecord()->task->bronzePrize.prize).c_str());
		}
	}

	//--- update leader board ---
	//---- No.1 ----
	if( GameRecord::sharedGameRecord()->lba_scores[0] > 0 )
	{
		mLeadLb0->setString(std::string_format("%d", GameRecord::sharedGameRecord()->lba_scores[0]).c_str());
	}
	else {
		mLeadLb0->setString("-");
	}
	//---- No.2 ----
	if( GameRecord::sharedGameRecord()->lba_scores[1] > 0 )
	{
		mLeadLb1->setString(std::string_format("%d", GameRecord::sharedGameRecord()->lba_scores[1]).c_str());
	}
	else {
		mLeadLb1->setString("-");
	}
	//---- No.3 ----
	if( GameRecord::sharedGameRecord()->lba_scores[2] > 0 )
	{
		mLeadLb2->setString(std::string_format("%d", GameRecord::sharedGameRecord()->lba_scores[2]).c_str());
	}
	else {
		mLeadLb2->setString("-");
	}
}

void GameOver::onCreate() 
{
	//hot load
	cocos2d::SpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("ui-gameover.plist");
	cocos2d::SpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("ui-change.plist");

	GameRecord::sharedGameRecord()->task->checkObjectives();//check objectives

	//评分邀请
	//AppController *appdel = (AppController*)UIApplication->sharedApplication()->delegate();
	//appdel->triggerRatePop();

	GamePlay *play = GamePlay::sharedGamePlay();
	play->scheduleMask(Color3B(0, 0, 0), 128, 0);
	cocosbuilder::NodeLoaderLibrary *pNodeLib = cocosbuilder::NodeLoaderLibrary::getInstance();
	cocosbuilder::CCBReader *pReader = new cocosbuilder::CCBReader(pNodeLib, this, this);
	mNode = pReader->readNodeGraphFromFile("ui-gameover.ccbi", this);
	//mNode = createUIByCCBI("ui-gameover.ccb", "CoinsMenu", GameOverLoader::loader(), this);
	pReader->release();
	mNode->setPosition(cocos2d::Vec2(UniversalFit::sharedUniversalFit()->baseLeft, 0));
	mParent->addChild(mNode, LAYER_MASK+1);

	//1.2.0 SinaWeibo
	if( UniversalFit::sharedUniversalFit()->shouldUsingSinaWeibo() )
	{
		/*
			 mTwitter->setNormalSpriteFrame(GameTool::getSpriteFrameByName("gowb.png"));
			 mTwitter->setSelectedSpriteFrame(GameTool::getSpriteFrameByName("gowb2.png"));
			 mTwitterAction->setDisplayFrame(GameTool::getSpriteFrameByName("gowb3.png"));
			 */
	}

	//update change
	switch (GameRecord::sharedGameRecord()->curr_char) {
		case 0:
			{
				mChange->setNormalImage(cocos2d::Sprite::createWithSpriteFrameName("go_change1.png"));
				mChange->setSelectedImage(cocos2d::Sprite::createWithSpriteFrameName("go_change2.png"));
			}
			break;
		case 1:
			{
				mChange->setNormalImage(cocos2d::Sprite::createWithSpriteFrameName("go_change3.png"));
				mChange->setSelectedImage(cocos2d::Sprite::createWithSpriteFrameName("go_change4.png"));
			}
			break;
		case 2:
			{
				mChange->setNormalImage(cocos2d::Sprite::createWithSpriteFrameName("go_change5.png"));
				mChange->setSelectedImage(cocos2d::Sprite::createWithSpriteFrameName("go_change6.png"));
			}
			break;
		case 3:
			{
				mChange->setNormalImage(cocos2d::Sprite::createWithSpriteFrameName("go_change7.png"));
				mChange->setSelectedImage(cocos2d::Sprite::createWithSpriteFrameName("go_change8.png"));
			}
			break;
	}

	//update Play Again
	if( GameRecord::sharedGameRecord()->char_contract[play->roleId] > 0 )
	{
		mPlayAgain->setNormalImage(cocos2d::Sprite::createWithSpriteFrameName("go_pa1.png"));
		mPlayAgain->setSelectedImage(cocos2d::Sprite::createWithSpriteFrameName("go_pa2.png"));
		mCost = 0;
	}
	else {
		mPlayAgain->setNormalImage(cocos2d::Sprite::createWithSpriteFrameName("go_pa1x.png"));
		mPlayAgain->setSelectedImage(cocos2d::Sprite::createWithSpriteFrameName("go_pa2x.png"));
		mCost = 100;
	}

	/*
		 MidBridge::sharedMidbridge()->setFacebookCallback(this, menu_selector(doneFacebook:));
		 MidBridge::sharedMidbridge()->setTwitterCallback(this, menu_selector(doneTwitter:));
		 */
	mPostingFacebook = false;
	mPostingTwitter = false;

	//    if( !this->haveFacebookCoins() )
	//    {
	//        mFacebookCoins->setVisible(false);
	//    }

	if( !this->haveTwitterCoins() )
	{
		mTwitterCoins->setVisible(false);
	}

	/*
		 if( !ABSocial->isAvailable() )
		 {
		 mTwitter->setColor(Color3B(128, 128, 128));
		 mTwitterAction->setColor(Color3B(128, 128, 128));
		 mTwitter.isEnabled = false;

		 mFacebook->setColor(Color3B(128, 128, 128));
		 mFacebookAction->setColor(Color3B(128, 128, 128));
		 mFacebook.isEnabled = false;
		 }
		 */


	if( play->mode == MODE_CLASSIC )
	{
		mClassic->setVisible(true);
		mArcade->setVisible(false);
		this->updateClassic();
	}
	else
	{
		mArcade->setVisible(true);
		mClassic->setVisible(false);
		this->updateArcade();
	}
	mLeadLb0->setBMFontFilePath("ab40.fnt");
	mLeadLb1->setBMFontFilePath("ab40.fnt");
	mLeadLb2->setBMFontFilePath("ab40.fnt");
}

void GameOver::onUpdate(float delta) 
{
	//do nothing
}

void GameOver::onDestroy() 
{
	GamePlay *play = GamePlay::sharedGamePlay();
	play->unscheduleMask();
	mParent->removeChild(mNode, true);

	//hot release
	cocos2d::SpriteFrameCache::sharedSpriteFrameCache()->removeSpriteFramesFromFile("ui-gameover.plist");
	unloadTextureFromeSpriteFrameFile("ui-gameover.plist");
	cocos2d::SpriteFrameCache::sharedSpriteFrameCache()->removeSpriteFramesFromFile("ui-change.plist");
	unloadTextureFromeSpriteFrameFile("ui-change.plist");

	/*
		 MidBridge::sharedMidbridge()->setFacebookCallback(NULL, menu_selector(doneFacebook:));
		 MidBridge::sharedMidbridge()->setTwitterCallback(NULL, menu_selector(doneTwitter:));
		 */
}

void GameOver::onChange(cocos2d::Ref*)
{
	GameTool::PlaySound("click.mp3");
	GamePlay *play = GamePlay::sharedGamePlay();
	play->change();
}

void GameOver::onMenu(cocos2d::Ref*)
{

	GameTool::PlaySound("click.mp3");
	GamePlay *play = GamePlay::sharedGamePlay();
	play->exit();
}

void GameOver::onRestart(cocos2d::Ref*) 
{
	GameTool::PlaySound("click.mp3");
	if( GameRecord::sharedGameRecord()->coins >= mCost )
	{
		if( mCost > 0 )
		{
			GameTool::PlaySound("buy.mp3");
			cocos2d::CCLog("cost=%d", mCost);
		}
		GameRecord::sharedGameRecord()->makeCoins(-mCost);
		GamePlay *play = GamePlay::sharedGamePlay();
		play->restart();
	}
	else {
		GameTool::PlaySound("error.mp3");
	}
}

void GameOver::onFacebook(cocos2d::Ref*)
{
	//    if( !mPostingFacebook )
	//    {
	//        mPostingFacebook = true;
	//        
	//        GameTool::PlaySound("click.mp3");
	//        
	//        //do some facebook actions
	//        
	//        mFacebookAction->setDisplayFrame(GameTool::getSpriteFrameByName("ftloading.png"));
	//        mFacebookAction->stopAllActions();
	//        RotateBy *rb = cocos2d::RotateBy::create(1, 360);
	//        RepeatForever *rf = cocos2d::RepeatForever::create(rb);
	//        mFacebookAction->runAction(rf);
	//    }
}

void GameOver::onTwitter(cocos2d::Ref*)
{
	//    if( !mPostingTwitter )
	//    {
	//        mPostingTwitter = true;
	//        
	//        GameTool::PlaySound("click.mp3");
	//        AppController *del = (AppController*)UIApplication->sharedApplication().delegate;
	//        
	//        bool sina = UniversalFit::sharedUniversalFit()->shouldUsingSinaWeibo();
	//        ABSocial *social = NULL;
	//        if( sina )
	//        {
	//            social = ABSocial->feedOnSinaWeibo();
	//            social.text = std::string_format("刚刚在#LittleNinjaRush#的标准模式中跑了%dm，谁来挑战！", mScore);;
	//        }
	//        else
	//        {
	//            social = ABSocial->feedOnTwitter();
	//            social.text = std::string_format("I have just got %dm in #LittleNinjaRush, beat that!", mScore);
	//        }
	//        /*
	//        social.image = makeScreenshot();
	//        social.url = NSURL->URLWithString(TWITTER_URL);
	//        social.callbackObject = MidBridge->sharedMidbridge();
	//        social.callbackSelector =, );
	//        social->present(del.navController);
	//        */
	//        
	//        mTwitterAction->setDisplayFrame(GameTool::getSpriteFrameByName("ftloading.png"));
	//        mTwitterAction->stopAllActions();
	//        RotateBy *rb = cocos2d::RotateBy::create(1, 360);
	//        RepeatForever *rf = cocos2d::RepeatForever::create(rb);
	//        mTwitterAction->runAction(rf);
	//    }
}

bool GameOver::haveFacebookCoins() 
{
	//    NSDate *last = NSDate->dateWithTimeIntervalSince1970(GameRecord::sharedGameRecord()->share_facebook);
	//    NSDateComponents *lastcomp = NSCalendar->currentCalendar()->components(NSDayCalendarUnit | NSMonthCalendarUnit | NSYearCalendarUnit | NSWeekdayCalendarUnit, last);
	//    NSDate *today = NSDate->date();
	//    NSDateComponents *todaycomp = NSCalendar->currentCalendar()->components(NSDayCalendarUnit | NSMonthCalendarUnit | NSYearCalendarUnit | NSWeekdayCalendarUnit, today);
	//    if( lastcomp.year != todaycomp.year || lastcomp.month != todaycomp.month || lastcomp.day != todaycomp.day )
	//    {
	//        return true;
	//    }
	return false;
}

bool GameOver::haveTwitterCoins() 
{
	//    NSDate *last = NSDate->dateWithTimeIntervalSince1970(GameRecord::sharedGameRecord()->share_twitter);
	//    NSDateComponents *lastcomp = NSCalendar->currentCalendar()->components(NSDayCalendarUnit | NSMonthCalendarUnit | NSYearCalendarUnit | NSWeekdayCalendarUnit, last);
	//    NSDate *today = NSDate->date();
	//    NSDateComponents *todaycomp = NSCalendar->currentCalendar()->components(NSDayCalendarUnit | NSMonthCalendarUnit | NSYearCalendarUnit | NSWeekdayCalendarUnit, today);
	//    if( lastcomp.year != todaycomp.year || lastcomp.month != todaycomp.month || lastcomp.day != todaycomp.day )
	//    {
	//        return true;
	//    }
	return false;
}

void GameOver::doneFacebook(cocos2d::CCInteger* res) 
{
	//    int result = res->intValue();
	//    mPostingFacebook = false;
	//    
	//    mFacebookAction->stopAllActions();
	//    mFacebookAction->setRotation(0);
	//    mFacebookAction->setDisplayFrame(GameTool::getSpriteFrameByName("gofb3.png"));
	//    
	//    if( this->haveFacebookCoins() && result > 0 )
	//    {
	//        GameRecord::sharedGameRecord()->makeCoins(100);
	//        GameTool::PlaySound("getcoin.mp3");
	//        GameRecord::sharedGameRecord()->share_facebook = NSDate->date()->timeIntervalSince1970();
	//        GameRecord::sharedGameRecord()->checkPoint();
	//        
	//        GamePlay::sharedGamePlay()->pushNotification("You have got 100 coins\nfor sharing your score.", NULL, 5);
	//        mFacebookCoins->setVisible(false);
	//    }
}

void GameOver::doneTwitter(cocos2d::CCInteger* res)
{
	//    int result = res->intValue();
	//    mPostingTwitter = false;
	//    
	//    mTwitterAction->stopAllActions();
	//    mTwitterAction->setRotation(0);
	//    if( UniversalFit::sharedUniversalFit()->shouldUsingSinaWeibo() )
	//    {
	//        mTwitterAction->setDisplayFrame(GameTool::getSpriteFrameByName("gowb3.png"));
	//    }
	//    else
	//    {
	//        mTwitterAction->setDisplayFrame(GameTool::getSpriteFrameByName("gott3.png"));
	//    }
	//    
	//    if( this->haveTwitterCoins() && result > 0 )
	//    {
	//        GameRecord::sharedGameRecord()->makeCoins(100);
	//        GameTool::PlaySound("getcoin.mp3");
	//        GameRecord::sharedGameRecord()->share_twitter = NSDate->date()->timeIntervalSince1970();
	//        GameRecord::sharedGameRecord()->checkPoint();
	//        
	//        GamePlay::sharedGamePlay()->pushNotification("You have got 100 coins\nfor sharing your score.", NULL, 5);
	//        mTwitterCoins->setVisible(false);
	//    }
}


SEL_MenuHandler GameOver::onResolveCCBCCMenuItemSelector(cocos2d::Ref * pTarget, const char* pSelectorName)
{
	CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "onChange", GameOver::onChange);
	CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "onMenu", GameOver::onMenu);
	CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "onRestart", GameOver::onRestart);
	CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "onFacebook", GameOver::onFacebook);
	CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "onTwitter", GameOver::onTwitter);

	return NULL;
}

cocos2d::extension::Control::Handler   GameOver::onResolveCCBCCControlSelector(cocos2d::Ref * pTarget, const char* pSelectorName)
{
	cocos2d::CCLog("Control");
	return NULL;
}
bool GameOver::onAssignCCBMemberVariable(cocos2d::Ref* pTarget, const char* pMemberVariableName, Node* pNode)
{
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "mDistance", Label *, mDistance);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "mCoins", Label *, mCoins);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "mChange", MenuItemImage *, mChange);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "mDailyObjective", Label *, mDailyObjective);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "mWeeklyObjective", Label *, mWeeklyObjective);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "mMonthlyObjective", Label *, mMonthlyObjective);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "mDailyCrown", Node *, mDailyCrown);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "mWeeklyCrown", Node *, mWeeklyCrown);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "mMonthlyCrown", Node *, mMonthlyCrown);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "mDailyIcon", Sprite *, mDailyIcon);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "mWeeklyIcon", Sprite *, mWeeklyIcon);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "mMonthlyIcon", Sprite *, mMonthlyIcon);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "mLeadSp0", Sprite *, mLeadSp0);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "mLeadSp1", Sprite *, mLeadSp1);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "mLeadSp2", Sprite *, mLeadSp2);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "mLeadLb0", Label *, mLeadLb0);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "mLeadLb1", Label *, mLeadLb1);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "mLeadLb2", Label *, mLeadLb2);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "mPlayAgain", MenuItemImage *, mPlayAgain);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "mTwitter", MenuItemImage *, mTwitter);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "mFacebook", MenuItemImage *, mFacebook);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "mFacebookAction", Sprite *, mFacebookAction);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "mTwitterAction", Sprite *, mTwitterAction);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "mFacebookCoins", Sprite *, mFacebookCoins);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "mTwitterCoins", Sprite *, mTwitterCoins);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "mClassic", Node *, mClassic);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "mArcade", Node *, mArcade);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "mArcadeScore", Label *, mArcadeScore);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "mGoldCoin", Sprite *, mGoldCoin);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "mSilverCoin", Sprite *, mSilverCoin);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "mBronzeCoin", Sprite *, mBronzeCoin);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "mGoldScore", Label *, mGoldScore);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "mSilverScore", Label *, mSilverScore);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "mBronzeScore", Label *, mBronzeScore);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "mGoldPrize", Label *, mGoldPrize);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "mSilverPrize", Label *, mSilverPrize);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "mBronzePrize", Label *, mBronzePrize);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "mCup", Sprite *, mCup);

	return false;
}
