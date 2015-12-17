#include "GameInterface.h"
#include "GamePlay.h"
#include "GameConstants.h"
#include "GameRecord.h"
#include "GameData.h"
#include "Tasks.h"
#include "UniversalFit.h"
#include "GameScript.h"
#include "ArcadeMode.h"

;
USING_NS_CC_EXT;

GameInterface* GameInterface::interface(cocos2d::Node * parent) 
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
//    GamePlay *play = GamePlay::sharedGamePlay();
//    
//    //*** COMMON Interface ***
//    
//    //interface
//    mUIBg = cocos2d::Sprite::createWithSpriteFrameName("ui-bg.png");
//    mUIBg->setAnchorPoint(cocos2d::Vec2(0, 0));
//    mUIBg->setPosition(cocos2d::Vec2(0, 0));
//    mParent->addChild(mUIBg,  LAYER_UI);
//    
//    //SP槽
//    mSpBg = cocos2d::Sprite::createWithSpriteFrameName("ui-spell-bg.png");
//    mSpBg->setAnchorPoint(cocos2d::Vec2(0, 0));
//    mSpBg->setPosition(cocos2d::Vec2(31, 4));
//    mSpBg->setScaleX(0);
//    mParent->addChild(mSpBg, LAYER_UI);
//    
//    mSpBgTop = cocos2d::Sprite::createWithSpriteFrameName("ui-spell-bgtop.png");
//    mSpBgTop->setAnchorPoint(cocos2d::Vec2(0, 0));
//    mSpBgTop->setPosition(cocos2d::Vec2(31, 4));
//    mParent->addChild(mSpBgTop, LAYER_UI);
//    
//    mSpFgNode = cocos2d::Node::create();
//    mSpFgNode->setPosition(cocos2d::Vec2(31, 4));
//    mParent->addChild(mSpFgNode, LAYER_UI);
//    mSpFg = cocos2d::Sprite::createWithSpriteFrameName("ui-spell-fg.png");
//    mSpFg->setAnchorPoint(cocos2d::Vec2(0, 0));
//    mSpFg->setPosition(cocos2d::Vec2(0, 0));
//    mSpFg->setScaleX(0);
//    mSpFgNode->addChild(mSpFg, LAYER_UI);
//    mSpFgTop = cocos2d::Sprite::createWithSpriteFrameName("ui-spell-fgtop.png");
//    mSpFgTop->setAnchorPoint(cocos2d::Vec2(0, 0));
//    mSpFgTop->setPosition(cocos2d::Vec2(mSpFg->getPosition().x, 0));
//    mSpFgTop->setVisible(false);
//    mSpFgNode->addChild(mSpFgTop);
//cocos2d::CCBlink *usaBlink = cocos2d::CCBlink::create(60, 300);
//cocos2d::CCRepeatForever *usaRep = cocos2d::CCRepeatForever::create(usaBlink);
//    mSpFgNode->runAction(usaRep);
//    
//    //动态使用框子
//    int gn = play->mainrole->maxSP/play->mainrole->activeSP;
//    if( gn > 4 || gn < 1 )
//    {
//        gn = 1;
//    }
//    mSpellGrid = cocos2d::Sprite::createWithSpriteFrameName(cocos2d::CCString::createWithFormat("ui-spell%d.png", gn)->getCString());
//    mSpellGrid->setAnchorPoint(cocos2d::Vec2(0, 0));
//    mSpellGrid->setPosition(cocos2d::Vec2(23.5f, 2));
//    mParent->addChild(mSpellGrid, LAYER_UI+1);
//    mSpCache = 0;
//    
//    //spell
//    int rid = GameRecord::sharedGameRecord()->curr_char;
//    int spid = GameRecord::sharedGameRecord()->char_equip_spell[rid];
//    Special *spell = (Special*)GameData::fetchSpecials()->objectAtIndex(spid);
//    mSpell = cocos2d::Sprite::create(spell->icon->getCString());
//    mSpell->setAnchorPoint(cocos2d::Vec2(0.5, 0.5));
//    mSpell->setPosition(cocos2d::Vec2(14, 14));
//    mParent->addChild(mSpell,  LAYER_UI);
//    mSpellName = cocos2d::Sprite::createWithSpriteFrameName(cocos2d::CCString::createWithFormat("name%d.png", GameRecord::sharedGameRecord()->curr_char)->getCString());
//    mSpellName->setAnchorPoint(cocos2d::Vec2(0, 0));
//    mSpellName->setPosition(cocos2d::Vec2(37, 13));
//    mParent->addChild(mSpellName,  LAYER_UI);
//    
//    //combo meter
//    mHitNode = cocos2d::Node::create();
//    mHitNode->setPosition(cocos2d::Vec2( UniversalFit::sharedUniversalFit()->playSize.width-5, 150));
//    mHitNode->setVisible(false);
//    mParent->addChild(mHitNode, LAYER_UI);
//    mHits = cocos2d::Sprite::createWithSpriteFrameName("hits.png");
//    mHits->setAnchorPoint(cocos2d::Vec2(1, 0.5f));
//    mHits->setPosition(cocos2d::Vec2(0, 0));
//    mHitNode->addChild(mHits);
//    mHitCount = cocos2d::CCLabelBMFont::create("0", "ab70.fnt");
//    mHitCount->setAnchorPoint(cocos2d::Vec2(1, 0.5f));
//    mHitCount->setPosition(cocos2d::Vec2(-(mHits->getContentSize().width + 5), 0));
//    mHitNode->addChild(mHitCount);
//    
//    //dart meter
//    mDartMeter = cocos2d::Node::create();
//    mDartMeter->setPosition(cocos2d::Vec2(136, 0));
//    mParent->addChild(mDartMeter, LAYER_UI);
//    mDartDis = 50.0f/play->mainrole->dart;
//    if( mDartDis > 12 )
//    {
//        mDartDis = 12;
//    }
//    for(int i=0; i<play->mainrole->dart; ++i)
//    {
//cocos2d::Sprite *d = cocos2d::Sprite::createWithSpriteFrameName("ui-dart.png");
//        d->setAnchorPoint(cocos2d::Vec2(0, 0));
//        d->setPosition(cocos2d::Vec2( mDartDis*i, 0));
//        mDartMeter->addChild(d);
//    }
//    mDartCache = 5;
//    
//    if( play->mode == MODE_CLASSIC )
//    {//*** Classic Mode ***
//        //距离
//        mDistanceNumber = cocos2d::CCLabelBMFont::create("0", "ra50.fnt");
//        mDistanceNumber->setAnchorPoint(cocos2d::Vec2(0, 1));
//        mDistanceNumber->setPosition(cocos2d::Vec2(5, SCREEN_HEIGHT-5));
//        mParent->addChild(mDistanceNumber, LAYER_UI);
//        mDistanceM = cocos2d::CCLabelBMFont::create("m", "ra50.fnt");
//        mDistanceM->setAnchorPoint(cocos2d::Vec2(0, 1));
//        mDistanceM->setPosition(cocos2d::Vec2(5+10, SCREEN_HEIGHT-5));
//        mParent->addChild(mDistanceM, LAYER_UI);
//        mDistanceCache = 0;
//        
//        //金币
//        mCoinNumber = cocos2d::CCLabelBMFont::create("0", "ra40.fnt");
//        mCoinNumber->setAnchorPoint(cocos2d::Vec2(0, 1));
//        mCoinNumber->setPosition(cocos2d::Vec2(5, SCREEN_HEIGHT - mDistanceNumber->getContentSize().height));
//        mParent->addChild(mCoinNumber, LAYER_UI);
//        mCoinMark = cocos2d::Sprite::createWithSpriteFrameName("go_coin.png");
//        mCoinMark->setAnchorPoint(cocos2d::Vec2(0, 0));
//        mCoinMark->setPosition(cocos2d::Vec2(8+8, SCREEN_HEIGHT - mDistanceNumber->getContentSize().height - 15));
//        mParent->addChild(mCoinMark, LAYER_UI);
//        
//        //fever coins
//        mFeverCoins = cocos2d::Node::create();
//        mFeverCoins->setPosition(cocos2d::Vec2( UniversalFit::sharedUniversalFit()->playSize.width-5, 170));
//        mFeverCoins->setVisible(false);
//        mParent->addChild(mFeverCoins, LAYER_UI);
//cocos2d::Sprite *fevercoin = cocos2d::Sprite::createWithSpriteFrameName("fevercoins.png");
//        fevercoin->setAnchorPoint(cocos2d::Vec2(1, 0.5f));
//        mFeverCoins->addChild(fevercoin);
//        mFeverFont = cocos2d::CCLabelBMFont::create("+0", "ab56.fnt");
//        mFeverFont->setAnchorPoint(cocos2d::Vec2(1, 0.5f));
//        mFeverFont->setPosition(cocos2d::Vec2(-(fevercoin->getContentSize().width+5), 0));
//        mFeverCoins->addChild(mFeverFont);
//        
//        //hp meter
//        for(int i=0; i<play->mainrole->maxHP; ++i)
//        {
//            mHPMeters[i] = cocos2d::Sprite::createWithSpriteFrameName("heart.png");
//            float ahw = mHPMeters[i]->getContentSize().width*play->mainrole->maxHP + 1.5f*(play->mainrole->maxHP-1);
//            float off = (UniversalFit::sharedUniversalFit()->playSize.width - ahw)/2;
//            mHPMeters[i]->setAnchorPoint(cocos2d::Vec2(0, 1));
//            mHPMeters[i]->setPosition(cocos2d::Vec2( off+i*mHPMeters[i]->getContentSize().width + 1.5f*i, SCREEN_HEIGHT-5));
//            mParent->addChild(mHPMeters[i], LAYER_UI);
//        }
//        mHPCache = play->mainrole->maxHP;
//        
//        //buttons
//        if( GameRecord::sharedGameRecord()->item_fly > 0 ||
//           GameRecord::sharedGameRecord()->item_sp > 0 )
//        {
//            mMenu = cocos2d::CCMenu::create( NULL);
//            mMenu->setPosition(cocos2d::Vec2(0, 0));
//            play->addChild(mMenu, LAYER_UI);
//            if( GameRecord::sharedGameRecord()->item_fly > 0 )
//            {
//                mFly = cocos2d::CCMenuItemSprite::create(cocos2d::Sprite::create("icon100.png"), Sprite::create("icon100.png"), this, menu_selector(GameInterface::onFly));
//                mFly->setAnchorPoint(cocos2d::Vec2(0, 0));
//                mFly->setPosition(cocos2d::Vec2( 340, 10));
//                mMenu->addChild(mFly);
//                
//                mFlyBg = cocos2d::Sprite::createWithSpriteFrameName("djan.png");
//                mFlyBg->setAnchorPoint(cocos2d::Vec2(0, 0));
//                mFlyBg->setPosition(ccpAdd(mFly->getPosition(), Vec2(3, 0)));
//                play->addChild(mFlyBg, LAYER_UI-1);
//                mFlyNumber = cocos2d::CCLabelBMFont::create(cocos2d::CCString::createWithFormat("%d", GameRecord::sharedGameRecord()->item_fly)->getCString(), "ra50.fnt");
//                mFlyNumber->setAnchorPoint(cocos2d::Vec2(0.5f, 0));
//                mFlyNumber->setPosition(ccpAdd(mFly->getPosition(), Vec2(25, -10)));
//                play->addChild(mFlyNumber, LAYER_UI+1);
//            }
//            if( GameRecord::sharedGameRecord()->item_sp > 0 )
//            {
//                mSP = cocos2d::CCMenuItemSprite::create(cocos2d::Sprite::create("icon102.png"), Sprite::create("icon102.png"), this, menu_selector(GameInterface::onSP));
//                mSP->setAnchorPoint(cocos2d::Vec2(0, 0));
//                mSP->setPosition(cocos2d::Vec2( 400, 10));
//                mMenu->addChild(mSP);
//                
//                mSPBg = cocos2d::Sprite::createWithSpriteFrameName("djan.png");
//                mSPBg->setAnchorPoint(cocos2d::Vec2(0, 0));
//                mSPBg->setPosition(ccpAdd(mSP->getPosition(), Vec2(3, 0)));
//                play->addChild(mSPBg, LAYER_UI-1);
//                mSPNumber = cocos2d::CCLabelBMFont::create(cocos2d::CCString::createWithFormat("%d", GameRecord::sharedGameRecord()->item_sp)->getCString(), "ra50.fnt");
//                mSPNumber->setAnchorPoint(cocos2d::Vec2(0.5f, 0));
//                mSPNumber->setPosition(ccpAdd(mSP->getPosition(), Vec2(25, -10)));
//                play->addChild(mSPNumber, LAYER_UI+1);
//            }
//        }
//        else {
//            mMenu = NULL;
//        }
//    }
//    else
//    {//*** Arcade Mode ***
//        //score
//        mScoreNumber = cocos2d::CCLabelBMFont::create("0", "ra50.fnt");
//        mScoreNumber->setAnchorPoint(cocos2d::Vec2(0, 1));
//        mScoreNumber->setPosition(cocos2d::Vec2(5, SCREEN_HEIGHT-5));
//        mParent->addChild(mScoreNumber, LAYER_UI);
//        mScoreCache = 0;
//        
//        //speed * boost
//        cocosbuilder::NodeLoaderLibrary *pNodeLib = cocosbuilder::NodeLoaderLibrary::getInstance();
//        cocosbuilder::CCBReader *pReader = new cocosbuilder::CCBReader(pNodeLib, this, this);
//cocos2d::Node *mSpeedBoost = pReader->readNodeGraphFromFile("ui-arcade-speed.ccbi", this);
//        pReader->release();
//        
//        mSpeedBoost->setPosition(cocos2d::Vec2(UniversalFit::sharedUniversalFit()->playSize.width, 0));
//        mSpeedState = 0;
//        mParent->addChild(mSpeedBoost, LAYER_UI);
//        
//        //timer
//        mTimerLabel = cocos2d::CCLabelBMFont::create("90", "ra50.fnt");
//        mTimerLabel->setAnchorPoint(cocos2d::Vec2(0.5f, 1));
//        mTimerLabel->setPosition(cocos2d::Vec2(UniversalFit::sharedUniversalFit()->playSize.width/2, UniversalFit::sharedUniversalFit()->playSize.height - 5));
//        mParent->addChild(mTimerLabel, LAYER_UI);
//        mTimerCache = 90;
//        mTimerMark = cocos2d::Sprite::createWithSpriteFrameName("time.png");
//        mTimerMark->setAnchorPoint(cocos2d::Vec2(0.5f, 1));
//        cocos2d::Point postm = mTimerLabel->getPosition();
//        postm.y -= mTimerLabel->getContentSize().height - 5;
//        mTimerMark->setPosition(postm);
//        mParent->addChild(mTimerMark, LAYER_UI);
//        
//        //cup
//        mCupCache = 0;
//        mCup = cocos2d::Sprite::createWithSpriteFrameName("tp-bronze.png");
//        mCup->setAnchorPoint(cocos2d::Vec2(1, 0));
//        mCup->setPosition(cocos2d::Vec2(UniversalFit::sharedUniversalFit()->playSize.width - 10, 25));
//        mCup->setVisible(false);
//        mParent->addChild(mCup, LAYER_UI);
//        
//        //arcade x
//        mArcadeX = cocos2d::CCLabelBMFont::create("x0", "ab70.fnt");
//        mArcadeX->setAnchorPoint(cocos2d::Vec2(1, 0.5f));
//        mArcadeX->setPosition(cocos2d::Vec2(UniversalFit::sharedUniversalFit()->playSize.width - 10, 180));
//        mArcadeX->setVisible(false);
//        mArcadeX->setColor(ccc3(255, 204, 0));
//        mParent->addChild(mArcadeX, LAYER_UI);
//    }
//    
//    mFlyUsed = false;
//    mPotionUsed = false;
//    mUnlimtedDarts = false;
}

