#include "Dart.h"
#include "GameConstants.h"
#include "GamePlay.h"
#include "StaticParticle.h"
#include "AnimatedParticle.h"
#include "IceSpike.h"
#include "GameRecord.h"
#include "UniversalFit.h"

Dart* Dart::dart(std::string& shap, cocos2d::Point pos, cocos2d::Point dir, int typ, Node* parent)
{
  Dart* ret = Dart::create();
  ret->mParent = parent;
  ret->pos = pos;
  ret->direction = dir;
  ret->type = typ;
  ret->mTail = NULL;
  ret->mTrace.index = -1;
  ret->mTrace.uid = -1;
  ret->mShap = shap;
  return ret;
}

void Dart::addTail() 
{
//  if( mTail == NULL )
//  {
//cocos2d::CCString *tailstr = cocos2d::CCString::createWithFormat("%s_tail.png", mShap->_string.substr(0, mShap->_string.length()-4).c_str());
//    mTail = cocos2d::Sprite::createWithSpriteFrameName(tailstr->getCString());
//    mTail->setAnchorPoint(cocos2d::Vec2(0.5f, 0.95f));
//    mTail->setPosition(mSprite->getPosition());
//    mTail->setOpacity(0);
//    mParent->addChild( mTail, LAYER_ROLE);
//    mTail->setRotation(90-CC_RADIANS_TO_DEGREES(ccpToAngle(direction)));
//cocos2d::CCFadeIn *fi = cocos2d::CCFadeIn::create(0.5f);
//    mTail->runAction(fi);
//  }
}

void Dart::addSTail(cocos2d::CCString * ani, int aid) 
{
  if( mSTail == NULL )
  {
    mSTail = GTAnimatedSprite::spriteWithGTAnimation(GTAnimation::loadedAnimationSet(ani->getCString()));
    mSTail->playGTAnimation(aid, true);
    mSTail->setAnchorPoint(cocos2d::Vec2(0.5f, 0.95f));
    mSTail->setPosition(mSprite->getPosition());
    mSTail->setOpacity(0);
    mParent->addChild( mSTail, LAYER_ROLE);
    mSTail->setRotation(90-CC_RADIANS_TO_DEGREES(ccpToAngle(direction)));
cocos2d::CCFadeIn *fi = cocos2d::CCFadeIn::create(0.5f);
    mSTail->runAction(fi);
  }
}

