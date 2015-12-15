#include "NewbieNinja.h"
#include "GameConstants.h"
#include "GamePlay.h"
#include "Item.h"
#include "GameRecord.h"
#include "UniversalFit.h"
#include "FootPrint.h"



NewbieNinja* NewbieNinja::role(cocos2d::CCNode * parent) 
{
    NewbieNinja *em = NewbieNinja::create();
    em->mParent = parent;
    return em;
}

void NewbieNinja::onCreate() 
{
    GamePlay *play = GamePlay::sharedGamePlay();
    mSprite = GTAnimatedSprite::spriteWithGTAnimation(GTAnimation::loadedAnimationSet("enemy"));
    mSprite->setAnchorPoint(cocos2d::ccp(0.4f, 0.0625f));
    int y = cocos2d::CCRANDOM_0_1()*RESPAWN_Y;
    if( play->state == STATE_RUSH )
    {
        mSprite->setPosition(cocos2d::ccp(UniversalFit::sharedUniversalFit()->playSize.width+100, RESPAWN_YMIN+y));
    }
    else {
        mSprite->setPosition(cocos2d::ccp(-100, RESPAWN_YMIN+y));
    }
    mSprite->playGTAnimation(0, true);
    mParent->addChild(mSprite, LAYER_ROLE+RESPAWN_Y-y);

    mState = 0;//0 onstage 1 run 2 prepare 3 shoot 4 escape 5 dead
    mDartCount = 0;
    mTargetPos = 20+(UniversalFit::sharedUniversalFit()->playSize.width-40)*CCRANDOM_0_1();
    mFlag = true;
    mSpeed = ENEMY_NNRUNSPEED;
}

void NewbieNinja::onUpdate(float delta) 
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
                mSprite->setPosition(np);
            }
            else {
                np.x = mTargetPos;
                //上场就奔跑
                mState = 1;
                mTimer = 0;
            }
            if( playend )
            {
                mSprite->playGTAnimation(0, true);
            }
        }
            break;
        case 1:// run
        {
            mTimer += delta;
            if( mTimer > NNINJA_POLLTIME )
            {
                if( randomInt(100) < NNINJA_AGGRISIVE )
                {
                    if( mDartCount < NNINJA_MAXDART && GamePlay::sharedGamePlay()->count_attack <= 0 )
                    {
                        mState = 2;
                        mTimer = 0;
                        mSprite->playGTAnimation(6, true);
                        //play effect
                        GTAnimatedEffect *eff = GTAnimatedEffect::create(GTAnimation::loadedAnimationSet("effect"), 7, false);
                        eff->setPosition(cocos2d::ccp(47, 19));
                        mSprite->addChild(eff);
                    }
                    else {
                        mState = 4;
                    }
                }
                mTimer = 0;
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
            float prepare = NNINJA_PREPARE;
            if( play->mode == MODE_ARCADE )
            {
                prepare *= play->arcade->prepare;
            }
            if( mTimer > prepare && GamePlay::sharedGamePlay()->count_attack <= 0 )
            {
                mState = 3;
                mTimer = 0;
                mDartCount++;
            }
        }
            break;
        case 3:// shoot
        {
            cocos2d::Point target = play->mainrole->center();
            if( play->mainrole2 != NULL )
            {
                if( randomInt(2) == 0 )
                {
                    target = play->mainrole2->center();
                }
            }
            cocos2d::Point dir = ccpNormalize(ccpSub(target, this->center()));
            float angle = ccpToAngle(dir);
            angle += cocos2d::CC_DEGREES_TO_RADIANS(-NNINJA_ACCURATE)+CC_DEGREES_TO_RADIANS(2*NNINJA_ACCURATE)*CCRANDOM_0_1();
            dir = ccpForAngle(angle);
            play->darts->addObject(play->manager->addGameObject(Dart::dart(cocos2d::CCString::create("dart.png"), this->center(), dir, -1, mParent)));
            mSprite->playGTAnimation(5, false);
            
            if( randomInt(2) == 0 )
            {
                mTargetPos = 20+(UniversalFit::sharedUniversalFit()->playSize.width-40)*CCRANDOM_0_1();
                mState = 0;
                mSpeed = (0.3f+0.4f*CCRANDOM_0_1())*ENEMY_NNRUNSPEED;
            }
            else {
                mState = 1;
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
    
    //snow step
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
bool NewbieNinja::collisionWithCircle(cocos2d::Point cc, float rad) 
{
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
      if( mSprite && (exCollisionWithCircles(mSprite->getPosition(), 6, 12, 9, cc, rad) ||
          exCollisionWithCircles(mSprite->getPosition(), 17, 27, 13, cc, rad)))
      {
        return true;
      }
    }  
    return false;
}

//受到伤害
bool NewbieNinja::deliverHit(int type, cocos2d::Point dir) 
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
            mSprite->playGTAnimation(1+randomInt(2), false);
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
            play->arcade->killScore(NNINJA_SCORE, play->combo, this->center());
        }
        else
        {
            play->arcade->killScore(NNINJA_SCORE, 0, this->center());
        }
    }
    
    //SP
    play->mainrole->gainSP(2);
    
    mState = 5;
    
    //随机掉落道具
    Item::triggerItem(0, mSprite->getPosition());
    
    //achievement killing
    GameRecord::sharedGameRecord()->task->dispatchTask(ACH_KILLING, 1);
    play->killingCount++;
    GameRecord::sharedGameRecord()->task->dispatchTask(ACH_KILLGENIN, 1);
    if( play->runwithoutkill == 0 )
    {
        play->runwithoutkill = play->distance/PLAY_DISMETER;
    }
    
    return true;
}

cocos2d::Point NewbieNinja::position() 
{
    return mSprite->getPosition();
}

void NewbieNinja::setPosition(cocos2d::Point pos) 
{
    mSprite->setPosition(pos);
}

cocos2d::Point NewbieNinja::center() 
{
    return ccpAdd(mSprite->getPosition(), ccp(9, 20));
}

bool NewbieNinja::supportAimAid() 
{
    if( mState == 5 )
    {
        return false;
    }
    return  true;
}

void NewbieNinja::toggleVisible(bool flag) 
{
    mSprite->setVisible(flag);
}

void NewbieNinja::onDestroy() 
{
    GamePlay *play = GamePlay::sharedGamePlay();
    play->enemies->removeObject(this);
    mParent->removeChild(mSprite, false);
}


