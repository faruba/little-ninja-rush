#include "AppDelegate.h"
#include "TitleMenu.h"
#include "SimpleAudioEngine.h"
#include "GameConstants.h"
#include "SelectMenu.h"
#include "CEClipedNode.h"
#include "CollectionMenu.h"
//#include "Reachability.h"
#include "GameRecord.h"
#include "ShopMenu.h"
#include "ExtraSelect.h"
//#include "GameCenterController.h"
#include "GamePlay.h"
#include "PublicLoad.h"
#include "GameTool.h"
#include "Tasks.h"
#include "UniversalFit.h"
//#include "ABSystem.h"
//#include "ABMoreGame.h"
//#include "FriendList.h"
//#include "ABDelivery.h"

//demo
#include "GamePlay.h"
#include "Loading.h"

;
USING_NS_CC_EXT;
using namespace CocosDenshion;
bool gIntroFlag = false;
bool gPopFlag = false;

//Title Style
//#define TITLESTYLE_HALLOWEEN
#define TITLESTYLE_SPRING


class PopOption :
public CCNode/*<UIAlertViewDelegate>*/,
public CCBMemberVariableAssigner,
public CCBSelectorResolver
{
public:
    //-- auto assign --
cocos2d::CCMenuItemImage *mMusic;
cocos2d::CCMenuItemImage *mSfx;
cocos2d::CCMenuItemImage *mPushNotification;
cocos2d::CCMenuItemImage *mTutorial;
cocos2d::CCMenuItemImage *miCloud;
    
    //---- friends
cocos2d::CCLabelBMFont *mCoin;
cocos2d::CCNode *mShadeCode;

    TitleMenu *master;

    void onCloseOption(cocos2d::Ref*);
    void onToggleMusic(cocos2d::Ref*);
    void onToggleSfx(cocos2d::Ref*);
    void onTogglePushNotification(cocos2d::Ref*);
    void onToggleTutorial(cocos2d::Ref*);
    void onRateUs(cocos2d::Ref*);
    void onToggleiCloud(cocos2d::Ref*);

    void onAddFriend(cocos2d::Ref*);

    static PopOption* optionWithType(int typ);//0=set, 1=gainfriend 2=setfriend)
    
    //cocosbuilder support
    bool onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, CCNode* pNode)
    {
CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "mMusic", CCMenuItemImage*, mMusic);
CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "mSfx", CCMenuItemImage*, mSfx);
CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "mPushNotification", CCMenuItemImage*, mPushNotification);
CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "mTutorial", CCMenuItemImage*, mTutorial);
CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "miCloud", CCMenuItemImage*, miCloud);
CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "mCoin", CCLabelBMFont*, mCoin);
CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "mShadeCode", CCNode*, mShadeCode);
        return false;
    }
    
    SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char* pSelectorName)
    {
CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "onCloseOption", PopOption::onCloseOption);
CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "onToggleMusic", PopOption::onToggleMusic);
CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "onToggleSfx", PopOption::onToggleSfx);
CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "onTogglePushNotification", PopOption::onTogglePushNotification);
CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "onToggleTutorial", PopOption::onToggleTutorial);
CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "onRateUs", PopOption::onRateUs);
CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "onToggleiCloud", PopOption::onToggleiCloud);
CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "onAddFriend", PopOption::onAddFriend);
        return NULL;
    }
    
    cocos2d::extension::Control::Handler   onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char* pSelectorName)
    {
        return NULL;
    }
    
    CREATE_FUNC(PopOption);
    
    bool init(){return true;};
    
private:
    void loadSetting();
    void loadFriendGain();
    void loadFriendAdd();
    void loadDelivery();
};

PopOption* PopOption::optionWithType(int typ)
{
    PopOption *ret = PopOption::create();
    switch (typ) {
        case 0:
        {
            ret->loadSetting();
        }
            break;
        case 1:
        {
            ret->loadFriendGain();
        }
            break;
        case 2:
        {
            ret->loadFriendAdd();
        }
            break;
        case 3:
        {
            ret->loadDelivery();
        }
    }
    return ret;
}

void PopOption::loadSetting()
{
NodeLoaderLibrary *pNodeLib = NodeLoaderLibrary::sharedNodeLoaderLibrary();
CCBReader *pReader = new CCBReader(pNodeLib, this, this);
cocos2d::CCNode *node = pReader->readNodeGraphFromFile("menu-titleoption.ccbi", this);
    pReader->release();
    
    this->addChild(node);
    
    //sync button state
    //sychornize music & sfx
    if( SimpleAudioEngine::sharedEngine()->getBackgroundMusicVolume() == 0 )
    {
        mMusic->setNormalImage(cocos2d::CCSprite::createWithSpriteFrameName("settingoff.png"));
        mMusic->setSelectedImage(cocos2d::CCSprite::createWithSpriteFrameName("settingoff.png"));
    }
    else {
        mMusic->setNormalImage(cocos2d::CCSprite::createWithSpriteFrameName("settingon.png"));
        mMusic->setSelectedImage(cocos2d::CCSprite::createWithSpriteFrameName("settingon.png"));
    }
    if( SimpleAudioEngine::sharedEngine()->getEffectsVolume() == 0 )
    {
        mSfx->setNormalImage(cocos2d::CCSprite::createWithSpriteFrameName("settingoff.png"));
        mSfx->setSelectedImage(cocos2d::CCSprite::createWithSpriteFrameName("settingoff.png"));
    }
    else {
        mSfx->setNormalImage(cocos2d::CCSprite::createWithSpriteFrameName("settingon.png"));
        mSfx->setSelectedImage(cocos2d::CCSprite::createWithSpriteFrameName("settingon.png"));
    }
    //sync push notification
    if( GameRecord::sharedGameRecord()->game_notify < 0 )
    {
        mPushNotification->setNormalImage(cocos2d::CCSprite::createWithSpriteFrameName("settingoff.png"));
        mPushNotification->setSelectedImage(cocos2d::CCSprite::createWithSpriteFrameName("settingoff.png"));
    }
    else {
        mPushNotification->setNormalImage(cocos2d::CCSprite::createWithSpriteFrameName("settingon.png"));
        mPushNotification->setSelectedImage(cocos2d::CCSprite::createWithSpriteFrameName("settingon.png"));
    }
    //sync tutorial
    if( GameRecord::sharedGameRecord()->game_tutorial == 0 )
    {
        mTutorial->setNormalImage(cocos2d::CCSprite::createWithSpriteFrameName("settingon.png"));
        mTutorial->setSelectedImage(cocos2d::CCSprite::createWithSpriteFrameName("settingon.png"));
    }
    else {
        mTutorial->setNormalImage(cocos2d::CCSprite::createWithSpriteFrameName("settingoff.png"));
        mTutorial->setSelectedImage(cocos2d::CCSprite::createWithSpriteFrameName("settingoff.png"));
    }
    //sync icloud
    if( GameRecord::sharedGameRecord()->setting_icloud > 0 )
    {
        miCloud->setNormalImage(cocos2d::CCSprite::createWithSpriteFrameName("settingon.png"));
        miCloud->setSelectedImage(cocos2d::CCSprite::createWithSpriteFrameName("settingon.png"));
    }
    else {
        miCloud->setNormalImage(cocos2d::CCSprite::createWithSpriteFrameName("settingoff.png"));
        miCloud->setSelectedImage(cocos2d::CCSprite::createWithSpriteFrameName("settingoff.png"));
    }
}

