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
cocos2d::SpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("ui-merchant.plist");
    
    GamePlay *play = GamePlay::sharedGamePlay();
    play->scheduleMask(Color3B(0, 0, 0), 128, 0);
    play->paused = true;
    NodeLoaderLibrary *pNodeLib = NodeLoaderLibrary::sharedNodeLoaderLibrary();
CCBReader *pReader = new CCBReader(pNodeLib);
cocos2d::Node *node = pReader->readNodeGraphFromFile("merchant", this);
    pReader->release();
    mMerchantbox = node;
    mMerchantbox->setPosition(cocos2d::Vec2(UniversalFit::sharedUniversalFit()->baseLeft, -SCREEN_HEIGHT));
    play->addChild(mMerchantbox, LAYER_MASK+1);
    
    //animation set
cocos2d::CCMoveTo *mt1 = cocos2d::CCMoveTo::create(0.5f, Vec2(UniversalFit::sharedUniversalFit()->baseLeft, 0));
    mMerchantbox->runAction(mt1);
cocos2d::DelayTime *dt2 = cocos2d::DelayTime::create(0.5f);
cocos2d::FadeIn *fi2 = cocos2d::FadeIn::create(0.5f);
cocos2d::Sequence *sq2 = cocos2d::Sequence::create(dt2, fi2, NULL);
    mTalkbox->runAction(sq2);
    
    mCoins->setString(cocos2d::CCString::createWithFormat("%d", GameRecord::sharedGameRecord()->coins)->getCString());
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
cocos2d::SpriteFrameCache::sharedSpriteFrameCache()->removeSpriteFramesFromFile("ui-merchant.plist");
    unloadTextureFromeSpriteFrameFile("ui-merchant.plist");
}

void MerchantPanel::updateBuy1() 
{
    if( mIC1 > 0 )
    {
        mBuyItem1->setNormalImage(cocos2d::Sprite::createWithSpriteFrameName("lt_buy.png"));
        mBuyItem1->setSelectedImage(cocos2d::Sprite::createWithSpriteFrameName("lt_buy.png"));
    }
    else {
        mBuyItem1->setNormalImage(cocos2d::Sprite::createWithSpriteFrameName("lt_soldout.png"));
        mBuyItem1->setSelectedImage(cocos2d::Sprite::createWithSpriteFrameName("lt_soldout.png"));
    }
    mItemCount1->setString(cocos2d::CCString::createWithFormat("%d", mIC1)->getCString());
}

void MerchantPanel::updateBuy2() 
{
    if( mIC2 > 0 )
    {
        mBuyItem2->setNormalImage(cocos2d::Sprite::createWithSpriteFrameName("lt_buy.png"));
        mBuyItem2->setSelectedImage(cocos2d::Sprite::createWithSpriteFrameName("lt_buy.png"));
    }
    else {
        mBuyItem2->setNormalImage(cocos2d::Sprite::createWithSpriteFrameName("lt_soldout.png"));
        mBuyItem2->setSelectedImage(cocos2d::Sprite::createWithSpriteFrameName("lt_soldout.png"));
    }
    mItemCount2->setString(cocos2d::CCString::createWithFormat("%d", mIC2)->getCString());
}

void MerchantPanel::updateBuy3() 
{
    if( mIC3 > 0 )
    {
        mBuyItem3->setNormalImage(cocos2d::Sprite::createWithSpriteFrameName("lt_buy.png"));
        mBuyItem3->setSelectedImage(cocos2d::Sprite::createWithSpriteFrameName("lt_buy.png"));
    }
    else {
        mBuyItem3->setNormalImage(cocos2d::Sprite::createWithSpriteFrameName("lt_soldout.png"));
        mBuyItem3->setSelectedImage(cocos2d::Sprite::createWithSpriteFrameName("lt_soldout.png"));
    }
    mItemCount3->setString(cocos2d::CCString::createWithFormat("%d", mIC3)->getCString());
}

