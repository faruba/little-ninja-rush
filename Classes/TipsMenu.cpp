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

;
USING_NS_CC_EXT;

//Scene* TipsMenu::scene() 
//{
//    Scene *ret = cocos2d::Scene::create();
//    
//    TipsMenu *tm = TipsMenu::create();
//    
//    //universal
//    ret->setScale(UniversalFit::sharedUniversalFit()->scaleFactor);
//    ret->setAnchorPoint(cocos2d::ccp(0, 0));
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
    if( !Layer::init() )
    {
        return false;
    }
    return true;
}

void TipsMenu::onEnter()
{
    PublicLoad::menuExtra()->loadAll();
    cocosbuilder::NodeLoaderLibrary *pNodeLib = cocosbuilder::NodeLoaderLibrary::getInstance();
    cocosbuilder::CCBReader *pReader = new cocosbuilder::CCBReader(pNodeLib, this, this);
cocos2d::CCNode *mNode = pReader->readNodeGraphFromFile("menu-tips.ccbi", this);
    pReader->release();
    
    this->addChild(mNode);
    
    mCount = NULL;
    mTip = NULL;
    
    mIndex = 0;
    this->updateTip(mIndex);
    
    this->setSceneIntro();
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
        mCount = cocos2d::CCLabelTTF::create(cocos2d::CCString::createWithFormat("%d/%d", index+1, GameData::fetchTips()->count())->getCString(), GFONT_NAME, GFONT_SIZE_LARGE);
        //mCount = [CCLabelTTF, CCString::createWithFormat("%d/%d", index+1, GameData::fetchTips)->count()()
        //                            fontName:TIP_FONTNAME fontSize:24];
        mCount->setAnchorPoint(cocos2d::ccp(0.5f, 0.5f));
        mCount->setPosition(cocos2d::ccp(240, 50));
        this->addChild(mCount);
    }
    else {
        //mCount->setString(cocos2d::CCString::createWithFormat("%d/%d", index+1, GameData::fetchTips)->count())();
        mCount->setString(cocos2d::CCString::createWithFormat("%d/%d", index+1, GameData::fetchTips()->count())->getCString());
    }
    if( mTip != NULL )
    {
        this->removeChild(mTip, true);
    }
cocos2d::CCString *tip = (cocos2d::CCString*) GameData::fetchTips()->objectAtIndex(index);
cocos2d::CCSize size = cocos2d::CCSizeMake(300, 100);
    //CGSize actualSize = tip->sizeWithFont([UIFont, TIP_FONTNAME, 24)
    //                    constrainedToSize:size
    //                        lineBreakMode:UILineBreakModeWordWrap];
    mTip = cocos2d::CCLabelTTF::create(tip->getCString(), GFONT_NAME, GFONT_SIZE_LARGE, size, kCCTextAlignmentCenter);
    mTip->setAnchorPoint(cocos2d::ccp(0.5f, 0.5f));
    mTip->setPosition(cocos2d::ccp(240, 160));
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
        SimpleAudioEngine::sharedEngine()->playEffect(cocos2d::CCFileUtils::sharedFileUtils()->fullPathForFilename("sound/menu-change.mp3").c_str());
        setSceneOutro(ExtraSelect::scene());
    }
}

void TipsMenu::onLeft() 
{
    if( mIndex > 0 )
    {
        SimpleAudioEngine::sharedEngine()->playEffect(cocos2d::CCFileUtils::sharedFileUtils()->fullPathForFilename("sound/click.mp3").c_str());
        mIndex--;
        this->updateTip(mIndex);
    }
}

void TipsMenu::onRight() 
{
    if( mIndex < GameData::fetchTips()->count() - 1 )
    {
        SimpleAudioEngine::sharedEngine()->playEffect(cocos2d::CCFileUtils::sharedFileUtils()->fullPathForFilename("sound/click.mp3").c_str());
        mIndex++;
        this->updateTip(mIndex);
    }
}