void PopOption::loadFriendGain()
{
    //feature removed from android
//    CCNode *node = [CCBReader nodeGraphFromFile:"menu-titlefriendgain.ccb" owner:this);
//    this->addChild(node);
//    CCString* coin = cocos2d::CCString::createWithFormat("%d", FriendList->sharedFriendList()->bounsCount());
//    mCoin->setString(coin);
}

void PopOption::loadFriendAdd()
{
    //feature removed from android
//    CCNode *node = [CCBReader nodeGraphFromFile:"menu-titlefriendset.ccb" owner:this);
//    this->addChild(node);
//    CCString* coin = cocos2d::CCString::createWithFormat("%d", FriendList->sharedFriendList()->bounsCount());
//    mCoin->setString(coin);
//    CCLabelTTF *label = [CCLabelTTF, ABSystem->queryUserCode(), "Helvetica", 24);
//    label->setAnchorPoint(cocos2d::ccp(0, 0.5f));
//    mShadeCode->addChild(label);
}

void PopOption::loadDelivery()
{
    //feature removed from android
//    CCNode *sp = [ABDelivery->sharedDelivery(), this, @selector(onOpenDelivery), @selector(onCloseOption));
//    sp->setPosition(
//    this->addChild(sp);
    
//  sp->scale(0);
//  float sq0Duration[] = {0.2f, 0.1f, 0.05f, 0.05f};
//  float sq0Scale[] = {1.2f, 0.9f, 1.1f, 1};
//  CCSequence *sq0 = createScaleSequence(sq0Duration, sq0Scale, 4);
//  sp->runAction(seq);
}

void PopOption::onAddFriend(cocos2d::Ref*)
{
    //feature removed from android
//    if( ABSystem->isSystemVersionAbove(ABSYSVER_5) )
//    {
//        UIAlertView *alert = [UIAlertView->alloc()->init() autorelease);
//        alert.alertViewStyle = UIAlertViewStylePlainTextInput;
//        alert.title = "Add A Friend";
//        alert.message = "Enter a friend's Share Code";
//        alert.delegate = this;
//        [alert addButtonWithTitle:"OK");
//        [alert addButtonWithTitle:"Cancel");
//        alert->show();
//    }
//    else
//    {
//        UIAlertView *alert = [UIAlertView->alloc(), "Sorry", "The \"Share with friends\" feature only available on the deivce with iOS5 or above.", NULL, "OK", NULL);
//        alert->show();
//        alert->release();
//    }
}

//feature removed from android
//void TitleMenu::UIAlertView * alertView, NSInteger buttonIndex()
//{
//    if( buttonIndex == 0 )
//    {
//        int ret = FriendList->sharedFriendList()->addFriend([alertView, 0).text);
//        switch (ret) {
//            case 0:
//            {
//                SimpleAudioEngine::sharedEngine()->playEffect(cocos2d::CCFileUtils::sharedFileUtils()->fullPathForFilename("sound/getcoin.mp3");
//                mCoin->setString(cocos2d::CCString::createWithFormat("%d", FriendList->sharedFriendList()->bounsCount()));
//                UIAlertView *alert = [UIAlertView->alloc(), "Success", "Share code has been added.", NULL, "OK", NULL);
//                alert->show();
//                alert->release();
//            }
//                break;
//            case 1:
//            {
//                UIAlertView *alert = [UIAlertView->alloc(), "Sorry", "Your code is incorrect.", NULL, "OK", NULL);
//                alert->show();
//                alert->release();
//            }
//                break;
//            case 2:
//            {
//                UIAlertView *alert = [UIAlertView->alloc(), "Sorry", "This code is already used before.", NULL, "OK", NULL);
//                alert->show();
//                alert->release();
//            }
//                break;
//            case 3:
//            {
//                UIAlertView *alert = [UIAlertView->alloc(), "Sorry", "Share your code with your friends and ask their share code to complete ", NULL, "OK", NULL);
//                alert->show();
//                alert->release();
//            }
//                break;
//        }
//    }
//}

//feature removed from android
//void TitleMenu::onOpenDelivery()
//{
//    UIApplication->sharedApplication()->openURL([NSURL, ABDelivery->sharedDelivery()->deliveryURL()));
//    master->hideOpt();
//}

void PopOption::onCloseOption(cocos2d::Ref*)
{
    master->hideOpt();
}

void PopOption::onToggleSfx(cocos2d::Ref*)
{
    SimpleAudioEngine::sharedEngine()->playEffect(cocos2d::CCFileUtils::sharedFileUtils()->fullPathForFilename("sound/click.mp3").c_str());
    if( GameRecord::sharedGameRecord()->setting_sfx )
    {
        GameRecord::sharedGameRecord()->setting_sfx = 0;
        mSfx->setNormalImage(cocos2d::CCSprite::createWithSpriteFrameName("settingoff.png"));
        mSfx->setSelectedImage(cocos2d::CCSprite::createWithSpriteFrameName("settingoff.png"));
        SimpleAudioEngine::sharedEngine()->setEffectsVolume(0);
    }
    else{
        GameRecord::sharedGameRecord()->setting_sfx = 1;
        mSfx->setNormalImage(cocos2d::CCSprite::createWithSpriteFrameName("settingon.png"));
        mSfx->setSelectedImage(cocos2d::CCSprite::createWithSpriteFrameName("settingon.png"));
        SimpleAudioEngine::sharedEngine()->setEffectsVolume(1);
    }
}

void PopOption::onToggleMusic(cocos2d::Ref*)
{
    SimpleAudioEngine::sharedEngine()->playEffect(cocos2d::CCFileUtils::sharedFileUtils()->fullPathForFilename("sound/click.mp3").c_str());
    if( GameRecord::sharedGameRecord()->setting_music )
    {
        GameRecord::sharedGameRecord()->setting_music = 0;
        mMusic->setNormalImage(cocos2d::CCSprite::createWithSpriteFrameName("settingoff.png"));
        mMusic->setSelectedImage(cocos2d::CCSprite::createWithSpriteFrameName("settingoff.png"));
        SimpleAudioEngine::sharedEngine()->setBackgroundMusicVolume(0);
    }
    else{
        GameRecord::sharedGameRecord()->setting_music = 1;
        mMusic->setNormalImage(cocos2d::CCSprite::createWithSpriteFrameName("settingon.png"));
        mMusic->setSelectedImage(cocos2d::CCSprite::createWithSpriteFrameName("settingon.png"));
        SimpleAudioEngine::sharedEngine()->setBackgroundMusicVolume(1);
    }
}

