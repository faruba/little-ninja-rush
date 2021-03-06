#ifndef ___CLASSES_DART_H_
#define ___CLASSES_DART_H_
#include "GameTool.h"
#include "Darts.h"

class Dart:
	public Darts
{
	public:
		CREATE_FUNC(Dart);
		static Dart* dart(std::string& shap, cocos2d::Point pos, cocos2d::Point dir, int typ, Node* parent);

		virtual bool isEnemy();
		virtual cocos2d::Point position();
		virtual void onHitback(cocos2d::Point origin);
		virtual void onCreate();
		virtual void onUpdate(float delta);
		virtual void onDestroy();
		cocos2d::Node* mParent;
		cocos2d::Sprite* mSprite;
		cocos2d::Sprite* mTail;
		GTAnimatedSprite *mSTail;
		std::string mShap;
		bool mPaused;
		bool mIsEnemy;
		int mHitEffect;
		int mHitEffect2;

		int mParticle;
		float mParticleInterval;

		bool mRemoved;
		float mTimer;

		GOHandler mTrace;

		int type;
		cocos2d::Point direction;
		cocos2d::Point pos;
		float speed;
		bool blocked;


		void traceRole(GOHandler target);//跟踪角色
		void addTail();
		void addSTail(std::string ani, int aid);
};
#endif
