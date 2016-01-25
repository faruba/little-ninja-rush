#include "Role.h"
#include "GamePlay.h"
#include "Bomb.h"
#include "Item.h"
#include "Pumpkin.h"

//摔死的概率(%)
#define FALLDOWN (15)
//追上的速度
#define CHASE_SPEED (150.0f)
//掉队的速度
#define DROPOUT_SPEED (-50.0f)

bool Role::handleGameOver (float delta) {
	GamePlay* play = GamePlay::sharedGamePlay();

	if ( mState != Dead && play->gameOverTimer >= 0 && mState != Initializing ) {
		float ds = delta*(play->levelspeed - play->runspeed);
		cocos2d::Point np = mSprite->getPosition();
		np.x += ds;
		mSprite->setPosition(np);
		return true;
	}
	return false;
}

void Role::onCreate() {
  GameObject::onCreate();
  mSprite = GTAnimatedSprite::spriteWithGTAnimation(GTAnimation::loadedAnimationSet(animationSetName()));
  mSprite->setAnchorPoint(mAnchor);
  mIsCoward = true;

  //auto node = DrawNode::create();
  //for (auto & circle : mCollisionCircles) {
  //  node->drawCircle(circle.center, circle.radius, 180, 30, false, 1, 1, cocos2d::Color4F::RED);
  //}
  //mSprite->addChild(node);
}

void Role::switchStateDelegate(int state, RoleStateDelegate *delegate) {
  if (mStateDelegate[state] != nullptr) delete mStateDelegate[state];
  mStateDelegate[state] = delegate;
  if (delegate != nullptr) delegate->mRole = this;
}

void Role::onDestroy() {
  for (int i = 0; i < RoleState::StateCount; i+=1) {
    switchStateDelegate(i, nullptr);
  }

  GameObject::onDestroy();
}

///// Delegations
void RoleStateDelegate::pre_update (float delta) {
  mAnimationIsOver = mRole->mSprite->updateGTAnimation(delta);
}
void RoleStateDelegate::after_update (float delta) {
}
///// MessagerRepositioningStateDelegate
void MessagerRepositioningStateDelegate::onEnter () {
	mTargetPos = 20+(UniversalFit::sharedUniversalFit()->playSize.width-40)*CCRANDOM_0_1();
	if( randomInt(100) < FALLDOWN )
	{
		mFlagFalldown = true;
		mFalldown = 0.1f*UniversalFit::sharedUniversalFit()->playSize.width + 0.8f*CCRANDOM_0_1()*UniversalFit::sharedUniversalFit()->playSize.width;
	}
	else {
		mFlagFalldown = false;
		mFalldown = 0;
	}
}

void MessagerRepositioningStateDelegate::update (float delta) {
	GamePlay *play = GamePlay::sharedGamePlay();
  cocos2d::Point np = mRole->position();
  np.x += delta*mRole->mSpeed;
  mRole->setPosition(np);
  if( mFlagFalldown && mRole->position().x > mFalldown )
  {
    static_cast<DeadStateDelegate*>(mRole->mStateDelegate[Role::RoleState::Dead])->mIsDieForward = true;
    mRole->switchToState(Role::RoleState::Dead);
    mRole->mSprite->playGTAnimation(5, false);
    //随机掉落道具
    play->manager->addGameObject(Item::item(1, mRole->position(), mRole->mParent, false));
  }

  //TODO:FootPrint::goFootPrint(&(mRole->mStepSnow), mRole->position());
}
///// RepositioningStateDelegate
void RepositioningStateDelegate::update (float delta) {
  cocos2d::Point np = mRole->position();
  float ds = delta * mRole->mSpeed;
  float dis = mTargetPos - np.x;
  if ( fabsf(dis) > ds ) {
    np.x += (dis > 0)? ds : -ds;
    mRole->setPosition(np);
  } else {
    np.x = mTargetPos;
    mRole->switchToState(Role::RoleState::Running);
  }
  if ( mAnimationIsOver ) {
    mRole->mSprite->playGTAnimation(0, true);
  }
}

