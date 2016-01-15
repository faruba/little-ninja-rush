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
    mRole->mTimer = 0; //TODO:Refactor this
  }
  bool animationIsOver = mRole->mSprite->updateGTAnimation(delta); //TODO:Refactor this
  if ( animationIsOver ) {
    mRole->mSprite->playGTAnimation(0, true);
  }
}

void RepositioningStateDelegate::reset () {
	mTargetPos = 20+(UniversalFit::sharedUniversalFit()->playSize.width-40)*CCRANDOM_0_1();
}