//商人系统
void MerchantPanel::onMerchantClose() 
{
    GameTool::PlaySound("sound/click.mp3");
    GamePlay* play = GamePlay::sharedGamePlay();
    play->state = STATE_NORMAL;
    play->scheduleMask(Color3B(0, 0, 0), 0.2f, 1);
    play->paused = false;
    
    if( randomInt(2) == 0)
    {
        GameTool::PlaySound("sound/laugh1.mp3");
    }
    else {
        GameTool::PlaySound("sound/laugh2.mp3");
    }
    play->manager->removeGameObject(this);
}

void MerchantPanel::gainShurikenPiece() 
{/*
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
        GamePlay::sharedGamePlay()->pushNotification(sh->name, sh->icon, 4);
    }*/
}

void MerchantPanel::gainKatanaPiece() 
{/*
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
        GamePlay::sharedGamePlay()->pushNotification(sh->name, sh->icon, 4);
    }*/
}

void MerchantPanel::gainSpecialPiece() 
{/*
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
        GamePlay::sharedGamePlay()->pushNotification(sh->name, sh->icon, 4);
    }*/
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
        GameTool::PlaySound("sound/buy.mp3");
        this->gainShurikenPiece();
        switch (randomInt(3)) {
            case 0:
            {
                mTalkText->setDisplayFrame(GameTool::getSpriteFrameByName("lt_text2.png"));
            }
                break;
            case 1:
            {
                mTalkText->setDisplayFrame(GameTool::getSpriteFrameByName("lt_text3.png"));
            }
                break;
            case 2:
            {
                mTalkText->setDisplayFrame(GameTool::getSpriteFrameByName("lt_text4.png"));
            }
                break;
        }
        mIC1--;
        mItemCount1->setString(cocos2d::CCString::createWithFormat("%d", mIC1)->getCString());
        GameRecord::sharedGameRecord()->makeCoins(-cost);
        GameRecord::sharedGameRecord()->checkPoint();
        mCoins->setString(cocos2d::CCString::createWithFormat("%d", GameRecord::sharedGameRecord()->coins)->getCString());
        
        mItem1->stopAllActions();
cocos2d::FadeOut *fo1 = cocos2d::FadeOut::create(0.5f);
cocos2d::FadeIn *fi1 = cocos2d::FadeIn::create(0);
cocos2d::Sequence *sq = cocos2d::Sequence::create(fo1, fi1, NULL);
        mItem1->runAction(sq);
cocos2d::ScaleTo *st1 = cocos2d::ScaleTo::create(0.5f, 1.5f);
cocos2d::ScaleTo *st2 = cocos2d::ScaleTo::create(0, 1);
cocos2d::Sequence *sq2 = cocos2d::Sequence::create(st1, st2, NULL);
        mItem1->runAction(sq2);
        
        this->updateBuy1();
    }
    else {
        GameTool::PlaySound("sound/error.mp3");
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
         GameTool::PlaySound("sound/buy.mp3");
         this->gainKatanaPiece();
         switch (randomInt(3)) {
             case 0:
             {
                 mTalkText->setDisplayFrame(GameTool::getSpriteFrameByName("lt_text2.png"));
             }
                 break;
             case 1:
             {
                 mTalkText->setDisplayFrame(GameTool::getSpriteFrameByName("lt_text3.png"));
             }
                 break;
             case 2:
             {
                 mTalkText->setDisplayFrame(GameTool::getSpriteFrameByName("lt_text4.png"));
             }
                 break;
         }
         mIC2--;
         mItemCount2->setString(cocos2d::CCString::createWithFormat("%d", mIC2)->getCString());
         GameRecord::sharedGameRecord()->makeCoins(-cost);
         GameRecord::sharedGameRecord()->checkPoint();
         mCoins->setString(cocos2d::CCString::createWithFormat("%d", GameRecord::sharedGameRecord()->coins)->getCString());
         
         mItem2->stopAllActions();
cocos2d::FadeOut *fo1 = cocos2d::FadeOut::create(0.5f);
cocos2d::FadeIn *fi1 = cocos2d::FadeIn::create(0);
cocos2d::Sequence *sq = cocos2d::Sequence::create(fo1, fi1, NULL);
         mItem2->runAction(sq);
cocos2d::ScaleTo *st1 = cocos2d::ScaleTo::create(0.5f, 1.5f);
cocos2d::ScaleTo *st2 = cocos2d::ScaleTo::create(0, 1);
cocos2d::Sequence *sq2 = cocos2d::Sequence::create(st1, st2, NULL);
         mItem2->runAction(sq2);
         
         this->updateBuy2();
     }
     else {
         GameTool::PlaySound("sound/error.mp3");
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
        GameTool::PlaySound("sound/buy.mp3");
        this->gainSpecialPiece();
        switch (randomInt(3)) {
            case 0:
            {
                mTalkText->setDisplayFrame(GameTool::getSpriteFrameByName("lt_text2.png"));
            }
                break;
            case 1:
            {
                mTalkText->setDisplayFrame(GameTool::getSpriteFrameByName("lt_text3.png"));
            }
                break;
            case 2:
            {
                mTalkText->setDisplayFrame(GameTool::getSpriteFrameByName("lt_text4.png"));
            }
                break;
        }
        mIC3--;
        mItemCount3->setString(cocos2d::CCString::createWithFormat("%d", mIC3)->getCString());
        GameRecord::sharedGameRecord()->makeCoins(-cost);
        GameRecord::sharedGameRecord()->checkPoint();
        mCoins->setString(cocos2d::CCString::createWithFormat("%d", GameRecord::sharedGameRecord()->coins)->getCString());
        
        mItem3->stopAllActions();
cocos2d::FadeOut *fo1 = cocos2d::FadeOut::create(0.5f);
cocos2d::FadeIn *fi1 = cocos2d::FadeIn::create(0);
cocos2d::Sequence *sq = cocos2d::Sequence::create(fo1, fi1, NULL);
        mItem3->runAction(sq);
cocos2d::ScaleTo *st1 = cocos2d::ScaleTo::create(0.5f, 1.5f);
cocos2d::ScaleTo *st2 = cocos2d::ScaleTo::create(0, 1);
cocos2d::Sequence *sq2 = cocos2d::Sequence::create(st1, st2, NULL);
        mItem3->runAction(sq2);
        
        this->updateBuy3();
    }
    else {
        GameTool::PlaySound("sound/error.mp3");
    }
}

