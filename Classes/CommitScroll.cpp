//
//  CommitScroll.m
//  NinjiaRush
//
//  Created by 马 颂文 on 12-6-19.
//  Copyright (c) 2012年 TrinGame. All rights reserved.
//

#include "CommitScroll.h"
#include "GamePlay.h"
#include "GameOver.h"
#include "GameRecord.h"
#include "Collections.h"
#include "SimpleAudioEngine.h"
#include "GameConstants.h"
#include "Tasks.h"
#include "GameData.h"
#include "ObjectiveComplete.h"
#include "UniversalFit.h"
#include "ClassicMode.h"

#define PRIZE_COUNT (11)

float gPrizeRates[PRIZE_COUNT] = 
{
    0.5f,//无
    0.0f,//10金币
    0.08f,//100金币
    0.02f,//500金币
    0.05f,//复活
    0.05f,//下局双倍
    0.015f,//飞镖碎片
    0.015f,//刀刃碎片
    0.015f,//技能碎片
    0.02f,//血上限
    0.02f,//飞镖上限
};

const char *gPrizeItem[PRIZE_COUNT] = 
{
    "",
    "10枚金币",
    "100枚金币",
    "500枚金币",
    "原地复活",
    "下局双倍金币",
    "飞镖碎片",
    "忍者刀碎片",
    "技能卷轴碎片",
    "生命强化碎片",
    "飞镖强化碎片",
};



CommitScroll* CommitScroll::commitScorll() 
{
    CommitScroll *ret = CommitScroll::create();
    return ret;
}

void CommitScroll::onCreate() 
{
    //hot load
cocos2d::SpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("ui-scroll.plist");
    
    GamePlay *play = GamePlay::sharedGamePlay();
    play->scheduleMask(Color3B(0, 0, 0), 128, 0);
    cocosbuilder::NodeLoaderLibrary *pNodeLib = cocosbuilder::NodeLoaderLibrary::getInstance();
    cocosbuilder::CCBReader *pReader = new cocosbuilder::CCBReader(pNodeLib, this, this);
    mNode = pReader->readNodeGraphFromFile("ui-scroll", this);
    pReader->release();

    mNode->setPosition(cocos2d::Vec2(UniversalFit::sharedUniversalFit()->baseLeft, 0));
    play->addChild(mNode, LAYER_MASK+1);
    
    //TODO: 
    //mFlash->blendFunc =  (ccBlendFunc) { GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA };//fix flash bug
    
    mScrollCount->setString(cocos2d::CCString::createWithFormat("x%d", play->scrolls)->getCString());
    mOver = false;
cocos2d::CCRotateBy *rb1 = cocos2d::CCRotateBy::create(1, 60);
cocos2d::CCRepeatForever *rf1 = cocos2d::CCRepeatForever::create(rb1);
    mRotateCW->runAction(rf1);
cocos2d::CCRotateBy *rb2 = cocos2d::CCRotateBy::create(1, -30);
cocos2d::CCRepeatForever *rf2 = cocos2d::CCRepeatForever::create(rb2);
    mRotateACW->runAction(rf2);
cocos2d::CCRotateBy *rb3 = cocos2d::CCRotateBy::create(1, 20);
cocos2d::CCRepeatForever *rf3 = cocos2d::CCRepeatForever::create(rb3);
    mLightBack->runAction(rf3);
    mState = 0;
    mPrize = 0;
    
    mEffect = GTAnimatedSprite::spriteWithGTAnimation(GTAnimation::loadedAnimationSet("effect"));
    mEffect->setScale(2.42f);
    mEffect->setVisible(false);
    mEffect->setPosition(mScroll->getPosition());
    mNode->addChild(mEffect);
    
    mReviveFlag = false;
}

