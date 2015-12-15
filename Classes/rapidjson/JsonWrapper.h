//
//  JsonWrapper.h
//  LittleNinjaRushAP
//
//  Created by 马 颂文 on 13-3-10.
//  Copyright (c) 2013年 Trin Game. All rights reserved.
//

#ifndef __LittleNinjaRushAP__JsonWrapper__
#define __LittleNinjaRushAP__JsonWrapper__

#include "cocos2d.h"

class JsonWrapper
{
public:
    static cocos2d::CCObject* parseJson(cocos2d::CCString *pStr);
    static const char* dumpJson(cocos2d::CCDictionary *pDic);
};

#endif /* defined(__LittleNinjaRushAP__JsonWrapper__) */
