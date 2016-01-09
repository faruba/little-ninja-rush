#include "GameScene.h"
#include "GameConstants.h"
#include "GamePlay.h"
#include "Scene1.h"
#include "Scene2.h"
#include "Scene3.h"
#include "Scene4.h"
#include "Scene5.h"
#include "Scene6.h"
#include "Scene7.h"
#include "UniversalFit.h"

int gSceneSeq[SCENE_COUNT];
int gSceneIndex;
bool gFirstScene = true;
bool gHalloween = false;
bool gChristmas = false;

GameScene* GameScene::scene(cocos2d::Node * parent) 
{
	GameScene* gs = GameScene::create();
	gs->mParent = parent;

	return gs;
}

void GameScene::initSceneSequence() 
{
	gSceneIndex = 0;
	while( gSceneIndex < SCENE_COUNT )
	{
		int x = randomInt(SCENE_COUNT);
		bool found = false;
		for(int i=0; i<gSceneIndex; ++i)
		{
			if( x == gSceneSeq[i] )
			{
				found = true;
				break;
			}
		}
		if( !found )
		{
			gSceneSeq[gSceneIndex] = x;
			gSceneIndex++;
		}
	}
	gSceneIndex = 0;
	gFirstScene = true;
	gHalloween = UniversalFit::isDuringHalloween();
	gChristmas = UniversalFit::isDuringChristmas();
}

int GameScene::getCurrentSceneId() 
{
	if( gFirstScene )
	{
		if( gHalloween )
		{
			return 5;//halloween scene
		}
		else if( gChristmas )
		{
			return 6;
		}
		else
		{
			return gSceneSeq[gSceneIndex];
		}
	}
	else
	{
		return gSceneSeq[gSceneIndex];
	}
}

void GameScene::nextScene() 
{
	if(gFirstScene)
	{
		gFirstScene = false;
	}
	else
	{
		gSceneIndex++;
		if( gSceneIndex >= SCENE_COUNT )
		{
			gSceneIndex = 0;
		}
	}
}

void GameScene::onCreate() 
{
	mScene = NULL;
	mSceneBgNode = cocos2d::Node::create();
	mSceneFbNode = cocos2d::Node::create();
	mSceneFgNode = cocos2d::Node::create();
	mParent->addChild(mSceneBgNode, LAYER_SCENEBG);
	//add tiles
	mTiles = RingTile::tile(60, UniversalFit::sharedUniversalFit()->playSize.width, 50);
	mTiles->retain();
	mTiles->setPosition(cocos2d::Vec2(0, 0));
	mParent->addChild(mTiles, LAYER_SCENEBG+1);
	mParent->addChild(mSceneFbNode, LAYER_ROLE-1);
	mParent->addChild(mSceneFgNode, LAYER_SCENEFG);

	//mStepSound = -1;
}

void GameScene::onUpdate(float delta) 
{
	GamePlay *play = GamePlay::sharedGamePlay();
	if( play->count_runscene <= 0 || (play->gameOverTimer>=0 && play->gameOverTimer<PLAY_GOSLIDETIME))
	{
		float ds = delta*play->runspeed;
		mTiles->runTiles(ds);
		mScene->update(ds);
	}
}

void GameScene::setScene(int sid) 
{
	//debug
	cocos2d::CCLog("setScene(%d)", sid);
	GamePlay *play = GamePlay::sharedGamePlay();
	mCurrScene = sid;
	mSceneBgNode->removeAllChildrenWithCleanup(true);
	mSceneFbNode->removeAllChildrenWithCleanup(true);
	mSceneFgNode->removeAllChildrenWithCleanup(true);
	mTiles->reset();
	play->clearFootPrints();
	if( mScene != NULL )
	{
		cocos2d::CCLog("unload scene");
		mScene->unloadScene();
		mScene->release();
		mScene = NULL;
	}

	//stop effect
	// GameScene::if( mStepSound >= 0 ) 
	//    {
	//        SimpleAudioEngine::sharedEngine()->stopEffect(mStepSound);
	//        mStepSound = -1;
	//    }

	// TODO:
	mCurrScene = 0;
	switch (mCurrScene) {
		case 0:
			{
				Scene1 *scene = Scene1::create();
				scene->retain();
				mScene = scene;
			}
			break;
		case 1:
			{
				Scene2 *scene = Scene2::create();
				scene->retain();
				mScene = scene;
			}
			break;
		case 2:
			{
				Scene3 *scene = Scene3::create();
				scene->retain();
				mScene = scene;
			}
			break;
		case 3:
			{
				Scene4 *scene = Scene4::create();
				scene->retain();
				mScene = scene;
			}
			break;
		case 4:
			{
				Scene5 *scene = Scene5::create();
				scene->retain();
				//begin sound
				//mStepSound = GameTool::PlaySound("waterstep.mp3", true);
				mScene = scene;
			}
			break;
		case 5:
			{
				Scene6 *scene = Scene6::create();
				scene->retain();
				mScene = scene;
			}
			break;
		case 6:
			{
				Scene7 *scene = Scene7::create();
				scene->retain();
				mScene = scene;
			}
			break;
	}
	cocos2d::CCLog("load scene");
	mScene->loadScene();
	mScene->createScene(mSceneBgNode, mSceneFbNode, mSceneFgNode, mTiles);

}

int GameScene::currentScene() 
{
	return mCurrScene;
}

float GameScene::offset() 
{
	return mTiles->offset();
}

void GameScene::onDestroy() 
{
	cocos2d::CCLog("unload scene");
	mTiles->reset();
	mScene->unloadScene();
	mParent->removeChild(mSceneBgNode, true);
	mParent->removeChild(mTiles, false);
	mParent->removeChild(mSceneFgNode, true);
	mParent->removeChild(mSceneFbNode, true);
	mTiles->release();
	mScene->release();
}