void PopOption::onTogglePushNotification(cocos2d::Ref*)
{
    //feature removed from android
//    SimpleAudioEngine::sharedEngine()->playEffect(cocos2d::CCFileUtils::sharedFileUtils()->fullPathForFilename("sound/click.mp3");
//    if( GameRecord::sharedGameRecord()->game_notify < 0 )
//    {
//        GameRecord::sharedGameRecord()->game_notify = 1;
//        mPushNotification->setNormalImage(cocos2d::CCSprite::createWithSpriteFrameName("settingon.png"));
//        mPushNotification->setSelectedImage(cocos2d::CCSprite::createWithSpriteFrameName("settingon.png"));
//    }
//    else {
//        GameRecord::sharedGameRecord()->game_notify = -1;
//        mPushNotification->setNormalImage(cocos2d::CCSprite::createWithSpriteFrameName("settingoff.png"));
//        mPushNotification->setSelectedImage(cocos2d::CCSprite::createWithSpriteFrameName("settingoff.png"));
//    }
//    AppController *delgate = (AppController*)UIApplication->sharedApplication().delegate;
//    delgate->scheduleLocalNotifuication();
}

void PopOption::onToggleTutorial(cocos2d::Ref*)
{
    SimpleAudioEngine::sharedEngine()->playEffect(cocos2d::CCFileUtils::sharedFileUtils()->fullPathForFilename("sound/click.mp3").c_str());
    if( GameRecord::sharedGameRecord()->game_tutorial == 0 )
    {
        GameRecord::sharedGameRecord()->game_tutorial = 1;
        mTutorial->setNormalImage(cocos2d::CCSprite::createWithSpriteFrameName("settingoff.png"));
        mTutorial->setSelectedImage(cocos2d::CCSprite::createWithSpriteFrameName("settingoff.png"));
    }
    else {
        GameRecord::sharedGameRecord()->game_tutorial = 0;
        mTutorial->setNormalImage(cocos2d::CCSprite::createWithSpriteFrameName("settingon.png"));
        mTutorial->setSelectedImage(cocos2d::CCSprite::createWithSpriteFrameName("settingon.png"));
    }
}

void PopOption::onToggleiCloud(cocos2d::Ref*)
{
    //feature removed from android
//    SimpleAudioEngine::sharedEngine()->playEffect(cocos2d::CCFileUtils::sharedFileUtils()->fullPathForFilename("sound/click.mp3");
//    if( GameRecord::sharedGameRecord()->setting_icloud > 0 )
//    {
//        GameRecord::sharedGameRecord()->setting_icloud = 0;
//        miCloud->setNormalImage(cocos2d::CCSprite::createWithSpriteFrameName("settingoff.png"));
//        miCloud->setSelectedImage(cocos2d::CCSprite::createWithSpriteFrameName("settingoff.png"));
//    }
//    else {
//        GameRecord::sharedGameRecord()->setting_icloud = 1;
//        miCloud->setNormalImage(cocos2d::CCSprite::createWithSpriteFrameName("settingon.png"));
//        miCloud->setSelectedImage(cocos2d::CCSprite::createWithSpriteFrameName("settingon.png"));
//    }
}

void PopOption::onRateUs(cocos2d::Ref*)
{
    //feature removed from android
//CCString *str = "itms-apps://ax.itunes.apple.com/WebObjects/MZStore.woa/wa/viewContentsUserReviews?type=Purple+Software&id=540290969";
//    UIApplication->sharedApplication()->openURL([NSURL, str));
//    
//    GameRecord::sharedGameRecord()->rate_pop = -1;
}

class PopObj :
public CCNode,
public CCBMemberVariableAssigner,
public CCBSelectorResolver
{
public:
    CEClipedNode *mBoard;
cocos2d::CCNode *mRode;
    float mTimer;
    bool mFlag;
    //-- auto assign --
cocos2d::CCLabelTTF *mDailyObjective;
cocos2d::CCLabelTTF *mWeeklyObjective;
cocos2d::CCLabelTTF *mMonthlyObjective;
cocos2d::CCNode *mDailyCrown;
cocos2d::CCNode *mWeeklyCrown;
cocos2d::CCNode *mMonthlyCrown;
cocos2d::CCSprite *mDailyIcon;
cocos2d::CCSprite *mWeeklyIcon;
cocos2d::CCSprite *mMonthlyIcon;
cocos2d::CCNode *mClassic;
cocos2d::CCNode *mArcade;
cocos2d::CCSprite *mGoldCoin;
cocos2d::CCSprite *mSilverCoin;
cocos2d::CCSprite *mBronzeCoin;
cocos2d::CCLabelBMFont *mGoldScore;
cocos2d::CCLabelBMFont *mGoldPrize;
cocos2d::CCLabelBMFont *mSilverScore;
cocos2d::CCLabelBMFont *mSilverPrize;
cocos2d::CCLabelBMFont *mBronzeScore;
cocos2d::CCLabelBMFont *mBronzePrize;
cocos2d::CCMenu *mMenu;
cocos2d::CCMenuItemImage *mSwitch;
cocos2d::CCMenuItemImage *mSwitch2;
    
    bool mDisplay;//display content true=classic false=arcade

    TitleMenu *master;

    void onCreate();
    void onDestroy();
    void onFlip();
    void onDoneAnimation();
    
    //callbacks
    void onChangeDisplay(cocos2d::Ref*);
    void onClose(cocos2d::Ref*);
    
    //cocosbuilder support
    bool onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, CCNode* pNode)
    {
CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "mDailyObjective", CCLabelTTF*, mDailyObjective);
CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "mWeeklyObjective", CCLabelTTF*, mWeeklyObjective);
CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "mMonthlyObjective", CCLabelTTF*, mMonthlyObjective);
CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "mDailyCrown", CCNode*, mDailyCrown);
CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "mWeeklyCrown", CCNode*, mWeeklyCrown);
CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "mMonthlyCrown", CCNode*, mMonthlyCrown);
CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "mDailyIcon", CCSprite*, mDailyIcon);
CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "mWeeklyIcon", CCSprite*, mWeeklyIcon);
CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "mMonthlyIcon", CCSprite*, mMonthlyIcon);
CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "mClassic", CCNode*, mClassic);
CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "mArcade", CCNode*, mArcade);
CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "mGoldCoin", CCSprite*, mGoldCoin);
CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "mSilverCoin", CCSprite*, mSilverCoin);
CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "mBronzeCoin", CCSprite*, mBronzeCoin);
CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "mGoldScore", CCLabelBMFont*, mGoldScore);
CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "mGoldPrize", CCLabelBMFont*, mGoldPrize);
CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "mSilverScore", CCLabelBMFont*, mSilverScore);
CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "mSilverPrize", CCLabelBMFont*, mSilverPrize);
CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "mBronzeScore", CCLabelBMFont*, mBronzeScore);
CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "mBronzePrize", CCLabelBMFont*, mBronzePrize);
CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "mMenu", CCMenu*, mMenu);
CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "mSwitch", CCMenuItemImage*, mSwitch);
CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "mSwitch2", CCMenuItemImage*, mSwitch2);
        return false;
    }
    
    SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char* pSelectorName)
    {
        // TODO:
CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "onClose", PopObj::onClose);
CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "onChangeDisplay", PopObj::onChangeDisplay);
        return NULL;
    }
    
    cocos2d::extension::Control::Handler onResolveCCBCCControlSelector(cocos2d::CCObject * pTarget, const char* pSelectorName)
    {
        return NULL;
    }
    
    CREATE_FUNC(PopObj);
    
    bool init(){return true;};
    
