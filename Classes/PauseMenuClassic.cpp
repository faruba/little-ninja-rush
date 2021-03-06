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
	//update objectives TODO:refactor this
	cocos2d::Label* labels[] = { mDailyObjective, mWeeklyObjective, mMonthlyObjective };
	cocos2d::Sprite* icons[] = { mDailyIcon, mWeeklyIcon, mMonthlyIcon };
	cocos2d::Node* crowns[] = {mDailyCrown, mWeeklyCrown, mMonthlyCrown};
	GameTool::UpdateObjectives(labels,icons,crowns);

	//update best score
	mBest->setString(std::string_format("最佳: %dm", GameRecord::sharedGameRecord()->score_high).c_str());
}

void PauseMenuClassic::updateArcade() 
{
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

	//update best score
	mBest->setString(std::string_format("最佳: %d", GameRecord::sharedGameRecord()->arcade_high).c_str());
}

void PauseMenuClassic::onCreate() 
{
	//hot load pause resource
	cocos2d::SpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("ui-pause.plist");
	cocos2d::SpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("ui-change.plist");

	GamePlay *play = GamePlay::sharedGamePlay();
	play->scheduleMask(Color3B(0, 0, 0), 128, 0);

	NodeLoaderLibrary *pNodeLib = NodeLoaderLibrary::sharedNodeLoaderLibrary();
	CCBReader *pReader = new CCBReader(pNodeLib, this, this);
	cocos2d::Node *node = pReader->readNodeGraphFromFile("pause_classic.ccbi", this);
	pReader->release();

	mBoard = CEClipedNode::create();
	mBoard->addChild(node);
	cocos2d::CCRect rect = cocos2d::Rect(60, 0, SCREEN_WIDTH-60, SCREEN_HEIGHT);
	//CCRect rect = cocos2d::Rect(60, 0, SCREEN_WIDTH*4, SCREEN_HEIGHT*4);
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
	GameTool::PlaySound("click.mp3");
	GamePlay *play = GamePlay::sharedGamePlay();
	play->manager->removeGameObject(this);
	play->exit();
}

void PauseMenuClassic::resume(Ref* ref)
{
	GameTool::PlaySound("click.mp3");
	GamePlay *play = GamePlay::sharedGamePlay();
	play->manager->removeGameObject(this);
	play->resume();
}

void PauseMenuClassic::retry(Ref* ref)
{
	GameTool::PlaySound("click.mp3");
	GamePlay *play = GamePlay::sharedGamePlay();
	play->manager->removeGameObject(this);
	play->restart();
}

void PauseMenuClassic::change(Ref* ref)
{
	/*
		 GameTool::PlaySound("click.mp3");
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
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "mSfx", MenuItemImage*, mSfx);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "mMusic", MenuItemImage*, mMusic);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "mQuit", MenuItemImage*, mQuit);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "mResume", MenuItemImage*, mResume);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "mRetry", MenuItemImage*, mRetry);
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "mChange", MenuItemImage*, mChange);

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
