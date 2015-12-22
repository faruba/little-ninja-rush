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
    mDistance->setString(cocos2d::CCString::createWithFormat("%dm", dm)->getCString());
    mCoins->setString(cocos2d::CCString::createWithFormat("%d", play->coins)->getCString());
    
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
    
    //update objectives
    if( GameRecord::sharedGameRecord()->task->dailyObjective->uiid >= 0 )
    {
        Achievement *obj = Tasks::dailyObjectiveWithUiid(GameRecord::sharedGameRecord()->task->dailyObjective->uiid);
        mDailyObjective->setString(Tasks::stringForObjective(obj->desc, obj->achieveCode,  obj->achieveNumber, GameRecord::sharedGameRecord()->task->dailyObjective->count)->getCString());
        mDailyObjective->setColor(ccc3(255, 255, 255));
        
        mDailyIcon->setVisible(true);
        Achievement *ach = Tasks::dailyObjectiveWithUiid(GameRecord::sharedGameRecord()->task->dailyObjective->uiid);
cocos2d::Sprite *icon = cocos2d::Sprite::createWithSpriteFrameName(ach->icon->getCString());
        icon->setPosition(cocos2d::Vec2(mDailyIcon->getContentSize().width/2, mDailyIcon->getContentSize().height/2));
        mDailyIcon->addChild(icon);
    }
    else {
        mDailyObjective->setString("已完成！");
        mDailyObjective->setColor(ccc3(128, 128, 128));
    }
    if( GameRecord::sharedGameRecord()->task->weeklyObjective->uiid >= 0 )
    {
        Achievement *obj = Tasks::weeklyObjectiveWithUiid(GameRecord::sharedGameRecord()->task->weeklyObjective->uiid);
        mWeeklyObjective->setString(Tasks::stringForObjective(obj->desc, obj->achieveCode, obj->achieveNumber, GameRecord::sharedGameRecord()->task->weeklyObjective->count)->getCString());
        mWeeklyObjective->setColor(ccc3(255, 255, 255));
        
        mWeeklyIcon->setVisible(true);
        Achievement *ach = Tasks::weeklyObjectiveWithUiid(GameRecord::sharedGameRecord()->task->weeklyObjective->uiid);
cocos2d::Sprite *icon = cocos2d::Sprite::createWithSpriteFrameName(ach->icon->getCString());
        icon->setPosition(cocos2d::Vec2(mWeeklyIcon->getContentSize().width/2, mWeeklyIcon->getContentSize().height/2));
        mWeeklyIcon->addChild(icon);
    }
    else {
        mWeeklyObjective->setString("已完成！");
        mWeeklyObjective->setColor(ccc3(128, 128, 128));
    }
    if( GameRecord::sharedGameRecord()->task->monthlyObjective->uiid >= 0 )
    {
        Achievement *obj = Tasks::monthlyObjectiveWithUiid(GameRecord::sharedGameRecord()->task->monthlyObjective->uiid);
        mMonthlyObjective->setString(Tasks::stringForObjective(obj->desc, obj->achieveCode, obj->achieveNumber, GameRecord::sharedGameRecord()->task->monthlyObjective->count)->getCString());
        mMonthlyObjective->setColor(ccc3(255, 255, 255));
        
        mMonthlyIcon->setVisible(true);
        Achievement *ach = Tasks::monthlyObjectiveWithUiid(GameRecord::sharedGameRecord()->task->monthlyObjective->uiid);
cocos2d::Sprite *icon = cocos2d::Sprite::createWithSpriteFrameName(ach->icon->getCString());
        icon->setPosition(cocos2d::Vec2(mMonthlyIcon->getContentSize().width/2, mMonthlyIcon->getContentSize().height/2));
        mMonthlyIcon->addChild(icon);
    }
    else {
        mMonthlyObjective->setString("已完成！");
        mMonthlyObjective->setColor(ccc3(128, 128, 128));
    }
    //update crown
    for(int i=0; i<GameRecord::sharedGameRecord()->task->dailyObjective->index; ++i)
    {
cocos2d::Sprite *crown = cocos2d::Sprite::createWithSpriteFrameName(cocos2d::CCString::createWithFormat("crown%d.png", i)->getCString());
        crown->setPosition(cocos2d::Vec2(32-16*i, 0));
        mDailyCrown->addChild(crown);
    }
    for(int i=0; i<GameRecord::sharedGameRecord()->task->weeklyObjective->index; ++i)
    {
cocos2d::Sprite *crown = cocos2d::Sprite::createWithSpriteFrameName(cocos2d::CCString::createWithFormat("crown%d.png", i)->getCString());
        crown->setPosition(cocos2d::Vec2(32-16*i, 0));
        mWeeklyCrown->addChild(crown);
    }
    for(int i=0; i<GameRecord::sharedGameRecord()->task->monthlyObjective->index; ++i)
    {
cocos2d::Sprite *crown = cocos2d::Sprite::createWithSpriteFrameName(cocos2d::CCString::createWithFormat("crown%d.png", i)->getCString());
        crown->setPosition(cocos2d::Vec2(32-16*i, 0));
        mMonthlyCrown->addChild(crown);
    }
    
    //---- No.1 ----
    if( GameRecord::sharedGameRecord()->lb_scores[0] > 0 )
    {
        mLeadLb0->setString(cocos2d::CCString::createWithFormat("%dm", GameRecord::sharedGameRecord()->lb_scores[0])->getCString());
    }
    else {
        mLeadLb0->setString("-");
    }
    //---- No.2 ----
    if( GameRecord::sharedGameRecord()->lb_scores[1] > 0 )
    {
        mLeadLb1->setString(cocos2d::CCString::createWithFormat("%dm", GameRecord::sharedGameRecord()->lb_scores[1])->getCString());
    }
    else {
        mLeadLb1->setString("-");
    }
    //---- No.3 ----
    if( GameRecord::sharedGameRecord()->lb_scores[2] > 0 )
    {
        mLeadLb2->setString(cocos2d::CCString::createWithFormat("%dm", GameRecord::sharedGameRecord()->lb_scores[2])->getCString());
    }
    else {
        mLeadLb2->setString("-");
    }
}

