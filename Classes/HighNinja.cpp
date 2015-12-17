//
//  HighNinja.m
//  NinjiaRush
//
//  Created by 马 颂文 on 12-5-9.
//  Copyright (c) 2012年 TrinGame. All rights reserved.
//

#include "HighNinja.h"
#include "GameConstants.h"
#include "GamePlay.h"
#include "SimpleAudioEngine.h"
#include "PEffect.h"
#include "Item.h"
#include "GameRecord.h"
#include "UniversalFit.h"
#include "FootPrint.h"



HighNinja* HighNinja::role(cocos2d::Node * parent) 
{
    HighNinja *em = HighNinja::create();
    em->mParent = parent;
    return em;
}

void HighNinja::onCreate() 
{
    mSprite = GTAnimatedSprite::spriteWithGTAnimation(GTAnimation::loadedAnimationSet("hninja"));
    mSprite->setAnchorPoint(cocos2d::Vec2(0.4f, 0.0625f));
    int y = CCRANDOM_0_1()*RESPAWN_Y;
    mSprite->setPosition(cocos2d::Vec2(20+(UniversalFit::sharedUniversalFit()->playSize.width-40)*CCRANDOM_0_1(), RESPAWN_YMIN+y));
    mSprite->playGTAnimation(0, true);
    mSprite->setVisible(false);
    mParent->addChild(mSprite, LAYER_ROLE+RESPAWN_Y-y);
    
    mState = 0;//0 onstage 1 run 2 prepare 3 shoot 4 escape 5 dead
    mDartCount = 0;
    mTargetPos = 20+(UniversalFit::sharedUniversalFit()->playSize.width-40)*CCRANDOM_0_1();
    mFlag = true;
    mSpeed = ENEMY_NNRUNSPEED;
    mTimer = 0;
    mSpell = 0;
    
    //出场特效
    GTAnimatedEffect *eff = GTAnimatedEffect::create(GTAnimation::loadedAnimationSet("effect"), 5, false);
    eff->setAnchorPoint(mSprite->getAnchorPoint());
    eff->setPosition(mSprite->getPosition());
    mParent->addChild(eff, LAYER_ROLE+RESPAWN_Y-y);
    
    mWoodLink.uid=-1;
    mWoodLink.index=-1;
}

