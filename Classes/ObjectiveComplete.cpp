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
	play->scheduleMask(Color3B(0, 0, 0), 128, 0);
	cocosbuilder::NodeLoaderLibrary *pNodeLib = cocosbuilder::NodeLoaderLibrary::getInstance();
	cocosbuilder::CCBReader *pReader = new cocosbuilder::CCBReader(pNodeLib, this, this);
	mNode = pReader->readNodeGraphFromFile("ui-objective", this);
	pReader->release();
	mNode->setPosition(cocos2d::Vec2(UniversalFit::sharedUniversalFit()->baseLeft, 0));
	play->addChild(mNode, LAYER_MASK+1);
	cocos2d::CCRect clip = UniversalFit::sharedUniversalFit()->transformRect(cocos2d::Rect(0, 142, UniversalFit::sharedUniversalFit()->playSize.width, 36));
	mObjRect = CEClipedNode::create();
	mObjRect->setClipRect(&clip);
	mObjRect->setPosition(cocos2d::Vec2(12, 12));
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
			GameTool::PlaySound("getcoin.mp3");
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
		mBounsCoins->setString(std::string_format("%04d", mCurrBounsCoins).c_str());
	}
	if( mCurrBounsScrolls != mTargetBounsScrolls )
	{
		mScroll->setVisible(true);
		mBounsScroll->setString(std::string_format("x%d", mTargetBounsScrolls).c_str());
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
					cocos2d::SpriteFrameCache *cache = cocos2d::SpriteFrameCache::sharedSpriteFrameCache();
					mObjType->setDisplayFrame(cache->spriteFrameByName(std::string_format("objc-%d.png", mCurrIndex).c_str()));
					mObjRect->setPosition(cocos2d::Vec2(12, 12));
					cocos2d::Node *currobj = this->genObjectiveInfo(mCurrAch, mCurrIndex);
					currobj->setPosition(cocos2d::Vec2(18, 18));
					mObjRect->addChild(currobj);
					cocos2d::Point np = mBanner->getPosition();
					np.x = -240;
					mBanner->setPosition(np);
					//add crowns
					for(int i=0; i<mCurrObj->index; ++i)
					{
						cocos2d::Sprite *crown = cocos2d::Sprite::createWithSpriteFrameName(std::string_format("crown%d.png", i).c_str());
						crown->setPosition(cocos2d::Vec2(-20*i, 0));
						mCrowns->addChild(crown);
					}
					mStateTimer = 0;
					GameTool::PlaySound("flip.mp3");
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
					GameTool::PlaySound("getcrown.mp3");
					std::string crownname = std::string_format("crown%d.png", mCurrObj->index);
					cocos2d::Sprite *crown = cocos2d::Sprite::createWithSpriteFrameName(crownname.c_str());
					crown->setPosition(cocos2d::Vec2(-20*mCurrObj->index, 0));
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
					Tasks *task = GameRecord::sharedGameRecord()->task;
					ObjectiveManager *managers[] = { &task->dailyObjective, &task->weeklyObjective, &task->monthlyObjective };
					if ( mCurrObj->index < 2 ) {
						managers[mCurrIndex]->refresh();
						mCurrObj = &(managers[mCurrIndex]->currentObjective);
						mCurrAch = managers[mCurrIndex]->info();
						cocos2d::Node *newobj = this->genObjectiveInfo(mCurrAch, mCurrIndex);
						newobj->setPosition(cocos2d::Vec2(18, 54));
						mObjRect->addChild(newobj);
					} else {
						mCurrAch = NULL;
						cocos2d::Node *newobj = this->genObjectiveInfo(NULL, mCurrIndex);
						newobj->setPosition(cocos2d::Vec2(18, 54));
						mObjRect->addChild(newobj);
						//mark completed
						for (int i = 0; i < sizeof(managers); i++) {
							managers[i]->currentObjective.index = 3;
							managers[i]->currentObjective.uiid = -1;
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
					GameTool::PlaySound("flip.mp3");
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

void ObjectiveComplete::retriveObjectiveInfo(int index, Objective ** obj, const Achievement ** ach)
{
	*obj = NULL;
	*ach = NULL;
	Tasks *task = GameRecord::sharedGameRecord()->task;
	ObjectiveManager *managers[] = { &task->dailyObjective, &task->weeklyObjective, &task->monthlyObjective };
	*ach = managers[index]->info();
  *obj = &(managers[index]->currentObjective);
}

cocos2d::Node* ObjectiveComplete::genObjectiveInfo(const Achievement*  ach, int typ)
{
	if( ach == NULL )
	{
		cocos2d::Node *sp = cocos2d::Node::create();
		switch (typ) {
			case 0:
				{
					cocos2d::Label *desc = cocos2d::Label::create("日常任务\n已全部完成！", GFONT_NAME, GFONT_SIZE_NORMAL );
					desc->setColor(Color3B(128, 128, 128));
					desc->setAnchorPoint(cocos2d::Vec2(0, 0.5f));
					desc->setPosition(cocos2d::Vec2(54, 0));
					sp->addChild(desc);
				}
				break;
			case 1:
				{
					cocos2d::Label *desc = cocos2d::Label::create("周常任务\n已全部完成！", GFONT_NAME, GFONT_SIZE_NORMAL);
					desc->setColor(Color3B(128, 128, 128));
					desc->setAnchorPoint(cocos2d::Vec2(0, 0.5f));
					desc->setPosition(cocos2d::Vec2(54, 0));
					sp->addChild(desc);
				}
				break;
			case 2:
				{
					cocos2d::Label *desc = cocos2d::Label::create("月常任务\n已全部完成！", GFONT_NAME, GFONT_SIZE_NORMAL);
					desc->setColor(Color3B(128, 128, 128));
					desc->setAnchorPoint(cocos2d::Vec2(0, 0.5f));
					desc->setPosition(cocos2d::Vec2(54, 0));
					sp->addChild(desc);
				}
				break;
		}
		return sp;
	}
	else {
		cocos2d::Sprite *spbg = NULL;
		switch (typ) {
			case 0:
				{
					spbg = cocos2d::Sprite::createWithSpriteFrameName("ms_bg1.png");
				}
				break;
			case 1:
				{
					spbg = cocos2d::Sprite::createWithSpriteFrameName("ms_bg3.png");
				}
				break;
			case 2:
				{
					spbg = cocos2d::Sprite::createWithSpriteFrameName("ms_bg2.png");
				}
				break;
		}
		cocos2d::Sprite *spicon = cocos2d::Sprite::createWithSpriteFrameName(ach->icon.c_str());
		spicon->setPosition(cocos2d::Vec2(spbg->getContentSize().width/2, spbg->getContentSize().height/2));
		spbg->addChild(spicon, 0, 0);
		cocos2d::Label *desc = cocos2d::Label::createWithBMFont("ab34.fnt", ach->desc.c_str());
		desc->setAnchorPoint(cocos2d::Vec2(0, 0.5f));
		desc->setPosition(cocos2d::Vec2(54, 12));
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

SEL_MenuHandler ObjectiveComplete::onResolveCCBCCMenuItemSelector(cocos2d::Ref * pTarget, const char* pSelectorName)
{
	//  CCLog(pSelectorName);
	return NULL;
}

cocos2d::extension::Control::Handler   ObjectiveComplete::onResolveCCBCCControlSelector(cocos2d::Ref * pTarget, const char* pSelectorName)
{
	cocos2d::CCLog("Control");
	return NULL;
}
bool ObjectiveComplete::onAssignCCBMemberVariable(cocos2d::Ref* pTarget, const char* pMemberVariableName, Node* pNode)
{
	CCB_MEMBERVARIABLEASSIGNER_GLUE(this,"mBanner", Sprite *, mBanner)
		CCB_MEMBERVARIABLEASSIGNER_GLUE(this,"mObjType", Sprite *, mObjType)
		CCB_MEMBERVARIABLEASSIGNER_GLUE(this,"mCrowns", Node *, mCrowns)
		CCB_MEMBERVARIABLEASSIGNER_GLUE(this,"mBounsCoins", Label *, mBounsCoins)
		CCB_MEMBERVARIABLEASSIGNER_GLUE(this,"mScroll", Sprite *, mScroll)
		CCB_MEMBERVARIABLEASSIGNER_GLUE(this,"mBounsScroll", Label *, mBounsScroll)
		//  CCLog(pMemberVariableName);

		return false;
}