void RepositioningStateDelegate::onEnter () {
	mTargetPos = 20+(UniversalFit::sharedUniversalFit()->playSize.width-40)*CCRANDOM_0_1();
}
///// MessagerEnteringStateDelegate 
void MessagerEnteringStateDelegate::onEnter () {
	int y = CCRANDOM_0_1()*RESPAWN_Y;
	mRole->mParent->addChild(mRole->mSprite, LAYER_ROLE+RESPAWN_Y-y);
	mMode = randomInt(2);
  if ( mMode == 0 ) {
    mRole->setPosition(cocos2d::Vec2(-80, RESPAWN_YMIN+y));
  } else {
    mRole->setPosition(cocos2d::Vec2(80+UniversalFit::sharedUniversalFit()->playSize.width, RESPAWN_YMIN+y));
  }
}
void MessagerEnteringStateDelegate::update (float delta) {
  if ( mMode == 0 ) {
    mRole->mSprite->playGTAnimation(1, true);
    mRole->mSpeed = CHASE_SPEED;
    mRole->switchToState(Role::RoleState::Repositioning);
  } else {
    mRole->mSprite->playGTAnimation(0, true);
    mRole->switchToState(Role::RoleState::Running);
    mRole->mSpeed = DROPOUT_SPEED;
  }
}
///// BasicEnteringStateDelegate 
void BasicEnteringStateDelegate::onEnter () {
  int y = CCRANDOM_0_1()*RESPAWN_Y ;
  int x = (GamePlay::sharedGamePlay()->state == STATE_RUSH) ? UniversalFit::sharedUniversalFit()->playSize.width+INITIAL_OFFSET : -INITIAL_OFFSET;

  mRole->setPosition(cocos2d::Vec2(x, y + RESPAWN_YMIN ));
  mRole->mSprite->playGTAnimation(0, true);

  mRole->mParent->addChild(mRole->mSprite, LAYER_ROLE+RESPAWN_Y-y);
}
void BasicEnteringStateDelegate::update (float delta) {
    mRole->switchToState(Role::RoleState::Repositioning);
}
///// MechanicEnteringStateDelegate 
void MechanicEnteringStateDelegate::onEnter () {
	GamePlay *play = GamePlay::sharedGamePlay();
	int y = CCRANDOM_0_1()*RESPAWN_Y;
	mRole->setPosition(cocos2d::Vec2(UniversalFit::sharedUniversalFit()->playSize.width+100, RESPAWN_YMIN+y));
	mRole->mSprite->playGTAnimation(0, true);
	play->addChild(mRole->mSprite, LAYER_ROLE+RESPAWN_Y-y);
}
void MechanicEnteringStateDelegate::update (float delta) {
  mRole->switchToState(Role::RoleState::Running);
}
///// HighNinjaEnteringStateDelegate 
void HighNinjaEnteringStateDelegate::onEnter () {
  int y = CCRANDOM_0_1()*RESPAWN_Y;

  mRole->setPosition(cocos2d::Vec2(20+(UniversalFit::sharedUniversalFit()->playSize.width-40)*CCRANDOM_0_1(), RESPAWN_YMIN+y));
  mRole->mSprite->playGTAnimation(0, true);
  mRole->mSprite->setVisible(false);
  mRole->mParent->addChild(mRole->mSprite, LAYER_ROLE+RESPAWN_Y-y);
  mTimer = 0;

  //出场特效
  GTAnimatedEffect *eff = GTAnimatedEffect::create(GTAnimation::loadedAnimationSet("effect"), 5, false);
  eff->setAnchorPoint(mRole->mSprite->getAnchorPoint());
  eff->setPosition(mRole->mSprite->getPosition());
  mRole->mParent->addChild(eff, LAYER_ROLE+RESPAWN_Y-y);
}

void HighNinjaEnteringStateDelegate::update (float delta) {
  mTimer += delta;
  if( mTimer > 0.30f && mRole->mSprite->isVisible() == false )
  {
    mRole->mSprite->setVisible(true);
  }
  if( mTimer >= 0.4f )
  {
    mRole->switchToState(Role::RoleState::Running);
    mRole->mSprite->playGTAnimation(0, true);
  }
}
///// MiddleNinjaEnteringStateDelegate 
void MiddleNinjaEnteringStateDelegate::onEnter () {
  int y = CCRANDOM_0_1()*RESPAWN_Y;
  mTargetPos.x = UniversalFit::sharedUniversalFit()->playSize.width*(0.75f*CCRANDOM_0_1());
  mTargetPos.y = RESPAWN_YMIN + y;
  //计算起跳点
  cocos2d::Point rjp = Vec2::forAngle(PI*3.0f/5.0f);

  mRole->setPosition(mTargetPos + (rjp * 100));
  mRole->mSprite->playGTAnimation(7, true);
  mTimer = 0;
  mRole->mParent->addChild(mRole->mSprite, LAYER_ROLE+RESPAWN_Y-y);
}

