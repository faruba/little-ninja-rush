//
//  CCNumber.h
//  LittleNinjaRushAP
//
//  Created by 马 颂文 on 13-3-10.
//  Copyright (c) 2013年 Trin Game. All rights reserved.
//

#ifndef LittleNinjaRushAP_CCNumber_h
#define LittleNinjaRushAP_CCNumber_h

#include "cocos2d.h"

class CCNumber : public cocos2d::CCObject
{
public:
    CCNumber(float v)
    {
        m_fValue = v;
        m_nValue = m_fValue;
        m_bIsFloat = true;
    }
    CCNumber(int v)
    {
        m_nValue = v;
        m_fValue = m_nValue;
        m_bIsFloat = false;
    }
    
    
    float getFloat()
    {
        return m_fValue;
    }
    
    int getInt()
    {
        return m_nValue;
    }
    
    static CCNumber* create(float v)
    {
        CCNumber* pRet = new CCNumber(v);
        pRet->autorelease();
        return pRet;
    }
    
    static CCNumber* create(int v)
    {
        CCNumber* pRet = new CCNumber(v);
        pRet->autorelease();
        return pRet;
    }
    
private:
    float m_fValue;
    int m_nValue;
    bool m_bIsFloat;
};

#endif
