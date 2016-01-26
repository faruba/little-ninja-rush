#include "ArcadeMode.h"
#include "GameConstants.h"
#include "GamePlay.h"
#include "UniversalFit.h"
#include "GameRecord.h"
#include "CommitArcade.h"
#include "PopKill.h"

int gArcMax[9] =
{
	4, 5, 6, 7, 8, 9, 10, 11, 12
};

int gArcPiple[9] =
{
	3, 4, 5, 5, 5, 5, 5, 5, 5
};

#define ARSTATE_RUN (0)
#define ARSTATE_OVER (1)



ArcadeMode* ArcadeMode::mode() 
{
	return ArcadeMode::create();
}

void ArcadeMode::onCreate() 
{
	mRespawn = Respawn::create();
	mRespawn->retain();
	mRespawn->setMode(MODE_ARCADE);
	mRespawn->setLevel(1);
	mRespawn->setPipleLine(gArcPiple[0]);

	mRespawnLevCache = 1;

	//init mode properties
	score = 0;
	speed = ARCADE_INITSPEED;
	feverlevel = 0;

	timer = ARCADE_TIMER;

	mState = ARSTATE_RUN;
	mKillTimer = -1;
	mKillSteak = 0;
	prepare = ARCADE_PREPARE_BASE;
}

void ArcadeMode::onUpdate(float delta) 
{
	GamePlay *play = GamePlay::sharedGamePlay();

	float deltatime = play->deltaTime*play->gamespeed;

	if( play->paused )
	{
		deltatime = 0;
	}

	if( mKillTimer >= 0 )
	{
		mKillTimer -= deltatime;
		if( mKillTimer < 0 )
		{
			mKillTimer = -1;
			mKillSteak = 0;
		}
	}

	switch (mState) {
		case ARSTATE_RUN:
			{
				if( timer <= 0 )
				{
					play->runspeed = 0;
					mStateTimer = 0;
					mState = ARSTATE_OVER;

					play->count_attack++;
					play->count_control++;
					play->count_respawn++;

					//set player AI
					cocos2d::Point mainpos = play->mainrole->position();
					mainpos.x = UniversalFit::sharedUniversalFit()->screenSize.width + 100;
					play->mainrole->setAI(1, mainpos);
					if( play->mainrole2 != NULL )
					{
						play->mainrole2->setAI(1, mainpos);
					}
					play->gameOverTimer = 0;
					play->stopCombo();
					play->togglePauseButton(false);
					play->canPause = false;
				}
				else
				{
					//run timer
					timer -= deltatime;
					bool updatePiple = false;

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

					//check respawn level
					{
						int n = (ARCADE_TIMER - timer)/10 + 1;
						if( n != mRespawnLevCache )
						{
							mRespawn->setLevel(n);
							mRespawnLevCache = n % 8;
							updatePiple = true;
						}
					}

					//update prepare
					{
						prepare = ARCADE_PREPARE_BASE + ARCADE_PREPARE_A*(speed - ARCADE_INITSPEED);
					}

					//shrink speed
					{
						float ds = ARCADE_INITSPEED - speed;
						float step = deltatime*(fabs(ds)*ARCADE_SHRINKSA + ARCADE_SHRINKSB);
						if( fabs(step) >= fabs(ds) )
						{
							speed = ARCADE_INITSPEED;
						}
						else
						{
							speed += ds/fabs(ds)*step;
						}
						//update speed
						//bound check
						if( speed < ARCADE_MINSPEED )
						{
							speed = ARCADE_MINSPEED;
						}
						if( speed > ARCADE_MAXSPEED )
						{
							speed = ARCADE_MAXSPEED;
						}
						//fever check
						{
							int fl = feverlevel;
							switch ( feverlevel ) {
								case 0:
									{
										if( speed >= ARCADE_FEVER1 )
										{
											fl = 1;
										}
									}
									break;
								case 1:
									{
										if( speed >= ARCADE_FEVER2 )
										{
											fl = 2;
										}
										else if( speed < ARCADE_FEVER1 - ARCADE_FEVERGAP )
										{
											fl = 0;
										}
									}
									break;
								case 2:
									{
										if( speed < ARCADE_FEVER2 - ARCADE_FEVERGAP )
										{
											fl = 1;
										}

									}
									break;
							}
							if( fl != feverlevel )
							{
								//apply fever level
								feverlevel = fl;
                //little boss
								//play->setFeverLevel(feverlevel);
								//updatePiple = true;
                //end
								if( fl == 0 )
								{
									play->interface->setArcadeX(0);
								}
								else
								{
									play->interface->setArcadeX(fl+1);
								}
							}
						}
					}

					//update piple
					if( updatePiple )
					{
						mRespawn->setPipleLine(gArcPiple[mRespawnLevCache] + 2*feverlevel);
					}

					//calc max player
					{
						int maxplayer = gArcMax[mRespawnLevCache] + (speed - ARCADE_INITSPEED)*ARCADE_ENEMY_A;
						play->maxenemy = maxplayer;
					}

					//apply speed
					{
						float gsa = speed - ARCADE_INITSPEED;
						gsa *= 0.3f/500.0f;
						float gamespeed = 1 + gsa;
						play->arcadeSpeed = gamespeed;
					}

					//update cup
					{
						int ccup = 0;
						if( GameRecord::sharedGameRecord()->task->goldPrize.prize > 0 &&
								score >= GameRecord::sharedGameRecord()->task->goldPrize.score )
						{
							ccup = 3;
						}
						else if( GameRecord::sharedGameRecord()->task->silverPrize.prize > 0 &&
								score >= GameRecord::sharedGameRecord()->task->silverPrize.score )
						{
							ccup = 2;
						}
						else if( GameRecord::sharedGameRecord()->task->bronzePrize.prize > 0 &&
								score >= GameRecord::sharedGameRecord()->task->bronzePrize.score )
						{
							ccup = 1;
						}
						cup = ccup;
					}

					//respawn new enemy
					mRespawn->updateArcade(delta);
				}
			}
			break;
		case ARSTATE_OVER:
			{
				if( mStateTimer >= 0 )
				{
					mStateTimer += delta;
					if( mStateTimer > 2 )
					{
						mStateTimer = -1;

						//game over
						play->triggerDeath();
						play->manager->addGameObject(CommitArcade::commit());
					}
				}
			}
			break;
	}
}