void MiddleNinjaEnteringStateDelegate::update (float delta) {
  if( mRole->position().y > mTargetPos.y ) {
    float dis = delta*mRole->mSpeed;
    cocos2d::Point dir = mTargetPos - mRole->position();
    if (dir.getLengthSq() <= dis*dis ) {
      mRole->setPosition(mTargetPos);
      mRole->mSprite->playGTAnimation(8, false);
    } else {
      dir = Vec2::forAngle(PI*3.0f/5.0f);
      dir.x *= -1;
      dir.y *= -1;
      cocos2d::Point np = (mRole->position() + (dir * dis));
      mRole->setPosition(np);
    }
    mTimer = 0;
  } else {
    mTimer += delta;
    float k = 1.0f-mTimer/mRole->mSprite->playBackTime();
    float dis = k*mRole->mSpeed*delta;
    cocos2d::Point np = mRole->position();
    np.x += dis;
    mRole->setPosition(np);
    if( mAnimationIsOver || mTimer > mRole->mSprite->playBackTime() )
    {
      mTargetPos = mRole->position();
      mRole->switchToState(Role::RoleState::Running);
      mTimer = 0;
      mRole->mSprite->playGTAnimation(0, true);
    }
  }
}
///// PumpkinEnteringStateDelegate
void PumpkinEnteringStateDelegate::onEnter(){
  Pumpkin* pumkin = static_cast<Pumpkin*>(mRole);
	pumkin->mTargetPos = Vec2(50, 210);
  pumkin->mTargetSpeed = cocos2d::Point::forAngle(CC_DEGREES_TO_RADIANS(-45))* 100;
	pumkin->mPumpkinSpeed =cocos2d::Point::forAngle(CC_DEGREES_TO_RADIANS(-90))* 50;
	mFlySound = 3 + 7*CCRANDOM_0_1();
  mTimer = 0;
}

#define FLOATING_TIME (45)
static cocos2d::Vec2 fleeingPos =Vec2(UniversalFit::sharedUniversalFit()->playSize.width + 100, SCREEN_HEIGHT/2);
void PumpkinEnteringStateDelegate::update(float delta){
  //tick timer
  mTimer += delta;
  if( mTimer > FLOATING_TIME)
  {
    mRole->switchToState(Role::RoleState::Fleeing);
    return;
  }
  
  Pumpkin* pumkin = static_cast<Pumpkin*>(mRole);
  pumkin->updateTargetPos(delta);
  pumkin ->moveToTargetPos(delta);
  //fly sound
  {
    mFlySound -= delta;
    if( mFlySound < 0 )
    {
						GameTool::PlaySound("pumpkinfly.mp3");
						mFlySound = 3 + 7*CCRANDOM_0_1();
    }
  }

}
///// SantaRunningStateDelegate
void SantaRunningStateDelegate::onEnter () {
  mTimer = 3 + 3*CCRANDOM_0_1();
}

void SantaRunningStateDelegate::update (float delta) {
  if ( mTimer > 0 ) {
    mTimer -= delta;
  } else {
    mRole->switchToState(Role::RoleState::Repositioning);
    mRole->mSpeed = (0.3f+0.4f*CCRANDOM_0_1())*ENEMY_NNRUNSPEED;
  }
  if ( mAnimationIsOver )
  {
    mRole->mSprite->playGTAnimation(0, true);
  }
}
///// MechanicRunningStateDelegate 
void MechanicRunningStateDelegate::onEnter () {
  mTimer = 0;
}

void MechanicRunningStateDelegate::update (float delta) {
  if( mTimer > 0 ) {//attack cd
    mTimer -= delta;
  }
  float ds = mRole->mSpeed*delta;
  cocos2d::Point np = mRole->position();
  np.x -= ds;
  mRole->setPosition(np);
  //may attack
  if( np.x > 30 && np.x < UniversalFit::sharedUniversalFit()->playSize.width - 30 && mTimer <= 0 )
  {
    if( randomInt(3) == 0 ) {
      mRole->switchToState(Role::RoleState::PreparingToShoot);
      mRole->mSprite->playGTAnimation(1, true);
    } else {
      mTimer = MECHANIC_POLLTIME;
    }
  }
}
///// MessagerRunningStateDelegate 
void MessagerRunningStateDelegate::onEnter () {
  mTimer = 0;
  mAwake = 0.4f*UniversalFit::sharedUniversalFit()->playSize.width + 0.3f*CCRANDOM_0_1()*UniversalFit::sharedUniversalFit()->playSize.width;
}

