#include "Role.h"
#include "GamePlay.h"
#include "Bomb.h"

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
///// BasicEnteringStateDelegate 
void BasicEnteringStateDelegate::onEnter () {
  int y = CCRANDOM_0_1()*RESPAWN_Y ;
  int x = (GamePlay::sharedGamePlay()->state == STATE_RUSH) ? UniversalFit::sharedUniversalFit()->playSize.width+INITIAL_OFFSET : -INITIAL_OFFSET;

  mRole->setPosition(cocos2d::Vec2(x, y + RESPAWN_YMIN ));
  mRole->mSprite->playGTAnimation(0, true);

  mRole->mParent->addChild(mRole->mSprite, LAYER_ROLE+RESPAWN_Y-y);
}
void BasicEnteringStateDelegate::update (float delta) {
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
    if( mRole->mSprite->animationId() == 3 )
    {
      float ra = mTimer/mRole->mSprite->playBackTime();
      if( ra > 1 )
      {
        ra = 1;
      }
      ra = 1 - ra;
      float ds = delta*200*ra;
      np.x += ds;
    }
    else {
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