private:
    void displayClassic();
    void displayArcade();
};

void PopObj::displayClassic()
{
    //update objectives
    if( GameRecord::sharedGameRecord()->task->dailyObjective->uiid >= 0 )
    {
        Achievement *obj = Tasks::dailyObjectiveWithUiid(GameRecord::sharedGameRecord()->task->dailyObjective->uiid);
        mDailyObjective->setString(Tasks::stringForObjective(obj->desc ,obj->achieveCode , obj->achieveNumber ,GameRecord::sharedGameRecord()->task->dailyObjective->count)->getCString());
        mDailyObjective->setColor(ccc3(255, 255, 255));
        
        mDailyIcon->setVisible(true);
        Achievement *ach = Tasks::dailyObjectiveWithUiid(GameRecord::sharedGameRecord()->task->dailyObjective->uiid);
cocos2d::CCSprite *icon = cocos2d::CCSprite::createWithSpriteFrameName(ach->icon->getCString());
        icon->setPosition(cocos2d::ccp(mDailyIcon->getContentSize().width/2, mDailyIcon->getContentSize().height/2));
        mDailyIcon->addChild(icon);
        
        //判断是否完成
        if( GameRecord::sharedGameRecord()->task->dailyObjective->count >= ach->achieveNumber )
        {
            mDailyObjective->setColor(ccc3(128, 128, 128));
        }
    }
    else {
        mDailyObjective->setString("已完成！");
        mDailyObjective->setColor(ccc3(128, 128, 128));
    }
    if( GameRecord::sharedGameRecord()->task->weeklyObjective->uiid >= 0 )
    {
        Achievement *obj = Tasks::weeklyObjectiveWithUiid(GameRecord::sharedGameRecord()->task->weeklyObjective->uiid);
        mWeeklyObjective->setString(Tasks::stringForObjective(obj->desc ,obj->achieveCode ,obj->achieveNumber ,GameRecord::sharedGameRecord()->task->weeklyObjective->count)->getCString());
        mWeeklyObjective->setColor(ccc3(255, 255, 255));
        
        mWeeklyIcon->setVisible(true);
        Achievement *ach = Tasks::weeklyObjectiveWithUiid(GameRecord::sharedGameRecord()->task->weeklyObjective->uiid);
cocos2d::CCSprite *icon = cocos2d::CCSprite::createWithSpriteFrameName(ach->icon->getCString());
        icon->setPosition(cocos2d::ccp(mWeeklyIcon->getContentSize().width/2, mDailyIcon->getContentSize().height/2));
        mWeeklyIcon->addChild(icon);
        
        //判断是否完成
        if( GameRecord::sharedGameRecord()->task->weeklyObjective->count >= ach->achieveNumber )
        {
            mWeeklyObjective->setColor(ccc3(128, 128, 128));
        }
    }
    else {
        mWeeklyObjective->setString("已完成！");
        mWeeklyObjective->setColor(ccc3(128, 128, 128));
    }
    if( GameRecord::sharedGameRecord()->task->monthlyObjective->uiid >= 0 )
    {
        Achievement *obj = Tasks::monthlyObjectiveWithUiid(GameRecord::sharedGameRecord()->task->monthlyObjective->uiid);
        mMonthlyObjective->setString(Tasks::stringForObjective(obj->desc ,obj->achieveCode ,obj->achieveNumber ,GameRecord::sharedGameRecord()->task->monthlyObjective->count)->getCString());
        mMonthlyObjective->setColor(ccc3(255, 255, 255));
        
        mMonthlyIcon->setVisible(true);
        Achievement *ach = Tasks::monthlyObjectiveWithUiid(GameRecord::sharedGameRecord()->task->monthlyObjective->uiid);
cocos2d::CCSprite *icon = cocos2d::CCSprite::createWithSpriteFrameName(ach->icon->getCString());
        icon->setPosition(cocos2d::ccp(mMonthlyIcon->getContentSize().width/2, mDailyIcon->getContentSize().height/2));
        mMonthlyIcon->addChild(icon);
        
        //判断是否完成
        if( GameRecord::sharedGameRecord()->task->monthlyObjective->count >= ach->achieveNumber )
        {
            mMonthlyObjective->setColor(ccc3(128, 128, 128));
        }
    }
    else {
        mMonthlyObjective->setString("已完成！");
        mMonthlyObjective->setColor(ccc3(128, 128, 128));
    }
    //update crown
    for(int i=0; i<GameRecord::sharedGameRecord()->task->dailyObjective->index; ++i)
    {
cocos2d::CCSprite *crown = cocos2d::CCSprite::createWithSpriteFrameName(cocos2d::CCString::createWithFormat("crown%d.png", i)->getCString());
        crown->setPosition(cocos2d::ccp(32-16*i, 0));
        mDailyCrown->addChild(crown);
    }
    for(int i=0; i<GameRecord::sharedGameRecord()->task->weeklyObjective->index; ++i)
    {
cocos2d::CCSprite *crown = cocos2d::CCSprite::createWithSpriteFrameName(cocos2d::CCString::createWithFormat("crown%d.png", i)->getCString());
        crown->setPosition(cocos2d::ccp(32-16*i, 0));
        mWeeklyCrown->addChild(crown);
    }
    for(int i=0; i<GameRecord::sharedGameRecord()->task->monthlyObjective->index; ++i)
    {
cocos2d::CCSprite *crown = cocos2d::CCSprite::createWithSpriteFrameName(cocos2d::CCString::createWithFormat("crown%d.png", i)->getCString());
        crown->setPosition(cocos2d::ccp(32-16*i, 0));
        mMonthlyCrown->addChild(crown);
    }
}