void MessagerRunningStateDelegate::update (float delta) {
	GamePlay *play = GamePlay::sharedGamePlay();
  cocos2d::Point np = mRole->position();
  np.x += delta*mRole->mSpeed;
  mRole->setPosition(np);
  if( mRole->position().x < mAwake )
  {
    GTAnimatedEffect *hiteff2 = GTAnimatedEffect::create(GTAnimation::loadedAnimationSet("effect"), 2, false);
    hiteff2->setScale(0.5f);
    hiteff2->setAnchorPoint(cocos2d::Vec2(0.5f, 0.5f));
    hiteff2->setPosition(mRole->position());
    play->addChild(hiteff2, LAYER_MAINROLE+1);
    mRole->switchToState(Role::RoleState::Fleeing);
    mRole->mSprite->playGTAnimation(3, false);
  }
}
///// NinjaRunningStateDelegate 
void NinjaRunningStateDelegate::onEnter () {
  mTimer = 0;
}

void NinjaRunningStateDelegate::update (float delta) {
  mTimer += delta;
  if ( mTimer > mPollTime ) {
    if ( randomInt(100) < mAggressive ) {
      if ( mRole->haveDart() && GamePlay::sharedGamePlay()->count_attack <= 0 ) {
        mRole->switchToState(Role::RoleState::PreparingToShoot);
        mRole->mSprite->playGTAnimation(6, true);
        //play effect
        GTAnimatedEffect *eff = GTAnimatedEffect::create(GTAnimation::loadedAnimationSet("effect"), 7, false);
        eff->setPosition(cocos2d::Vec2(47, 19));
        mRole->mSprite->addChild(eff);
      } else if (mRole->mIsCoward) {
        mRole->switchToState(Role::RoleState::Fleeing);
      } else {
        if( randomInt(2)== 0 ) {
          mRole->switchToState(Role::RoleState::Repositioning);
          mRole->mSpeed = (0.3f+0.4f*CCRANDOM_0_1())*ENEMY_NNRUNSPEED;
        }
      }
    }
  }
  if ( mAnimationIsOver ) {
    mRole->mSprite->playGTAnimation(0, true);
  }
}
///// MPreparingStateDelegate 
void MPreparingStateDelegate::onEnter () {
  mTimer = 0;
}

void MPreparingStateDelegate::update (float delta) {
	GamePlay* play = GamePlay::sharedGamePlay();
  mTimer += delta;
  float prepare = MECHANIC_PREPARE;
  if( play->mode == MODE_ARCADE )
  {
    prepare *= play->arcade->prepare;
  }
  if( mTimer > prepare )
  {
    mRole->switchToState(Role::RoleState::Shooting);
  }
}
///// PreparingStateDelegate 
void PreparingStateDelegate::onEnter () {
  mTimer = 0;
}

void PreparingStateDelegate::update (float delta) {
	GamePlay* play = GamePlay::sharedGamePlay();
  mTimer += delta;
  float prepare = mPreparingTime;
  if( play->mode == MODE_ARCADE )
  {
    prepare *= play->arcade->prepare;
  }
  if( mTimer > prepare && GamePlay::sharedGamePlay()->count_attack <= 0 )
  {
    mRole->switchToState(Role::RoleState::Shooting);
    mRole->mDartCount++;
  }
}
///// HShootStateDelegate 
void HShootStateDelegate::onEnter () {
  mTimer = 0;
  mFlag = (randomInt(3) == 0);
}