void CommitScroll::onUpdate(float delta) 
{
//    GamePlay *play = GamePlay::sharedGamePlay();
//    if( mReviveFlag )
//    {
//        play->manager->removeGameObject(this);
//    }
//    if( mOver )
//    {
//        mOverTimer += delta;
//        if( mOverTimer > 1 )
//        {
//            play->manager->removeGameObject(this);
//        }
//    }
//    if( mEffect->isVisible() )
//    {
//        if( mEffect->updateGTAnimation(delta) )
//        {
//            mEffect->setVisible(false);
//        }
//    }
//    if( mState == 2 )
//    {
//        if( mPrize >= 2 )
//        {
//            mTimer+= delta;
//            float r = 1.0f - mTimer/1;
//            int op = 255*r;
//            mLightBack->setOpacity(op);
//cocos2d::Ref* node = NULL;
//CCARRAY_FOREACH( mRotateCW->getChildren(), node)
//            {
//cocos2d::Sprite *sp = (cocos2d::Sprite*)node;
//              sp->setOpacity(op);
//            }
//CCARRAY_FOREACH( mRotateACW->getChildren(), node)
//            {
//cocos2d::Sprite *sp = (cocos2d::Sprite*)node;
//              sp->setOpacity(op);
//            }
//            if( mTimer > 1 )
//            {
//                mLightBack->setOpacity(255);
//CCARRAY_FOREACH( mRotateCW->getChildren(), node)
//                {
//cocos2d::Sprite *sp = (cocos2d::Sprite*)node;
//                  sp->setOpacity(255);
//                }
//CCARRAY_FOREACH( mRotateACW->getChildren(), node)
//                {
//cocos2d::Sprite *sp = (cocos2d::Sprite*)node;
//                  sp->setOpacity(255);
//                }
//                mFlash->setVisible(false);
//                mFlash->setOpacity(255);
//                mRotateNode->setVisible(false);
//                mState = 0;
//                
//                if( play->scrolls <= 0 )
//                {
//                    mOver = true;
//                    mOverTimer = 0.5f;
//                }
//                else {
//                    mScroll->stopAllActions();
//                    mScroll->setDisplayFrame(cocos2d::SpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("js_bjzd.png"));
//cocos2d::CCScaleTo *sc1 = cocos2d::CCScaleTo::create(0.3f, 1.2f);
//cocos2d::CCScaleTo *sc2 = cocos2d::CCScaleTo::create(0.1f, 1.0f);
//cocos2d::CCSequence *sq = cocos2d::CCSequence::create(sc1, sc2, NULL);
//                    mScroll->runAction(sq);
//                }
//            }
//        }
//        else {
//            mState = 0;
//            if( play->scrolls <= 0 )
//            {
//                mOver = true;
//                mOverTimer = 0.5f;
//            }
//            else {
//                mScroll->setVisible(true);
//                mScroll->stopAllActions();
//                mScroll->setDisplayFrame(cocos2d::SpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("js_bjzd.png"));
//cocos2d::CCScaleTo *sc1 = cocos2d::CCScaleTo::create(0.3f, 1.2f);
//cocos2d::CCScaleTo *sc2 = cocos2d::CCScaleTo::create(0.1f, 1.0f);
//cocos2d::CCSequence *sq = cocos2d::CCSequence::create(sc1, sc2, NULL);
//                mScroll->runAction(sq);
//            }
//        }
//    }
//    if( mState == 1 && mPrize == 0 )
//    {
//        mTimer += delta;
//        if( mTimer > 1 )
//        {
//            mState = 2;
//            mTimer = 0;
//        }
//    }
}

void CommitScroll::onCashIn() 
{
    GamePlay *play = GamePlay::sharedGamePlay();
    if( play->scrolls > 0 )
    {
        //achievement cash in
        GameRecord::sharedGameRecord()->task->dispatchTask(ACH_CASHEDIN, play->scrolls);
        
        GameTool::PlaySound("sound/buy.mp3");
        play->coins += play->scrolls*PLAY_SCROLLPRICE;
        play->scrolls = 0;
        mScrollCount->setString(cocos2d::CCString::createWithFormat("x%d", play->scrolls)->getCString());
        mOver = true;
        mOverTimer = 0;
    }
}

