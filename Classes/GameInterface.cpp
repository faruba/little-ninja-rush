#include "GameInterface.h"
#include "GamePlay.h"
#include "GameConstants.h"
#include "GameRecord.h"
#include "GameData.h"
#include "Tasks.h"
#include "UniversalFit.h"
#include "GameScript.h"
#include "ArcadeMode.h"

USING_NS_CC;
USING_NS_CC_EXT;

GameInterface* GameInterface::interface(CCNode * parent) 
{
    GameInterface* ret = GameInterface::create();
    ret->mParent = parent;
    
    ret->mFlyBg = NULL;
    ret->mFlyNumber = NULL;
    ret->mSPBg = NULL;
    ret->mSPNumber = NULL;
    return ret;
}

void GameInterface::onCreate() 
{
    GamePlay *play = GamePlay::sharedGamePlay();
    
    //*** COMMON Interface ***
    
    //interface
    mUIBg = CCSprite::createWithSpriteFrameName("ui-bg.png");
    mUIBg->setAnchorPoint(ccp(0, 0));
    mUIBg->setPosition(ccp(0, 0));
    mParent->addChild(mUIBg,  LAYER_UI);
    
    //SP槽
    mSpBg = CCSprite::createWithSpriteFrameName("ui-spell-bg.png");
    mSpBg->setAnchorPoint(ccp(0, 0));
    mSpBg->setPosition(ccp(31, 4));
    mSpBg->setScaleX(0);
    mParent->addChild(mSpBg, LAYER_UI);
    
    mSpBgTop = CCSprite::createWithSpriteFrameName("ui-spell-bgtop.png");
    mSpBgTop->setAnchorPoint(ccp(0, 0));
    mSpBgTop->setPosition(ccp(31, 4));
    mParent->addChild(mSpBgTop, LAYER_UI);
    
    mSpFgNode = CCNode::create();
    mSpFgNode->setPosition(ccp(31, 4));
    mParent->addChild(mSpFgNode, LAYER_UI);
    mSpFg = CCSprite::createWithSpriteFrameName("ui-spell-fg.png");
    mSpFg->setAnchorPoint(ccp(0, 0));
    mSpFg->setPosition(ccp(0, 0));
    mSpFg->setScaleX(0);
    mSpFgNode->addChild(mSpFg, LAYER_UI);
    mSpFgTop = CCSprite::createWithSpriteFrameName("ui-spell-fgtop.png");
    mSpFgTop->setAnchorPoint(ccp(0, 0));
    mSpFgTop->setPosition(ccp(mSpFg->getPosition().x, 0));
    mSpFgTop->setVisible(false);
    mSpFgNode->addChild(mSpFgTop);
    CCBlink *usaBlink = CCBlink::create(60, 300);
    CCRepeatForever *usaRep = CCRepeatForever::create(usaBlink);
    mSpFgNode->runAction(usaRep);
    
    //动态使用框子
    int gn = play->mainrole->maxSP/play->mainrole->activeSP;
    if( gn > 4 || gn < 1 )
    {
        gn = 1;
    }
    mSpellGrid = CCSprite::createWithSpriteFrameName(CCString::createWithFormat("ui-spell%d.png", gn)->getCString());
    mSpellGrid->setAnchorPoint(ccp(0, 0));
    mSpellGrid->setPosition(ccp(23.5f, 2));
    mParent->addChild(mSpellGrid, LAYER_UI+1);
    mSpCache = 0;
    
    //spell
    int rid = GameRecord::sharedGameRecord()->curr_char;
    int spid = GameRecord::sharedGameRecord()->char_equip_spell[rid];
    Special *spell = (Special*)GameData::fetchSpecials()->objectAtIndex(spid);
    mSpell = CCSprite::create(spell->icon->getCString());
    mSpell->setAnchorPoint(ccp(0.5, 0.5));
    mSpell->setPosition(ccp(14, 14));
    mParent->addChild(mSpell,  LAYER_UI);
    mSpellName = CCSprite::createWithSpriteFrameName(CCString::createWithFormat("name%d.png", GameRecord::sharedGameRecord()->curr_char)->getCString());
    mSpellName->setAnchorPoint(ccp(0, 0));
    mSpellName->setPosition(ccp(37, 13));
    mParent->addChild(mSpellName,  LAYER_UI);
    
    //combo meter
    mHitNode = CCNode::create();
    mHitNode->setPosition(ccp( UniversalFit::sharedUniversalFit()->playSize.width-5, 150));
    mHitNode->setVisible(false);
    mParent->addChild(mHitNode, LAYER_UI);
    mHits = CCSprite::createWithSpriteFrameName("hits.png");
    mHits->setAnchorPoint(ccp(1, 0.5f));
    mHits->setPosition(ccp(0, 0));
    mHitNode->addChild(mHits);
    mHitCount = CCLabelBMFont::create("0", "ab70.fnt");
    mHitCount->setAnchorPoint(ccp(1, 0.5f));
    mHitCount->setPosition(ccp(-(mHits->getContentSize().width + 5), 0));
    mHitNode->addChild(mHitCount);
    
    //dart meter
    mDartMeter = CCNode::create();
    mDartMeter->setPosition(ccp(136, 0));
    mParent->addChild(mDartMeter, LAYER_UI);
    mDartDis = 50.0f/play->mainrole->dart;
    if( mDartDis > 12 )
    {
        mDartDis = 12;
    }
    for(int i=0; i<play->mainrole->dart; ++i)
    {
        CCSprite *d = CCSprite::createWithSpriteFrameName("ui-dart.png");
        d->setAnchorPoint(ccp(0, 0));
        d->setPosition(ccp( mDartDis*i, 0));
        mDartMeter->addChild(d);
    }
    mDartCache = 5;
    
    if( play->mode == MODE_CLASSIC )
    {//*** Classic Mode ***
        //距离
        mDistanceNumber = CCLabelBMFont::create("0", "ra50.fnt");
        mDistanceNumber->setAnchorPoint(ccp(0, 1));
        mDistanceNumber->setPosition(ccp(5, SCREEN_HEIGHT-5));
        mParent->addChild(mDistanceNumber, LAYER_UI);
        mDistanceM = CCLabelBMFont::create("m", "ra50.fnt");
        mDistanceM->setAnchorPoint(ccp(0, 1));
        mDistanceM->setPosition(ccp(5+10, SCREEN_HEIGHT-5));
        mParent->addChild(mDistanceM, LAYER_UI);
        mDistanceCache = 0;
        
        //金币
        mCoinNumber = CCLabelBMFont::create("0", "ra40.fnt");
        mCoinNumber->setAnchorPoint(ccp(0, 1));
        mCoinNumber->setPosition(ccp(5, SCREEN_HEIGHT - mDistanceNumber->getContentSize().height));
        mParent->addChild(mCoinNumber, LAYER_UI);
        mCoinMark = CCSprite::createWithSpriteFrameName("go_coin.png");
        mCoinMark->setAnchorPoint(ccp(0, 0));
        mCoinMark->setPosition(ccp(8+8, SCREEN_HEIGHT - mDistanceNumber->getContentSize().height - 15));
        mParent->addChild(mCoinMark, LAYER_UI);
        
        //fever coins
        mFeverCoins = CCNode::create();
        mFeverCoins->setPosition(ccp( UniversalFit::sharedUniversalFit()->playSize.width-5, 170));
        mFeverCoins->setVisible(false);
        mParent->addChild(mFeverCoins, LAYER_UI);
        CCSprite *fevercoin = CCSprite::createWithSpriteFrameName("fevercoins.png");
        fevercoin->setAnchorPoint(ccp(1, 0.5f));
        mFeverCoins->addChild(fevercoin);
        mFeverFont = CCLabelBMFont::create("+0", "ab56.fnt");
        mFeverFont->setAnchorPoint(ccp(1, 0.5f));
        mFeverFont->setPosition(ccp(-(fevercoin->getContentSize().width+5), 0));
        mFeverCoins->addChild(mFeverFont);
        
        //hp meter
        for(int i=0; i<play->mainrole->maxHP; ++i)
        {
            mHPMeters[i] = CCSprite::createWithSpriteFrameName("heart.png");
            float ahw = mHPMeters[i]->getContentSize().width*play->mainrole->maxHP + 1.5f*(play->mainrole->maxHP-1);
            float off = (UniversalFit::sharedUniversalFit()->playSize.width - ahw)/2;
            mHPMeters[i]->setAnchorPoint(ccp(0, 1));
            mHPMeters[i]->setPosition(ccp( off+i*mHPMeters[i]->getContentSize().width + 1.5f*i, SCREEN_HEIGHT-5));
            mParent->addChild(mHPMeters[i], LAYER_UI);
        }
        mHPCache = play->mainrole->maxHP;
        
        //buttons
        if( GameRecord::sharedGameRecord()->item_fly > 0 ||
           GameRecord::sharedGameRecord()->item_sp > 0 )
        {
            mMenu = CCMenu::create( NULL);
            mMenu->setPosition(ccp(0, 0));
            play->addChild(mMenu, LAYER_UI);
            if( GameRecord::sharedGameRecord()->item_fly > 0 )
            {
                mFly = CCMenuItemSprite::create(CCSprite::create("icon100.png"), CCSprite::create("icon100.png"), this, menu_selector(GameInterface::onFly));
                mFly->setAnchorPoint(ccp(0, 0));
                mFly->setPosition(ccp( 340, 10));
                mMenu->addChild(mFly);
                
                mFlyBg = CCSprite::createWithSpriteFrameName("djan.png");
                mFlyBg->setAnchorPoint(ccp(0, 0));
                mFlyBg->setPosition(ccpAdd(mFly->getPosition(), ccp(3, 0)));
                play->addChild(mFlyBg, LAYER_UI-1);
                mFlyNumber = CCLabelBMFont::create(CCString::createWithFormat("%d", GameRecord::sharedGameRecord()->item_fly)->getCString(), "ra50.fnt");
                mFlyNumber->setAnchorPoint(ccp(0.5f, 0));
                mFlyNumber->setPosition(ccpAdd(mFly->getPosition(), ccp(25, -10)));
                play->addChild(mFlyNumber, LAYER_UI+1);
            }
            if( GameRecord::sharedGameRecord()->item_sp > 0 )
            {
                mSP = CCMenuItemSprite::create(CCSprite::create("icon102.png"), CCSprite::create("icon102.png"), this, menu_selector(GameInterface::onSP));
                mSP->setAnchorPoint(ccp(0, 0));
                mSP->setPosition(ccp( 400, 10));
                mMenu->addChild(mSP);
                
                mSPBg = CCSprite::createWithSpriteFrameName("djan.png");
                mSPBg->setAnchorPoint(ccp(0, 0));
                mSPBg->setPosition(ccpAdd(mSP->getPosition(), ccp(3, 0)));
                play->addChild(mSPBg, LAYER_UI-1);
                mSPNumber = CCLabelBMFont::create(CCString::createWithFormat("%d", GameRecord::sharedGameRecord()->item_sp)->getCString(), "ra50.fnt");
                mSPNumber->setAnchorPoint(ccp(0.5f, 0));
                mSPNumber->setPosition(ccpAdd(mSP->getPosition(), ccp(25, -10)));
                play->addChild(mSPNumber, LAYER_UI+1);
            }
        }
        else {
            mMenu = NULL;
        }
    }
    else
    {//*** Arcade Mode ***
        //score
        mScoreNumber = CCLabelBMFont::create("0", "ra50.fnt");
        mScoreNumber->setAnchorPoint(ccp(0, 1));
        mScoreNumber->setPosition(ccp(5, SCREEN_HEIGHT-5));
        mParent->addChild(mScoreNumber, LAYER_UI);
        mScoreCache = 0;
        
        //speed * boost
        CCNodeLoaderLibrary *pNodeLib = CCNodeLoaderLibrary::sharedCCNodeLoaderLibrary();
        CCBReader *pReader = new CCBReader(pNodeLib, this, this);
        CCNode *mSpeedBoost = pReader->readNodeGraphFromFile("ui-arcade-speed.ccbi", this);
        pReader->release();
        
        mSpeedBoost->setPosition(ccp(UniversalFit::sharedUniversalFit()->playSize.width, 0));
        mSpeedState = 0;
        mParent->addChild(mSpeedBoost, LAYER_UI);
        
        //timer
        mTimerLabel = CCLabelBMFont::create("90", "ra50.fnt");
        mTimerLabel->setAnchorPoint(ccp(0.5f, 1));
        mTimerLabel->setPosition(ccp(UniversalFit::sharedUniversalFit()->playSize.width/2, UniversalFit::sharedUniversalFit()->playSize.height - 5));
        mParent->addChild(mTimerLabel, LAYER_UI);
        mTimerCache = 90;
        mTimerMark = CCSprite::createWithSpriteFrameName("time.png");
        mTimerMark->setAnchorPoint(ccp(0.5f, 1));
        CCPoint postm = mTimerLabel->getPosition();
        postm.y -= mTimerLabel->getContentSize().height - 5;
        mTimerMark->setPosition(postm);
        mParent->addChild(mTimerMark, LAYER_UI);
        
        //cup
        mCupCache = 0;
        mCup = CCSprite::createWithSpriteFrameName("tp-bronze.png");
        mCup->setAnchorPoint(ccp(1, 0));
        mCup->setPosition(ccp(UniversalFit::sharedUniversalFit()->playSize.width - 10, 25));
        mCup->setVisible(false);
        mParent->addChild(mCup, LAYER_UI);
        
        //arcade x
        mArcadeX = CCLabelBMFont::create("x0", "ab70.fnt");
        mArcadeX->setAnchorPoint(ccp(1, 0.5f));
        mArcadeX->setPosition(ccp(UniversalFit::sharedUniversalFit()->playSize.width - 10, 180));
        mArcadeX->setVisible(false);
        mArcadeX->setColor(ccc3(255, 204, 0));
        mParent->addChild(mArcadeX, LAYER_UI);
    }
    
    mFlyUsed = false;
    mPotionUsed = false;
    mUnlimtedDarts = false;
}