void Dart::onCreate() 
{
//  blocked = false;
//  mSprite = cocos2d::Sprite::createWithSpriteFrameName(mShap->getCString());
//  mSprite->setAnchorPoint(cocos2d::Vec2(0.5f, 0.5f));
//  mSprite->setPosition(pos);
//  mParent->addChild(mSprite, LAYER_MAINROLE+1);
//
//  mHitEffect = 2;
//  mHitEffect2 = 1;
//  mParticle = 0;
//  GamePlay *play = GamePlay::sharedGamePlay();
//  //CCLog("type %d", type);
//  switch(type)
//  {
//    case -1:
//      {//下忍的飞镖
//        mSprite->setScale(1.4f);
//        mIsEnemy = true;
//        speed = NNINJA_DARTSPEED*play->difficulty;
//cocos2d::CCRotateBy* rb = cocos2d::CCRotateBy::create(0.5f, 720);
//cocos2d::CCRepeatForever* rf = cocos2d::CCRepeatForever::create(rb);
//        mSprite->runAction(rf);
//      }
//      break;
//    case -2:
//      {//中忍的飞镖
//        mSprite->setScale(1.4f);
//        mIsEnemy = true;
//        speed = MNINJA_DARTSPEED*play->difficulty;
//cocos2d::CCRotateBy* rb = cocos2d::CCRotateBy::create(0.5f, 720);
//cocos2d::CCRepeatForever* rf = cocos2d::CCRepeatForever::create(rb);
//        mSprite->runAction(rf);
//      }
//      break;
//    case -3:
//      {//上忍的飞镖
//        mSprite->setScale(1.4f);
//        mIsEnemy = true;
//        speed = HNINJA_DARTSPEED*play->difficulty;
//cocos2d::CCRotateBy* rb = cocos2d::CCRotateBy::create(0.5f, 720);
//cocos2d::CCRepeatForever* rf = cocos2d::CCRepeatForever::create(rb);
//        mSprite->runAction(rf);
//      }
//      break;
//    case -4:
//      {//冰冻飞镖
//        mSprite->setScale(1.4f);
//        mIsEnemy = true;
//        speed = HNINJA_DARTSPEED*play->difficulty/2;
//        GameTool::PlaySound("sound/freeze_fly.mp3");
//cocos2d::CCRotateBy* rb = cocos2d::CCRotateBy::create(0.5f, 720);
//cocos2d::CCRepeatForever* rf = cocos2d::CCRepeatForever::create(rb);
//        mSprite->runAction(rf);
//
//        mParticle = 2;
//        mParticleInterval = 0.05f;
//      }
//      break;
//    case -5:
//      {//八方手里剑
//        mSprite->setScale(1.4f);
//        this->addTail();
//cocos2d::CCRotateBy* rb = cocos2d::CCRotateBy::create(0.5f, 720);
//cocos2d::CCRepeatForever* rf = cocos2d::CCRepeatForever::create(rb);
//        mSprite->runAction(rf);
//        speed = 700;
//      }
//      break;
//    case -6:
//      {//神工
//        mSprite->setScale(1.4f);
//        mIsEnemy = true;
//        speed = HNINJA_DARTSPEED*play->difficulty*2;
//cocos2d::CCRotateBy* rb = cocos2d::CCRotateBy::create(0.5f, 720);
//cocos2d::CCRepeatForever* rf = cocos2d::CCRepeatForever::create(rb);
//        mSprite->runAction(rf);
//      }
//      break;
//    case 0:
//      {//主角的普通飞镖
//        mSprite->setScale(1.4f);
//        this->addTail();
//cocos2d::CCRotateBy* rb = cocos2d::CCRotateBy::create(0.5f, 720);
//cocos2d::CCRepeatForever* rf = cocos2d::CCRepeatForever::create(rb);
//        mSprite->runAction(rf);
//      }
//      break;
//    case 1://火飞镖
//      {
//        this->addTail();
//        mHitEffect = 11;
//cocos2d::CCRotateBy* rb = cocos2d::CCRotateBy::create(0.5f, 720);
//cocos2d::CCRepeatForever* rf = cocos2d::CCRepeatForever::create(rb);
//        mSprite->runAction(rf);
//
//        mParticle = 1;
//        mParticleInterval = 0.05f;
//      }
//      break;
//    case 2://雷飞镖
//      {
//        this->addSTail(cocos2d::CCString::create("misc"), 0);
//        mHitEffect = 12;
//        //旋转代码
//cocos2d::CCRotateBy* rb = cocos2d::CCRotateBy::create(0.5f, 720);
//cocos2d::CCRepeatForever* rf = cocos2d::CCRepeatForever::create(rb);
//        mSprite->runAction(rf);
//      }
//      break;
//    case 3://激光标
//      {
//        mSprite->setRotation(-90 - CC_RADIANS_TO_DEGREES(ccpToAngle(direction)));
//        mHitEffect = 20;
//        mHitEffect2 = 21;
//      }
//      break;
//    case 4://暗影镖
//      {
//        mSprite->setScale(1.4f);
//        this->addTail();
//cocos2d::CCRotateBy* rb = cocos2d::CCRotateBy::create(0.5f, 720);
//cocos2d::CCRepeatForever* rf = cocos2d::CCRepeatForever::create(rb);
//        mSprite->runAction(rf);
//        mHitEffect = 14;
//
//        mParticle = 5;
//        mParticleInterval = 0.02f;
//      }
//      break;
//    case 5://蝴蝶镖
//      {
//        mSprite->setRotation(-90 - CC_RADIANS_TO_DEGREES(ccpToAngle(direction)));
//        this->addTail();
//        mHitEffect = 22;
//        mHitEffect2 = 23;
//
//        mParticle = -2;
//        mParticleInterval = 0.1f;
//      }
//      break;
//    case 6://冰飞镖
//      {
//        mSprite->setScale(1.4f);
//        this->addTail();
//cocos2d::CCRotateBy* rb = cocos2d::CCRotateBy::create(0.5f, 720);
//cocos2d::CCRepeatForever* rf = cocos2d::CCRepeatForever::create(rb);
//        mSprite->runAction(rf);
//        mHitEffect = 20;
//        mHitEffect2 = 21;
//
//        mParticle = 2;
//        mParticleInterval = 0.05f;
//      }
//      break;
//    case 7://樱花镖
//      {
//        mSprite->setScale(1.4f);
//        this->addTail();
//cocos2d::CCRotateBy* rb = cocos2d::CCRotateBy::create(0.5f, 720);
//cocos2d::CCRepeatForever* rf = cocos2d::CCRepeatForever::create(rb);
//        mSprite->runAction(rf);
//        mHitEffect = 15;
//        mHitEffect2 = 16;
//
//        mParticle = 3;
//        mParticleInterval = 0.05f;
//      }
//      break;
//    case 8://真空镖
//      {
//        mSprite->setScale(1.4f);
//        this->addTail();
//cocos2d::CCRotateBy* rb = cocos2d::CCRotateBy::create(0.5f, 720);
//cocos2d::CCRepeatForever* rf = cocos2d::CCRepeatForever::create(rb);
//        mSprite->runAction(rf);
//        mHitEffect = 18;
//        mHitEffect2 = 19;
//
//        mParticle = 4;
//        mParticleInterval = 0.05f;
//      }
//      break;
//    case 9://追踪镖
//      {
//        mSprite->setScale(1.4f);
//cocos2d::CCRotateBy* rb = cocos2d::CCRotateBy::create(0.5f, 720);
//cocos2d::CCRepeatForever* rf = cocos2d::CCRepeatForever::create(rb);
//        mSprite->runAction(rf);
//        GameObject *atar = (GameObject*)play->nearestEnemy(direction);
//        if( atar != NULL )
//        {
//          this->traceRole(atar->handler());
//        }
//
//        mHitEffect = 17;
//
//        mParticle = 6;
//        mParticleInterval = 0.016f;
//      }
//      break;
//    case 10://黄金镖
//      {
//        mSprite->setScale(1.4f);
//        this->addTail();
//cocos2d::CCRotateBy* rb = cocos2d::CCRotateBy::create(0.5f, 720);
//cocos2d::CCRepeatForever* rf = cocos2d::CCRepeatForever::create(rb);
//        mSprite->runAction(rf);
//
//        mParticle = -1;
//        mParticleInterval = 0.05f;
//      }
//      break;
//    case 11:
//      {//彩虹镖
//        mSprite->setScale(1.4f);
//        this->addTail();
//cocos2d::CCRotateBy* rb = cocos2d::CCRotateBy::create(0.5f, 720);
//cocos2d::CCRepeatForever* rf = cocos2d::CCRepeatForever::create(rb);
//        mSprite->runAction(rf);
//
//        mHitEffect = 24;
//        mHitEffect2 = 25;
//      }
//      break;
//    case 12:
//      {
//        mSprite->setScale(1.4f);
//        this->addTail();
//cocos2d::CCRotateBy* rb = cocos2d::CCRotateBy::create(0.5f, 720);
//cocos2d::CCRepeatForever* rf = cocos2d::CCRepeatForever::create(rb);
//        mSprite->runAction(rf);
//      }
//      break;
//    case 13:
//      {
//        mSprite->setScale(1.4f);
//        this->addTail();
//cocos2d::CCRotateBy* rb = cocos2d::CCRotateBy::create(0.5f, 720);
//cocos2d::CCRepeatForever* rf = cocos2d::CCRepeatForever::create(rb);
//        mSprite->runAction(rf);
//      }
//      break;
//    case 14:
//      {
//        mSprite->setScale(1.4f);
//        this->addTail();
//cocos2d::CCRotateBy* rb = cocos2d::CCRotateBy::create(0.5f, 720);
//cocos2d::CCRepeatForever* rf = cocos2d::CCRepeatForever::create(rb);
//        mSprite->runAction(rf);
//      }
//      break;
//    case 15:
//      {
//        mSprite->setScale(1.4f);
//        this->addTail();
//cocos2d::CCRotateBy* rb = cocos2d::CCRotateBy::create(0.5f, 720);
//cocos2d::CCRepeatForever* rf = cocos2d::CCRepeatForever::create(rb);
//        mSprite->runAction(rf);
//      }
//      break;
//  }
//  if( type >= 0 )
//  {
//    mIsEnemy = false;
//    speed = play->mainrole->dartSpeed;
//  }
//
//  mPaused = false;
//  mRemoved = false;
}

