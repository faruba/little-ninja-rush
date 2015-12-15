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

USING_NS_CC;
USING_NS_CC_EXT;

//CCScene* TipsMenu::scene() 
//{
//    CCScene *ret = CCScene::create();
//    
//    TipsMenu *tm = TipsMenu::create();
//    
//    //universal
//    ret->setScale(UniversalFit::sharedUniversalFit()->scaleFactor);
//    ret->setAnchorPoint(ccp(0, 0));
//    ret->setPosition(
//    
//    CEClipedNode *clip = CEClipedNode::create();
//    clip->setClipRect(UniversalFit::sharedUniversalFit()->clipRect);
//    
//    clip->addChild(tm);
//    
//    ret->addChild(clip);
//    return ret;
//}

bool TipsMenu::init()
{
    if( !CCLayer::init() )
    {
        return false;
    }
    return true;
}

void TipsMenu::onEnter()
{
    PublicLoad::menuExtra()->loadAll();
    
    CCNodeLoaderLibrary *pNodeLib = CCNodeLoaderLibrary::sharedCCNodeLoaderLibrary();
    CCBReader *pReader = new CCBReader(pNodeLib, this, this);
    CCNode *mNode = pReader->readNodeGraphFromFile("menu-tips.ccbi", this);
    pReader->release();
    
    this->addChild(mNode);
    
    mCount = NULL;
    mTip = NULL;
    
    mIndex = 0;
    this->updateTip(mIndex);
    
    this->setSceneIntro();
    
    CCLayer::onEnter();
}

void TipsMenu::onExit() 
{
    CCLayer::onExit();
}

void TipsMenu::updateTip(int index)
{
    if( mCount == NULL )
    {
        mCount = CCLabelTTF::create(CCString::createWithFormat("%d/%d", index+1, GameData::fetchTips()->count())->getCString(), GFONT_NAME, GFONT_SIZE_LARGE);
        //mCount = [CCLabelTTF, CCString::createWithFormat("%d/%d", index+1, GameData::fetchTips)->count()()
        //                            fontName:TIP_FONTNAME fontSize:24];
        mCount->setAnchorPoint(ccp(0.5f, 0.5f));
        mCount->setPosition(ccp(240, 50));
        this->addChild(mCount);
    }
    else {
        //mCount->setString(CCString::createWithFormat("%d/%d", index+1, GameData::fetchTips)->count())();
        mCount->setString(CCString::createWithFormat("%d/%d", index+1, GameData::fetchTips()->count())->getCString());
    }
    if( mTip != NULL )
    {
        this->removeChild(mTip, true);
    }
    CCString *tip = (CCString*) GameData::fetchTips()->objectAtIndex(index);
    CCSize size = CCSizeMake(300, 100);
    //CGSize actualSize = tip->sizeWithFont([UIFont, TIP_FONTNAME, 24)
    //                    constrainedToSize:size
    //                        lineBreakMode:UILineBreakModeWordWrap];
    mTip = CCLabelTTF::create(tip->getCString(), GFONT_NAME, GFONT_SIZE_LARGE, size, kCCTextAlignmentCenter);
    mTip->setAnchorPoint(ccp(0.5f, 0.5f));
    mTip->setPosition(ccp(240, 160));
    this->addChild(mTip);
    if( index == 0 )
    {
        mLeft->setOpacity(128);
    }
    else {
        mLeft->setOpacity(255);
    }
    if( index == GameData::fetchTips()->count()-1 )
    {
        mRight->setOpacity(128);
    }
    else {
        mRight->setOpacity(255);
    }
}

void TipsMenu::onBack() 
{
    if( !mIntroFlag )
    {
        this->removeChild(mTip, true);
        this->removeChild(mCount, true);
        SimpleAudioEngine::sharedEngine()->playEffect(CCFileUtils::sharedFileUtils()->fullPathForFilename("sound/menu-change.mp3").c_str());
        setSceneOutro(ExtraSelect::scene());
    }
}

void TipsMenu::onLeft() 
{
    if( mIndex > 0 )
    {
        SimpleAudioEngine::sharedEngine()->playEffect(CCFileUtils::sharedFileUtils()->fullPathForFilename("sound/click.mp3").c_str());
        mIndex--;
        this->updateTip(mIndex);
    }
}

void TipsMenu::onRight() 
{
    if( mIndex < GameData::fetchTips()->count() - 1 )
    {
        SimpleAudioEngine::sharedEngine()->playEffect(CCFileUtils::sharedFileUtils()->fullPathForFilename("sound/click.mp3").c_str());
        mIndex++;
        this->updateTip(mIndex);
    }
}