void HShootStateDelegate::update (float delta) {
  if ( mAnimationIsOver ) {
    mRole->mSprite->playGTAnimation(0, true);
  }
  mTimer -= delta;
  GamePlay* play = GamePlay::sharedGamePlay();
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
    if( mFlag ) {//冰锥术
      cocos2d::Point dst;
      dst.x = UniversalFit::sharedUniversalFit()->playSize.width/2 + UniversalFit::sharedUniversalFit()->playSize.width*CCRANDOM_0_1()/2;
      dst.y = play->mainrole->position().y;
      cocos2d::Point dir = ccpNormalize(dst - mRole->center());
      std::string shape = "dart.png";
      play->darts->addObject(play->manager->addGameObject(Dart::dart(shape, mRole->center(), dir, -4, mRole->mParent)));
    }
    else {
      cocos2d::Point dir = ccpNormalize(ccpSub(target, mRole->center()));
      float angle = ccpToAngle(dir);
      angle += CC_DEGREES_TO_RADIANS(-MNINJA_ACCURATE)+CC_DEGREES_TO_RADIANS(2*MNINJA_ACCURATE)*CCRANDOM_0_1();
      dir = Vec2::forAngle(angle);
      std::string shape = "dart.png";
      play->darts->addObject(play->manager->addGameObject(Dart::dart(shape, mRole->center(), dir, -2, mRole->mParent)));
    }

    mRole->mSprite->playGTAnimation(5, false);
    mTimer = 0.5f;
    if( mFlag || mRole->mDartCount >= 2)
    {
      if( randomInt(3) < 2 )
      {
        //mRepositionDelegate->mTargetPos = UniversalFit::sharedUniversalFit()->playSize.width*(0.2f+0.6f*CCRANDOM_0_1());
        mRole->mSpeed = (0.3f+0.4f*CCRANDOM_0_1())*ENEMY_NNRUNSPEED;
        mRole->switchToState(Role::RoleState::Repositioning);
      }
      else {
        mRole->switchToState(Role::RoleState::Running);
      }
      mTimer = 0;
    }
    mRole->mDartCount += 1;
  }
}
///// MShootStateDelegate 
void MShootStateDelegate::onEnter () {
  mTimer = 0;
  mFlag = false;
}

void MShootStateDelegate::update (float delta) {
  if ( mAnimationIsOver ) {
    mRole->mSprite->playGTAnimation(0, true);
  }
  mTimer -= delta;
  GamePlay* play = GamePlay::sharedGamePlay();
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
      play->darts->addObject(play->manager->addGameObject(Bomb::bomb(mRole->center(), target, mRole->mParent)));
      mFlag = true;
    }
    else {
      cocos2d::Point dir = ccpNormalize(ccpSub(target, mRole->center()));
      float angle = ccpToAngle(dir);
      angle += CC_DEGREES_TO_RADIANS(-MNINJA_ACCURATE)+CC_DEGREES_TO_RADIANS(2*MNINJA_ACCURATE)*CCRANDOM_0_1();
      dir = Vec2::forAngle(angle);
      std::string shape = "dart.png";
      play->darts->addObject(play->manager->addGameObject(Dart::dart(shape, mRole->center(), dir, -2, mRole->mParent)));
    }

    mRole->mSprite->playGTAnimation(5, false);
    mTimer = 0.5f;
    if( mFlag )
    {
      if( randomInt(3) < 2 )
      {
        //mRepositionDelegate->mTargetPos = UniversalFit::sharedUniversalFit()->playSize.width*(0.2f+0.6f*CCRANDOM_0_1());
        mRole->mSpeed = (0.3f+0.4f*CCRANDOM_0_1())*ENEMY_NNRUNSPEED;
        mRole->switchToState(Role::RoleState::Repositioning);
      }
      else {
        mRole->switchToState(Role::RoleState::Running);
      }
      mTimer = 0;
    }
    mFlag = true;
  }
}
///// MechanicShootStateDelegate 
void MechanicShootStateDelegate::onEnter () {
  mCount = 0;
  mTimer = 0.5;
}

void MechanicShootStateDelegate::update (float delta) {
  GamePlay* play = GamePlay::sharedGamePlay();
  if( mTimer > 0 )
  {
    mTimer -= delta;
    if( mTimer <= 0 )
    {
      GameTool::PlaySound("duofa.mp3");
    }
  }
  if( mTimer <= 0 )
  {
    if( mCount < MECHANIC_LAUNCH )
    {
      //launch dart
      cocos2d::Point dir = ccpForAngle(CC_DEGREES_TO_RADIANS(-90-MECHANIC_ANGLE+CCRANDOM_0_1()*MECHANIC_ANGLE*2));
      std::string dart = "dart.png";
      play->darts->addObject(play->manager->addGameObject(Dart::dart(dart, mRole->center(), dir, -6, play)));
      mTimer = MECHANIC_LAUNCHCD;
      mCount++;
    }
    else {
      mRole->mSprite->playGTAnimation(0, true);
      mRole->switchToState(Role::RoleState::Running);
    }
  }
}
///// ShootStateDelegate 
void ShootStateDelegate::onEnter () {
}