//
void PopObj::displayArcade()
{
    //update trophies
    //gold
    {
        if( GameRecord::sharedGameRecord()->task->goldPrize->prize < 0 )
        {//completed
            mGoldCoin->setVisible(false);
            mGoldPrize->setVisible(false);
            mGoldScore->setString("已完成！");
            mGoldScore->setColor(ccc3(128, 128, 128));
        }
        else
        {//not completed
            mGoldScore->setString(cocos2d::CCString::createWithFormat("%d", GameRecord::sharedGameRecord()->task->goldPrize->score)->getCString());
            mGoldPrize->setString(cocos2d::CCString::createWithFormat("%d", GameRecord::sharedGameRecord()->task->goldPrize->prize)->getCString());
        }
    }
    //silver
    {
        if( GameRecord::sharedGameRecord()->task->silverPrize->prize < 0 )
        {//completed
            mSilverCoin->setVisible(false);
            mSilverPrize->setVisible(false);
            mSilverScore->setString("已完成！");
            mSilverScore->setColor(ccc3(128, 128, 128));
        }
        else
        {//not completed
            mSilverScore->setString(cocos2d::CCString::createWithFormat("%d", GameRecord::sharedGameRecord()->task->silverPrize->score)->getCString());
            mSilverPrize->setString(cocos2d::CCString::createWithFormat("%d", GameRecord::sharedGameRecord()->task->silverPrize->prize)->getCString());
        }
    }
    //bronze
    {
        if( GameRecord::sharedGameRecord()->task->bronzePrize->prize < 0 )
        {//completed
            mBronzeCoin->setVisible(false);
            mBronzePrize->setVisible(false);
            mBronzeScore->setString("已完成！");
            mBronzeScore->setColor(ccc3(128, 128, 128));
        }
        else
        {//not completed
            mBronzeScore->setString(cocos2d::CCString::createWithFormat("%d", GameRecord::sharedGameRecord()->task->bronzePrize->score)->getCString());
            mBronzePrize->setString(cocos2d::CCString::createWithFormat("%d", GameRecord::sharedGameRecord()->task->bronzePrize->prize)->getCString());
        }
    }
}

void PopObj::onCreate()
{
cocos2d::CCNode * node = createUIByCCBI("menu-titlepop", "TitleMenu", TitleMenuLayerLoader::loader(), this);
    mBoard = CEClipedNode::create();
    mBoard->addChild(node);
cocos2d::CCRect rect = UniversalFit::sharedUniversalFit()->transformRect(cocos2d::CCRectMake(60, 0, SCREEN_WIDTH-60, SCREEN_HEIGHT));
    //CCLog("CLIP = %fx%f %fx%f", rect.origin.x, rect.origin.y, rect.size.width, rect.size.height);
    mBoard->setClipRect(&rect);
    //CCRect rect = cocos2d::CCRectMake(60, 0, SCREEN_WIDTH-60, SCREEN_HEIGHT);
    //mBoard->setClipRect(&rect);
    mBoard->setPosition(cocos2d::ccp(-301, 58));
    this->addChild(mBoard);
    
    mRode = cocos2d::CCSprite::createWithSpriteFrameName("index_jz.png");
    mRode->setPosition(cocos2d::ccp(70, 170));
    this->addChild(mRode);
    mTimer = 0;
    mFlag = false;
    mMenu->setVisible(false);
    
    mDisplay = true;
    if( mDisplay )
    {
        mClassic->setVisible(true);
        mArcade->setVisible(false);
        this->displayClassic();
    }
    else
    {
        mClassic->setVisible(false);
        mArcade->setVisible(true);
        this->displayArcade();
    }
    
    //run animation
cocos2d::CCMoveTo *mt = cocos2d::CCMoveTo::create(0.2f, ccp(-301+381, 58));
cocos2d::CCCallFunc *cf = cocos2d::CCCallFunc::create(this, callfunc_selector(PopObj::onDoneAnimation));
cocos2d::CCSequence *seq = cocos2d::CCSequence::createWithTwoActions(mt, cf);
    
    mBoard->runAction(seq);
    mSwitch->setVisible(false);
    mSwitch2->setVisible(false);
}

void PopObj::onDoneAnimation()
{
    mBoard->setPosition(cocos2d::ccp( -301 + 381, 58));
    mMenu->setVisible(true);
    //CCRect rect = cocos2d::CCRectMake(0, 0, UniversalFit::sharedUniversalFit()->screenSize.width, UniversalFit::sharedUniversalFit()->screenSize.height);
    //CCRect rect = cocos2d::CCRectMake(0, 0, 10000, 10000);
    //mBoard->setClipRect(&rect);
    mSwitch->setVisible(true);
    mSwitch2->setVisible(true);
}

void PopObj::onDestroy()
{
    this->removeChild(mRode, true);
    this->removeChild(mBoard, true);
}

void PopObj::onClose(cocos2d::Ref*)
{
    master->hideObjs();
}

void PopObj::onFlip()
{
    if( mDisplay )
    {
        mClassic->setVisible(true);
        mArcade->setVisible(false);
        this->displayClassic();
    }
    else
    {
        mClassic->setVisible(false);
        mArcade->setVisible(true);
        this->displayArcade();
    }
}
                           
void PopObj::onChangeDisplay(cocos2d::Ref*)
{
    mDisplay = !mDisplay;
    
    //play animation
    //CCRect rect = cocos2d::CCRectMake(60, 0, SCREEN_WIDTH-60, SCREEN_HEIGHT);
cocos2d::CCRect rect = UniversalFit::sharedUniversalFit()->transformRect(cocos2d::CCRectMake(60, 0, SCREEN_WIDTH-60, SCREEN_HEIGHT));
    mBoard->setClipRect(&rect);
cocos2d::CCMoveTo *mt1 = cocos2d::CCMoveTo::create(0.2f, ccp(-301, 58));
cocos2d::CCCallFunc *cf1 = cocos2d::CCCallFunc::create(this, (callfunc_selector(PopObj::onFlip)));
cocos2d::CCMoveTo *mt2 = cocos2d::CCMoveTo::create(0.2f, ccp(-301+381, 58));
cocos2d::CCCallFunc *cf2 = cocos2d::CCCallFunc::create(this, (callfunc_selector(PopObj::onDoneAnimation)));
    std::vector<FiniteTimeAction*> *sqa = new std::vector<FiniteTimeAction*>(); //TODO:memory management?
    sqa->push_back(mt1);
    sqa->push_back(cf1);
    sqa->push_back(mt2);
    sqa->push_back(cf2);
    /* TODO:uncomment this
cocos2d::CCSequence *seq = cocos2d::CCSequence::create(sqa);
    //CCSequence *seq = cocos2d::CCSequence::create(mt1, cf1, mt2, cf2);
    
    mBoard->runAction(seq);
     */
    
    mSwitch->setVisible(false);
    mSwitch2->setVisible(false);
    
    SimpleAudioEngine::sharedEngine()->playEffect(cocos2d::CCFileUtils::sharedFileUtils()->fullPathForFilename("sound/flip.mp3").c_str());
}

#ifdef TITLESTYLE_SPRING
Sakura* Sakura::create(cocos2d::CCNode *parent)
{
  Sakura *ret = new Sakura;
  ret->init();
  ret->mSprite = cocos2d::CCSprite::createWithSpriteFrameName("scene-snow.png");
  ret->mSprite->setPosition(cocos2d::ccp((SCREEN_WIDTH+300)*CCRANDOM_0_1(), SCREEN_HEIGHT+50 ));
  ret->mParent = parent;
  //ret->mFlipX = 0.5f + CCRANDOM_0_1();
  ret->mWindX = CCRANDOM_0_1();
  ret->mSpeed = CCRANDOM_0_1()*50 - 80;
  ret->mSpeedY = 40 + 30*CCRANDOM_0_1();
  parent->addChild(ret->mSprite);
  return ret;
}

