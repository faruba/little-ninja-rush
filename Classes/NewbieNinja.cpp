#include "NewbieNinja.h"
#include "GameConstants.h"
#include "GamePlay.h"
#include "Item.h"
#include "GameRecord.h"
#include "UniversalFit.h"
#include "FootPrint.h"

void NewbieNinja::onCreate() {
  mCollisionCircles.push_back(Circle(cocos2d::Vec2(6, 12), 9));
  mCollisionCircles.push_back(Circle(cocos2d::Vec2(17, 27), 13));

  Role::onCreate();

  int y = CCRANDOM_0_1()*RESPAWN_Y ;
  int x = (GamePlay::sharedGamePlay()->state == STATE_RUSH) ? UniversalFit::sharedUniversalFit()->playSize.width+100 : -100;

  mSprite->setPosition(cocos2d::Vec2(x, y + RESPAWN_YMIN ));
  mSprite->playGTAnimation(0, true);
  mParent->addChild(mSprite, LAYER_ROLE+RESPAWN_Y-y);

  mDartCount = 0;
  mTargetPos = 20+(UniversalFit::sharedUniversalFit()->playSize.width-40)*CCRANDOM_0_1();
  mFlag = true;
  mSpeed = ENEMY_NNRUNSPEED;
}

void NewbieNinja::onUpdate(float delta) {
    bool playend = mSprite->updateGTAnimation(delta);
    GamePlay* play = GamePlay::sharedGamePlay();
    bool removeflag = false;
    bool gameOver = handleGameOver(delta);

    if (!gameOver) {//主角死亡的处理
        switch (mState) {
          case Entering:// onstage
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
                mState = Running;
                mTimer = 0;
              }
              if( playend )
              {
                mSprite->playGTAnimation(0, true);
              }
            }
            break;
          case Running:// run
            {
              mTimer += delta;
              if( mTimer > NNINJA_POLLTIME )
              {
                if( randomInt(100) < NNINJA_AGGRISIVE )
                {
                  if( mDartCount < NNINJA_MAXDART && GamePlay::sharedGamePlay()->count_attack <= 0 )
                  {
                    mState = PreparingToShoot;
                    mTimer = 0;
                    mSprite->playGTAnimation(6, true);
                    //play effect
                    GTAnimatedEffect *eff = GTAnimatedEffect::create(GTAnimation::loadedAnimationSet("effect"), 7, false);
                    eff->setPosition(cocos2d::Vec2(47, 19));
                    mSprite->addChild(eff);
                  }
                  else {
                    mState = Fleeing;
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
          case PreparingToShoot:// prepare
            {
              mTimer += delta;
              float prepare = NNINJA_PREPARE;
              if( play->mode == MODE_ARCADE )
              {
                prepare *= play->arcade->prepare;
              }
              if( mTimer > prepare && GamePlay::sharedGamePlay()->count_attack <= 0 )
              {
                mState = Shooting;
                mTimer = 0;
                mDartCount++;
              }
            }
            break;
          case Shooting:// shoot
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
              angle += CC_DEGREES_TO_RADIANS(-NNINJA_ACCURATE)+CC_DEGREES_TO_RADIANS(2*NNINJA_ACCURATE)*CCRANDOM_0_1();
              dir = ccpForAngle(angle);
              std::string shape = "dart.png";
              play->darts->addObject(play->manager->addGameObject(Dart::dart(shape, this->center(), dir, -1, mParent)));
              mSprite->playGTAnimation(5, false);

              if( randomInt(2) == 0 )
              {
                mTargetPos = 20+(UniversalFit::sharedUniversalFit()->playSize.width-40)*CCRANDOM_0_1();
                mState = Entering;
                mSpeed = (0.3f+0.4f*CCRANDOM_0_1())*ENEMY_NNRUNSPEED;
              }
              else {
                mState = Running;
              }
            }
            break;
          case Fleeing:// escape
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
          case Dead:// dead
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
    
    //snow step
    if( mState != Dead )
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

//受到伤害
bool NewbieNinja::deliverHit(int type, cocos2d::Point dir) 
{
    mTimer = 0;
    if( mState == Dead )
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
    if( mState == PreparingToShoot || mState == Fleeing )
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
    
    mState = Dead;
    
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

cocos2d::Point NewbieNinja::center() 
{
    return ccpAdd(mSprite->getPosition(), Vec2(9, 20));
}

bool NewbieNinja::supportAimAid() 
{
    return !( mState == Dead );
}

void NewbieNinja::onDestroy() 
{
    GamePlay *play = GamePlay::sharedGamePlay();
    play->enemies->removeObject(this);
    mParent->removeChild(mSprite, false);
}
