//
//  TipsMenu.m
//  NinjiaRush
//
//  Created by 马 颂文 on 12-9-5.
//  Copyright (c) 2012年 TrinGame. All rights reserved.
//

#include "TipsMenu.h"
#include "ExtraSelect.h"
#include "GameData.h"
#include "PublicLoad.h"
#include "GameConstants.h"
#include "UniversalFit.h"

USING_NS_CC_EXT;

bool TipsMenu::init()
{
    if( !Layer::init() )
    {
        return false;
    }
    return true;
}

void TipsMenu::onEnter()
{
  mUISwapper.onEnter();
    PublicLoad::menuExtra()->loadAll();
    cocosbuilder::NodeLoaderLibrary *pNodeLib = cocosbuilder::NodeLoaderLibrary::getInstance();
    cocosbuilder::CCBReader *pReader = new cocosbuilder::CCBReader(pNodeLib, this, this);
cocos2d::Node *mNode = pReader->readNodeGraphFromFile("menu-tips.ccbi", this);
    pReader->release();
    
    this->addChild(mNode);
    
    mCount = NULL;
    mTip = NULL;
    
    mIndex = 0;
    this->updateTip(mIndex);
    
    mUISwapper.setSceneIntro(this);
cocos2d::Layer::onEnter();
}

void TipsMenu::onExit() 
{
cocos2d::Layer::onExit();
}

void TipsMenu::updateTip(int index)
{
    if( mCount == NULL )
    {
        mCount = cocos2d::Label::create(cocos2d::CCString::createWithFormat("%d/%d", index+1, GameData::fetchTips().size())->getCString(), GFONT_NAME, GFONT_SIZE_LARGE);
        //mCount = [Label, CCString::createWithFormat("%d/%d", index+1, GameData::fetchTips)->size()()
        //                            fontName:TIP_FONTNAME fontSize:24];
        mCount->setAnchorPoint(cocos2d::Vec2(0.5f, 0.5f));
        mCount->setPosition(cocos2d::Vec2(240, 50));
        this->addChild(mCount);
    }
    else {
        //mCount->setString(cocos2d::CCString::createWithFormat("%d/%d", index+1, GameData::fetchTips)->size())();
        mCount->setString(cocos2d::CCString::createWithFormat("%d/%d", index+1, GameData::fetchTips().size())->getCString());
    }
    if( mTip != NULL )
    {
        this->removeChild(mTip, true);
    }
    std::string tip =  GameData::fetchTips()[index];
cocos2d::Size size = cocos2d::CCSizeMake(300, 100);
    //CGSize actualSize = tip->sizeWithFont([UIFont, TIP_FONTNAME, 24)
    //                    constrainedToSize:size
    //                        lineBreakMode:UILineBreakModeWordWrap];
    mTip = cocos2d::Label::create(tip.c_str(), GFONT_NAME, GFONT_SIZE_LARGE, size, kCCTextAlignmentCenter);
    mTip->setAnchorPoint(cocos2d::Vec2(0.5f, 0.5f));
    mTip->setPosition(cocos2d::Vec2(240, 160));
    this->addChild(mTip);
    if( index == 0 )
    {
        mLeft->setOpacity(128);
    }
    else {
        mLeft->setOpacity(255);
    }
    if( index == GameData::fetchTips().size()-1 )
    {
        mRight->setOpacity(128);
    }
    else {
        mRight->setOpacity(255);
    }
}

void TipsMenu::onBack() 
{
    if(mUISwapper.isDone())
    {
        this->removeChild(mTip, true);
        this->removeChild(mCount, true);
        GameTool::PlaySound("sound/menu-change.mp3");
        mUISwapper.setSceneOutro(GameTool::scene<ExtraSelect>(), this);
    }
}

void TipsMenu::onLeft() 
{
    if( mIndex > 0 )
    {
        GameTool::PlaySound("sound/click.mp3");
        mIndex--;
        this->updateTip(mIndex);
    }
}

void TipsMenu::onRight() 
{
    if( mIndex < GameData::fetchTips().size() - 1 )
    {
        GameTool::PlaySound("sound/click.mp3");
        mIndex++;
        this->updateTip(mIndex);
    }
}

bool TipsMenu::onAssignCCBMemberVariable(cocos2d::Ref* pTarget, const char* pMemberVariableName, Node* pNode)
{
CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "mLeft", CCMenuItemImage*, mLeft);
CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "mRight", CCMenuItemImage*, mRight);
    return false;
}

SEL_MenuHandler TipsMenu::onResolveCCBCCMenuItemSelector(cocos2d::Ref * pTarget, const char* pSelectorName)
{
//CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "onLeft", TipsMenu::onLeft);
//CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "onRight", TipsMenu::onRight);
//CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "onBack", TipsMenu::onBack);
    return NULL;
}

cocos2d::extension::Control::Handler   TipsMenu::onResolveCCBCCControlSelector(cocos2d::Ref *, const char*)
{
    return NULL;
}