void GameOver::updateArcade() 
{
    GamePlay *play = GamePlay::sharedGamePlay();
    int score = play->arcade->score;

    mArcadeScore->setString(cocos2d::CCString::createWithFormat("%d", score)->getCString());
    switch (play->arcade->cup) {
        case 0:
        {
            mCup->setVisible(false);
        }
            break;
        case 1:
        {
            mCup->setVisible(true);
            mCup->setDisplayFrame(cocos2d::SpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("tp-bronze.png"));
        }
            break;
        case 2:
        {
            mCup->setVisible(true);
            mCup->setDisplayFrame(cocos2d::SpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("tp-silver.png"));
        }
            break;
        case 3:
        {
            mCup->setVisible(true);
            mCup->setDisplayFrame(cocos2d::SpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("tp-gold.png"));
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
        if( GameRecord::sharedGameRecord()->task->goldPrize->prize < 0 )
        {//completed
            mGoldCoin->setVisible(false);
            mGoldPrize->setVisible(false);
            mGoldScore->setString("已完成！");
            mGoldScore->setColor(ccc3(128, 128, 128));
        }
        else
        {//not completed
            mGoldScore->setString(cocos2d::CCString::createWithFormat("%d", GameRecord::sharedGameRecord()->task->goldPrize->score)->getCString());
            mGoldPrize->setString(cocos2d::CCString::createWithFormat("%d", GameRecord::sharedGameRecord()->task->goldPrize->prize)->getCString());
        }
    }
    //silver
    {
        if( GameRecord::sharedGameRecord()->task->silverPrize->prize < 0 )
        {//completed
            mSilverCoin->setVisible(false);
            mSilverPrize->setVisible(false);
            mSilverScore->setString("已完成！");
            mSilverScore->setColor(ccc3(128, 128, 128));
        }
        else
        {//not completed
            mSilverScore->setString(cocos2d::CCString::createWithFormat("%d", GameRecord::sharedGameRecord()->task->silverPrize->score)->getCString());
            mSilverPrize->setString(cocos2d::CCString::createWithFormat("%d", GameRecord::sharedGameRecord()->task->silverPrize->prize)->getCString());
        }
    }
    //bronze
    {
        if( GameRecord::sharedGameRecord()->task->bronzePrize->prize < 0 )
        {//completed
            mBronzeCoin->setVisible(false);
            mBronzePrize->setVisible(false);
            mBronzeScore->setString("已完成！");
            mBronzeScore->setColor(ccc3(128, 128, 128));
        }
        else
        {//not completed
            mBronzeScore->setString(cocos2d::CCString::createWithFormat("%d", GameRecord::sharedGameRecord()->task->bronzePrize->score)->getCString());
            mBronzePrize->setString(cocos2d::CCString::createWithFormat("%d", GameRecord::sharedGameRecord()->task->bronzePrize->prize)->getCString());
        }
    }
    
    //--- update leader board ---
    //---- No.1 ----
    if( GameRecord::sharedGameRecord()->lba_scores[0] > 0 )
    {
        mLeadLb0->setString(cocos2d::CCString::createWithFormat("%d", GameRecord::sharedGameRecord()->lba_scores[0])->getCString());
    }
    else {
        mLeadLb0->setString("-");
    }
    //---- No.2 ----
    if( GameRecord::sharedGameRecord()->lba_scores[1] > 0 )
    {
        mLeadLb1->setString(cocos2d::CCString::createWithFormat("%d", GameRecord::sharedGameRecord()->lba_scores[1])->getCString());
    }
    else {
        mLeadLb1->setString("-");
    }
    //---- No.3 ----
    if( GameRecord::sharedGameRecord()->lba_scores[2] > 0 )
    {
        mLeadLb2->setString(cocos2d::CCString::createWithFormat("%d", GameRecord::sharedGameRecord()->lba_scores[2])->getCString());
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
    play->scheduleMask(ccc3(0, 0, 0), 128, 0);
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
        mTwitter->setNormalSpriteFrame(cocos2d::SpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("gowb.png"));
        mTwitter->setSelectedSpriteFrame(cocos2d::SpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("gowb2.png"));
        mTwitterAction->setDisplayFrame(cocos2d::SpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("gowb3.png"));
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
        mTwitter->setColor(ccc3(128, 128, 128));
        mTwitterAction->setColor(ccc3(128, 128, 128));
        mTwitter.isEnabled = false;
        
        mFacebook->setColor(ccc3(128, 128, 128));
        mFacebookAction->setColor(ccc3(128, 128, 128));
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

void GameOver::onChange() 
{
    GameTool::PlaySound("sound/click.mp3");
    GamePlay *play = GamePlay::sharedGamePlay();
    play->change();
}

void GameOver::onMenu() 
{
    
    GameTool::PlaySound("sound/click.mp3");
    GamePlay *play = GamePlay::sharedGamePlay();
    play->exit();
}

void GameOver::onRestart() 
{
    GameTool::PlaySound("sound/click.mp3");
    if( GameRecord::sharedGameRecord()->coins >= mCost )
    {
        if( mCost > 0 )
        {
            GameTool::PlaySound("sound/buy.mp3");
cocos2d::CCLog("cost=%d", mCost);
        }
        GameRecord::sharedGameRecord()->makeCoins(-mCost);
        GamePlay *play = GamePlay::sharedGamePlay();
        play->restart();
    }
    else {
        GameTool::PlaySound("sound/error.mp3");
    }
}

void GameOver::onFacebook() 
{
//    if( !mPostingFacebook )
//    {
//        mPostingFacebook = true;
//        
//        SimpleAudioEngine::sharedEngine()->playEffect(cocos2d::CCFileUtils::sharedFileUtils()->fullPathForFilename("sound/click.mp3");
//        
//        //do some facebook actions
//        
//        mFacebookAction->setDisplayFrame(cocos2d::SpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("ftloading.png"));
//        mFacebookAction->stopAllActions();
//        CCRotateBy *rb = cocos2d::CCRotateBy::create(1, 360);
//        CCRepeatForever *rf = cocos2d::CCRepeatForever::create(rb);
//        mFacebookAction->runAction(rf);
//    }
}

void GameOver::onTwitter() 
{
//    if( !mPostingTwitter )
//    {
//        mPostingTwitter = true;
//        
//        SimpleAudioEngine::sharedEngine()->playEffect(cocos2d::CCFileUtils::sharedFileUtils()->fullPathForFilename("sound/click.mp3");
//        AppController *del = (AppController*)UIApplication->sharedApplication().delegate;
//        
//        bool sina = UniversalFit::sharedUniversalFit()->shouldUsingSinaWeibo();
//        ABSocial *social = NULL;
//        if( sina )
//        {
//            social = ABSocial->feedOnSinaWeibo();
//            social.text = cocos2d::CCString::createWithFormat("刚刚在#LittleNinjaRush#的标准模式中跑了%dm，谁来挑战！", mScore);;
//        }
//        else
//        {
//            social = ABSocial->feedOnTwitter();
//            social.text = cocos2d::CCString::createWithFormat("I have just got %dm in #LittleNinjaRush, beat that!", mScore);
//        }
//        /*
//        social.image = makeScreenshot();
//        social.url = NSURL->URLWithString(TWITTER_URL);
//        social.callbackObject = MidBridge->sharedMidbridge();
//        social.callbackSelector =, );
//        social->present(del.navController);
//        */
//        
//        mTwitterAction->setDisplayFrame(cocos2d::SpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("ftloading.png"));
//        mTwitterAction->stopAllActions();
//        CCRotateBy *rb = cocos2d::CCRotateBy::create(1, 360);
//        CCRepeatForever *rf = cocos2d::CCRepeatForever::create(rb);
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
//    mFacebookAction->setDisplayFrame(cocos2d::SpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("gofb3.png"));
//    
//    if( this->haveFacebookCoins() && result > 0 )
//    {
//        GameRecord::sharedGameRecord()->makeCoins(100);
//        SimpleAudioEngine::sharedEngine()->playEffect(cocos2d::CCFileUtils::sharedFileUtils()->fullPathForFilename("sound/getcoin.mp3");
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
//        mTwitterAction->setDisplayFrame(cocos2d::SpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("gowb3.png"));
//    }
//    else
//    {
//        mTwitterAction->setDisplayFrame(cocos2d::SpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("gott3.png"));
//    }
//    
//    if( this->haveTwitterCoins() && result > 0 )
//    {
//        GameRecord::sharedGameRecord()->makeCoins(100);
//        SimpleAudioEngine::sharedEngine()->playEffect(cocos2d::CCFileUtils::sharedFileUtils()->fullPathForFilename("sound/getcoin.mp3");
//        GameRecord::sharedGameRecord()->share_twitter = NSDate->date()->timeIntervalSince1970();
//        GameRecord::sharedGameRecord()->checkPoint();
//        
//        GamePlay::sharedGamePlay()->pushNotification("You have got 100 coins\nfor sharing your score.", NULL, 5);
//        mTwitterCoins->setVisible(false);
//    }
}


SEL_MenuHandler GameOver::onResolveCCBCCMenuItemSelector(cocos2d::Ref * pTarget, const char* pSelectorName)
{
//CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "onChange", GameOver::onChange);
//CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "onMenu", GameOver::onMenu);
//CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "onRestart", GameOver::onRestart);
//CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "onFacebook", GameOver::onFacebook);
//CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "onTwitter", GameOver::onTwitter);
//    
    return NULL;
}

cocos2d::extension::Control::Handler   GameOver::onResolveCCBCCControlSelector(cocos2d::Ref * pTarget, const char* pSelectorName)
{
cocos2d::CCLog("Control");
  return NULL;
}
bool GameOver::onAssignCCBMemberVariable(cocos2d::Ref* pTarget, const char* pMemberVariableName, Node* pNode)
{
  //CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "mCoins", Label *, mCoins);
CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "mDistance", Label *, mDistance);
CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "mCoins", Label *, mCoins);
CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "mChange", CCMenuItemImage *, mChange);
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
CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "mPlayAgain", CCMenuItemImage *, mPlayAgain);
CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "mTwitter", CCMenuItemImage *, mTwitter);
CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "mFacebook", CCMenuItemImage *, mFacebook);
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