void GameInterface::onUpdateClassic(float delta, GamePlay* play) 
{
  
    //更新距离
    int dm = play->distance/PLAY_DISMETER;
    if( dm != mDistanceCache )
    {
        mDistanceNumber->setString(CCString::createWithFormat("%d", dm)->getCString());
        mDistanceCache = dm;
        mDistanceM->setPosition(ccp(5+10*strlen(mDistanceNumber->getString()), SCREEN_HEIGHT-5));
    }
    
    //更新HP
    if( play->mainrole->HP != mHPCache )
    {
        CCSpriteFrameCache *framecache = CCSpriteFrameCache::sharedSpriteFrameCache();
        for(int i=0; i<play->mainrole->maxHP; ++i)
        {
            if( i<play->mainrole->HP )
            {
                mHPMeters[i]->setDisplayFrame(framecache->spriteFrameByName("heart.png"));
            }
            else {
                mHPMeters[i]->setDisplayFrame(framecache->spriteFrameByName("heart2.png"));
            }
        }
        mHPCache = play->mainrole->HP;
    }
    
    //更新coin
    if( play->coins != mCoinCache )
    {
        mCoinCache = play->coins;
        mCoinNumber->setString(CCString::createWithFormat("%d", mCoinCache)->getCString());
        mCoinMark->setPosition(ccp(8+8*strlen(mCoinNumber->getString()), SCREEN_HEIGHT - mDistanceNumber->getContentSize().height - 15));
    }
    
    if( play->state == STATE_TUTORIAL )
    {
        this->toggleShowItemButtons(false);
    }
    else {
        this->toggleShowItemButtons(true);
    }
    
    //hide button
    if( dm > 100 && mMenu != NULL )
    {
        play->removeChild(mMenu, true);
        mMenu = NULL;
        if( mFlyBg != NULL )
        {
            play->removeChild(mFlyBg, true);
            mFlyBg = NULL;
        }
        if( mFlyNumber != NULL )
        {
            play->removeChild(mFlyNumber, true);
            mFlyNumber = NULL;
        }
        if( mSPBg != NULL )
        {
            play->removeChild(mSPBg, true);
            mSPBg = NULL;
        }
        if( mSPNumber != NULL )
        {
            play->removeChild(mSPNumber, true);
            mSPNumber = NULL;
        }
    }
    
}

