#include "Role.h"
#include "GamePlay.h"

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
  mState = Entering;

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
///// NinjaRunningStateDelegate 
void NinjaRunningStateDelegate::onEnter () {
  mTimer = 0;
}

void NinjaRunningStateDelegate::update (float delta) {
  mTimer += delta;
  if ( mTimer > NNINJA_POLLTIME ) {
    if ( randomInt(100) < NNINJA_AGGRISIVE ) {
      if ( mRole->mDartCount < NNINJA_MAXDART && GamePlay::sharedGamePlay()->count_attack <= 0 ) {
        mRole->switchToState(Role::RoleState::PreparingToShoot);
        mRole->mSprite->playGTAnimation(6, true);
        //play effect
        GTAnimatedEffect *eff = GTAnimatedEffect::create(GTAnimation::loadedAnimationSet("effect"), 7, false);
        eff->setPosition(cocos2d::Vec2(47, 19));
        mRole->mSprite->addChild(eff);
      } else {
        mRole->switchToState(Role::RoleState::Fleeing);
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
  float prepare = NNINJA_PREPARE;
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
  dir = ccpForAngle(angle);
  std::string shape = "dart.png";
  play->darts->addObject(play->manager->addGameObject(Dart::dart(shape, mRole->center(), dir, -1, mRole->mParent)));
  mRole->mSprite->playGTAnimation(5, false);

  if( randomInt(2) == 0 )
  {
    mRole->switchToState(Role::RoleState::Entering);
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
  if( mRole->mSprite->getPosition().x > -100 )
  {
    cocos2d::Point np = mRole->mSprite->getPosition();
    np.x -= ds;
    mRole->mSprite->setPosition(np);
  }
  else {
    //销毁对象
    //TODO:removeflag = true;
  }
}
///// DeadStateDelegate
void DeadStateDelegate::onEnter () {
  mTimer = 0;
}

void DeadStateDelegate::update (float delta) {
  bool playend = mRole->mSprite->updateGTAnimation(delta);
  GamePlay* play = GamePlay::sharedGamePlay();
  if( playend )
  {
    //fix pos
    float ds = delta*play->runspeed;
    cocos2d::Point np = mRole->mSprite->getPosition();
    np.x -= ds;
    mRole->mSprite->setPosition(np);

    if( mRole->mSprite->getPosition().x < -100 )
    {
      //TODO:removeflag = true;
    }
  }
  else {
    mTimer += delta;
    if(mTimer>0.3f && mRole->mFlag)
    {
      int n = 1 + randomInt(3);
      GameTool::PlaySound(std::string_format("ahh%d.mp3", n).c_str());
      mRole->mFlag = false;
    }
    //fix pos
    cocos2d::Point np = mRole->mSprite->getPosition();
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
    mRole->mSprite->setPosition(np);
  }
}