void Dart::onUpdate(float delta) 
{
  GamePlay* play = GamePlay::sharedGamePlay();
  bool removeflag = false;
  //暂停和恢复动作
  if( play->paused && !mPaused )
  {
    mPaused = true;
    mSprite->pauseSchedulerAndActions();
  }
  if( mPaused && !play->paused )
  {
    mPaused = false;
    mSprite->resumeSchedulerAndActions();
  }

  //碰撞检测
  if( mIsEnemy == false )
  {
cocos2d::Ref* node = NULL;
CCARRAY_FOREACH(play->enemies, node)
    {
      Role * em = (Role*)node;
      if( em->collisionWithCircle(pos, 7) )
      {
        bool hit = em->deliverHit(HIT_DART, direction);
        removeflag = true;
        if( hit )
        {
          GTAnimatedEffect *hiteff = GTAnimatedEffect::create(GTAnimation::loadedAnimationSet("effect"), mHitEffect2, false);
          hiteff->setAnchorPoint(cocos2d::Vec2(0.5f, 0.5f));
          hiteff->setPosition(em->center());
          hiteff->setRotation(60 - CC_RADIANS_TO_DEGREES( ccpToAngle(direction) ) + 60*CCRANDOM_0_1());
          mParent->addChild(hiteff, LAYER_MAINROLE+1);

          GTAnimatedEffect *hiteff2 = GTAnimatedEffect::create(GTAnimation::loadedAnimationSet("effect"), mHitEffect, false);
          hiteff2->setAnchorPoint(cocos2d::Vec2(0.5f, 0.5f));
          hiteff2->setPosition(em->center());
          mParent->addChild(hiteff2, LAYER_ROLE);

          switch (type) {
            case 1://fire
              {
                GameTool::PlaySound("sound/hit-fire.mp3");
                //achievement enemy burnt
                GameRecord::sharedGameRecord()->task->dispatchTask(ACH_ENEMYBURNT, 1);
              }
              break;
            case 2://thunder
              {
                GameTool::PlaySound("sound/hit-thunder.mp3");
              }
              break;
            case 6:
              {//冰飞镖
                for(int i=0; i<4; ++i)
                {
                  play->manager->addGameObject(StaticParticle::particleIcePiece(em->center(), direction));
                }
                GameTool::PlaySound("sound/ice-hurt.mp3");
              }
              break;
            case 7:
              {//樱花镖 击中效果
                for(int i=0; i<10; ++i)
                {
                  play->manager->addGameObject(StaticParticle::particleDart(em->center(), direction, 3));
                }
                GameTool::PlaySound("sound/hit.mp3");
              }
              break;
            case 9://狼牙
              {
                GameTool::PlaySound("sound/bloodfangs.mp3");
              }
              break;
            case 10://黄金
              {
                GameTool::PlaySound("sound/golden.mp3");
              }
              break;
            default:
              {
                GameTool::PlaySound("sound/hit.mp3");
              }
          }
          //achievement reflect to death
          if( blocked )
          {
            GameRecord::sharedGameRecord()->task->dispatchTask(ACH_REFLECTTODEATH, 1);
            play->reflectKill++;
          }
        }
        break;
      }
    }
  }
  else {
    bool hit = false;
    cocos2d::Point xpos;
    if( play->mainrole->collisionWithCircle(pos, 5) )
    {
      hit = play->mainrole->deliverHit(HIT_DART, direction);
      removeflag = true;
      xpos = play->mainrole->center();
    }
    //分身术
    if( play->mainrole2 != NULL && play->mainrole2->collisionWithCircle(pos, 5) )
    {
      hit = play->mainrole2->deliverHit(HIT_DART, direction);
      removeflag = true;
      xpos = play->mainrole2->center();
    }
    if( hit )
    {
      GameTool::PlaySound("sound/hit.mp3");
      GTAnimatedEffect *hiteff = GTAnimatedEffect::create(GTAnimation::loadedAnimationSet("effect"), 1, false);
      hiteff->setAnchorPoint(cocos2d::Vec2(0.5f, 0.5f));
      hiteff->setPosition(xpos);
      hiteff->setRotation(60 - CC_RADIANS_TO_DEGREES( ccpToAngle(direction) ) + 60*CCRANDOM_0_1());
      mParent->addChild(hiteff, LAYER_MAINROLE+1);

      GTAnimatedEffect *hiteff2 = GTAnimatedEffect::create(GTAnimation::loadedAnimationSet("effect"), mHitEffect, false);
      hiteff2->setAnchorPoint(cocos2d::Vec2(0.5f, 0.5f));
      hiteff2->setPosition(xpos);
      mParent->addChild(hiteff2, LAYER_ROLE);
    }
  }

  //跟踪代码
  Role *traced = (Role*)play->manager->gameObjectWithHandler(mTrace);
  if( traced != NULL )
  {
    cocos2d::Point rdir = ccpSub(traced->center(), mSprite->getPosition());
    float angle = ccpAngleSigned(direction, rdir);
    if( angle == angle )
    {
      float limit = CC_DEGREES_TO_RADIANS(100.0f*delta);
      if( fabsf(angle) <= limit )
      {
        direction = ccpNormalize(rdir);
      }
      else {
        float oa = ccpToAngle(direction);
        if( angle > 0 )
        {
          oa += limit;
        }
        else {
          oa -= limit;
        }
        direction = ccpForAngle(oa);
      }
    }
  }

  //飞行
  cocos2d::Point ds = ccpMult(direction, speed*delta);
  cocos2d::Point np = ccpAdd(ds, mSprite->getPosition());
  mSprite->setPosition(np);
  pos = np;
  if( mTail != NULL )
  {
    mTail->setPosition(mSprite->getPosition());
  }
  if( mSTail != NULL )
  {
    mSTail->updateGTAnimation(delta);
    mSTail->setPosition(mSprite->getPosition());
  }

  //越界清除
  if( np.x < -50 || np.x > UniversalFit::sharedUniversalFit()->playSize.width+50 || np.y < -50 || np.y > SCREEN_HEIGHT+50)
  {
    GamePlay* play = GamePlay::sharedGamePlay();
    removeflag = true;

    //achievement never miss
    if( !this->isEnemy() && !blocked )
    {
      play->missshoot = true;
    }
  }

  if( type == -4 )
  {
    //落地
    if( mSprite->getPosition().y <= play->mainrole->position().y )
    {
      play->enemies->addObject(play->manager->addGameObject(IceSpike::spike(mSprite->getPosition(), mParent)));
      removeflag = true;
    }
  }
  //粒子效果
  if( mParticle != 0 )
  {
    mTimer -= delta;
    if( mTimer <= 0 )
    {
      mTimer = mParticleInterval;
      if( mParticle > 0 )
      {
        play->manager->addGameObject(StaticParticle::particleDart(mSprite->getPosition(), direction, mParticle));
      }
      else {
        play->manager->addGameObject(AnimatedParticle::particleDart(mSprite->getPosition(), direction, -mParticle));
      }
    }
  }

  if( mRemoved )
  {
    mRemoved = false;
    removeflag = true;
  }

  if( removeflag )
  {
    play->manager->removeGameObject(this);
  }
}