void GameInterface::onUpdateArcade(float delta, GamePlay* play) 
{
    ArcadeMode *arcade = play->arcade;
    
    //update score
    if( arcade->score != mScoreCache )
    {
        mScoreNumber->setString(CCString::createWithFormat("%d", arcade->score)->getCString());
        mScoreCache = arcade->score;
    }
    
    //update time
    {
        int currtime = arcade->timer;
        float leak = arcade->timer - currtime;
        if( leak > 0 )
        {
            currtime++;
        }
        if( currtime != mTimerCache )
        {
            mTimerLabel->setString(CCString::createWithFormat("%d", currtime)->getCString());
            mTimerCache = currtime;
        }
    }
    
    //update cup
    if( mCupCache != arcade->cup )
    {
        switch (arcade->cup) {
            case 0:
            {
                mCup->setVisible(false);
            }
                break;
            case 1:
            {
                mCup->setVisible(true);
                mCup->setDisplayFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("tp-bronze.png"));
                SimpleAudioEngine::sharedEngine()->playEffect(CCFileUtils::sharedFileUtils()->fullPathForFilename("sound/getscroll.mp3").c_str());
            }
                break;
            case 2:
            {
                mCup->setVisible(true);
                mCup->setDisplayFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("tp-silver.png"));
                SimpleAudioEngine::sharedEngine()->playEffect(CCFileUtils::sharedFileUtils()->fullPathForFilename("sound/getscroll.mp3").c_str());
            }
                break;
            case 3:
            {
                mCup->setVisible(true);
                mCup->setDisplayFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("tp-gold.png"));
                SimpleAudioEngine::sharedEngine()->playEffect(CCFileUtils::sharedFileUtils()->fullPathForFilename("sound/getscroll.mp3").c_str());
            }
                break;
        }
        mCupCache = arcade->cup;
    }
    
    //update speed boost
    {
        int targetstate = 0;
        if( arcade->speed < ARCADE_INITSPEED )
        {//red flash
            mSpeedBg->setColor(ccc3(116, 10, 10));
            mSpeedNormal->setVisible(false);
            mSpeedFever->setVisible(false);
            CCSize redsize = mSpeedRed->getContentSize();
            redsize.width = (arcade->speed - ARCADE_MINSPEED)/(ARCADE_INITSPEED-ARCADE_MINSPEED)*12.5f;
            mSpeedRed->setContentSize(redsize);
            targetstate = 1;
        }
        else if( arcade->speed < ARCADE_FEVER1 )
        {//normal
            mSpeedBg->setColor(ccc3(117, 89, 6));
            mSpeedRed->setVisible(true);
            CCSize redsize = mSpeedRed->getContentSize();
            redsize.width = 12.5f;
            mSpeedRed->setContentSize(redsize);
            mSpeedNormal->setVisible(true);
            CCSize normalsize = mSpeedNormal->getContentSize();
            normalsize.width = (arcade->speed - ARCADE_INITSPEED)/(ARCADE_FEVER1 - ARCADE_INITSPEED)*37.5f;
            mSpeedNormal->setContentSize(normalsize);
            mSpeedFever->setVisible(false);
            targetstate = 0;
        }
        else if( arcade->speed < ARCADE_FEVER2 )
        {//yellow flash
            mSpeedBg->setColor(ccc3(117, 89, 6));
            mSpeedRed->setVisible(true);
            CCSize redsize = mSpeedRed->getContentSize();
            redsize.width = 12.5f;
            mSpeedRed->setContentSize(redsize);
            mSpeedNormal->setVisible(true);
            CCSize normalsize = mSpeedNormal->getContentSize();
            normalsize.width = 37.5f;
            mSpeedNormal->setContentSize(normalsize);
            mSpeedFever->setVisible(true);
            CCSize feversize = mSpeedFever->getContentSize();
            feversize.width = (arcade->speed - ARCADE_FEVER1)/(ARCADE_FEVER2 - ARCADE_FEVER1)*25;
            mSpeedFever->setContentSize(feversize);
            targetstate = 2;
        }
        else
        {//yellow flash 2
            mSpeedBg->setColor(ccc3(117, 89, 6));
            mSpeedRed->setVisible(true);
            CCSize redsize = mSpeedRed->getContentSize();
            redsize.width = 12.5f;
            mSpeedRed->setContentSize(redsize);
            mSpeedNormal->setVisible(true);
            CCSize normalsize = mSpeedNormal->getContentSize();
            normalsize.width = 37.5f;
            mSpeedNormal->setContentSize(normalsize);
            mSpeedFever->setVisible(true);
            CCSize feversize = mSpeedFever->getContentSize();
            feversize.width = 25 + (arcade->speed - ARCADE_FEVER2)/(ARCADE_MAXSPEED - ARCADE_FEVER2)*25;
            mSpeedFever->setContentSize(feversize);
            targetstate = 2;
        }
        if( targetstate != mSpeedState )
        {
            //cancel first one
            if( mSpeedState == 1 )
            {
                mSpeedRed->stopAllActions();
            }
            else if( mSpeedState == 2 )
            {
                mSpeedFever->stopAllActions();
            }
            //set current one
            if( targetstate == 1 )
            {
                CCBlink *bnk = CCBlink::create(1, 5);
                CCRepeatForever *rf = CCRepeatForever::create(bnk);
                mSpeedRed->runAction(rf);
            }
            else if( targetstate == 2 )
            {
                CCBlink *bnk = CCBlink::create(1, 5);
                CCRepeatForever *rf = CCRepeatForever::create(bnk);
                mSpeedFever->runAction(rf);
            }
            mSpeedState = targetstate;
        }
    }
}

