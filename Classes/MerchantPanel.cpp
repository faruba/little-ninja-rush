//
//  MerchantPanel.m
//  NinjiaRush
//
//  Created by 马 颂文 on 12-7-10.
//  Copyright (c) 2012年 TrinGame. All rights reserved.
//

#include "MerchantPanel.h"
#include "GamePlay.h"
#include "Collections.h"
#include "GameRecord.h"
#include "GameData.h"
#include "UniversalFit.h"

int gCost[] = 
{
    1000,
    1000,
    1000,
    2500,
};



MerchantPanel* MerchantPanel::merchantPanel() 
{
    MerchantPanel *ret = MerchantPanel::create();
    
    return ret;
}

void MerchantPanel::onCreate() 
{
    //hot load
    CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("ui-merchant.plist");
    
    GamePlay *play = GamePlay::sharedGamePlay();
    play->scheduleMask(ccc3(0, 0, 0), 128, 0);
    play->paused = true;
    NodeLoaderLibrary *pNodeLib = NodeLoaderLibrary::sharedNodeLoaderLibrary();
    CCBReader *pReader = new CCBReader(pNodeLib);
    CCNode *node = pReader->readNodeGraphFromFile("merchant", this);
    pReader->release();
    mMerchantbox = node;
    mMerchantbox->setPosition(ccp(UniversalFit::sharedUniversalFit()->baseLeft, -SCREEN_HEIGHT));
    play->addChild(mMerchantbox, LAYER_MASK+1);
    
    //animation set
    CCMoveTo *mt1 = CCMoveTo::create(0.5f, ccp(UniversalFit::sharedUniversalFit()->baseLeft, 0));
    mMerchantbox->runAction(mt1);
    
    CCDelayTime *dt2 = CCDelayTime::create(0.5f);
    CCFadeIn *fi2 = CCFadeIn::create(0.5f);
    CCSequence *sq2 = CCSequence::create(dt2, fi2, NULL);
    mTalkbox->runAction(sq2);
    
    mCoins->setString(CCString::createWithFormat("%d", GameRecord::sharedGameRecord()->coins)->getCString());
    //统计还剩多少片
    int shuriken = 0;
    for(int i=ITEM_SHURIKENSTART; i<ITEM_SHURIKENEND; ++i)
    {
        shuriken += GameRecord::sharedGameRecord()->collection->itemLostPiece(i);
    }
    shuriken += GameRecord::sharedGameRecord()->collection->itemLostPiece(39);
    shuriken += GameRecord::sharedGameRecord()->collection->itemLostPiece(40);
    shuriken += GameRecord::sharedGameRecord()->collection->itemLostPiece(41);
    shuriken += GameRecord::sharedGameRecord()->collection->itemLostPiece(42);
    int katana = 0;
    for(int i=ITEM_KATANASTART; i<ITEM_KATANAEND; ++i)
    {
        katana += GameRecord::sharedGameRecord()->collection->itemLostPiece(i);
    }
    katana += GameRecord::sharedGameRecord()->collection->itemLostPiece(43);
    katana += GameRecord::sharedGameRecord()->collection->itemLostPiece(44);
    katana += GameRecord::sharedGameRecord()->collection->itemLostPiece(45);
    katana += GameRecord::sharedGameRecord()->collection->itemLostPiece(46);
    int special = 0;
    for(int i=ITEM_SPECIALSTART; i<ITEM_SPECIALEND; ++i)
    {
        special += GameRecord::sharedGameRecord()->collection->itemLostPiece(i);
    }
    special += GameRecord::sharedGameRecord()->collection->itemLostPiece(38);
    
    mIC1 = shuriken > 3 ? 3 : shuriken;
    mIC2 = katana > 3 ? 3 : katana;
    mIC3 = special > 3 ? 3 : special;
    this->updateBuy1();
    this->updateBuy2();
    this->updateBuy3();
}

void MerchantPanel::onUpdate(float delta) 
{
}

void MerchantPanel::onDestroy() 
{
    GamePlay *play = GamePlay::sharedGamePlay();
    play->removeChild(mMerchantbox, true);
    play->unscheduleMask();
    
    //hot release
    CCSpriteFrameCache::sharedSpriteFrameCache()->removeSpriteFramesFromFile("ui-merchant.plist");
    unloadTextureFromeSpriteFrameFile("ui-merchant.plist");
}