void CommitScroll::onScroll() 
{
    GamePlay *play = GamePlay::sharedGamePlay();
    if( mState == 0 )
    {
        if( play->scrolls > 0 )
        {
            play->scrolls--;
            mScrollCount->setString(cocos2d::CCString::createWithFormat("x%d", play->scrolls)->getCString());
            
            int prize = this->lottery();
            if( prize == 0 )
            {
                GameRecord::sharedGameRecord()->task->dispatchTask(ACH_EMPITYSCROLL, 1);
            }
            
            //弹出效果
            mScroll->stopAllActions();
            if( prize < 2 )
            {
                GameTool::PlaySound("sound/smoke.mp3");
                mEffect->playGTAnimation(5, false);
                mEffect->setVisible(true);
                
                if( prize == 1 )
                {
                    const char *filename = "prize-1.png";
                    mScroll->setDisplayFrame(cocos2d::SpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(filename));
                    mScroll->setScale(0.01f);
cocos2d::CCScaleTo *sc1 = cocos2d::CCScaleTo::create(0.4f, 1.5f);
cocos2d::CCScaleTo *sc2 = cocos2d::CCScaleTo::create(0.2f, 1.0f);
cocos2d::CCSequence *sq = cocos2d::CCSequence::create(sc1, sc2, NULL);
                    mScroll->runAction(sq);
                }
                else {
                    mScroll->setVisible(false);
                }
            }
            else {
                GameTool::PlaySound("sound/openscroll.mp3");
                mFlash->setVisible(true);
cocos2d::CCFadeOut *fo1 = cocos2d::CCFadeOut::create(3);
                mFlash->runAction(fo1);
                mRotateNode->setVisible(true);
                
                const char *filename = cocos2d::CCString::createWithFormat("prize-%d.png", prize)->getCString();
                mScroll->setDisplayFrame(cocos2d::SpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(filename));
                mScroll->setScale(0.01f);
cocos2d::CCScaleTo *sc1 = cocos2d::CCScaleTo::create(0.4f, 1.5f);
cocos2d::CCScaleTo *sc2 = cocos2d::CCScaleTo::create(0.2f, 1.0f);
cocos2d::CCSequence *sq = cocos2d::CCSequence::create(sc1, sc2, NULL);
                mScroll->runAction(sq);
            }
            
            mState = 1;
            mTimer = 0;
        }
    }
    else if (mState == 1 )
    {
        mItemName->setString("");
        this->implementPrize();
        
        mScroll->stopAllActions();
cocos2d::CCScaleTo *sc1 = cocos2d::CCScaleTo::create(0.1f, 1.2f);
cocos2d::CCScaleTo *sc2 = cocos2d::CCScaleTo::create(0.3f, 0);
cocos2d::CCSequence *sq = cocos2d::CCSequence::create(sc1, sc2, NULL);
        mScroll->runAction(sq);
        
        mState = 2;
        mTimer = 0;
    }
}

int CommitScroll::lottery() 
{
    float sum = 0;
    float weight[PRIZE_COUNT];
    //initialize fixed weight
    for(int i=0; i<4; ++i)
    {
        weight[i] = gPrizeRates[i];
        sum += gPrizeRates[i];
    }
    //adding dynamic weight
    {//for revive rate
        if( GamePlay::sharedGamePlay()->reviveCount > 3 )
        {
            weight[4] = 0;
            //sum do not need to plus zero
        }
        else
        {
            weight[4] = gPrizeRates[4];
            sum += gPrizeRates[4];
        }
    }
    {//for double coin next round
        if( this->isPrizeAvailable(5) )
        {
            weight[5] = gPrizeRates[5];
            sum += weight[5];
        }
        else {
            weight[5] = 0;
            sum += weight[5];
        }
    }
    {//for shurikens 6
        int cnt = 0;
        for( int i=ITEM_SHURIKENSTART; i<ITEM_SHURIKENEND; ++i)
        {
            if( !GameRecord::sharedGameRecord()->collection->isItemCompleted(i) )
            {
                cnt++;
            }
        }
        weight[6] = gPrizeRates[6]*cnt;
        sum += weight[6];
    }
    {//for katanas 7
        int cnt = 0;
        for( int i=ITEM_KATANASTART; i<ITEM_KATANAEND; ++i)
        {
            if( !GameRecord::sharedGameRecord()->collection->isItemCompleted(i) )
            {
                cnt++;
            }
        }
        weight[7] = gPrizeRates[7]*cnt;
        sum += weight[7];
    }
    {//for specials 8
        int cnt = 0;
        for( int i=ITEM_SPECIALSTART; i<ITEM_SPECIALEND; ++i)
        {
            if( !GameRecord::sharedGameRecord()->collection->isItemCompleted(i) )
            {
                cnt++;
            }
        }
        weight[8] = gPrizeRates[8]*cnt;
        sum += weight[8];
    }
    //for pu life 9
    if( this->isPrizeAvailable(9) )
    {
        weight[9] = gPrizeRates[9];
        sum += gPrizeRates[9];
    }
    else {
        weight[9] = 0;
        sum += gPrizeRates[9];
    }
    //for pu shuirken 10
    if( this->isPrizeAvailable(10) )
    {
        weight[10] = gPrizeRates[10];
        sum += gPrizeRates[10];
    }
    else {
        weight[10] = 0;
        sum += gPrizeRates[10];
    }
    //rolling lottey...
    int prize = 0;
    float res = 1.0f*randomInt(100000)/100000.0f;
    float roller = 0;
    for(int i=0; i<PRIZE_COUNT; ++i)
    {
        float key = weight[i]/sum;
        if( res < key + roller )
        {
            prize = i;
            break;
        }
        else {
            roller += key;
        }
    }
    
    mPrize = prize;
    mItemName->setString(gPrizeItem[mPrize]);
    mItemName->stopAllActions();
cocos2d::CCFadeIn *fo = cocos2d::CCFadeIn::create(2);
    mItemName->runAction(fo);
    return prize;
}