void GameInterface::onUpdate(float delta) 
{
    GamePlay *play = GamePlay::sharedGamePlay();
    
    //update common
    //更新SP
    if( play->mainrole->SP != mSpCache )
    {
        float rate1 = 1.0f*play->mainrole->SP/play->mainrole->maxSP;
        mSpBg->setScaleX(rate1);
        int posx = mSpBg->getPosition().x + mSpBg->getContentSize().width*rate1;
        mSpBgTop->setPosition(ccp(posx, mSpBg->getPosition().y));
        
        mSpCache = play->mainrole->SP;
        int n = play->mainrole->activeSP*(play->mainrole->SP/play->mainrole->activeSP);
        float rate2 = 1.0f*n/play->mainrole->maxSP;
        int posx2 = mSpFg->getContentSize().width*rate2;
        mSpFgTop->setPosition(ccp(posx2, 0));
        mSpFg->setScaleX(rate2);
        if( n == 0 )
        {
            mSpFgTop->setVisible(false);
        }
        else {
            mSpFgTop->setVisible(true);
        }
        
        CCSpriteFrameCache *share = CCSpriteFrameCache::sharedSpriteFrameCache();
        if( play->mainrole->SP == play->mainrole->maxSP )
        {
            mSpBg->setDisplayFrame(share->spriteFrameByName("ui-spell-bg2.png"));
            mSpBgTop->setDisplayFrame(share->spriteFrameByName("ui-spell-bgtop2.png"));
            mSpFg->setDisplayFrame(share->spriteFrameByName("ui-spell-fg2.png"));
            mSpFgTop->setDisplayFrame(share->spriteFrameByName("ui-spell-fgtop2.png"));
        }
        else {
            mSpBg->setDisplayFrame(share->spriteFrameByName("ui-spell-bg.png"));
            mSpBgTop->setDisplayFrame(share->spriteFrameByName("ui-spell-bgtop.png"));
            mSpFg->setDisplayFrame(share->spriteFrameByName("ui-spell-fg.png"));
            mSpFgTop->setDisplayFrame(share->spriteFrameByName("ui-spell-fgtop.png"));
        }
    }
    
    //更新DART
    if( play->mainrole->dart != mDartCache )
    {
        this->rebuildDarts(mUnlimtedDarts);
    }
    if( mUnlimtedDarts )
    {
        mUnlimtedDartsTimer += delta;
        float circle = 0.5f;
        float min = 0;
        float max = 255;
        int n = mUnlimtedDartsTimer/circle;
        float left = mUnlimtedDartsTimer - circle*n;
        int opc = 0;
        if( left < circle/2 )
        {
            opc = min + left/(circle/2)*max;
        }
        else {
            opc = min + (circle - left)/(circle/2)*max;
        }
        for( int i=0; i<mDartCache; ++i)
        {
            CCSprite *dm = (CCSprite*)(mDartMeter->getChildByTag(i));
            dm->setOpacity(opc);
        }
    }
    
    //update special mode
    if( play->mode == MODE_CLASSIC )
    {
        this->onUpdateClassic(delta, play);
    }
    else
    {
        this->onUpdateArcade(delta, play);
    }    
}