void ShootStateDelegate::update (float delta) {
	GamePlay* play = GamePlay::sharedGamePlay();
  cocos2d::Point target = play->mainrole->center();
  if( play->mainrole2 != NULL )
  {
    if( randomInt(2) == 0 )
    {
      target = play->mainrole2->center();
    }
  }
  cocos2d::Point dir = ccpNormalize(ccpSub(target, mRole->center()));
  float angle = ccpToAngle(dir);
  angle += CC_DEGREES_TO_RADIANS(-NNINJA_ACCURATE)+CC_DEGREES_TO_RADIANS(2*NNINJA_ACCURATE)*CCRANDOM_0_1();
  dir = Vec2::forAngle(angle);
  std::string shape = "dart.png";
  play->darts->addObject(play->manager->addGameObject(Dart::dart(shape, mRole->center(), dir, -1, mRole->mParent)));
  mRole->mSprite->playGTAnimation(5, false);

  if( randomInt(2) == 0 )
  {
    mRole->switchToState(Role::RoleState::Repositioning);
    mRole->mSpeed = (0.3f+0.4f*CCRANDOM_0_1())*ENEMY_NNRUNSPEED;
  }
  else {
    mRole->switchToState(Role::RoleState::Running);
  }
}
///// MessagerFleeStateDelegate
void MessagerFleeStateDelegate::onEnter () {
}

//加速速度
#define ACCELER_SPEED (250.0f)
void MessagerFleeStateDelegate::update (float delta) {
  if ( mAnimationIsOver ) {
    mRole->switchToState(Role::RoleState::Repositioning);
    mRole->mSprite->playGTAnimation(2, true);
    mRole->mSpeed = ACCELER_SPEED;
  }
}
///// FleeStateDelegate
void FleeStateDelegate::onEnter () {
}

void FleeStateDelegate::update (float delta) {
  float ds = delta*ENEMY_NNRUNSPEED;
  if( mRole->position().x > -100 )
  {
    cocos2d::Point np = mRole->position();
    np.x -= ds;
    mRole->setPosition(np);
  }
}
///// DummyStateDelegate
void DummyStateDelegate::onEnter () {
}
void DummyStateDelegate::update (float delta) {
  if( mRole->mSprite->isVisible() == true ) {
    mRole->switchToState(Role::RoleState::Running);
  }
}
///// HDeadStateDelegate
void HDeadStateDelegate::onEnter () {
  mTimer = 0;
  mFlag = true;
}

void HDeadStateDelegate::update (float delta) {
  GamePlay* play = GamePlay::sharedGamePlay();
  if( mAnimationIsOver )
  {
    //fix pos
    float ds = delta*play->runspeed;
    cocos2d::Point np = mRole->position();
    np.x -= ds;
    mRole->setPosition(np);
  }
  else {
    mTimer += delta;
    if(mTimer>0.3f && mFlag)
    {
      int n = 1 + randomInt(3);
      GameTool::PlaySound(std::string_format("ahh%d.mp3", n).c_str());
      mFlag = false;
    }
    //fix pos
    cocos2d::Point np = mRole->position();
    if ( mRole->mSprite->animationId() == 3 ) {
      float ra = mTimer/mRole->mSprite->playBackTime();
      if( ra > 1 )
      {
        ra = 1;
      }
      ra = 1 - ra;
      float ds = delta*200*ra;
      np.x += ds;
    } else {
      float ra = mTimer/mRole->mSprite->playBackTime();
      if( ra > 1 )
      {
        ra = 1;
      }
      float ds = delta*300*ra;
      np.x -= ds;
    }
    mRole->setPosition(np);
  }
}
///// DeadStateDelegate
void DeadStateDelegate::onEnter () {
  mTimer = 0;
  mFlag = true;
}

void DeadStateDelegate::update (float delta) {
  GamePlay* play = GamePlay::sharedGamePlay();
  if( mAnimationIsOver )
  {
    //fix pos
    float ds = delta*play->runspeed;
    cocos2d::Point np = mRole->position();
    np.x -= ds;
    mRole->setPosition(np);
  } else {
    mTimer += delta;
    if (mTimer>0.3f && mFlag) {
      int n = 1 + randomInt(3);
      GameTool::PlaySound(std::string_format("ahh%d.mp3", n).c_str());
      mFlag = false;
    }
    //fix pos
    cocos2d::Point np = mRole->position();
    float ra = mTimer/mRole->mSprite->playBackTime();
    if ( ra > 1 ) {
      ra = 1;
    }
    if (mIsDieForward) {
      ra = (1 - ra) * 200;
    } else {
      ra = -ra * 300;
    }
    np.x += delta*ra;
    mRole->setPosition(np);
  }
}
