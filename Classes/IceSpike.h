#ifndef ___CLASSES_ICESPIKE_H_
#define ___CLASSES_ICESPIKE_H_
//
//  IceSpike.h
//  NinjiaRush
//
//  Created by 马 颂文 on 12-5-18.
//  Copyright (c) 2012年 TrinGame. All rights reserved.
//

#include "GameTool.h"
#include "Role.h"

class IceSpike:
  public Role
{
  public:
    static IceSpike* spike(CCPoint pos, CCNode* parent);
    CREATE_FUNC(IceSpike);

    virtual void onCreate();
    virtual void onUpdate(float delta);
    virtual void onDestroy();

    virtual bool collisionWithCircle(CCPoint cc, float rad);
    virtual bool deliverHit(int type, CCPoint dir);
    virtual CCPoint position();
    virtual void setPosition(CCPoint pos);
    virtual CCPoint center();
    virtual bool supportAimAid();
    virtual void toggleVisible(bool flag);

    CCNode *mParent;

    GTAnimatedSprite *mSprite;
    CCPoint mPos;

    int mState;
    bool mFlag;


};
#endif