void GameInterface::toggleShowItemButtons(bool flag) 
{
    if( mMenu != NULL )
    {
        mMenu->setVisible(flag);
        if( mFlyBg != NULL && !mFlyUsed )
        {
            mFlyBg->setVisible(flag);
            mFlyNumber->setVisible(flag);
        }
        if( mSPBg != NULL && !mPotionUsed )
        {
            mSPBg->setVisible(flag);
            mSPNumber->setVisible(flag);
        }
    }
}

void GameInterface::popFeverCoins(int coins) 
{
    mFeverCoins->stopAllActions();
    mFeverFont->setString(CCString::createWithFormat("+%d", coins)->getCString());
    mFeverCoins->setScale(0.8f);
    CCScaleTo *a1 = CCScaleTo::create(0.1f, 1.1f);
    CCScaleTo *a2 = CCScaleTo::create(0.2f, 1);
    CCDelayTime *a3 = CCDelayTime::create(3);
    CCHide *a4 = CCHide::create();
    CCSequence *sq = CCSequence::create(a1, a2, a3, a4, NULL);
    mFeverCoins->runAction(sq);
    mFeverCoins->setVisible(true);
}

void GameInterface::popHits(int number) 
{
    if( number>=PLAY_FEVERCOMBO && GamePlay::sharedGamePlay()->mode == MODE_CLASSIC )
    {
        mHits->setColor(ccc3(227, 0, 0));
        mHitCount->setColor(ccc3(227, 0, 0));
    }
    else {
        mHits->setColor(ccc3(255, 204, 0));
        mHitCount->setColor(ccc3(255, 204, 0));
    }
    mHitNode->stopAllActions();
    mHitCount->setString(CCString::createWithFormat("%d", number)->getCString());
    mHitNode->setScale(0.01f);
    CCScaleTo *a1 = CCScaleTo::create(0.1f, 1.5f);
    CCScaleTo *a2 = CCScaleTo::create(0.2f, 1);
    CCSequence *sq = CCSequence::create(a1, a2, NULL);
    mHitNode->runAction(sq);
    mHitNode->setVisible(true);
}