void ArcadeMode::onMainHit() 
{
    //little boss
  return;
  //end
	GamePlay *play = GamePlay::sharedGamePlay();
	//Apply Speed
	speed = ARCADE_MINSPEED;
	feverlevel = 0;
	play->interface->setArcadeX(0);
	play->setFeverLevel(0);

	//this->updateSpeed();//speed will be updated every frame
	//中断combo
	play->stopCombo();

	play->scheduleMask(Color3B(255, 0, 0), 0.5f, 1);
}

void ArcadeMode::killScore(int sc, int comb, cocos2d::Point pos) 
{
	if( mState == ARSTATE_RUN )
	{
		int add = sc;
		int sp = sc;
		if( comb > 0 )
		{
			bool steak = false;
			//kill steak
			if( mKillTimer >= 0 )
			{
				mKillTimer += 0.5f;
				mKillSteak++;
				int sc = mKillSteak;
				if( sc > 10)
				{
					sc = 10;
				}
				for(int i=1; i<sc; ++i)
				{
					add *= 2;
				}
				//pop kill
				GamePlay::sharedGamePlay()->manager->addGameObject(PopKill::popKill(true, mKillSteak, pos));
				steak = true;
			}
			else
			{
				mKillSteak = 1;
				mKillTimer = 1;
			}

			add += ARCADE_COMBOBGN + ARCADE_COMBOAdd*(comb-1);
			if( comb > 1 && !steak )
			{
				//pop kill
				GamePlay::sharedGamePlay()->manager->addGameObject(PopKill::popKill(false, ARCADE_COMBOBGN + ARCADE_COMBOAdd*(comb-1), pos));
			}
			sp *= 2;
		}
		add *= feverlevel+1;
		score += add;

		//Apply Speed
		speed += sp;
		//this->updateSpeed();//speed will be updated every frame
	}
}

void ArcadeMode::addScore(int sc, cocos2d::Point pos) 
{
	if( mState == ARSTATE_RUN )
	{
		score += sc;
		//pop kill
		GamePlay::sharedGamePlay()->manager->addGameObject(PopKill::popKill(false, sc, pos));
	}
}

void ArcadeMode::onDestroy() 
{
	mRespawn->release();
	mRespawn = NULL;
}