void GameInterface::onUpdateClassic(float delta, GamePlay* play) 
{
//  
//    //更新距离
//    int dm = play->distance/PLAY_DISMETER;
//    if( dm != mDistanceCache )
//    {
//        mDistanceNumber->setString(cocos2d::CCString::createWithFormat("%d", dm)->getCString());
//        mDistanceCache = dm;
//        mDistanceM->setPosition(cocos2d::Vec2(5+10*strlen(mDistanceNumber->getString()), SCREEN_HEIGHT-5));
//    }
//    
//    //更新HP
//    if( play->mainrole->HP != mHPCache )
//    {
//cocos2d::SpriteFrameCache *framecache = cocos2d::SpriteFrameCache::sharedSpriteFrameCache();
//        for(int i=0; i<play->mainrole->maxHP; ++i)
//        {
//            if( i<play->mainrole->HP )
//            {
//                mHPMeters[i]->setDisplayFrame(framecache->spriteFrameByName("heart.png"));
//            }
//            else {
//                mHPMeters[i]->setDisplayFrame(framecache->spriteFrameByName("heart2.png"));
//            }
//        }
//        mHPCache = play->mainrole->HP;
//    }
//    
//    //更新coin
//    if( play->coins != mCoinCache )
//    {
//        mCoinCache = play->coins;
//        mCoinNumber->setString(cocos2d::CCString::createWithFormat("%d", mCoinCache)->getCString());
//        mCoinMark->setPosition(cocos2d::Vec2(8+8*strlen(mCoinNumber->getString()), SCREEN_HEIGHT - mDistanceNumber->getContentSize().height - 15));
//    }
//    
//    if( play->state == STATE_TUTORIAL )
//    {
//        this->toggleShowItemButtons(false);
//    }
//    else {
//        this->toggleShowItemButtons(true);
//    }
//    
//    //hide button
//    if( dm > 100 && mMenu != NULL )
//    {
//        play->removeChild(mMenu, true);
//        mMenu = NULL;
//        if( mFlyBg != NULL )
//        {
//            play->removeChild(mFlyBg, true);
//            mFlyBg = NULL;
//        }
//        if( mFlyNumber != NULL )
//        {
//            play->removeChild(mFlyNumber, true);
//            mFlyNumber = NULL;
//        }
//        if( mSPBg != NULL )
//        {
//            play->removeChild(mSPBg, true);
//            mSPBg = NULL;
//        }
//        if( mSPNumber != NULL )
//        {
//            play->removeChild(mSPNumber, true);
//            mSPNumber = NULL;
//        }
//    }
    
}

