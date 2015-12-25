//
//  MiddleNinja.m
//  NinjiaRush
//
//  Created by 马 颂文 on 12-5-4.
//  Copyright (c) 2012年 TrinGame. All rights reserved.
//

#include "MiddleNinja.h"
#include "GameConstants.h"
#include "GamePlay.h"
#include "Item.h"
#include "Bomb.h"
#include "SimpleAudioEngine.h"
#include "GameRecord.h"
#include "UniversalFit.h"
#include "FootPrint.h"

MiddleNinja* MiddleNinja::role(cocos2d::Node * parent) 
{
    MiddleNinja *em = MiddleNinja::create();
    em->mParent = parent;
    return em;
}

void MiddleNinja::onCreate() 
{
    int y = CCRANDOM_0_1()*RESPAWN_Y;
    mTargetPos.x = UniversalFit::sharedUniversalFit()->playSize.width*(0.75f*CCRANDOM_0_1());
    mTargetPos.y = RESPAWN_YMIN + y;
    //计算起跳点
    cocos2d::Point rjp = ccpForAngle(PI*3.0f/5.0f);
    
    mSprite = GTAnimatedSprite::spriteWithGTAnimation(GTAnimation::loadedAnimationSet("mninja"));
    mSprite->setAnchorPoint(cocos2d::Vec2(0.4f, 0.0625f));
    mSprite->setPosition(ccpAdd(mTargetPos, ccpMult(rjp, 100)));
    mSprite->playGTAnimation(7, true);
    mParent->addChild(mSprite, LAYER_ROLE+RESPAWN_Y-y);
    
    mState = 0;//0 onstage 1 run 2 prepare 3 shoot 4 escape 5 dead
    mDartCount = 0;
    mFlag = true;
    mSpeed = ENEMY_NNRUNSPEED*3;
}