void Dart::traceRole(GOHandler target) 
{
  mTrace = target;
}

bool Dart::isEnemy() 
{
  return mIsEnemy;
}

cocos2d::Point Dart::position() 
{
  return pos;
}

void Dart::onHitback(cocos2d::Point origin) 
{
  if( type != -4 )
  {
    mIsEnemy = false;
    speed = DARTSPEED_HERO;
    //计算反射角
    cocos2d::Point dcp = ccpSub(origin, pos);
    float ang = ccpAngleSigned(direction, dcp);
    float oga = ccpToAngle(direction);
    oga -= PI - 2*ang;
    //direction = play->autoAim(ccpForAngle(oga));
    direction = ccpForAngle(oga);
    this->addTail();

    blocked = true;
  }
  else {
    //击碎
    mRemoved = true;
    //achievement slice ice dart
    GameRecord::sharedGameRecord()->task->dispatchTask(ACH_SLICEICEDART, 1);
    GamePlay *play = GamePlay::sharedGamePlay();
    for( int i=0; i<4; ++i)
    {
      play->manager->addGameObject(StaticParticle::particleIce(mSprite->getPosition(), false));
      play->manager->addGameObject(StaticParticle::particleIceAir(mSprite->getPosition(), false));
    }
  }
}

void Dart::onDestroy() 
{
  GamePlay *play = GamePlay::sharedGamePlay();
  mParent->removeChild(mSprite, true);
  if( mTail != NULL )
  {
    mParent->removeChild(mTail, true);
  }
  if( mSTail != NULL )
  {
    mParent->removeChild(mSTail, true);
  }
  play->darts->removeObject(this);
}