void HighNinja::onUpdate(float delta) 
{
    bool playend = mSprite->updateGTAnimation(delta);
    GamePlay* play = GamePlay::sharedGamePlay();
    bool removeflag = false;
    
    if( mState < 5 && play->gameOverTimer >= 0 && mState != 0 )
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
                mTimer += delta;
                if( mTimer > 0.30f && mSprite->isVisible() == false )
                {
                    mSprite->setVisible(true);
                }
                if( mTimer >= 0.4f )
                {
                    mState = 1;
                    mTimer = 0;
                    mSprite->playGTAnimation(0, true);
                }
            }
                break;
            case 1:// run
            {
                if( mSprite->getPosition().x != mTargetPos )
                {
                    float ds = delta*mSpeed;
                    float dis = mTargetPos - mSprite->getPosition().x;
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
                        np.x = mTargetPos;
                        mTimer = 0;
                    }
                    mSprite->setPosition(np);
                }
                else {
                    mTimer += delta;
                    if( mTimer > HNINJA_POLLTIME )
                    {
                        if( randomInt(100) < HNINJA_AGGRISIVE )
                        {
                            if( GamePlay::sharedGamePlay()->count_attack <= 0 )
                            {
                                mState = 2;
                                mTimer = 0;
                                mSprite->playGTAnimation(6, true);
                                //play effect
                                GTAnimatedEffect *eff = GTAnimatedEffect::create(GTAnimation::loadedAnimationSet("effect"), 7, false);
                                eff->setPosition(cocos2d::Vec2(47, 19));
                                mSprite->addChild(eff);
                            }
                        }
                        else {
                            if( randomInt(2)== 0 )
                            {
                                mTargetPos = 20+(UniversalFit::sharedUniversalFit()->playSize.width-40)*CCRANDOM_0_1();
                                mSpeed = (0.3f+0.4f*CCRANDOM_0_1())*ENEMY_NNRUNSPEED;
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
                float prepare = HNINJA_PREPARE;
                if( play->mode == MODE_ARCADE )
                {
                    prepare *= play->arcade->prepare;
                }
                if( mTimer > prepare && GamePlay::sharedGamePlay()->count_attack <= 0 )
                {
                    mState = 3;
                    mTimer = 0;
                    mDartCount = 0;
                    mFlag = false;
                    if( randomInt(3) == 0 )
                    {
                        mFlag = true;
                    }
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
                    if( mFlag )
                    {//冰锥术
                        cocos2d::Point dst;
                        dst.x = UniversalFit::sharedUniversalFit()->playSize.width/2 + UniversalFit::sharedUniversalFit()->playSize.width*CCRANDOM_0_1()/2;
                        dst.y = play->mainrole->position().y;
                        cocos2d::Point dir = ccpNormalize(ccpSub(dst, this->center()));
                        
                        play->darts->addObject(play->manager->addGameObject(Dart::dart(cocos2d::CCString::create("bfbdr.png"), this->center(), dir, -4, mParent)));
                    }
                    else {
                        cocos2d::Point dir = ccpNormalize(ccpSub(target, this->center()));
                        float angle = ccpToAngle(dir);
                        angle += CC_DEGREES_TO_RADIANS(-HNINJA_ACCURATE)+CC_DEGREES_TO_RADIANS(2*HNINJA_ACCURATE)*CCRANDOM_0_1();
                        dir = ccpForAngle(angle);
                        play->darts->addObject(play->manager->addGameObject(Dart::dart(cocos2d::CCString::create("dart.png"), this->center(), dir, -3, mParent)));
                    }
                    
                    mSprite->playGTAnimation(5, false);
                    mTimer = 0.3f;
                    
                    if( mDartCount>=2 || mFlag )
                    {
                        if( randomInt(3) < 2 )
                        {
                            mTargetPos = UniversalFit::sharedUniversalFit()->playSize.width*(0.2f+0.6f*CCRANDOM_0_1());
                            mState = 1;
                            mTimer = 0;
                            mSpeed = (0.3f+0.4f*CCRANDOM_0_1())*ENEMY_NNRUNSPEED;
                        }
                        else {
                            mState = 1;
                            mTimer = 0;
                        }
                    }
                    mDartCount++;
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
                        SimpleAudioEngine::sharedEngine()->playEffect(cocos2d::CCString::createWithFormat("ahh%d.mp3", n)->getCString());
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
            case 6:
            {//替身术
                if( mSprite->isVisible() == true )
                {
                    mState = 1;
                    mTargetPos = mSprite->getPosition().x;
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
bool HighNinja::collisionWithCircle(cocos2d::Point cc, float rad) 
{
    if( mState == 6 )
    {
        return false;
    }
    if( mState == 5 )
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
        if( mSprite != NULL )//shame defense
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
bool HighNinja::deliverHit(int type, cocos2d::Point dir) 
{
    GamePlay *play = GamePlay::sharedGamePlay();
    //替身术
    if( mState != 5 && mState != 2 && mState != 3 && mSpell == 0 )
    {
        mSpell = 1;
        //发动替身术
        SimpleAudioEngine::sharedEngine()->playEffect(cocos2d::CCFileUtils::sharedFileUtils()->fullPathForFilename("sound/smoke.mp3").c_str());
        GTAnimatedEffect *eff = GTAnimatedEffect::create(GTAnimation::loadedAnimationSet("effect"), 5, false);
        eff->setAnchorPoint(mSprite->getAnchorPoint());
        eff->setPosition(mSprite->getPosition());
        mParent->addChild(eff, LAYER_ROLE+RESPAWN_Y+RESPAWN_YMIN-mSprite->getPosition().y);
        mState = 6;
        mTimer = 0;
        mPoint = dir;
        
        //-------
        mSprite->setVisible(false);
        Wood*w = Wood::wood(this->center(), mPoint, mParent, this->handler(), mSprite->getAnchorPoint());
        mWoodLink = w->handler();
        play->manager->addGameObject(w);
        //-------
        return false;
    }
    mTimer = 0;
    if( mState == 5 )
    {
        mSprite->playGTAnimation(4, false);
        mFlag = false;
    }
    else {
        if( dir.x > 0 )
        {
            mSprite->playGTAnimation(3, false);
        }
        else {
            mSprite->playGTAnimation(1+randomInt(2 ), false);
        }
        mFlag = true;
    }
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
            play->arcade->killScore(HNINJA_SCORE, play->combo, this->center());
        }
        else
        {
            play->arcade->killScore(HNINJA_SCORE, 0, this->center());
        }
    }
    
    //SP
    play->mainrole->gainSP(4);
    
    mState = 5;
    //随机掉落道具
    Item::triggerItem(2, mSprite->getPosition());
    
    //achievement killing
    GameRecord::sharedGameRecord()->task->dispatchTask(ACH_KILLING, 1);
    play->killingCount++;
    GameRecord::sharedGameRecord()->task->dispatchTask(ACH_KILLJONIN, 1);
    if( mSpell == 0 )
    {
        GameRecord::sharedGameRecord()->task->dispatchTask(ACH_JONINBYONESTRIKE, 1);
        play->killJoninCri++;
    }
    if( play->runwithoutkill == 0 )
    {
        play->runwithoutkill = play->distance/PLAY_DISMETER;
    }
    
    return true;
}

cocos2d::Point HighNinja::position() 
{
    return mSprite->getPosition();
}

void HighNinja::setPosition(cocos2d::Point pos) 
{
    mSprite->setPosition(pos);
}

cocos2d::Point HighNinja::center() 
{
    return ccpAdd(mSprite->getPosition(), Vec2(9, 20));
}

bool HighNinja::supportAimAid() 
{
    if( mState == 5 || !mSprite->isVisible())
    {
        return false;
    }
    return  true;
}

void HighNinja::toggleVisible(bool flag) 
{
    mSprite->setVisible(flag);
}

void HighNinja::onDestroy() 
{
    GamePlay *play = GamePlay::sharedGamePlay();
    play->enemies->removeObject(this);
    mParent->removeChild(mSprite, false);
    
    GameObject *wood = play->manager->gameObjectWithHandler(mWoodLink);
    if( wood != NULL )
    {
        play->manager->removeGameObject(wood);
    }
}