void GameInterface::updateHits(float time) 
{
    if( mHitNode->numberOfRunningActions() <= 0 )
    {
        float k = 0.1f*time + 0.5f;
        if( k > 1 )
        {
            k = 1;
        }
        if( k < 0.5f )
        {
            k = 0.5f;
        }
        mHitNode->setScale(k);
    }
}

void GameInterface::fadeHits() 
{
    mHitNode->stopAllActions();
    CCScaleTo *a1 = CCScaleTo::create(0.5f, 0);
    CCHide *a2 = CCHide::create();
    CCSequence *sq = CCSequence::create(a1, a2, NULL);
    mHitNode->runAction(sq);
}

void GameInterface::setArcadeX(int x) 
{
    mArcadeX->setString(CCString::createWithFormat("x%d", x)->getCString());
    if( x <= 0 )
    {
        CCScaleTo *st = CCScaleTo::create(1, 0);
        CCHide *hd = CCHide::create();
        CCSequence *seq = CCSequence::create(st,  hd, NULL);
        mArcadeX->runAction(seq);
    }
    else
    {
        mArcadeX->setScale(0);
        mArcadeX->setVisible(true);
        CCScaleTo *st1 = CCScaleTo::create(0.3f, 1.5f);
        CCScaleTo *st2 = CCScaleTo::create(0.2f, 0.8f);
        CCScaleTo *st3 = CCScaleTo::create(0.1f, 1.2f);
        CCScaleTo *st4 = CCScaleTo::create(0.1f, 1);
        CCSequence *seq = CCSequence::create(st1, st2, st3, st4, NULL);
        mArcadeX->runAction(seq);
    }
}