void MerchantPanel::updateBuy1() 
{
    if( mIC1 > 0 )
    {
        mBuyItem1->setNormalImage(CCSprite::createWithSpriteFrameName("lt_buy.png"));
        mBuyItem1->setSelectedImage(CCSprite::createWithSpriteFrameName("lt_buy.png"));
    }
    else {
        mBuyItem1->setNormalImage(CCSprite::createWithSpriteFrameName("lt_soldout.png"));
        mBuyItem1->setSelectedImage(CCSprite::createWithSpriteFrameName("lt_soldout.png"));
    }
    mItemCount1->setString(CCString::createWithFormat("%d", mIC1)->getCString());
}

void MerchantPanel::updateBuy2() 
{
    if( mIC2 > 0 )
    {
        mBuyItem2->setNormalImage(CCSprite::createWithSpriteFrameName("lt_buy.png"));
        mBuyItem2->setSelectedImage(CCSprite::createWithSpriteFrameName("lt_buy.png"));
    }
    else {
        mBuyItem2->setNormalImage(CCSprite::createWithSpriteFrameName("lt_soldout.png"));
        mBuyItem2->setSelectedImage(CCSprite::createWithSpriteFrameName("lt_soldout.png"));
    }
    mItemCount2->setString(CCString::createWithFormat("%d", mIC2)->getCString());
}

void MerchantPanel::updateBuy3() 
{
    if( mIC3 > 0 )
    {
        mBuyItem3->setNormalImage(CCSprite::createWithSpriteFrameName("lt_buy.png"));
        mBuyItem3->setSelectedImage(CCSprite::createWithSpriteFrameName("lt_buy.png"));
    }
    else {
        mBuyItem3->setNormalImage(CCSprite::createWithSpriteFrameName("lt_soldout.png"));
        mBuyItem3->setSelectedImage(CCSprite::createWithSpriteFrameName("lt_soldout.png"));
    }
    mItemCount3->setString(CCString::createWithFormat("%d", mIC3)->getCString());
}

//商人系统
void MerchantPanel::onMerchantClose() 
{
    SimpleAudioEngine::sharedEngine()->playEffect(CCFileUtils::sharedFileUtils()->fullPathForFilename("sound/click.mp3").c_str());
    GamePlay* play = GamePlay::sharedGamePlay();
    play->state = STATE_NORMAL;
    play->scheduleMask(ccc3(0, 0, 0), 0.2f, 1);
    play->paused = false;
    
    if( randomInt(2) == 0)
    {
        SimpleAudioEngine::sharedEngine()->playEffect(CCFileUtils::sharedFileUtils()->fullPathForFilename("sound/laugh1.mp3").c_str());
    }
    else {
        SimpleAudioEngine::sharedEngine()->playEffect(CCFileUtils::sharedFileUtils()->fullPathForFilename("sound/laugh2.mp3").c_str());
    }
    play->manager->removeGameObject(this);
}

void MerchantPanel::gainShurikenPiece() 
{
    int poss[ITEM_SHURIKENEND - ITEM_SHURIKENSTART];
    int count = 0;
    for( int i=ITEM_SHURIKENSTART; i<ITEM_SHURIKENEND; ++i)
    {
        Shuriken* sh = GameData::queryShuriken(i);
        if( !GameRecord::sharedGameRecord()->collection->isItemCompleted(i) && sh != NULL )
        {
            poss[count] = i;
            count++;
        }
    }
    int res = randomInt(count);
    GameRecord::sharedGameRecord()->collection->gainItemPiece(poss[res]);
    //pop notification
    if( GameRecord::sharedGameRecord()->collection->isItemCompleted(poss[res]) )
    {
        Shuriken* sh = GameData::queryShuriken(poss[res]);
        GamePlay::pushNotification(sh->name, sh->icon, 4);
    }
}

void MerchantPanel::gainKatanaPiece() 
{
    int poss[ITEM_KATANAEND - ITEM_KATANASTART];
    int count = 0;
    for( int i=ITEM_KATANASTART; i<ITEM_KATANAEND; ++i)
    {
        Katana* sh = GameData::queryKatana(i);
        if( !GameRecord::sharedGameRecord()->collection->isItemCompleted(i) && sh != NULL )
        {
            poss[count] = i;
            count++;
        }
    }
    int res = randomInt(count);
    GameRecord::sharedGameRecord()->collection->gainItemPiece(poss[res]);
    //pop notification
    if( GameRecord::sharedGameRecord()->collection->isItemCompleted(poss[res]) )
    {
        Katana* sh = GameData::queryKatana(poss[res]);
        GamePlay::pushNotification(sh->name, sh->icon, 4);
    }
}

