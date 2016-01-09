#include "ClassicMode.h"
#include "GamePlay.h"
#include "GameRecord.h"
#include "GameConstants.h"
#include "SpeedLine.h"
#include "Tasks.h"
#include "GameOver.h"
#include "CommitScroll.h"
#include "ObjectiveComplete.h"
#include "ClassicContinue.h"

#include "UniversalFit.h"

ClassicMode* ClassicMode::mode() 
{
	return ClassicMode::create();
}

void ClassicMode::onCreate() 
{
	mRespawn = Respawn::create();
	mRespawn->setMode(MODE_CLASSIC);
	mRespawn->setLevel(1);
	mRespawn->retain();

	mSpeedlineTimer = 0;
	mUsedMagicCredit = false;
}

void ClassicMode::onUpdate(float delta) 
{
	GamePlay *play = GamePlay::sharedGamePlay();
	switch (play->state) {
		case STATE_NORMAL://正常状态
			{
				play->distance += play->runspeed*delta;
				if(play->distance > play->changeflag - play->runspeed*3 )
				{
					play->state = STATE_CHANGESCENE;
					play->count_attack++;
					play->count_respawn++;
					play->substate = 0;
					play->mainrole->toggleInvincible(true);
					if( play->mainrole2 != NULL )
					{
						play->mainrole2->toggleInvincible(true);
					}
				}
				//简单的死亡判断
				if( play->mainrole->HP<=0 )
				{
					play->stopCombo();

					play->canPause = false;
					//game over
					play->state = STATE_DEAD;
					play->substate = 0;
					play->stateTimer = 0;
					play->count_control++;
					play->count_attack++;
					play->count_respawn++;
					play->count_runscene++;
					play->gameOverTimer = 0;
					play->scheduleSpeed(0, 1, 0);

					//achievement die in scene
					if( play->scenecount == 1 )
					{
						GameRecord::sharedGameRecord()->task->dispatchTask(ACH_DIEINSCENE1, 1);
					}
					if( play->tiles->currentScene() == 3 )
					{
						GameRecord::sharedGameRecord()->task->dispatchTask(ACH_DIEINSCENE3, 1);
					}
				}
				//combo 衰减
				if( play->combo > 0 )
				{
					play->comboTimer -= delta;
					play->interface->updateHits(play->comboTimer);

					if( play->comboTimer <= 0 )
					{
						play->stopCombo();
					}
				}
			}
			break;
		case STATE_CHANGESCENE://切换场景
			{
				switch (play->substate) {
					case 0:
						{
							play->distance += play->runspeed*delta;
							if( play->distance > play->changeflag )
							{
								play->substate = 1;//running away
								play->count_control++;
								play->mainrole->setAI(1, Vec2( 600, 0));
								//分身术
								if( play->mainrole2 != NULL )
								{
									play->mainrole2->setAI(1, Vec2( 600, 0));
								}
								play->stateTimer = 0;
								play->scheduleMask(Color3B(0, 0, 0), GAME_SCENEFADE, 2);
							}
						}
						break;
					case 1:
						{//切换场景
							play->stateTimer += delta;
							if( play->stateTimer > GAME_SCENEFADE )
							{
								//load new scene
								GameScene::nextScene();
								play->tiles->setScene(GameScene::getCurrentSceneId());
								//--------------
								//清除场上的敌人
								while( play->enemies->count() > 0 )
								{
									GameObject* em = (GameObject*)play->enemies->lastObject();
									play->enemies->removeLastObject();
									play->manager->removeGameObject(em);
								}

								play->scheduleMask(Color3B(0, 0, 0), GAME_SCENEFADE, 1);

								if( play->mainrole2 != NULL )
								{//分身术
									play->mainrole->setAI(2, Vec2(-150+SPECIAL_FENSHEN, PLAY_PLAYERLINE));
									play->mainrole->setAI(1, Vec2(UniversalFit::sharedUniversalFit()->playSize.width/2+SPECIAL_FENSHEN/2, 0));
									play->mainrole2->setAI(2, Vec2(-150, PLAY_PLAYERLINE));
									play->mainrole2->setAI(1, Vec2(UniversalFit::sharedUniversalFit()->playSize.width/2-SPECIAL_FENSHEN/2, 0));
								}
								else {
									play->mainrole->setAI(2, Vec2(-100, PLAY_PLAYERLINE));
									play->mainrole->setAI(1, Vec2(UniversalFit::sharedUniversalFit()->playSize.width/2, 0));
								}
								play->substate = 2;
								play->stateTimer = 0;
								play->scenecount++;
								//提高游戏难度
								play->difficulty += GAME_DIFFGROWTH;
								play->runspeed += GAME_RUNSPEEDGROWTH;
								play->levelspeed = play->runspeed;
								if( play->runspeed > GAME_MAXRUNSPEED )
								{
									play->runspeed = GAME_MAXRUNSPEED;
								}

								//-------------
								if( play->scenecount > 3 )
								{
									int n = (play->scenecount - 4)/2 + 1;
									play->maxenemy = 7 + n;
								}
								else {
									play->maxenemy = GAME_INITMAXENEMY + play->scenecount - 1;
								}
								//-------------

								mRespawn->setLevel(play->scenecount);
								//#combo time 补偿
								if( play->comboTimer > 0 )
								{
									play->comboTimer += 3;
									play->interface->updateHits(play->comboTimer);
								}

								//achievement scene passed
								GameRecord::sharedGameRecord()->task->dispatchTask(ACH_SCENEPASSED, 1);
								if( play->sceneSpell == 0 )
								{
									GameRecord::sharedGameRecord()->task->dispatchTask(ACH_SCENESWITHOUTSPELL, 1);
								}
								play->sceneSpell = 0;
								if( !play->getitem )
								{
									GameRecord::sharedGameRecord()->task->dispatchTask(ACH_falseITEM, 1);
								}
								play->getitem = false;
								if( !play->missshoot )
								{
									GameRecord::sharedGameRecord()->task->dispatchTask(ACH_NEVERMISS, 1);
								}
								play->missshoot = false;
							}
						}
						break;
					case 2:
						{
							play->stateTimer += delta;
							if( play->stateTimer > GAME_SCENEFADE )
							{
								play->state = 0;
								play->count_attack--;
								play->count_control--;
								play->count_respawn--;
								play->changeflag = play->scenecount*PLAY_SCENELEN*PLAY_DISMETER;

								play->mainrole->toggleInvincible(false);
								if( play->mainrole2 != NULL )
								{
									play->mainrole2->toggleInvincible(false);
								}
							}
						}
						break;
				}
			}
			break;
		case STATE_DEAD://game over?
			{
				float k = (PLAY_GOSLIDETIME-play->gameOverTimer)/PLAY_GOSLIDETIME;
				if( k<0 )
				{
					k=0;
				}
				play->runspeed = k*play->levelspeed;

				play->stateTimer += delta;
				if( play->stateTimer > 3)
					if( play->stateTimer > 3 && play->substate == 0 )
					{
						//play->scrolls += 50; //TODO: this is test
						if( play->scrolls <= 0 )
						{
							this->goOver();
						}
						else {
							play->manager->addGameObject(CommitScroll::commitScorll());
						}
						play->togglePauseButton(false);
						play->substate = 1;
					}
			}
			break;
		case STATE_MERCHANT://商人状态
			{

			}
			break;
		case STATE_TUTORIAL://教程状态
			{

			}
			break;
		case STATE_REVIVE://复活
			{
				play->stateTimer += delta;
				if( play->stateTimer < PLAY_REVIVETIME )
				{
					if( play->stateTimer > 0.5f )
					{
						float k = (play->stateTimer-0.5f)/1.0f;
						play->runspeed = k*play->levelspeed;
					}
				}
				else {
					play->canPause = true;
					play->count_control--;
					play->count_respawn--;
					play->count_attack--;
					play->runspeed = play->levelspeed;
					play->state = STATE_NORMAL;
					play->stateTimer = 0;
					play->substate = 0;
					play->togglePauseButton(true);
				}
			}
			break;
		case STATE_RUSH:
			{
				play->distance += play->runspeed*delta;
				if( play->distance/PLAY_DISMETER > 800 )
				{
					play->count_attack--;
					play->count_control--;
					play->runspeed = play->levelspeed;
					play->state = STATE_NORMAL;
					play->mainrole->toggleShadow(false);
					play->mainrole->setShadowSpeed(0);
					play->mainrole->toggleInvincible(false);
				}
				else {
					float dm = play->distance/PLAY_DISMETER;
					if( dm < 700 )
					{
						play->runspeed = play->levelspeed*(dm/100.0f + 3.0f);
					}
					else {
						play->runspeed = play->levelspeed*(-dm/25.0f + 33);
					}
					if( play->runspeed > 3*play->levelspeed )
					{
						mSpeedlineTimer+=delta;
						if( mSpeedlineTimer > 0.05f )
						{
							play->manager->addGameObject(SpeedLine::line());
							mSpeedlineTimer = 0;
						}
					}
				}
			}
			break;
	}
	mRespawn->updateClassic(delta);
}

void ClassicMode::goOver() 
{
	GamePlay *play = GamePlay::sharedGamePlay();
	if( !mUsedMagicCredit && GameRecord::sharedGameRecord()->item_credit > 0 )
	{
		play->manager->addGameObject(ClassicContinue::classicContinue());
		mUsedMagicCredit = true;
	}
	else
	{
		play->triggerDeath();
		if( GamePlay::sharedGamePlay()->completeSomeObjectives() )
		{
			play->manager->addGameObject(ObjectiveComplete::commitObjective());
		}
		else {
			play->manager->addGameObject(GameOver::gameOver(play));
		}
	}
}

void ClassicMode::onDestroy() 
{
	mRespawn->release();
	mRespawn = NULL;
}
