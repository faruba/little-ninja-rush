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

