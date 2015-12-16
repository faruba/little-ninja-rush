#include "Item.h"
#include "GamePlay.h"
#include "AnimatedParticle.h"
#include "StaticParticle.h"
#include "GameScript.h"
#include "GameTool.h"
#include "GameRecord.h"
#include "Tasks.h"
#include "UniversalFit.h"

#define GRAVITY    (-2000.0f)
#define V0         (600.0f)
#define MINLIMIT   (-300.0f)

#define V00 (-100)
#define V0V (200)

#define VX0 (-150)
#define VXV (300)

//item weight
int gSuppplyWeight[] = 
{
    10, 10, 10, 10,//八方手里剑
    10, 10, 10, 10,//隐身
    0, 10, 10, 10,//子弹时间
    0,  8,  9, 10,//加血
    0,  8,  9, 10,//加气
    0,  8,  9, 10,//一闪
    10, 10, 10, 10,//流星雨
    10, 10, 10, 10,//飞镖无限
    7,  8,  9,  19,//全屏分身
};

int gEffectLevel[] = {13, 18, 20, 24};



Item* Item::item(int tid, cocos2d::Point pos, CCNode * parent, bool rock) 
{
    Item *ret = Item::create();
    ret->mParent = parent;
    ret->mTid = tid;
    ret->mPos = pos;
    if( rock )
    {
        ret->mSpeedX = VX0 + CCRANDOM_0_1()*VXV;
        ret->mSpeed = V0 + V00 + CCRANDOM_0_1()*V0V;
    }
    else
    {
        ret->mSpeedX = 0;
        ret->mSpeed = V0;
    }
    return ret;
}

//掉落规则
void Item::triggerItem(int degree, cocos2d::Point pos) 
{
    GamePlay *play = GamePlay::sharedGamePlay();
    int rate =45 + 10*degree;
    if( play->combo >= PLAY_FEVERCOMBO )
    {
        rate += 5*(play->combo-PLAY_FEVERCOMBO);
    }
    if( randomInt(100) < rate )
    {
        if( randomInt(9) != 0 )//no coins in arcade mode
        {
            if( play->mode == MODE_CLASSIC )
            {
                play->manager->addGameObject(Item::item(0, pos, play, false));
            }
        }
        else {
            //掉道具
            int cnt = 0;
            int avv[10];
            int wei[10];
            for(int i=2; i<=10; ++i)
            {
                int index = i+GAME_UPGRADEOFFSET;
                int level = GameRecord::sharedGameRecord()->item_upgrade[index];
                int weight = gSuppplyWeight[index*4+level];
                if( Item::isItemAccept(i) && weight > 0 )
                {
                    avv[cnt] = i;
                    wei[cnt] = weight;
                    cnt++;
                }
            }
            int rnd = randomInt(90);
            int res = -1;
            int counter = 0;
            for( int i=0; i<cnt; ++i)
            {
                counter += wei[i];
                if( rnd < counter )
                {
                    res = i;
                    break;
                }
            }
            if( res >= 0 )
            {
                bool checkflag = true;
                if( play->mode == MODE_ARCADE && res == 5 )
                {//no health
                    checkflag = false;
                }
                if( checkflag )
                {
                    play->manager->addGameObject(Item::item(avv[res], pos, play, false));
                }
            }
        }
    }
}

bool Item::isItemAccept(int item) 
{
    GamePlay *play = GamePlay::sharedGamePlay();
    switch (item) {
        case 5://加血
        {
            if( play->mainrole->HP == play->mainrole->maxHP )
            {
                return false;
            }
        }
            break;
        case 6://加气
        {
            if( play->mainrole->SP == play->mainrole->maxSP )
            {
                return false;
            }
        }
            break;
        case 4://八方手里剑等
        case 7:
        case 9:
        case 10:
        {
            int cnt = 0;
cocos2d::Ref* node = NULL;
CCARRAY_FOREACH(play->enemies, node)
  {
    Role * em = (Role*)node;
                if( em->supportAimAid() )
                {
                    cocos2d::Point pos = em->position();
                    if( pos.x > 0 && pos.x < UniversalFit::sharedUniversalFit()->playSize.width )
                    {
                        cnt++;
                    }
                }
            }
            if( cnt < 3 )
            {
                return false;
            }
        }
            break;
    }
    return true;
}