bool Sakura::init()
{
  return true;
}
bool Sakura::Update(float delta, float wind)
{
  float fl = mSprite->getScale();
//  fl += delta*mFlipX;
//  if( fl > 1 )
//  {
//    fl = 1;
//    mFlipX *= -1;
//  }
//  if( fl < -1 )
//  {
//    fl = -1;
//    mFlipX *= -1;
//  }
  mSprite->setScaleX(fl);
  cocos2d::Point np = mSprite->getPosition();
  np.y -= delta*mSpeedY;
  mSpeed += delta*wind;
  np.x += delta*mSpeed;
  mSprite->setPosition(np);
  if( np.y < -10 || np.x < -10 )
  {
    mParent->removeChild(mSprite, false);
    return true;
  }
  return false;
}
#endif

bool TitleMenu::init()
{
    if ( !CCLayer::init() )
    {
        return false;
    }

    scheduleUpdate();
    
    mPop = NULL;
    mOpt = NULL;
    return true;
}

void TitleMenu::onEnter()
{
  // 装载资源
  PublicLoad::menuTitle()->loadAll();
  GameRecord::sharedGameRecord()->task->checkObjectives();

  mIntroFlag = false;
  mSceneIntro = NULL;

    //enable keypad for back button
    this->setKeypadEnabled(true);
cocos2d::CCNode * node = createUIByCCBI("menu-title", "TitleMenu", TitleMenuLayerLoader::loader(), this);
  if(node != NULL) {
    this->addChild(node);
  }
  //select SinaWeibo / Twitter
  if(UniversalFit::shouldUsingSinaWeibo())
  {
    mTwitter->setNormalSpriteFrame(cocos2d::CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("contact3.png"));
    mTwitter->setSelectedSpriteFrame(cocos2d::CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("contact3.png"));
  }
  
  for(int i=0; i<RUN; ++i)
  {
    mAni[i] = NULL;
  }

    {
      float sq0Duration[] = {0.2f, 0.2f};
      float sq0Scale[] = {1.2f, 1.0f};
cocos2d::CCSequence *sq0 = createScaleSequence(sq0Duration, sq0Scale, 2);
      mLogo->runAction(sq0);
cocos2d::CCDelayTime *dt1 = cocos2d::CCDelayTime::create(0.6f);
cocos2d::CCShow *st1 = cocos2d::CCShow::create();
cocos2d::CCSequence *sq = (cocos2d::CCSequence*)CCSequence::create(dt1, st1, NULL);
      mMainButton->runAction(sq);
cocos2d::CCDelayTime *dt2 = cocos2d::CCDelayTime::create(0.6f);
cocos2d::CCShow *st2 = cocos2d::CCShow::create();
cocos2d::CCSequence *sq2 = (cocos2d::CCSequence*)CCSequence::create(dt2, st2, NULL);
      mMainButton->runAction(sq2);
    }
    
  mSakura = cocos2d::CCArray::create();
  mSakura->retain();
  mWind = 0;
  
  if( !SimpleAudioEngine::sharedEngine()->isBackgroundMusicPlaying() )
  {
    SimpleAudioEngine::sharedEngine()->playBackgroundMusic(cocos2d::CCFileUtils::sharedFileUtils()->fullPathForFilename("sound/menu.mp3").c_str(), true);
  }
  
  // TODO:
  if( !gIntroFlag )
  {
    //AppController *appdel = (AppController*)[UIApplication->sharedApplication() delegate);
    //appdel->scheduleLocalNotifuication();
    //
    //[GameCenterController->sharedGameCenterController() initGameCenter);
    //
    ////check point when enter title at the first time
    GameRecord::sharedGameRecord()->checkPoint();
    //
    //set sceneintro sprites
  }
  setSceneIntro();
  gIntroFlag = true;
  
//TODO    [GameCenterController->sharedGameCenterController() loadFriendsLeaderboard);
//    //check new objectives
  mNew->setVisible(false);
  if( GameRecord::sharedGameRecord()->task->newrefresh )
  {
cocos2d::CCDelayTime *dt1 = cocos2d::CCDelayTime::create(1.0f);
cocos2d::CCShow *sh1 = cocos2d::CCShow::create();
cocos2d::CCScaleTo *st1 = cocos2d::CCScaleTo::create(0.2f, 1.5f);
cocos2d::CCScaleTo *st2 = cocos2d::CCScaleTo::create(0.1f, 1.0f);
cocos2d::CCArray *sqa = cocos2d::CCArray::create();
      sqa->addObject(dt1);
      sqa->addObject(sh1);
      sqa->addObject(st1);
      sqa->addObject(st2);
      /* TODO:uncomment this
cocos2d::CCSequence *sq = cocos2d::CCSequence::create(sqa);
      
      mNew->runAction(sq);
       */
  }

  //check new collections
  mCNew->setVisible(false);
  if( GameRecord::sharedGameRecord()->collection->newlist->count() > 0 )
  {
cocos2d::CCDelayTime *dt1 = cocos2d::CCDelayTime::create(1.5f);
cocos2d::CCShow *sh1 = cocos2d::CCShow::create();
cocos2d::CCScaleTo *st1 = cocos2d::CCScaleTo::create(0.2f, 1.5f);
cocos2d::CCScaleTo *st2 = cocos2d::CCScaleTo::create(0.1f, 1);
cocos2d::CCArray *sqa = cocos2d::CCArray::create();
      sqa->addObject(dt1);
      sqa->addObject(sh1);
      sqa->addObject(st1);
      sqa->addObject(st2);
      /* TODO:uncomment this
cocos2d::CCSequence *sq = cocos2d::CCSequence::create(sqa);
      mCNew->runAction(sq);
       */
  }

  for(int i=0; i<5; ++i)
  {
      mStarTimers[i] = 0;
  }
  
//    //show friends
//    if( FriendList->sharedFriendList()->canHaveBouns() )
//    {
//        [this showOpt:1);
//        SimpleAudioEngine::sharedEngine()->playEffect(cocos2d::CCFileUtils::sharedFileUtils()->fullPathForFilename("sound/getcoin.mp3");
//        GameRecord::sharedGameRecord()->coins += [FriendList->sharedFriendList() bounsCount);
//        [FriendList->sharedFriendList() dealBouns);
//    }
  
  //mPopTimer = 0;
  
  if( !gPopFlag )
  {
      if( CCRANDOM_0_1() < 0.6f )
      {
          gPopFlag = true;
      }
  }
cocos2d::CCLayer::onEnter();
}

void TitleMenu::showObjs()
{
    mMask->setOpacity(128);
    mMask->setVisible(true);
    mPop = PopObj::create();
    mMask->addChild(mPop);
    mPop->master = this;
    mPop->onCreate();
    
    mMainButton->setTouchEnabled(false);
    mMiniButton->setTouchEnabled(false);
    
    mNew->setVisible(false);
    GameRecord::sharedGameRecord()->task->newrefresh = false;
}
//
void TitleMenu::hideObjs()
{
    SimpleAudioEngine::sharedEngine()->playEffect(cocos2d::CCFileUtils::sharedFileUtils()->fullPathForFilename("sound/click.mp3").c_str());
    mMask->setVisible(false);
    mPop->onDestroy();
    mMask->removeAllChildrenWithCleanup(true);
    
    mMainButton->setTouchEnabled(true);
    mMiniButton->setTouchEnabled(true);
    
    mPop = NULL;
}