void MerchantPanel::gainSpecialPiece() 
{
    int poss[ITEM_SPECIALEND - ITEM_SPECIALSTART];
    int count = 0;
    for( int i=ITEM_SPECIALSTART; i<ITEM_SPECIALEND; ++i)
    {
        Special* sh = GameData::querySpecial(i);
        if( !GameRecord::sharedGameRecord()->collection->isItemCompleted(i) && sh != NULL )
        {
            poss[count] = i;
            count++;
        }
    }
    int res = randomInt(count);
    GameRecord::sharedGameRecord()->collection->gainItemPiece(poss[res]);
    //pop notification
    if( GameRecord::sharedGameRecord()->collection->isItemCompleted(poss[res]) )
    {
        Special* sh = GameData::querySpecial(poss[res]);
        GamePlay::pushNotification(sh->name, sh->icon, 4);
    }
}

void MerchantPanel::onMerchantBuy1() 
{
    if( mIC1 <= 0 )
    {
        return;
    }
    int cost = gCost[0];
    if( GameRecord::sharedGameRecord()->coins >= cost )
    {
        SimpleAudioEngine::sharedEngine()->playEffect(CCFileUtils::sharedFileUtils()->fullPathForFilename("sound/buy.mp3").c_str());
        this->gainShurikenPiece();
        switch (randomInt(3)) {
            case 0:
            {
                mTalkText->setDisplayFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("lt_text2.png"));
            }
                break;
            case 1:
            {
                mTalkText->setDisplayFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("lt_text3.png"));
            }
                break;
            case 2:
            {
                mTalkText->setDisplayFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("lt_text4.png"));
            }
                break;
        }
        mIC1--;
        mItemCount1->setString(CCString::createWithFormat("%d", mIC1)->getCString());
        GameRecord::sharedGameRecord()->makeCoins(-cost);
        GameRecord::sharedGameRecord()->checkPoint();
        mCoins->setString(CCString::createWithFormat("%d", GameRecord::sharedGameRecord()->coins)->getCString());
        
        mItem1->stopAllActions();
        CCFadeOut *fo1 = CCFadeOut::create(0.5f);
        CCFadeIn *fi1 = CCFadeIn::create(0);
        CCSequence *sq = CCSequence::create(fo1, fi1, NULL);
        mItem1->runAction(sq);
        CCScaleTo *st1 = CCScaleTo::create(0.5f, 1.5f);
        CCScaleTo *st2 = CCScaleTo::create(0, 1);
        CCSequence *sq2 = CCSequence::create(st1, st2, NULL);
        mItem1->runAction(sq2);
        
        this->updateBuy1();
    }
    else {
        SimpleAudioEngine::sharedEngine()->playEffect(CCFileUtils::sharedFileUtils()->fullPathForFilename("sound/error.mp3").c_str());
    }
}

void MerchantPanel::onMerchantBuy2() 
{
    if( mIC2 <= 0 )
    {
        return;
    }
     int cost = gCost[1];
     if( GameRecord::sharedGameRecord()->coins >= cost )
     {
         SimpleAudioEngine::sharedEngine()->playEffect(CCFileUtils::sharedFileUtils()->fullPathForFilename("sound/buy.mp3").c_str());
         this->gainKatanaPiece();
         switch (randomInt(3)) {
             case 0:
             {
                 mTalkText->setDisplayFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("lt_text2.png"));
             }
                 break;
             case 1:
             {
                 mTalkText->setDisplayFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("lt_text3.png"));
             }
                 break;
             case 2:
             {
                 mTalkText->setDisplayFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("lt_text4.png"));
             }
                 break;
         }
         mIC2--;
         mItemCount2->setString(CCString::createWithFormat("%d", mIC2)->getCString());
         GameRecord::sharedGameRecord()->makeCoins(-cost);
         GameRecord::sharedGameRecord()->checkPoint();
         mCoins->setString(CCString::createWithFormat("%d", GameRecord::sharedGameRecord()->coins)->getCString());
         
         mItem2->stopAllActions();
         CCFadeOut *fo1 = CCFadeOut::create(0.5f);
         CCFadeIn *fi1 = CCFadeIn::create(0);
         CCSequence *sq = CCSequence::create(fo1, fi1, NULL);
         mItem2->runAction(sq);
         CCScaleTo *st1 = CCScaleTo::create(0.5f, 1.5f);
         CCScaleTo *st2 = CCScaleTo::create(0, 1);
         CCSequence *sq2 = CCSequence::create(st1, st2, NULL);
         mItem2->runAction(sq2);
         
         this->updateBuy2();
     }
     else {
         SimpleAudioEngine::sharedEngine()->playEffect(CCFileUtils::sharedFileUtils()->fullPathForFilename("sound/error.mp3").c_str());
     }
}