void CommitScroll::implementPrize() 
{
    /*
    GamePlay *play = GamePlay::sharedGamePlay();
    //implement prize
    switch (mPrize) {
        case 1://Gold 10
        {
            play->coins += 10;
            GameTool::PlaySound("sound/getcoin.mp3");
        }
            break;
        case 2://Gold 100
        {
            play->coins += 100;
            GameTool::PlaySound("sound/getcoin.mp3");
        }
            break;
        case 3://Gold 500
        {
            play->coins += 500;
            GameTool::PlaySound("sound/getcoin.mp3");
        }
            break;
        case 4://Revive
        {
            mReviveFlag = true;
            GameTool::PlaySound("sound/charge.mp3");
            play->reviveCount++;
        }
            break;
        case 5://x2 Gold Next Run
        {
            GameRecord::sharedGameRecord()->game_doublecoin++;
            GameTool::PlaySound("sound/getcoin.mp3");
        }
            break;
            
        case 6://Piece Of Shuriken
        {
            float sweight[ITEM_SHURIKENEND - ITEM_SHURIKENSTART];
            float ssum = 0;
            int index = 0;
            for( int i=ITEM_SHURIKENSTART; i<ITEM_SHURIKENEND; ++i)
            {
                Shuriken* sh = GameData::queryShuriken(i);
                if( !GameRecord::sharedGameRecord()->collection->isItemCompleted(i) && sh != NULL )
                {
                    sweight[index] = sh->wght;
                    ssum += sweight[index];
                }
                else {
                    sweight[index] = 0;
                }
                
                index++;
            }
            float sroller = 0;
            float sres = 1.0f*randomInt(100000)/100000.0f;
            
            //debug print
            //CCLog("sres = %f", sres);
            
            for( int i=0; i<(ITEM_SHURIKENEND - ITEM_SHURIKENSTART); ++i)
            {
                float skey = sweight[i]/ssum;
                
                //debug print
                //CCLog("- %d - weight = %f / rate = %f", i, sweight[i], sroller + skey);
                
                if( sres < sroller + skey )
                {
                    GameRecord::sharedGameRecord()->collection->gainItemPiece(ITEM_SHURIKENSTART+i);
                    //pop notification
                    int uiid = ITEM_SHURIKENSTART+i;
                    if( GameRecord::sharedGameRecord()->collection->isItemCompleted(uiid) )
                    {
                        Shuriken *sh = GameData::queryShuriken(uiid);
                        GamePlay::sharedGamePlay()->pushNotification(sh->name, sh->icon, 4);
                    }
                    break;
                }
                else {
                    sroller += skey;
                }
            }
        }
            break;
        case 7://Piece Of Katana
        {
            float sweight[ITEM_KATANAEND - ITEM_KATANASTART];
            float ssum = 0;
            int index = 0;
            for( int i=ITEM_KATANASTART; i<ITEM_KATANAEND; ++i)
            {
                Katana* sh = GameData::queryKatana(i);
                if( !GameRecord::sharedGameRecord()->collection->isItemCompleted(i) && sh != NULL )
                {
                    sweight[index] = sh->wght;
                    ssum += sweight[index];
                }
                else {
                    sweight[index] = 0;
                }
                
                index++;
            }
            float sroller = 0;
            float sres = 1.0f*randomInt(100000)/100000.0f;
            
            //debug print
            //CCLog("sres = %f", sres);
            
            for( int i=0; i<(ITEM_KATANAEND - ITEM_KATANASTART); ++i)
            {
                float skey = sweight[i]/ssum;
                
                //debug print
                //CCLog("- %d - weight = %f / rate = %f", i, sweight[i], sroller + skey);
                
                if( sres < sroller + skey )
                {
                    GameRecord::sharedGameRecord()->collection->gainItemPiece(ITEM_KATANASTART+i);
                    //pop notification
                    int uiid = ITEM_KATANASTART+i;
                    if( GameRecord::sharedGameRecord()->collection->isItemCompleted(uiid) )
                    {
                        Katana *sh = GameData::queryKatana(uiid);
                        GamePlay::sharedGamePlay()->pushNotification(sh->name, sh->icon, 4);
                    }
                    break;
                }
                else {
                    sroller += skey;
                }
            }
        }
            break;
        case 8://Piece Of Special
        {
            float sweight[ITEM_SPECIALEND - ITEM_SPECIALSTART];
            float ssum = 0;
            int index = 0;
            for( int i=ITEM_SPECIALSTART; i<ITEM_SPECIALEND; ++i)
            {
                Special* sh = GameData::querySpecial(i);
                if( !GameRecord::sharedGameRecord()->collection->isItemCompleted(i) && sh != NULL )
                {
                    sweight[index] = sh->wght;
                    ssum += sweight[index];
                }
                else {
                    sweight[index] = 0;
                }
                
                index++;
            }
            float sroller = 0;
            float sres = 1.0f*randomInt(100000)/100000.0f;
            
            //debug print
            //CCLog("sres = %f", sres);
            
            for( int i=0; i<(ITEM_SPECIALEND - ITEM_SPECIALSTART); ++i)
            {
                float skey = sweight[i]/ssum;
                
                //debug print
                //CCLog("- %d - weight = %f / rate = %f", i, sweight[i], sroller + skey);
                
                if( sres < sroller + skey )
                {
                    GameRecord::sharedGameRecord()->collection->gainItemPiece(ITEM_SPECIALSTART+i);
                    //pop notification
                    int uiid = ITEM_SPECIALSTART+i;
                    if( GameRecord::sharedGameRecord()->collection->isItemCompleted(uiid) )
                    {
                        Special *sh = GameData::querySpecial(uiid);
                        GamePlay::sharedGamePlay()->pushNotification(sh->name, sh->icon, 4);
                    }
                    break;
                }
                else {
                    sroller += skey;
                }
            }
        }
            break;
        case 9://Power Up for HP
        {
            GameRecord::sharedGameRecord()->collection->life_piece++;
            //pop notification
//            if( GameRecord::sharedGameRecord()->collection->life_piece%9 == 0 )
//            {
//                GamePlay::sharedGamePlay()->pushNotification(cocos2d::CCString::create("lifeplus.png"), CCString::create("pulife.png"), 4);
//            }
        }
            break;
        case 10://Power Up for Shuriken
        {
//            GameRecord::sharedGameRecord()->collection->dart_piece++;
//            //pop notification
//            if( GameRecord::sharedGameRecord()->collection->dart_piece%9 == 0 )
//            {
//                GamePlay::sharedGamePlay()->pushNotification(cocos2d::CCString::create("dartplus.png"), CCString::create("pushuriken.png"), 4);
//            }
        }
            break;
    }
    */
}

