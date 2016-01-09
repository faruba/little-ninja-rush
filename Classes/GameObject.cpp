//
//  GameObject.m
//  NinjiaRush
//
//  Created by 马 颂文 on 12-5-31.
//  Copyright (c) 2012年 TrinGame. All rights reserved.
//

#include "GameObject.h"

using namespace cocos2d;

GameObject *gDummy = NULL;

GameObject* GameObject::dummyObject() 
{
	if( gDummy == NULL )
	{
		gDummy = new GameObject();
	}
	return gDummy;
}

bool GameObject::init()
{
	mHandler.index = -1;
	mHandler.uid = -1;
	return true;
}

void GameObject::setHandler(int index, int uid) 
{
	mHandler.index = index;
	mHandler.uid = uid;
}

GOHandler GameObject::handler() 
{
	return mHandler;
}

void GameObject::onCreate(){}

void GameObject::onUpdate(float delta){}

void GameObject::onDestroy(){}


