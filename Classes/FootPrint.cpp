#include "FootPrint.h"
#include "GamePlay.h"


void FootPrint::goFootPrint (float *pv, CCPoint pos)
{
    GamePlay *play = GamePlay::sharedGamePlay();
    if( play->tiles->currentScene() == 6 )
    {
        float v = *pv;
        if( v > 0 )
        {
            float ds = pos.x - v;
            if( fabs(ds) > FOOTPRINT_INTERVAL )
            {
                CCSprite *sp = CCSprite::createWithSpriteFrameName("jiaoyin1.png");
                sp->setOpacity(150);
                sp->setPosition(pos);
                play->footprints->addChild(sp);
                *pv = -pos.x;
            }
            else
            {
                *pv -= play->deltaDistance;
            }
        }
        else
        {
            float ds = pos.x - fabs(v);
            if( fabs(ds) > FOOTPRINT_INTERVAL )
            {
                CCSprite *sp = CCSprite::createWithSpriteFrameName("jiaoyin2.png");
                sp->setOpacity(150);
                sp->setPosition(pos);
                play->footprints->addChild(sp);
                *pv = pos.x;
            }
            else
            {
                *pv += play->deltaDistance;
            }
        }
    }
}

FootPrint* FootPrint::footprint() 
{
    return FootPrint::create();
}

void FootPrint::onCreate() 
{
}

void FootPrint::onDestroy() 
{
}

void FootPrint::onUpdate(float delta) 
{
    GamePlay *play = GamePlay::sharedGamePlay();
    if( play->footprints != NULL )
    {
        if( play->count_runscene <= 0 || (play->gameOverTimer>=0 && play->gameOverTimer<PLAY_GOSLIDETIME))
        {
            float ds = delta*play->runspeed;
            
            for(unsigned int i=0; i<play->footprints->getChildrenCount(); ++i)
            {
                CCSprite *sp = (CCSprite*)play->footprints->getChildren()->objectAtIndex(i);
                CCPoint pos = sp->getPosition();
                pos.x -= ds;
                sp->setPosition(pos);
                if( pos.x + sp->getContentSize().width < 0 )
                {
                    play->footprints->removeChild(sp, false);
                    i--;
                }
            }
        }
    }
}


