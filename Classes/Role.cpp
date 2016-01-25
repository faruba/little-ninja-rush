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

void Role::drawCollision () {
  for (auto & circle : mCollisionCircles) {
    static_cast<GamePlay*>(mParent)->drawCircle(circle.center + position(), circle.radius);
  }
}