void MiddleNinja::onUpdate(float delta) 
{
    GamePlay* play = GamePlay::sharedGamePlay();
    bool playend = mSprite->updateGTAnimation(delta);
    bool removeflag = false;
    
    if( mState < 5 && play->gameOverTimer >= 0 && mState != 0)
    {//主角死亡的处理
        float ds = delta*(play->levelspeed - play->runspeed);
        cocos2d::Point np = mSprite->getPosition();
        np.x += ds;
        mSprite->setPosition(np);
    }
    else {
        switch (mState) {
            case 0:// onstage
            {
                if( mSprite->getPosition().y > mTargetPos.y )
                {
                    float dis = delta*mSpeed;
                    cocos2d::Point dir = ccpSub(mTargetPos, mSprite->getPosition());
                    if( ccpLengthSQ(dir) <= dis*dis )
                    {
                        mSprite->setPosition(mTargetPos);
                        mSprite->playGTAnimation(8, false);
                    }
                    else {
                        dir = ccpForAngle(PI*3.0f/5.0f);
                        dir.x *= -1;
                        dir.y *= -1;
                        cocos2d::Point np = ccpAdd(mSprite->getPosition(), ccpMult(dir, dis));
                        mSprite->setPosition(np);
                    }
                    mTimer = 0;
                }
                else {
                    mTimer += delta;
                    float k = 1.0f-mTimer/mSprite->playBackTime();
                    float dis = k*mSpeed*delta;
                    cocos2d::Point np = mSprite->getPosition();
                    np.x += dis;
                    mSprite->setPosition(np);
                    if( playend || mTimer >mSprite->playBackTime() )
                    {
                        mTargetPos = mSprite->getPosition();
                        mState = 1;
                        mTimer = 0;
                        mSprite->playGTAnimation(0, true);
                    }
                }
            }
                break;
            case 1:// run
            {
                if( mSprite->getPosition().x != mTargetPos.x )
                {
                    float ds = delta*mSpeed;
                    float dis = mTargetPos.x - mSprite->getPosition().x;
                    cocos2d::Point np = mSprite->getPosition();
                    if( fabsf(dis) > ds )
                    {
                        if( dis > 0 )
                        {
                            np.x += ds;
                        }
                        else {
                            np.x -= ds;
                        }
                    }
                    else {
                        np.x = mTargetPos.x;
                        mTimer = 0;
                    }
                    mSprite->setPosition(np);
                }
                else {
                    mTimer += delta;
                    if( mTimer > MNINJA_POLLTIME )
                    {
                        if( randomInt(100) < MNINJA_AGGRISIVE )
                        {
                            if( mDartCount < MNINJA_MAXDART && GamePlay::sharedGamePlay()->count_attack <= 0 )
                            {
                                mState = 2;
                                mTimer = 0;
                                mSprite->playGTAnimation(6, true);
                                //play effect
                                GTAnimatedEffect *eff = GTAnimatedEffect::create(GTAnimation::loadedAnimationSet("effect"), 7, false);
                                eff->setPosition(cocos2d::Vec2(47, 19));
                                mSprite->addChild(eff);
                            }
                            else {
                                mState = 4;
                            }
                        }
                        mTimer = 0;
                    }
                    
                }
                if( playend )
                {
                    mSprite->playGTAnimation(0, true);
                }
            }
                break;
            case 2:// prepare
            {
                mTimer += delta;
                float prepare = MNINJA_PREPARE;
                if( play->mode == MODE_ARCADE )
                {
                    prepare *= play->arcade->prepare;
                }
                if( mTimer > prepare && GamePlay::sharedGamePlay()->count_attack <= 0 )
                {
                    mState = 3;
                    mTimer = 0;
                    mFlag = false;
                    mDartCount++;
                }
            }
                break;
            case 3:// shoot
            {
                if( playend )
                {
                    mSprite->playGTAnimation(0, true);
                }
                mTimer -= delta;
                if( mTimer <= 0 )
                {
                    cocos2d::Point target = play->mainrole->center();
                    if( play->mainrole2 != NULL )
                    {
                        if( randomInt(2) == 0 )
                        {
                            target = play->mainrole2->center();
                        }
                    }
                    if( randomInt(3) == 0 && !mFlag )
                    {
                        play->darts->addObject(play->manager->addGameObject(Bomb::bomb(this->center(), target, mParent)));
                        mFlag = true;
                    }
                    else {
                        cocos2d::Point dir = ccpNormalize(ccpSub(target, this->center()));
                        float angle = ccpToAngle(dir);
                        angle += CC_DEGREES_TO_RADIANS(-MNINJA_ACCURATE)+CC_DEGREES_TO_RADIANS(2*MNINJA_ACCURATE)*CCRANDOM_0_1();
                        dir = ccpForAngle(angle);
                        std::string shape = "dart.png";
                        play->darts->addObject(play->manager->addGameObject(Dart::dart(shape, this->center(), dir, -2, mParent)));
                    }
                    
                    mSprite->playGTAnimation(5, false);
                    mTimer = 0.5f;
                    if( mFlag )
                    {
                        if( randomInt(3) < 2 )
                        {
                            mTargetPos.x = UniversalFit::sharedUniversalFit()->playSize.width*(0.2f+0.6f*CCRANDOM_0_1());
                            mState = 1;
                            mTimer = 0; 
                            mSpeed = (0.3f+0.4f*CCRANDOM_0_1())*ENEMY_NNRUNSPEED;
                        }
                        else {
                            mTargetPos.x = mSprite->getPosition().x;
                            mState = 1;
                            mTimer = 0;
                        }
                    }
                    mFlag = true;
                }
            }
                break;
            case 4:// escape
            {
                float ds = delta*ENEMY_NNRUNSPEED;
                if( mSprite->getPosition().x > -100 )
                {
                    cocos2d::Point np = mSprite->getPosition();
                    np.x -= ds;
                    mSprite->setPosition(np);
                }
                else {
                    //销毁对象
                    removeflag = true;
                }
                
            }
                break;
            case 5:// dead
            {
                if( playend )
                {
                    //fix pos
                    float ds = delta*play->runspeed;
                    cocos2d::Point np = mSprite->getPosition();
                    np.x -= ds;
                    mSprite->setPosition(np);
                    
                    if( mSprite->getPosition().x < -100 )
                    {
                        removeflag = true;
                    }
                }
                else {
                    mTimer += delta;
                    if(mTimer>0.3f && mFlag)
                    {
                        int n = 1 + randomInt(3);
                        GameTool::PlaySound(cocos2d::CCString::createWithFormat("ahh%d.mp3", n)->getCString());
                        mFlag = false;
                    }
                    //fix pos
                    cocos2d::Point np = mSprite->getPosition();
                    if( mSprite->animationId() == 3 )
                    {
                        float ra = mTimer/mSprite->playBackTime();
                        if( ra > 1 )
                        {
                            ra = 1;
                        }
                        ra = 1 - ra;
                        float ds = delta*200*ra;
                        np.x += ds;
                    }
                    else {
                        float ra = mTimer/mSprite->playBackTime();
                        if( ra > 1 )
                        {
                            ra = 1;
                        }
                        float ds = delta*300*ra;
                        np.x -= ds;
                    }
                    mSprite->setPosition(np);
                }
            }
                break;
        }
    }
    //rush offset
    if( play->state == STATE_RUSH )
    {
        float offset = (play->runspeed - play->levelspeed)*delta;
        cocos2d::Point np = mSprite->getPosition();
        np.x -= offset;
        mSprite->setPosition(np);
    }
    
    if( mState != 5 )
    {
        FootPrint::goFootPrint(&mStepSnow, mSprite->getPosition());
    }
    
    //删除检查
    if( mSprite->getPosition().x < -120 || mSprite->getPosition().x > 120+UniversalFit::sharedUniversalFit()->playSize.width )
    {
        removeflag = true;
    }
    
    if( removeflag )
    {
        play->manager->removeGameObject(this);
    }
}