void MerchantPanel::onMerchantBuy4() 
{
    int cost = gCost[3];
    if( GameRecord::sharedGameRecord()->coins >= cost )
    {
        GameTool::PlaySound("sound/buy.mp3");
        GameRecord::sharedGameRecord()->collection->magic_piece++;
        switch (randomInt(3)) {
            case 0:
            {
                mTalkText->setDisplayFrame(GameTool::getSpriteFrameByName("lt_text2.png"));
            }
                break;
            case 1:
            {
                mTalkText->setDisplayFrame(GameTool::getSpriteFrameByName("lt_text3.png"));
            }
                break;
            case 2:
            {
                mTalkText->setDisplayFrame(GameTool::getSpriteFrameByName("lt_text4.png"));
            }
                break;
        }
        GameRecord::sharedGameRecord()->makeCoins(-cost);
        GameRecord::sharedGameRecord()->checkPoint();
        mCoins->setString(cocos2d::CCString::createWithFormat("%d", GameRecord::sharedGameRecord()->coins)->getCString());
        
        mItem4->stopAllActions();
cocos2d::FadeOut *fo1 = cocos2d::FadeOut::create(0.5f);
cocos2d::FadeIn *fi1 = cocos2d::FadeIn::create(0);
cocos2d::Sequence *sq = cocos2d::Sequence::create(fo1, fi1, NULL);
        mItem4->runAction(sq);
cocos2d::ScaleTo *st1 = cocos2d::ScaleTo::create(0.5f, 1.5f);
cocos2d::ScaleTo *st2 = cocos2d::ScaleTo::create(0, 1);
cocos2d::Sequence *sq2 = cocos2d::Sequence::create(st1, st2, NULL);
        mItem4->runAction(sq2);
    }
    else {
        GameTool::PlaySound("sound/error.mp3");
    }
}