void MerchantPanel::onMerchantBuy3() 
{
    if( mIC3 <= 0 )
    {
        return;
    }
    int cost = gCost[2];
    if( GameRecord::sharedGameRecord()->coins >= cost )
    {
        SimpleAudioEngine::sharedEngine()->playEffect(CCFileUtils::sharedFileUtils()->fullPathForFilename("sound/buy.mp3").c_str());
        this->gainSpecialPiece();
        switch (randomInt(3)) {
            case 0:
            {
                mTalkText->setDisplayFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("lt_text2.png"));
            }
                break;
            case 1:
            {
                mTalkText->setDisplayFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("lt_text3.png"));
            }
                break;
            case 2:
            {
                mTalkText->setDisplayFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("lt_text4.png"));
            }
                break;
        }
        mIC3--;
        mItemCount3->setString(CCString::createWithFormat("%d", mIC3)->getCString());
        GameRecord::sharedGameRecord()->makeCoins(-cost);
        GameRecord::sharedGameRecord()->checkPoint();
        mCoins->setString(CCString::createWithFormat("%d", GameRecord::sharedGameRecord()->coins)->getCString());
        
        mItem3->stopAllActions();
        CCFadeOut *fo1 = CCFadeOut::create(0.5f);
        CCFadeIn *fi1 = CCFadeIn::create(0);
        CCSequence *sq = CCSequence::create(fo1, fi1, NULL);
        mItem3->runAction(sq);
        CCScaleTo *st1 = CCScaleTo::create(0.5f, 1.5f);
        CCScaleTo *st2 = CCScaleTo::create(0, 1);
        CCSequence *sq2 = CCSequence::create(st1, st2, NULL);
        mItem3->runAction(sq2);
        
        this->updateBuy3();
    }
    else {
        SimpleAudioEngine::sharedEngine()->playEffect(CCFileUtils::sharedFileUtils()->fullPathForFilename("sound/error.mp3").c_str());
    }
}

void MerchantPanel::onMerchantBuy4() 
{
    int cost = gCost[3];
    if( GameRecord::sharedGameRecord()->coins >= cost )
    {
        SimpleAudioEngine::sharedEngine()->playEffect(CCFileUtils::sharedFileUtils()->fullPathForFilename("sound/buy.mp3").c_str());
        GameRecord::sharedGameRecord()->collection->magic_piece++;
        switch (randomInt(3)) {
            case 0:
            {
                mTalkText->setDisplayFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("lt_text2.png"));
            }
                break;
            case 1:
            {
                mTalkText->setDisplayFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("lt_text3.png"));
            }
                break;
            case 2:
            {
                mTalkText->setDisplayFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("lt_text4.png"));
            }
                break;
        }
        GameRecord::sharedGameRecord()->makeCoins(-cost);
        GameRecord::sharedGameRecord()->checkPoint();
        mCoins->setString(CCString::createWithFormat("%d", GameRecord::sharedGameRecord()->coins)->getCString());
        
        mItem4->stopAllActions();
        CCFadeOut *fo1 = CCFadeOut::create(0.5f);
        CCFadeIn *fi1 = CCFadeIn::create(0);
        CCSequence *sq = CCSequence::create(fo1, fi1, NULL);
        mItem4->runAction(sq);
        CCScaleTo *st1 = CCScaleTo::create(0.5f, 1.5f);
        CCScaleTo *st2 = CCScaleTo::create(0, 1);
        CCSequence *sq2 = CCSequence::create(st1, st2, NULL);
        mItem4->runAction(sq2);
    }
    else {
        SimpleAudioEngine::sharedEngine()->playEffect(CCFileUtils::sharedFileUtils()->fullPathForFilename("sound/error.mp3").c_str());
    }
}


