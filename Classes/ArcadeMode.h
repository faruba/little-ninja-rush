#ifndef ___CLASSES_ARCADEMODE_H_
#define ___CLASSES_ARCADEMODE_H_
#include "Respawn.h"
#include "GameObject.h"
#include "GameTool.h"

class ArcadeMode:
	public GameObject
{
	public:
		CREATE_FUNC(ArcadeMode);

		virtual bool init() {return true;};
		virtual void onCreate();
		virtual void onUpdate(float);
		virtual void onDestroy();


		Respawn *mRespawn;

		//Current Respawn Level
		int mRespawnLevCache;

		int mState;
		float mStateTimer;

		float mKillTimer;
		int   mKillSteak;

		static ArcadeMode* mode();

		//*** Properties ***
		int score;
		float timer;
		float speed;//1000 = 1.0x
		int feverlevel;//0 1 2
		int cup;//0-no 1-bronze 2-silver 3-gold
		float prepare;

		//called when mainrole is hurt
		void onMainHit();

		//获得得分 and speed
		void killScore(int sc, int comb, cocos2d::Point pos);
		//gain score
		void addScore(int sc, cocos2d::Point pos);

};
#endif