void TitleMenu::showOpt(int typ)
{
  mMask->setOpacity(128);
  mMask->setVisible(true);

  mOpt = PopOption::optionWithType(typ);
  mMask->addChild(mOpt);
  mOpt->master = this;

  mMainButton->setTouchEnabled(false);
  mMiniButton->setTouchEnabled(false);
}

void TitleMenu::hideOpt()
{
    SimpleAudioEngine::sharedEngine()->playEffect(cocos2d::CCFileUtils::sharedFileUtils()->fullPathForFilename("sound/click.mp3").c_str());
    mMask->setVisible(false);
    mMask->removeAllChildrenWithCleanup(true);
    
    mMainButton->setTouchEnabled(true);
    mMiniButton->setTouchEnabled(true);
    
    mOpt = NULL;
}
//
//CCSprite* TitleMenu::int nid()
//{
//    switch (nid) {
//        case 0:
//            return mStar1;
//        case 1:
//            return mStar2;
//        case 2:
//            return mStar3;
//        case 3:
//            return mStar4;
//        case 4:
//            return mStar5;
//        default:
//            return NULL;
//    }
//}
//
void TitleMenu::update(float delta)
{
//#ifdef TITLESTYLE_SPRING
  //飘云
  {
    float ds = delta*10;
    cocos2d::Point np = mCloud1->getPosition();
    np.x -= ds;
    if( np.x < -100 )
    {
      np.x = SCREEN_WIDTH+100;
    }
    mCloud1->setPosition(np);
  }
  {
    float ds = delta*5;
    cocos2d::Point np = mCloud2->getPosition();
    np.x -= ds;
    if( np.x < -100 )
    {
      np.x = SCREEN_WIDTH+100;
    }
    mCloud2->setPosition(np);
  }
  //------- wind sakura -------
  if( CCRANDOM_0_1() > 0.9f )
  {
    if( CCRANDOM_0_1() > 0.7f )
    {
      mSakura->addObject(Sakura::create(mSakuraNode2)); 
    }
    else {
      mSakura->addObject(Sakura::create(mSakuraNode)); 
    }
  }
  mWindx += delta*3;
  mWind = 50.0f*(sinf(mWindx) - 0.5f);
  for( unsigned int i=0; i<mSakura->count(); ++i)
  {
    Sakura *s = (Sakura*)mSakura->objectAtIndex(i);
    if( s->Update(delta, mWind) )
    {
      mSakura->removeObject(s);
      i--;
    }
  }
//#endif
//#ifdef TITLESTYLE_HALLOWEEN
//    {
//        for(int i=0; i<5; ++i)
//        {
//            mStarTimers[i] += delta;
//            if( mStarTimers[i] > CCRANDOM_0_1()*10 )
//            {
//                mStarTimers[i] = -3;
//                this->getStar(i)->setOpacity(255);
//            }
//            if( mStarTimers[i] < 0 )
//            {
//                float k = -mStarTimers[i]/3.0f;
//                this->getStar(i)->setOpacity(100 + 155*k);
//            }
//        }
//    }
//#endif
    //run enemy
    for(int i=0; i<RUN; ++i)
    {
        if( mAni[i] == NULL )
        {
            if( CCRANDOM_0_1() < 0.0005f )
            {
cocos2d::CCString *enm = NULL;
                switch (randomInt(3)) {
                    case 0:
                    {
                        enm = cocos2d::CCString::create("enemy");
                    }
                        break;
                    case 1:
                    {
                        enm = cocos2d::CCString::create("mninja");
                    }
                        break;
                    case 2:
                    {
                        enm = cocos2d::CCString::create("hninja");
                    }
                        break;
                }
                mAni[i] = GTAnimatedSprite::spriteWithGTAnimation(GTAnimation::loadedAnimationSet(enm->getCString()));
                mAni[i]->setAnchorPoint(cocos2d::ccp(0.4f, 0.0625f));
                mAni[i]->playGTAnimation(0, true);
                mAni[i]->setPosition(cocos2d::ccp(-50, 64 + CCRANDOM_0_1()*36));
                mEnemies->addChild(mAni[i], 5);
                mSpeed[i] = 200 + CCRANDOM_0_1()*100;
            }
        }
        else {
            mAni[i]->updateGTAnimation(delta);
            cocos2d::Point np = mAni[i]->getPosition();
            np.x += delta*mSpeed[i];
            mAni[i]->setPosition(np);
            if( np.x > SCREEN_WIDTH + 50 )
            {
                this->removeChild(mAni[i], true);
                mAni[i] = NULL;
            }
        }
    }

//    if( mPop == NULL && mOpt == NULL )
//    {
//        if( !gPopFlag && ABDelivery->sharedDelivery()->isReadyForDelivery() )
//        {
////            mPopTimer += delta;
////            if( mPopTimer > 2 )
////            {
////                mPopTimer = 0;
////                if( randomInt(10) == 0 )
////                {
////                    [this showOpt:3);
////                    gPopFlag = true;
////                }
////            }
//            [this showOpt:3);
//            gPopFlag = true;
//        }
//    }
}

void TitleMenu::onExit()
{
    removeAllChildrenWithCleanup(true);
    PublicLoad::menuTitle()->unloadAll();
cocos2d::CCLayer::onExit();
}

void TitleMenu::onPlayClassic()
{
cocos2d::CCLog("Player");
  GamePlay::sharedGamePlay()->setGameMode(MODE_CLASSIC);
  SimpleAudioEngine::sharedEngine()->playEffect(cocos2d::CCFileUtils::sharedFileUtils()->fullPathForFilename("sound/menu-change.mp3").c_str());
  setSceneOutro(SelectMenu::scene());
}

void TitleMenu::onPlayArcade()
{
  GamePlay::sharedGamePlay()->setGameMode(MODE_ARCADE);
  SimpleAudioEngine::sharedEngine()->playEffect(cocos2d::CCFileUtils::sharedFileUtils()->fullPathForFilename("sound/menu-change.mp3").c_str());
  setSceneOutro(SelectMenu::scene());
}

void TitleMenu::onCollections()
{
  CollectionMenu::setNavBack(0);
  SimpleAudioEngine::sharedEngine()->playEffect(cocos2d::CCFileUtils::sharedFileUtils()->fullPathForFilename("sound/menu-change.mp3").c_str());
  setSceneOutro(CollectionMenu::scene());
}

void TitleMenu::onStore()
{
  ShopMenu::setNavBack(0);
  SimpleAudioEngine::sharedEngine()->playEffect(cocos2d::CCFileUtils::sharedFileUtils()->fullPathForFilename("sound/menu-change.mp3").c_str());
  setSceneOutro(ShopMenu::scene());
}

