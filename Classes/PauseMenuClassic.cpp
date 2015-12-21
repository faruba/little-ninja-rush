#include "PauseMenuClassic.h"
#include "GamePlay.h"
#include "SimpleAudioEngine.h"
#include "GameRecord.h"
#include "UniversalFit.h"

;
USING_NS_CC_EXT;

PauseMenuClassic* PauseMenuClassic::pauseMenu() 
{
  return PauseMenuClassic::create();
}

void PauseMenuClassic::updateClassic() 
{
    //update objectives
    if( GameRecord::sharedGameRecord()->task->dailyObjective->uiid >= 0 )
    {
        Achievement *obj = Tasks::dailyObjectiveWithUiid(GameRecord::sharedGameRecord()->task->dailyObjective->uiid);
        mDailyObjective->setString(Tasks::stringForObjective(obj->desc ,obj->achieveCode , obj->achieveNumber ,GameRecord::sharedGameRecord()->task->dailyObjective->count)->getCString());
        mDailyObjective->setColor(ccc3(255, 255, 255));
        
        mDailyIcon->setVisible(true);
        Achievement *ach = Tasks::dailyObjectiveWithUiid(GameRecord::sharedGameRecord()->task->dailyObjective->uiid);
cocos2d::Sprite *icon = cocos2d::Sprite::createWithSpriteFrameName(ach->icon->getCString());
        icon->setPosition(cocos2d::Vec2(mDailyIcon->getContentSize().width/2, mDailyIcon->getContentSize().height/2));
        mDailyIcon->addChild(icon);
        
        //判断是否完成
        if( GameRecord::sharedGameRecord()->task->dailyObjective->count >= ach->achieveNumber )
        {
            mDailyObjective->setColor(ccc3(128, 128, 128));
        }
    }
    else {
        mDailyObjective->setString("已完成！");
        mDailyObjective->setColor(ccc3(128, 128, 128));
    }
    if( GameRecord::sharedGameRecord()->task->weeklyObjective->uiid >= 0 )
    {
        Achievement *obj = Tasks::weeklyObjectiveWithUiid(GameRecord::sharedGameRecord()->task->weeklyObjective->uiid);
        mWeeklyObjective->setString(Tasks::stringForObjective(obj->desc ,obj->achieveCode ,obj->achieveNumber ,GameRecord::sharedGameRecord()->task->weeklyObjective->count)->getCString());
        mWeeklyObjective->setColor(ccc3(255, 255, 255));
        
        mWeeklyIcon->setVisible(true);
        Achievement *ach = Tasks::weeklyObjectiveWithUiid(GameRecord::sharedGameRecord()->task->weeklyObjective->uiid);
cocos2d::Sprite *icon = cocos2d::Sprite::createWithSpriteFrameName(ach->icon->getCString());
        icon->setPosition(cocos2d::Vec2(mWeeklyIcon->getContentSize().width/2, mDailyIcon->getContentSize().height/2));
        mWeeklyIcon->addChild(icon);
        
        //判断是否完成
        if( GameRecord::sharedGameRecord()->task->weeklyObjective->count >= ach->achieveNumber )
        {
            mWeeklyObjective->setColor(ccc3(128, 128, 128));
        }
    }
    else {
        mWeeklyObjective->setString("已完成！");
        mWeeklyObjective->setColor(ccc3(128, 128, 128));
    }
    if( GameRecord::sharedGameRecord()->task->monthlyObjective->uiid >= 0 )
    {
        Achievement *obj = Tasks::monthlyObjectiveWithUiid(GameRecord::sharedGameRecord()->task->monthlyObjective->uiid);
        mMonthlyObjective->setString(Tasks::stringForObjective(obj->desc ,obj->achieveCode ,obj->achieveNumber ,GameRecord::sharedGameRecord()->task->monthlyObjective->count)->getCString());
        mMonthlyObjective->setColor(ccc3(255, 255, 255));
        
        mMonthlyIcon->setVisible(true);
        Achievement *ach = Tasks::monthlyObjectiveWithUiid(GameRecord::sharedGameRecord()->task->monthlyObjective->uiid);
cocos2d::Sprite *icon = cocos2d::Sprite::createWithSpriteFrameName(ach->icon->getCString());
        icon->setPosition(cocos2d::Vec2(mMonthlyIcon->getContentSize().width/2, mDailyIcon->getContentSize().height/2));
        mMonthlyIcon->addChild(icon);
        
        //判断是否完成
        if( GameRecord::sharedGameRecord()->task->monthlyObjective->count >= ach->achieveNumber )
        {
            mMonthlyObjective->setColor(ccc3(128, 128, 128));
        }
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
    
    //update best score
    mBest->setString(cocos2d::CCString::createWithFormat("最佳: %dm", GameRecord::sharedGameRecord()->score_high)->getCString());
}

void PauseMenuClassic::updateArcade() 
{
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
    
    //update best score
    mBest->setString(cocos2d::CCString::createWithFormat("最佳: %d", GameRecord::sharedGameRecord()->arcade_high)->getCString());
}

void PauseMenuClassic::onCreate() 
{
    //hot load pause resource
cocos2d::SpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("ui-pause.plist");
cocos2d::SpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("ui-change.plist");
    
    GamePlay *play = GamePlay::sharedGamePlay();
    play->scheduleMask(ccc3(0, 0, 0), 128, 0);
    
    NodeLoaderLibrary *pNodeLib = NodeLoaderLibrary::sharedNodeLoaderLibrary();
CCBReader *pReader = new CCBReader(pNodeLib, this, this);
cocos2d::Node *node = pReader->readNodeGraphFromFile("pause_classic.ccbi", this);
    pReader->release();
    
    mBoard = CEClipedNode::create();
    mBoard->addChild(node);
cocos2d::CCRect rect = cocos2d::CCRectMake(60, 0, SCREEN_WIDTH-60, SCREEN_HEIGHT);
    //CCRect rect = cocos2d::CCRectMake(60, 0, SCREEN_WIDTH*4, SCREEN_HEIGHT*4);
    mBoard->setClipRect(new CCRect(UniversalFit::sharedUniversalFit()->transformRect(rect)));
    mBoard->setPosition(cocos2d::Vec2(UniversalFit::sharedUniversalFit()->baseLeft-301, 68));
    play->ui()->addChild(mBoard);
    
    mRode = cocos2d::Sprite::createWithSpriteFrameName("zt_jz.png");
    mRode->setPosition(cocos2d::Vec2(UniversalFit::sharedUniversalFit()->baseLeft + 60, 180));
    play->ui()->addChild(mRode);
    mTimer = 0;
    mFlag = false;
    
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
    
    //sychornize music & sfx
    if( SimpleAudioEngine::sharedEngine()->getBackgroundMusicVolume() == 0 )
    {
        mMusic->setNormalImage(cocos2d::Sprite::createWithSpriteFrameName("zt_yy3.png"));
        mMusic->setSelectedImage(cocos2d::Sprite::createWithSpriteFrameName("zt_yy4.png"));
    }
    else {
        mMusic->setNormalImage(cocos2d::Sprite::createWithSpriteFrameName("zt_yy1.png"));
        mMusic->setSelectedImage(cocos2d::Sprite::createWithSpriteFrameName("zt_yy2.png"));
    }
    if( SimpleAudioEngine::sharedEngine()->getEffectsVolume() == 0 )
    {
        mSfx->setNormalImage(cocos2d::Sprite::createWithSpriteFrameName("zt_yx3.png"));
        mSfx->setSelectedImage(cocos2d::Sprite::createWithSpriteFrameName("zt_yx4.png"));
    }
    else {
        mSfx->setNormalImage(cocos2d::Sprite::createWithSpriteFrameName("zt_yx.png"));
        mSfx->setSelectedImage(cocos2d::Sprite::createWithSpriteFrameName("zt_yx2.png"));
    }
    
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

void PauseMenuClassic::onUpdate(float delta) 
{
    GamePlay *play = GamePlay::sharedGamePlay();
    mTimer += play->deltaTime;
    if( mTimer <= 0.2f )
    {
        mBoard->setPosition(cocos2d::Vec2( UniversalFit::sharedUniversalFit()->baseLeft-301 + 371*mTimer/0.2f, 68));
    }
    if ( mTimer > 0.2f && !mFlag ) 
    {
        mFlag = true;
        mBoard->setPosition(cocos2d::Vec2( UniversalFit::sharedUniversalFit()->baseLeft-301 + 371, 68));
        mQuit->setVisible(true);
        mResume->setVisible(true);
        mRetry->setVisible(true);
    }
}

void PauseMenuClassic::onDestroy() 
{
    GamePlay *play = GamePlay::sharedGamePlay();
    play->unscheduleMask();
    play->ui()->removeChild(mRode,true);
    play->ui()->removeChild(mBoard,true);
    
    //hot release pause resource
cocos2d::SpriteFrameCache::sharedSpriteFrameCache()->removeSpriteFramesFromFile("ui-pause.plist");
    unloadTextureFromeSpriteFrameFile("ui-pause.plist");
cocos2d::SpriteFrameCache::sharedSpriteFrameCache()->removeSpriteFramesFromFile("ui-change.plist");
    unloadTextureFromeSpriteFrameFile("ui-change.plist");
}

void PauseMenuClassic::toggleSfx(Ref* ref)
{
    if( GameRecord::sharedGameRecord()->setting_sfx )
    {
        GameRecord::sharedGameRecord()->setting_sfx = 0;
        mSfx->setNormalImage(cocos2d::Sprite::createWithSpriteFrameName("zt_yx3.png"));
        mSfx->setSelectedImage(cocos2d::Sprite::createWithSpriteFrameName("zt_yx4.png"));
        SimpleAudioEngine::sharedEngine()->setEffectsVolume(0);
    }
    else{
        GameRecord::sharedGameRecord()->setting_sfx = 1;
        mSfx->setNormalImage(cocos2d::Sprite::createWithSpriteFrameName("zt_yx.png"));
        mSfx->setSelectedImage(cocos2d::Sprite::createWithSpriteFrameName("zt_yx2.png"));
        SimpleAudioEngine::sharedEngine()->setEffectsVolume(1);
    }
}

void PauseMenuClassic::toggleMusic(Ref* ref)
{
    if( GameRecord::sharedGameRecord()->setting_music )
    {
        GameRecord::sharedGameRecord()->setting_music = 0;
        mMusic->setNormalImage(cocos2d::Sprite::createWithSpriteFrameName("zt_yy3.png"));
        mMusic->setSelectedImage(cocos2d::Sprite::createWithSpriteFrameName("zt_yy4.png"));
        SimpleAudioEngine::sharedEngine()->setBackgroundMusicVolume(0);
    }
    else{
        GameRecord::sharedGameRecord()->setting_music = 1;
        mMusic->setNormalImage(cocos2d::Sprite::createWithSpriteFrameName("zt_yy1.png"));
        mMusic->setSelectedImage(cocos2d::Sprite::createWithSpriteFrameName("zt_yy2.png"));
        SimpleAudioEngine::sharedEngine()->setBackgroundMusicVolume(1);
    }
}

void PauseMenuClassic::quit(Ref* ref)
{
    GameTool::PlaySound("sound/click.mp3");
    GamePlay *play = GamePlay::sharedGamePlay();
    play->manager->removeGameObject(this);
    play->exit();
}

void PauseMenuClassic::resume(Ref* ref)
{
    GameTool::PlaySound("sound/click.mp3");
    GamePlay *play = GamePlay::sharedGamePlay();
    play->manager->removeGameObject(this);
    play->resume();
}

void PauseMenuClassic::retry(Ref* ref)
{
    GameTool::PlaySound("sound/click.mp3");
    GamePlay *play = GamePlay::sharedGamePlay();
    play->manager->removeGameObject(this);
    play->restart();
}

void PauseMenuClassic::change(Ref* ref)
{
    /*
    GameTool::PlaySound("sound/click.mp3");
    GamePlay *play = GamePlay::sharedGamePlay();
    play->change();
     */
}

//cocosbuilder support
bool PauseMenuClassic::onAssignCCBMemberVariable(cocos2d::Ref* pTarget, const char* pMemberVariableName, Node* pNode)
{
CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "mDailyObjective", Label*, mDailyObjective);
CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "mWeeklyObjective", Label*, mWeeklyObjective);
CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "mMonthlyObjective", Label*, mMonthlyObjective);
CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "mDailyCrown", Node*, mDailyCrown);
CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "mWeeklyCrown", Node*, mWeeklyCrown);
CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "mMonthlyCrown", Node*, mMonthlyCrown);
CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "mDailyIcon", Sprite*, mDailyIcon);
CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "mWeeklyIcon", Sprite*, mWeeklyIcon);
CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "mMonthlyIcon", Sprite*, mMonthlyIcon);
CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "mClassic", Node*, mClassic);
CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "mArcade", Node*, mArcade);
CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "mGoldCoin", Sprite*, mGoldCoin);
CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "mSilverCoin", Sprite*, mSilverCoin);
CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "mBronzeCoin", Sprite*, mBronzeCoin);
CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "mGoldScore", Label*, mGoldScore);
CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "mGoldPrize", Label*, mGoldPrize);
CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "mSilverScore", Label*, mSilverScore);
CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "mSilverPrize", Label*, mSilverPrize);
CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "mBronzeScore", Label*, mBronzeScore);
CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "mBronzePrize", Label*, mBronzePrize);
CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "mMenu", CCMenu*, mMenu);
CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "mObjDaily", Node*, mObjDaily);
CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "mObjWeekly", Node*, mObjWeekly);
CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "mObjMonthly", Node*, mObjMonthly);
CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "mBest", Label*, mBest);
CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "mSfx", CCMenuItemImage*, mSfx);
CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "mMusic", CCMenuItemImage*, mMusic);
CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "mQuit", CCMenuItemImage*, mQuit);
CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "mResume", CCMenuItemImage*, mResume);
CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "mRetry", CCMenuItemImage*, mRetry);
CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "mChange", CCMenuItemImage*, mChange);
    
    return false;
}

SEL_MenuHandler  PauseMenuClassic::onResolveCCBCCMenuItemSelector(cocos2d::Ref * pTarget, const char* pSelectorName)
{
CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "toggleSfx", PauseMenuClassic::toggleSfx);
CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "toggleMusic", PauseMenuClassic::toggleMusic);
CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "quit", PauseMenuClassic::quit);
CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "resume", PauseMenuClassic::resume);
CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "retry", PauseMenuClassic::retry);
CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "change", PauseMenuClassic::change);

    return NULL;
}

cocos2d::extension::Control::Handler  PauseMenuClassic::onResolveCCBCCControlSelector(cocos2d::Ref * pTarget, const char* pSelectorName)
{
    return NULL;
}