void Item::onCreate() 
{
    int ani = mTid;
    if( mTid > 1 )
    {
        int index = mTid + GAME_UPGRADEOFFSET;
        int level = GameRecord::sharedGameRecord()->item_upgrade[index];
        if( mTid == 6 && level == 0 )
        {//#tutorial fix
            level = 1;
        }
        ani = index*4 + level;
    }
    switch (mTid) {
        case 0:
        {
            GTAnimatedSprite *asp = GTAnimatedSprite::spriteWithGTAnimation(GTAnimation::loadedAnimationSet("item"));
            asp->playGTAnimation(ani, true);
            mSprite = asp;
        }
            break;
        case 1:
        {
            mSprite = cocos2d::CCSprite::createWithSpriteFrameName("scroll.png");
        }
            break;
        case -1:
        {
            mSprite = cocos2d::CCSprite::createWithSpriteFrameName("candy.png");
        }
            break;
        default:
        {
            mSprite = cocos2d::CCSprite::create(cocos2d::CCString::createWithFormat("icon%d.png", ani)->getCString());
        }
            break;
    }
    
    if( mTid < 2 )
    {
cocos2d::CCRotateBy *rot = cocos2d::CCRotateBy::create(1, 1080);
cocos2d::CCRepeatForever *rep = cocos2d::CCRepeatForever::create(rot);
        mSprite->runAction(rep);
        
        mSprite->setScale(0.1f);
cocos2d::CCScaleTo *scale1 = cocos2d::CCScaleTo::create(0.15f, 1.2f);
cocos2d::CCScaleTo *scale2 = cocos2d::CCScaleTo::create(0.15f, 1);
cocos2d::CCSequence *seq = cocos2d::CCSequence::create(scale1, scale2, NULL);
        mSprite->runAction(seq);
    }
    else
    {
        mSprite->setScale(0.05f);
cocos2d::CCScaleTo *scale1 = cocos2d::CCScaleTo::create(0.15f, 0.95f);
cocos2d::CCScaleTo *scale2 = cocos2d::CCScaleTo::create(0.15f, 0.8f);
cocos2d::CCSequence *seq = cocos2d::CCSequence::create(scale1, scale2, NULL);
        mSprite->runAction(seq);
    }
    
    mSprite->setPosition(mPos);
    mParent->addChild(mSprite, LAYER_MAINROLE+1);
    mTimer = 0;
    
    mPaused = false;
}