void TitleMenu::onFacebook()
{
    SimpleAudioEngine::sharedEngine()->playEffect(cocos2d::CCFileUtils::sharedFileUtils()->fullPathForFilename("sound/menu-change.mp3").c_str());
//    UIApplication->sharedApplication()->openURL([NSURL, "http://www.facebook.com/pages/Little-Ninja-Rush/397668056947647"));
}

void TitleMenu::onTwitter()
{
    SimpleAudioEngine::sharedEngine()->playEffect(cocos2d::CCFileUtils::sharedFileUtils()->fullPathForFilename("sound/menu-change.mp3").c_str());
//    if( UniversalFit::sharedUniversalFit()->shouldUsingSinaWeibo() )
//    {
//        UIApplication->sharedApplication()->openURL([NSURL, "http://weibo.com/u/2810380794"));
//    }
//    else
//    {
//        UIApplication->sharedApplication()->openURL([NSURL, "https://twitter.com/TrinGame"));
//    }
}

void TitleMenu::onLeaderboard()
{
    SimpleAudioEngine::sharedEngine()->playEffect(cocos2d::CCFileUtils::sharedFileUtils()->fullPathForFilename("sound/click.mp3").c_str());
//    if( GameCenterManager->isGameCenterAvailable() )
//    {
//        if( GameCenterManager->isGameCenterLoggedIn() )
//        {
//            [GameCenterController->sharedGameCenterController() showLeaderboard);
//        }
//        else {
//            UIAlertView *alert = [UIAlertView->alloc()->initWithTitle("Sorry", "Game Center is not available before you login.", this, "OK",  NULL) autorelease);
//            alert->show();
//        }
//    }
//    else {
//        UIAlertView *alert = [UIAlertView->alloc()->initWithTitle("Sorry", "Game Center is not available on your device.", this, "OK",  NULL) autorelease);
//        alert->show();
//    }
}

void TitleMenu::onAchievement()
{
    SimpleAudioEngine::sharedEngine()->playEffect(cocos2d::CCFileUtils::sharedFileUtils()->fullPathForFilename("sound/click.mp3").c_str());
//    if( GameCenterManager->isGameCenterAvailable() )
//    {
//        if( GameCenterManager->isGameCenterLoggedIn() )
//        {
//            [GameCenterController->sharedGameCenterController() showAchievements);
//        }
//        else {
//            UIAlertView *alert = [UIAlertView->alloc()->initWithTitle("Sorry", "Game Center is not available before you login.", this, "OK",  NULL) autorelease);
//            alert->show();
//        }
//    }
//    else {
//        UIAlertView *alert = [UIAlertView->alloc()->initWithTitle("Sorry", "Game Center is not available on your device.", this, "OK",  NULL) autorelease);
//        alert->show();
//    }
}

void TitleMenu::onExtra()
{
    SimpleAudioEngine::sharedEngine()->playEffect(cocos2d::CCFileUtils::sharedFileUtils()->fullPathForFilename("sound/menu-change.mp3").c_str());
    setSceneOutro(ExtraSelect::scene());
}

void TitleMenu::onShowObjectives()
{
    SimpleAudioEngine::sharedEngine()->playEffect(cocos2d::CCFileUtils::sharedFileUtils()->fullPathForFilename("sound/click.mp3").c_str());
    this->showObjs();
}

void TitleMenu::onOption()
{
  SimpleAudioEngine::sharedEngine()->playEffect(cocos2d::CCFileUtils::sharedFileUtils()->fullPathForFilename("sound/click.mp3").c_str());
  showOpt(0);
}

void TitleMenu::onMoreFun()
{
    //do nothing
}

void TitleMenu::onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event)
{
    if (keyCode == EventKeyboard::KeyCode::KEY_BACKSPACE) {
cocos2d::CCLog("KEY BACK CLICKED");
    exit(0);
  }
}

void TitleMenu::onShareCode()
{
  SimpleAudioEngine::sharedEngine()->playEffect(cocos2d::CCFileUtils::sharedFileUtils()->fullPathForFilename("sound/click.mp3").c_str());
  showOpt(2);
}

void TitleMenu::setSceneIntro()
{
  doSceneIntro(mSceneIntro, this);
}

void TitleMenu::setSceneOutro(cocos2d::CCScene* newscene)
{
  if( mIntroFlag )
  {
    return;
  }

  mIntroFlag = true;

  mNewScene = doSceneOutro(newscene, mSceneIntro, (SEL_CallFunc)(&TitleMenu::doneOutro), this);
}

void TitleMenu::doneOutro()
{
  mIntroFlag = false;
cocos2d::CCDirector::sharedDirector()->replaceScene(mNewScene);
  mNewScene->release();
}

SEL_MenuHandler TitleMenu::onResolveCCBCCMenuItemSelector(cocos2d::CCObject * pTarget, const char* pSelectorName)
{
    /* TODO: uncomment this
CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "onLeaderboard", TitleMenu::onLeaderboard)
CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "onShowObjectives", TitleMenu::onShowObjectives)
CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "onOption", TitleMenu::onOption)
CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "onFacebook", TitleMenu::onFacebook)
CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "onTwitter", TitleMenu::onTwitter)
CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "onAchievement", TitleMenu::onAchievement)
CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "onShareCode", TitleMenu::onShareCode)
CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "onPlayClassic", TitleMenu::onPlayClassic)
CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "onCollections", TitleMenu::onCollections)
CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "onStore", TitleMenu::onStore)
CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "onExtra", TitleMenu::onExtra)
CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "onPlayArcade", TitleMenu::onPlayArcade)
CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "onMoreFun", TitleMenu::onMoreFun)
    */
     return NULL;
}

cocos2d::extension::Control::Handler TitleMenu::onResolveCCBCCControlSelector(cocos2d::Ref * pTarget, const char* pSelectorName)
{
cocos2d::CCLog("Control");
  return NULL;
}
bool TitleMenu::onAssignCCBMemberVariable(cocos2d::CCObject* pTarget, const char* pMemberVariableName, CCNode* pNode)
{
CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "mCloud1", CCSprite*, mCloud1)
CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "mCloud2", CCSprite*, mCloud2)
CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "mLogo", CCSprite*, mLogo)
CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "mStar1", CCSprite*, mStar1) 
CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "mStar2", CCSprite*, mStar2) 
CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "mStar3", CCSprite*, mStar3) 
CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "mStar4", CCSprite*, mStar4) 
CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "mStar5", CCSprite*, mStar5) 
CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "mCNew", CCSprite*, mCNew) 
CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "mNew", CCSprite*, mNew) 
CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "mSakuraNode", CCNode*, mSakuraNode)
CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "mSakuraNode2", CCNode*, mSakuraNode2)
CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "mEnemies", CCNode*, mEnemies)
CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "mMiniButton", CCMenu*, mMiniButton) 
CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "mMainButton", CCMenu*, mMainButton) 
CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "mOption", CCMenuItemImage*, mOption) 
CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "mTwitter", CCMenuItemImage*, mTwitter) 
CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "mMask", CCLayerColor*, mMask) 
cocos2d::CCLog(pMemberVariableName);

  return false;
}