//碰撞检测
bool MiddleNinja::collisionWithCircle(cocos2d::Point cc, float rad) 
{
    if( mState == 0 )
    {
        return false;
    }
    else if( mState == 5 )
    {
        return false;
        //取消鞭尸功能
        //        if( exCollisionWithCircles(mSprite->getPosition, -20, 7, 11, cc, rad) ||
        //            exCollisionWithCircles(mSprite->getPosition, -5, 5, 5, cc, rad) ||
        //            exCollisionWithCircles(mSprite->getPosition, 5, 6, 5, cc, rad) )
        //        {
        //            return true;
        //        }
    }
    else {
        if( mSprite != NULL )
        {
            if( exCollisionWithCircles(mSprite->getPosition(), 6, 12, 9, cc, rad) ||
               exCollisionWithCircles(mSprite->getPosition(), 17, 27, 13, cc, rad) )
            {
                return true;
            }
        }
    }
    return false;
}

//受到伤害
bool MiddleNinja::deliverHit(int type, cocos2d::Point dir) 
{
    mTimer = 0;
    if( mState == 5 )
    {
        mSprite->playGTAnimation(4, false);
        mFlag = false;
    }
    else {
        if( dir.x > 0 )
        {
            mSprite->playGTAnimation(3 , false);
        }
        else {
            mSprite->playGTAnimation(1+randomInt(2 ), false);
        }
        mFlag = true;
    }
    GamePlay *play = GamePlay::sharedGamePlay();
    //combo
    bool isCombo = false;
    if( mState == 2 || mState == 4 )
    {
        play->makeCombo();
        isCombo = true;
    }
    // arcade combo
    if( play->mode == MODE_ARCADE )
    {
        if( isCombo )
        {
            play->arcade->killScore(MNINJA_SCORE, play->combo, this->center());
        }
        else
        {
            play->arcade->killScore(MNINJA_SCORE, 0, this->center());
        }
    }
    
    //SP
    play->mainrole->gainSP(3);
    
    mState = 5;
    //随机掉落道具
    Item::triggerItem(1, mSprite->getPosition());

    //achievement killing
    GameRecord::sharedGameRecord()->task->dispatchTask(ACH_KILLING, 1);
    play->killingCount++;
    GameRecord::sharedGameRecord()->task->dispatchTask(ACH_KILLCHUNIN, 1);
    if( type == HIT_BOMB )
    {
        GameRecord::sharedGameRecord()->task->dispatchTask(ACH_CHUNINBYBOMB, 1);
    }
    if( play->runwithoutkill == 0 )
    {
        play->runwithoutkill = play->distance/PLAY_DISMETER;
    }
    
    return true;
}

cocos2d::Point MiddleNinja::position() 
{
    return mSprite->getPosition();
}

void MiddleNinja::setPosition(cocos2d::Point pos) 
{
    mSprite->setPosition(pos);
}

cocos2d::Point MiddleNinja::center() 
{
    return ccpAdd(mSprite->getPosition(), Vec2(9, 20));
}

bool MiddleNinja::supportAimAid() 
{
    if( mState == 5 || mState == 0 )
    {
        return false;
    }
    return  true;
}

void MiddleNinja::toggleVisible(bool flag) 
{
    mSprite->setVisible(flag);
}

void MiddleNinja::onDestroy() 
{
    GamePlay *play = GamePlay::sharedGamePlay();
    play->enemies->removeObject(this);
    mParent->removeChild(mSprite, false);
}