void GameInterface::onDestroy() 
{
    GamePlay *play = GamePlay::sharedGamePlay();
    //remove common
    mParent->removeChild(mUIBg, false);
    mParent->removeChild(mSpell, false);
    mParent->removeChild(mSpellName, false);
    mParent->removeChild(mSpBg, false);
    mParent->removeChild(mSpBgTop, true);
    mParent->removeChild(mSpFgNode, true);
    mParent->removeChild(mSpellGrid, false);
    mParent->removeChild(mHitNode, true);
    for(int i=0; i<MAXHPMETER; ++i)
    {
        if( mHPMeters[i] != NULL )
        {
            mParent->removeChild(mHPMeters[i], true);
            mHPMeters[i] = NULL;
        }
    }
    mParent->removeChild(mDartMeter, true);
    if( mMenu != NULL )
    {
        mParent->removeChild(mMenu, true);
    }
    if( mFlyBg != NULL )
    {
        mParent->removeChild(mFlyBg, true);
    }
    if( mFlyNumber != NULL )
    {
        mParent->removeChild(mFlyNumber, true);
    }
    if( mSPBg != NULL )
    {
        mParent->removeChild(mSPBg, true);
    }
    if( mSPNumber != NULL )
    {
        mParent->removeChild(mSPNumber, true);
    }
    
    if( play->mode == MODE_CLASSIC )
    {//remove classic
        mParent->removeChild(mDistanceNumber, false);
        mParent->removeChild(mDistanceM, false);

        mParent->removeChild(mCoinNumber, false);
        mParent->removeChild(mCoinMark, false);
        mParent->removeChild(mFeverCoins, true);

    }
    else
    {//remove arcade
        mParent->removeChild(mScoreNumber, true);
        mParent->removeChild(mSpeedBoost, true);
        mParent->removeChild(mTimerLabel, true);
        mParent->removeChild(mTimerMark, true);
        mParent->removeChild(mCup, true);
        mParent->removeChild(mArcadeX, true);
    }
}

