#include "SpeedLine.h"
#include "GameConstants.h"
#include "GamePlay.h"
#include "UniversalFit.h"

SpeedLine *SpeedLine::line() 
{
  return SpeedLine::create();
}

void SpeedLine::onCreate() 
{
    mSprite = CCSprite::createWithSpriteFrameName("sdx.png");
    mSprite->setAnchorPoint(ccp(0.5f, 0.5f));
    mSprite->setPosition(ccp(UniversalFit::sharedUniversalFit()->playSize.width+mSprite->getContentSize().width, SCREEN_HEIGHT*CCRANDOM_0_1() ));
    mSpeed = 500 + 400*CCRANDOM_0_1();
    GamePlay::sharedGamePlay()->addChild(mSprite, LAYER_UI-1);
}

void SpeedLine::onUpdate(float delta) 
{
    CCPoint np = mSprite->getPosition();
    np.x -= mSpeed*delta;
    mSprite->setPosition(np);
    if( np.x <= -mSprite->getContentSize().width )
    {
        GamePlay::sharedGamePlay()->manager->removeGameObject(this);
    }
}

void SpeedLine::onDestroy() 
{
    GamePlay::sharedGamePlay()->removeChild(mSprite, false);
}