void TipsMenu::setSceneIntro() 
{
    if( mSceneIntro == NULL )
    {
        mSceneIntro = CCNode::create();
        this->addChild(mSceneIntro);
        CCSprite *left = CCSprite::create("door.png");
        left->setAnchorPoint(ccp(1, 0));
        mSceneIntro->addChild(left, 0, 0);
        CCSprite *right = CCSprite::create("door.png");
        right->setScaleX(-1);
        right->setAnchorPoint(ccp(1, 0));
        mSceneIntro->addChild(right, 0, 1);
    }
    CCSprite *left = (CCSprite*)(mSceneIntro->getChildByTag(0));
    left->setVisible(true);
    left->setPosition(ccp(SCREEN_WIDTH/2, 0));
    CCSprite *right = (CCSprite*)(mSceneIntro->getChildByTag(1));
    right->setVisible(true);
    right->setPosition(ccp(SCREEN_WIDTH/2, 0));
    
    CCDelayTime *dt1 = CCDelayTime::create(SCENEINTRO_DELAY);
    CCMoveBy *mb1 = CCMoveBy::create(SCENEINTRO_TIME, ccp(-SCREEN_WIDTH/2, 0));
    //CCHide *hd1 = CCHide->action();
    CCSequence *sq1 = CCSequence::create(dt1, mb1, NULL);
    left->runAction(sq1);
    CCDelayTime *dt2 = CCDelayTime::create(SCENEINTRO_DELAY);
    CCMoveBy *mb2 = CCMoveBy::create(SCENEINTRO_TIME, ccp(SCREEN_WIDTH/2, 0));
    //CCHide *hd2 = CCHide->action();
    CCSequence *sq2 = CCSequence::create(dt2, mb2, NULL);
    right->runAction(sq2);
    
    SimpleAudioEngine::sharedEngine()->playEffect(CCFileUtils::sharedFileUtils()->fullPathForFilename("sound/open.mp3").c_str());
}

void TipsMenu::setSceneOutro(CCScene* newscene) 
{
    if( mIntroFlag )
    {
        return;
    }
    mIntroFlag = true;
    
    mNewScene = newscene;
    mNewScene->retain();
    if( mSceneIntro == NULL )
    {
        mSceneIntro = CCNode::create();
        this->addChild(mSceneIntro);
        CCSprite *left = CCSprite::create("door.png");
        left->setAnchorPoint(ccp(1, 0));
        mSceneIntro->addChild(left, 0, 0);
        CCSprite *right = CCSprite::create("door.png");
        right->setScaleX(-1);
        right->setAnchorPoint(ccp(1, 0));
        mSceneIntro->addChild(right, 0, 1);
    }
    CCSprite *left = (CCSprite*)(mSceneIntro->getChildByTag(0));
    left->setVisible(true);
    left->setPosition(ccp(0, 0));
    CCSprite *right = (CCSprite*)(mSceneIntro->getChildByTag(1));
    right->setVisible(true);
    right->setPosition(ccp(SCREEN_WIDTH, 0));
    
    CCMoveBy *mb1 = CCMoveBy::create(SCENEOUTRO_TIME, ccp(SCREEN_WIDTH/2, 0));
    CCDelayTime *dt1 = CCDelayTime::create(SCENEOUTRO_DELAY);
    CCCallFunc *ca1 = CCCallFunc::create(this, callfunc_selector(TipsMenu::doneOutro));
    CCSequence *sq1 = CCSequence::create(mb1, dt1, ca1, NULL);
    left->runAction(sq1);
    
    CCMoveBy *mb2 = CCMoveBy::create(SCENEOUTRO_TIME, ccp(-SCREEN_WIDTH/2, 0));
    CCDelayTime *dt2 = CCDelayTime::create(SCENEOUTRO_DELAY);
    CCSequence *sq2 = CCSequence::create(mb2, dt2, NULL);
    right->runAction(sq2);
    
    SimpleAudioEngine::sharedEngine()->playEffect(CCFileUtils::sharedFileUtils()->fullPathForFilename("sound/close.mp3").c_str());
}

void TipsMenu::doneOutro() 
{
    mIntroFlag = false;
    CCDirector::sharedDirector()->replaceScene(mNewScene);
    mNewScene->release();
}

bool TipsMenu::onAssignCCBMemberVariable(CCObject* pTarget, const char* pMemberVariableName, CCNode* pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "mLeft", CCMenuItemImage*, mLeft);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "mRight", CCMenuItemImage*, mRight);
    return false;
}

SEL_MenuHandler TipsMenu::onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char* pSelectorName)
{
    CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "onLeft", TipsMenu::onLeft);
    CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "onRight", TipsMenu::onRight);
    CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "onBack", TipsMenu::onBack);
    return NULL;
}

SEL_CCControlHandler TipsMenu::onResolveCCBCCControlSelector(CCObject *, const char*)
{
    return NULL;
}