void TipsMenu::setSceneIntro() 
{
    if( mSceneIntro == NULL )
    {
        mSceneIntro = cocos2d::CCNode::create();
        this->addChild(mSceneIntro);
cocos2d::Sprite *left = cocos2d::Sprite::create("door.png");
        left->setAnchorPoint(cocos2d::ccp(1, 0));
        mSceneIntro->addChild(left, 0, 0);
cocos2d::Sprite *right = cocos2d::Sprite::create("door.png");
        right->setScaleX(-1);
        right->setAnchorPoint(cocos2d::ccp(1, 0));
        mSceneIntro->addChild(right, 0, 1);
    }
cocos2d::Sprite *left = (cocos2d::Sprite*)(mSceneIntro->getChildByTag(0));
    left->setVisible(true);
    left->setPosition(cocos2d::ccp(SCREEN_WIDTH/2, 0));
cocos2d::Sprite *right = (cocos2d::Sprite*)(mSceneIntro->getChildByTag(1));
    right->setVisible(true);
    right->setPosition(cocos2d::ccp(SCREEN_WIDTH/2, 0));
cocos2d::CCDelayTime *dt1 = cocos2d::CCDelayTime::create(SCENEINTRO_DELAY);
cocos2d::CCMoveBy *mb1 = cocos2d::CCMoveBy::create(SCENEINTRO_TIME, ccp(-SCREEN_WIDTH/2, 0));
    //CCHide *hd1 = cocos2d::CCHide->action();
cocos2d::CCSequence *sq1 = cocos2d::CCSequence::create(dt1, mb1, NULL);
    left->runAction(sq1);
cocos2d::CCDelayTime *dt2 = cocos2d::CCDelayTime::create(SCENEINTRO_DELAY);
cocos2d::CCMoveBy *mb2 = cocos2d::CCMoveBy::create(SCENEINTRO_TIME, ccp(SCREEN_WIDTH/2, 0));
    //CCHide *hd2 = cocos2d::CCHide->action();
cocos2d::CCSequence *sq2 = cocos2d::CCSequence::create(dt2, mb2, NULL);
    right->runAction(sq2);
    
    SimpleAudioEngine::sharedEngine()->playEffect(cocos2d::CCFileUtils::sharedFileUtils()->fullPathForFilename("sound/open.mp3").c_str());
}

void TipsMenu::setSceneOutro(cocos2d::Scene* newscene) 
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
        mSceneIntro = cocos2d::CCNode::create();
        this->addChild(mSceneIntro);
cocos2d::Sprite *left = cocos2d::Sprite::create("door.png");
        left->setAnchorPoint(cocos2d::ccp(1, 0));
        mSceneIntro->addChild(left, 0, 0);
cocos2d::Sprite *right = cocos2d::Sprite::create("door.png");
        right->setScaleX(-1);
        right->setAnchorPoint(cocos2d::ccp(1, 0));
        mSceneIntro->addChild(right, 0, 1);
    }
cocos2d::Sprite *left = (cocos2d::Sprite*)(mSceneIntro->getChildByTag(0));
    left->setVisible(true);
    left->setPosition(cocos2d::ccp(0, 0));
cocos2d::Sprite *right = (cocos2d::Sprite*)(mSceneIntro->getChildByTag(1));
    right->setVisible(true);
    right->setPosition(cocos2d::ccp(SCREEN_WIDTH, 0));
cocos2d::CCMoveBy *mb1 = cocos2d::CCMoveBy::create(SCENEOUTRO_TIME, ccp(SCREEN_WIDTH/2, 0));
cocos2d::CCDelayTime *dt1 = cocos2d::CCDelayTime::create(SCENEOUTRO_DELAY);
cocos2d::CCCallFunc *ca1 = cocos2d::CCCallFunc::create(this, callfunc_selector(TipsMenu::doneOutro));
cocos2d::CCSequence *sq1 = cocos2d::CCSequence::create(mb1, dt1, ca1, NULL);
    left->runAction(sq1);
cocos2d::CCMoveBy *mb2 = cocos2d::CCMoveBy::create(SCENEOUTRO_TIME, ccp(-SCREEN_WIDTH/2, 0));
cocos2d::CCDelayTime *dt2 = cocos2d::CCDelayTime::create(SCENEOUTRO_DELAY);
cocos2d::CCSequence *sq2 = cocos2d::CCSequence::create(mb2, dt2, NULL);
    right->runAction(sq2);
    
    SimpleAudioEngine::sharedEngine()->playEffect(cocos2d::CCFileUtils::sharedFileUtils()->fullPathForFilename("sound/close.mp3").c_str());
}

void TipsMenu::doneOutro() 
{
    mIntroFlag = false;
cocos2d::CCDirector::sharedDirector()->replaceScene(mNewScene);
    mNewScene->release();
}

bool TipsMenu::onAssignCCBMemberVariable(cocos2d::Ref* pTarget, const char* pMemberVariableName, CCNode* pNode)
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