void Item::onUpdate(float delta) 
{
    GamePlay *play = GamePlay::sharedGamePlay();

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
    
    if( mTid == 0 )
    {//金币需要播放动画
        GTAnimatedSprite *asp = (GTAnimatedSprite*)mSprite;
        asp->updateGTAnimation(delta);
    }
    mTimer += delta;
    
    float ds = mSpeed*delta;
    mSpeed += GRAVITY*delta;
    if( mSpeed < MINLIMIT )
    {
        mSpeed = MINLIMIT;
    }
    cocos2d::Point np = mSprite->getPosition();
    np.y += ds;
    np.x += mSpeedX*delta;
    mSprite->setPosition(np);
    
    //接物品
    int get = 0;
    MainRole *role = NULL;
    if( play->mainrole->collisionWithCircle(np, 25) )
    {
        get = 1;
        role = play->mainrole;
    }
    //分身术
    if( play->mainrole2 != NULL && play->mainrole2->collisionWithCircle(np, 25) )
    {
        get = 2;
        role = play->mainrole2;
    }
    if( get>0 )
    {
        play->getitem = true;
        play->manager->removeGameObject(this);
        //物品生效的代码
        cocos2d::Point pos = play->mainrole->center();
        if( get == 2 )
        {
            pos = play->mainrole2->center();
        }
        //死人只能吃金币和卷轴
        if( play->mainrole->HP <= 0 && mTid != 0 && mTid != 1 )
        {
            return;
        }
        switch (mTid) {
            case -1:
            {
                play->festivalPieces++;
                SimpleAudioEngine::sharedEngine()->playEffect(cocos2d::CCFileUtils::sharedFileUtils()->fullPathForFilename("sound/getscroll.mp3").c_str());
            }
                break;
            case 0:
            {
                int plus = 1;
                if( play->doubleCoin )
                {
                    plus *= 2;
                }
                if( play->doubleCoinIAP )
                {
                    plus *= 2;
                }
                
                play->coins += plus;
                
                //播放特效
                for( int i=0; i<10; ++i)
                {
                    play->manager->addGameObject(AnimatedParticle::particleCoin(pos, i));
                }
                SimpleAudioEngine::sharedEngine()->playEffect(cocos2d::CCFileUtils::sharedFileUtils()->fullPathForFilename("sound/getcoin.mp3").c_str());
            }
                break;
            case 1:
            {
                play->scrolls++;
                play->totalScorll++;
                //播放特效
                for( int i=0; i<10; ++i)
                {
                    play->manager->addGameObject(AnimatedParticle::particleCoin(pos, i));
                }
                SimpleAudioEngine::sharedEngine()->playEffect(cocos2d::CCFileUtils::sharedFileUtils()->fullPathForFilename("sound/getscroll.mp3").c_str());
                
                //achievement
                GameRecord::sharedGameRecord()->task->dispatchTask(ACH_COLLECTSCROLL, 1);
            }
                break;
            case 2://八方手里剑
            {
                //MessageManager::sharedMessageManager()->pushMsg(GameScript::sharedScript(), callfuncND_selector(GameScript::invokeSpell0), CCInteger::create(get));
                GameScript::sharedScript()->invokeSpell0(cocos2d::CCInteger::create(get));
                SimpleAudioEngine::sharedEngine()->playEffect(cocos2d::CCFileUtils::sharedFileUtils()->fullPathForFilename("sound/getitem.mp3").c_str());
            }
                break;
            case 3://隐身术
            {
                //MessageManager::sharedMessageManager()->pushMsg(GameScript::sharedScript(), callfuncND_selector(GameScript::invokeSpell2), NULL);
                GameScript::sharedScript()->invokeSpell2(NULL);
                SimpleAudioEngine::sharedEngine()->playEffect(cocos2d::CCFileUtils::sharedFileUtils()->fullPathForFilename("sound/getitem.mp3").c_str());
            }
                break;
            case 4://子弹时间
            {
                //MessageManager::sharedMessageManager()->pushMsg(GameScript::sharedScript(), callfuncND_selector(GameScript::invokeSpell3), NULL);
                GameScript::sharedScript()->invokeSpell3(NULL);
                SimpleAudioEngine::sharedEngine()->playEffect(cocos2d::CCFileUtils::sharedFileUtils()->fullPathForFilename("sound/getitem.mp3").c_str());
            }
                break;
            case 5://加血
            {
                //MessageManager::sharedMessageManager()->pushMsg(GameScript::sharedScript(), callfuncND_selector(GameScript::invokeSpell4), NULL);
                GameScript::sharedScript()->invokeSpell4(NULL, NULL);
                SimpleAudioEngine::sharedEngine()->playEffect(cocos2d::CCFileUtils::sharedFileUtils()->fullPathForFilename("sound/charge.mp3").c_str());
                for( int i=0; i<5; ++i)
                {
                    play->manager->addGameObject(StaticParticle::particleAddHP(role->center()));
                }
            }
                break;
            case 6://加气
            {
//                MessageManager::sharedMessageManager()->pushMsg(GameScript::sharedScript(), callfuncND_selector(GameScript::invokeSpell5), NULL);
                GameScript::sharedScript()->invokeSpell5(NULL, NULL);
                SimpleAudioEngine::sharedEngine()->playEffect(cocos2d::CCFileUtils::sharedFileUtils()->fullPathForFilename("sound/charge.mp3").c_str());
            }
                break;
            case 7://一闪
            {
                //MessageManager::sharedMessageManager()->pushMsg(GameScript::sharedScript(), callfuncND_selector(GameScript::invokeSpell6), NULL);
                GameScript::sharedScript()->invokeSpell6(NULL);
                SimpleAudioEngine::sharedEngine()->playEffect(cocos2d::CCFileUtils::sharedFileUtils()->fullPathForFilename("sound/getitem.mp3").c_str());
            }
                break;
            case 8://流星雨
            {
                //MessageManager::sharedMessageManager()->pushMsg(GameScript::sharedScript(), callfuncND_selector(GameScript::invokeSpell7), NULL);
                GameScript::sharedScript()->invokeSpell7(NULL);
                SimpleAudioEngine::sharedEngine()->playEffect(cocos2d::CCFileUtils::sharedFileUtils()->fullPathForFilename("sound/getitem.mp3").c_str());
            }
                break;
            case 9://飞镖无限
            {
                //MessageManager::sharedMessageManager()->pushMsg(GameScript::sharedScript(), callfuncND_selector(GameScript::invokeSpell8), NULL);
                GameScript::sharedScript()->invokeSpell8(NULL);
                SimpleAudioEngine::sharedEngine()->playEffect(cocos2d::CCFileUtils::sharedFileUtils()->fullPathForFilename("sound/getitem.mp3").c_str());
            }
                break;
            case 10://全屏分身
            {
                //MessageManager::sharedMessageManager()->pushMsg(GameScript::sharedScript(), callfuncND_selector(GameScript::invokeSpellRelease), CCInteger::create(SPELL_SHADOWKILL));
                GameScript::sharedScript()->invokeSpellRelease(cocos2d::CCInteger::create(SPELL_SHADOWKILL));
                SimpleAudioEngine::sharedEngine()->playEffect(cocos2d::CCFileUtils::sharedFileUtils()->fullPathForFilename("sound/getitem.mp3").c_str());
            }
                break;
        }
        if( mTid != 0 && mTid != 1 )
        {
            int index = mTid + GAME_UPGRADEOFFSET;
            int level = GameRecord::sharedGameRecord()->item_upgrade[index];
            int effect = gEffectLevel[level];
            GTAnimatedEffect *hiteff2 = GTAnimatedEffect::create(GTAnimation::loadedAnimationSet("effect"), effect, false);
            hiteff2->setAnchorPoint(cocos2d::ccp(0.5f, 0.5f));
            hiteff2->setScale(1.5f);
            hiteff2->setPosition(role->center());
            mParent->addChild(hiteff2, LAYER_ROLE);
        }
    }
    if( mSprite->getPosition().y < -100.0f )
    {
        play->manager->removeGameObject(this);
    }
}

void Item::onDestroy() 
{
    mParent->removeChild(mSprite, true);
}