void GameInterface::onFly() 
{
    //MessageManager::sharedMessageManager()->pushMsg(GameScript::sharedScript(), callfuncND_selector(GameScript::invokeSpellRelease), CCInteger::create(SPELL_FLY));
    GameScript::sharedScript()->invokeSpellRelease(CCInteger::create(SPELL_FLY));
    GameRecord::sharedGameRecord()->item_fly--;
    mFly->setVisible(false);
    mFlyBg->setVisible(false);
    mFlyNumber->setVisible(false);
    mFlyUsed = true;
    GameRecord::sharedGameRecord()->task->dispatchTask(ACH_USEGAGET, 1);
}

void GameInterface::onSP() 
{
    SimpleAudioEngine::sharedEngine()->playEffect(CCFileUtils::sharedFileUtils()->fullPathForFilename("sound/charge.mp3").c_str());
    //MessageManager::sharedMessageManager()->pushMsg(GameScript::sharedScript(), callfuncND_selector(GameScript::invokeSP), NULL);
    GameScript::sharedScript()->invokeSP();
    GameRecord::sharedGameRecord()->item_sp--;
    mSP->setVisible(false);
    mSPBg->setVisible(false);
    mSPNumber->setVisible(false);
    mPotionUsed = true;
    GameRecord::sharedGameRecord()->task->dispatchTask(ACH_USEPOTION, 1);
}

void GameInterface::rebuildDarts(bool unlimit) 
{
    GamePlay *play = GamePlay::sharedGamePlay();
    mDartMeter->removeAllChildrenWithCleanup(true);
    if( unlimit )
    {
        for(int i=0; i<play->mainrole->dart; ++i)
        {
            CCSprite *d = CCSprite::createWithSpriteFrameName("fbwx2.png");
            d->setAnchorPoint(ccp(0, 0));
            d->setPosition(ccp( mDartDis*i, 0));
            mDartMeter->addChild(d);
            
            CCSprite *m = CCSprite::createWithSpriteFrameName("fbwx1.png");
            m->setAnchorPoint(ccp(0, 0));
            m->setPosition(d->getPosition());
            mDartMeter->addChild(m, 1, i);
        }
    }
    else {
        for(int i=0; i<play->mainrole->dart; ++i)
        {
            CCSprite *d = CCSprite::createWithSpriteFrameName("ui-dart.png");
            d->setAnchorPoint(ccp(0, 0));
            d->setPosition(ccp( mDartDis*i, 0));
            mDartMeter->addChild(d);
        }
    }
    mDartCache = play->mainrole->dart;
}

void GameInterface::toggleUnlimtedDarts(bool flag) 
{
    this->rebuildDarts(flag);
    mUnlimtedDarts = flag;
    mUnlimtedDartsTimer = 0;
}

SEL_MenuHandler GameInterface::onResolveCCBCCMenuItemSelector(CCObject * pTarget, const char* pSelectorName)
{
    return NULL;
}

Control::Handler GameInterface::onResolveCCBCCControlSelector(CCObject * pTarget, const char* pSelectorName)
{
    return NULL;
}
bool GameInterface::onAssignCCBMemberVariable(CCObject* pTarget, const char* pMemberVariableName, CCNode* pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this,"mSpeedBg", CCLayerColor *, mSpeedBg)
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this,"mSpeedRed", CCLayerColor *, mSpeedRed)
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this,"mSpeedNormal", CCLayerColor *, mSpeedNormal)
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this,"mSpeedFever", CCLayerColor *, mSpeedFever)
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this,"mSpeedMask", CCSprite *, mSpeedMask)
    //  CCLog(pMemberVariableName);
    
    return false;
}