bool CommitScroll::isPrizeAvailable(int pid) 
{
    if( pid == 5 )
    {//double coins next time
        if( GameRecord::sharedGameRecord()->game_doublecoin > 0 )
        {
            return false;
        }
        return true;
    }
    if( pid == 6 )
    {//Piece Of Shuriken
        for( int i=ITEM_SHURIKENSTART; i<ITEM_SHURIKENEND; ++i)
        {
            if( !GameRecord::sharedGameRecord()->collection->isItemCompleted(i) )
            {
                return true;
            }
        }
        return false;
    }
    if( pid == 7 )
    {//Piece Of Katana
        for( int i=ITEM_KATANASTART; i<ITEM_KATANAEND; ++i)
        {
            if( !GameRecord::sharedGameRecord()->collection->isItemCompleted(i) )
            {
                return true;
            }
        }
        return false;
    }
    if( pid == 8 )
    {//Piece Of Special
        for( int i=ITEM_SPECIALSTART; i<ITEM_SPECIALEND; ++i)
        {
            if( !GameRecord::sharedGameRecord()->collection->isItemCompleted(i) )
            {
                return true;
            }
        }
        return false;
    }
    if( pid == 9 )
    {//Piece Of HP Maxium
        int thl = 0;
        for(int i=0; i<GAME_CHARCOUNT; ++i)
        {
            thl += (GameData::roleMaxHP(i) - GameData::roleCurrHP(i))*9;
        }
        if( GameRecord::sharedGameRecord()->collection->life_piece >= thl )
        {
            return false;
        }
    }
    if( pid == 10 )
    {//Piece Of Shuriken Maxium
        int thl = 0;
        for(int i=0; i<GAME_CHARCOUNT; ++i)
        {
            thl += (GameData::roleMaxDart(i) - GameData::roleCurrDart(i))*9;
        }
        if( GameRecord::sharedGameRecord()->collection->dart_piece >= thl )
        {
            return false;
        }
    }
    return true;
}