void GameInterface::onUpdateArcade(float delta, GamePlay* play) 
{
    ArcadeMode *arcade = play->arcade;
    
    //update score
    if( arcade->score != mScoreCache )
    {
        mScoreNumber->setString(cocos2d::CCString::createWithFormat("%d", arcade->score)->getCString());
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
            mTimerLabel->setString(cocos2d::CCString::createWithFormat("%d", currtime)->getCString());
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
                mCup->setDisplayFrame(cocos2d::SpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("tp-bronze.png"));
                SimpleAudioEngine::sharedEngine()->playEffect(cocos2d::CCFileUtils::sharedFileUtils()->fullPathForFilename("sound/getscroll.mp3").c_str());
            }
                break;
            case 2:
            {
                mCup->setVisible(true);
                mCup->setDisplayFrame(cocos2d::SpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("tp-silver.png"));
                SimpleAudioEngine::sharedEngine()->playEffect(cocos2d::CCFileUtils::sharedFileUtils()->fullPathForFilename("sound/getscroll.mp3").c_str());
            }
                break;
            case 3:
            {
                mCup->setVisible(true);
                mCup->setDisplayFrame(cocos2d::SpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("tp-gold.png"));
                SimpleAudioEngine::sharedEngine()->playEffect(cocos2d::CCFileUtils::sharedFileUtils()->fullPathForFilename("sound/getscroll.mp3").c_str());
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
cocos2d::CCSize redsize = mSpeedRed->getContentSize();
            redsize.width = (arcade->speed - ARCADE_MINSPEED)/(ARCADE_INITSPEED-ARCADE_MINSPEED)*12.5f;
            mSpeedRed->setContentSize(redsize);
            targetstate = 1;
        }
        else if( arcade->speed < ARCADE_FEVER1 )
        {//normal
            mSpeedBg->setColor(ccc3(117, 89, 6));
            mSpeedRed->setVisible(true);
cocos2d::CCSize redsize = mSpeedRed->getContentSize();
            redsize.width = 12.5f;
            mSpeedRed->setContentSize(redsize);
            mSpeedNormal->setVisible(true);
cocos2d::CCSize normalsize = mSpeedNormal->getContentSize();
            normalsize.width = (arcade->speed - ARCADE_INITSPEED)/(ARCADE_FEVER1 - ARCADE_INITSPEED)*37.5f;
            mSpeedNormal->setContentSize(normalsize);
            mSpeedFever->setVisible(false);
            targetstate = 0;
        }
        else if( arcade->speed < ARCADE_FEVER2 )
        {//yellow flash
            mSpeedBg->setColor(ccc3(117, 89, 6));
            mSpeedRed->setVisible(true);
cocos2d::CCSize redsize = mSpeedRed->getContentSize();
            redsize.width = 12.5f;
            mSpeedRed->setContentSize(redsize);
            mSpeedNormal->setVisible(true);
cocos2d::CCSize normalsize = mSpeedNormal->getContentSize();
            normalsize.width = 37.5f;
            mSpeedNormal->setContentSize(normalsize);
            mSpeedFever->setVisible(true);
cocos2d::CCSize feversize = mSpeedFever->getContentSize();
            feversize.width = (arcade->speed - ARCADE_FEVER1)/(ARCADE_FEVER2 - ARCADE_FEVER1)*25;
            mSpeedFever->setContentSize(feversize);
            targetstate = 2;
        }
        else
        {//yellow flash 2
            mSpeedBg->setColor(ccc3(117, 89, 6));
            mSpeedRed->setVisible(true);
cocos2d::CCSize redsize = mSpeedRed->getContentSize();
            redsize.width = 12.5f;
            mSpeedRed->setContentSize(redsize);
            mSpeedNormal->setVisible(true);
cocos2d::CCSize normalsize = mSpeedNormal->getContentSize();
            normalsize.width = 37.5f;
            mSpeedNormal->setContentSize(normalsize);
            mSpeedFever->setVisible(true);
cocos2d::CCSize feversize = mSpeedFever->getContentSize();
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
cocos2d::CCBlink *bnk = cocos2d::CCBlink::create(1, 5);
cocos2d::CCRepeatForever *rf = cocos2d::CCRepeatForever::create(bnk);
                mSpeedRed->runAction(rf);
            }
            else if( targetstate == 2 )
            {
cocos2d::CCBlink *bnk = cocos2d::CCBlink::create(1, 5);
cocos2d::CCRepeatForever *rf = cocos2d::CCRepeatForever::create(bnk);
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
        mSpBgTop->setPosition(cocos2d::Vec2(posx, mSpBg->getPosition().y));
        
        mSpCache = play->mainrole->SP;
        int n = play->mainrole->activeSP*(play->mainrole->SP/play->mainrole->activeSP);
        float rate2 = 1.0f*n/play->mainrole->maxSP;
        int posx2 = mSpFg->getContentSize().width*rate2;
        mSpFgTop->setPosition(cocos2d::Vec2(posx2, 0));
        mSpFg->setScaleX(rate2);
        if( n == 0 )
        {
            mSpFgTop->setVisible(false);
        }
        else {
            mSpFgTop->setVisible(true);
        }
cocos2d::SpriteFrameCache *share = cocos2d::SpriteFrameCache::sharedSpriteFrameCache();
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
cocos2d::Sprite *dm = (cocos2d::Sprite*)(mDartMeter->getChildByTag(i));
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
    mFeverFont->setString(cocos2d::CCString::createWithFormat("+%d", coins)->getCString());
    mFeverCoins->setScale(0.8f);
cocos2d::CCScaleTo *a1 = cocos2d::CCScaleTo::create(0.1f, 1.1f);
cocos2d::CCScaleTo *a2 = cocos2d::CCScaleTo::create(0.2f, 1);
cocos2d::CCDelayTime *a3 = cocos2d::CCDelayTime::create(3);
cocos2d::CCHide *a4 = cocos2d::CCHide::create();
cocos2d::CCSequence *sq = cocos2d::CCSequence::create(a1, a2, a3, a4, NULL);
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
    mHitCount->setString(cocos2d::CCString::createWithFormat("%d", number)->getCString());
    mHitNode->setScale(0.01f);
cocos2d::CCScaleTo *a1 = cocos2d::CCScaleTo::create(0.1f, 1.5f);
cocos2d::CCScaleTo *a2 = cocos2d::CCScaleTo::create(0.2f, 1);
cocos2d::CCSequence *sq = cocos2d::CCSequence::create(a1, a2, NULL);
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
cocos2d::CCScaleTo *a1 = cocos2d::CCScaleTo::create(0.5f, 0);
cocos2d::CCHide *a2 = cocos2d::CCHide::create();
cocos2d::CCSequence *sq = cocos2d::CCSequence::create(a1, a2, NULL);
    mHitNode->runAction(sq);
}

void GameInterface::setArcadeX(int x) 
{
    mArcadeX->setString(cocos2d::CCString::createWithFormat("x%d", x)->getCString());
    if( x <= 0 )
    {
cocos2d::CCScaleTo *st = cocos2d::CCScaleTo::create(1, 0);
cocos2d::CCHide *hd = cocos2d::CCHide::create();
cocos2d::CCSequence *seq = cocos2d::CCSequence::create(st,  hd, NULL);
        mArcadeX->runAction(seq);
    }
    else
    {
        mArcadeX->setScale(0);
        mArcadeX->setVisible(true);
cocos2d::CCScaleTo *st1 = cocos2d::CCScaleTo::create(0.3f, 1.5f);
cocos2d::CCScaleTo *st2 = cocos2d::CCScaleTo::create(0.2f, 0.8f);
cocos2d::CCScaleTo *st3 = cocos2d::CCScaleTo::create(0.1f, 1.2f);
cocos2d::CCScaleTo *st4 = cocos2d::CCScaleTo::create(0.1f, 1);
cocos2d::CCSequence *seq = cocos2d::CCSequence::create(st1, st2, st3, st4, NULL);
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
    GameScript::sharedScript()->invokeSpellRelease(cocos2d::CCInteger::create(SPELL_FLY));
    GameRecord::sharedGameRecord()->item_fly--;
    mFly->setVisible(false);
    mFlyBg->setVisible(false);
    mFlyNumber->setVisible(false);
    mFlyUsed = true;
    GameRecord::sharedGameRecord()->task->dispatchTask(ACH_USEGAGET, 1);
}

void GameInterface::onSP() 
{
    SimpleAudioEngine::sharedEngine()->playEffect(cocos2d::CCFileUtils::sharedFileUtils()->fullPathForFilename("sound/charge.mp3").c_str());
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
cocos2d::Sprite *d = cocos2d::Sprite::createWithSpriteFrameName("fbwx2.png");
            d->setAnchorPoint(cocos2d::Vec2(0, 0));
            d->setPosition(cocos2d::Vec2( mDartDis*i, 0));
            mDartMeter->addChild(d);
cocos2d::Sprite *m = cocos2d::Sprite::createWithSpriteFrameName("fbwx1.png");
            m->setAnchorPoint(cocos2d::Vec2(0, 0));
            m->setPosition(d->getPosition());
            mDartMeter->addChild(m, 1, i);
        }
    }
    else {
        for(int i=0; i<play->mainrole->dart; ++i)
        {
cocos2d::Sprite *d = cocos2d::Sprite::createWithSpriteFrameName("ui-dart.png");
            d->setAnchorPoint(cocos2d::Vec2(0, 0));
            d->setPosition(cocos2d::Vec2( mDartDis*i, 0));
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

SEL_MenuHandler GameInterface::onResolveCCBCCMenuItemSelector(cocos2d::Ref * pTarget, const char* pSelectorName)
{
    return NULL;
}

Control::Handler GameInterface::onResolveCCBCCControlSelector(cocos2d::Ref * pTarget, const char* pSelectorName)
{
    return NULL;
}
bool GameInterface::onAssignCCBMemberVariable(cocos2d::Ref* pTarget, const char* pMemberVariableName, Node* pNode)
{
CCB_MEMBERVARIABLEASSIGNER_GLUE(this,"mSpeedBg", LayerColor *, mSpeedBg)
CCB_MEMBERVARIABLEASSIGNER_GLUE(this,"mSpeedRed", LayerColor *, mSpeedRed)
CCB_MEMBERVARIABLEASSIGNER_GLUE(this,"mSpeedNormal", LayerColor *, mSpeedNormal)
CCB_MEMBERVARIABLEASSIGNER_GLUE(this,"mSpeedFever", LayerColor *, mSpeedFever)
CCB_MEMBERVARIABLEASSIGNER_GLUE(this,"mSpeedMask", Sprite *, mSpeedMask)
    //  CCLog(pMemberVariableName);
    
    return false;
}
