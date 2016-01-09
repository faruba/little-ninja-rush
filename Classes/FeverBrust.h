#ifndef ___CLASSES_FEVERBRUST_H_
#define ___CLASSES_FEVERBRUST_H_
//
//  FeverBrust.h
//  NinjiaRush
//
//  Created by 马 颂文 on 12-7-31.
//  Copyright (c) 2012年 TrinGame. All rights reserved.
//

#include "GameTool.h"

class FeverBrust:
	public GameObject
{
	public:
		CREATE_FUNC(FeverBrust);

		static FeverBrust* feverbrust(int eid, bool loop);

		virtual bool init(){return true;};
		virtual void onCreate();
		virtual void onUpdate(float delta);
		virtual void onDestroy();

		GTAnimatedSprite *mSprite;
		float mTimer;
		int mEffectId;
		bool mLoop;


};
#endif