void CommitScroll::onDestroy() 
{
    GamePlay *play = GamePlay::sharedGamePlay();
    play->unscheduleMask();
    play->removeChild(mNode, true);
    if( !mReviveFlag )
    {
        play->classic->goOver();
    }
    else {//Revive
        play->state = STATE_REVIVE;
        play->substate = 0;
        play->stateTimer = 0;
        play->gameOverTimer = -1;
        play->mainrole->HP = 1;
        play->mainrole->customAction(1);
        play->count_runscene--;
        play->gameOverTimer=-1;
        play->mainrole->timedInvincible(5);
    }
    
    //hot release
cocos2d::SpriteFrameCache::sharedSpriteFrameCache()->removeSpriteFramesFromFile("ui-scroll.plist");
    unloadTextureFromeSpriteFrameFile("ui-scroll.plist");
}

SEL_MenuHandler CommitScroll::onResolveCCBCCMenuItemSelector(cocos2d::Ref * pTarget, const char* pSelectorName)
{
//CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "onCashIn", CommitScroll::onCashIn)
//CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "onScroll", CommitScroll::onScroll)
    //  CCLog(pSelectorName);
  return NULL;
}

cocos2d::extension::Control::Handler   CommitScroll::onResolveCCBCCControlSelector(cocos2d::Ref * pTarget, const char* pSelectorName)
{
cocos2d::CCLog("Control");
  return NULL;
}
bool CommitScroll::onAssignCCBMemberVariable(cocos2d::Ref* pTarget, const char* pMemberVariableName, Node* pNode)
{
CCB_MEMBERVARIABLEASSIGNER_GLUE(this,"mArrow", Sprite *, mArrow)
CCB_MEMBERVARIABLEASSIGNER_GLUE(this,"mScroll", Sprite *, mScroll)
CCB_MEMBERVARIABLEASSIGNER_GLUE(this,"mTitle", Sprite *, mTitle)
CCB_MEMBERVARIABLEASSIGNER_GLUE(this,"mScrollCount", Label *, mScrollCount)
CCB_MEMBERVARIABLEASSIGNER_GLUE(this,"mRotateNode", Node *, mRotateNode)
CCB_MEMBERVARIABLEASSIGNER_GLUE(this,"mLightBack", Sprite *, mLightBack)
CCB_MEMBERVARIABLEASSIGNER_GLUE(this,"mRotateACW", Node *, mRotateACW)
CCB_MEMBERVARIABLEASSIGNER_GLUE(this,"mRotateCW", Node *, mRotateCW)
CCB_MEMBERVARIABLEASSIGNER_GLUE(this,"mFlash", LayerColor *, mFlash)
CCB_MEMBERVARIABLEASSIGNER_GLUE(this,"mItemName", Label *, mItemName)

  //  CCLog(pMemberVariableName);

  return false;
}
