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

USING_NS_CC_EXT;
using namespace CocosDenshion;
bool gPopFlag = false;

//Title Style
//#define TITLESTYLE_HALLOWEEN
#define TITLESTYLE_SPRING


class PopOption :
  public Node/*<UIAlertViewDelegate>*/,
  public CCBMemberVariableAssigner,
  public CCBSelectorResolver
{
  public:
    //-- auto assign --
    cocos2d::MenuItemImage *mMusic;
    cocos2d::MenuItemImage *mSfx;
    cocos2d::MenuItemImage *mPushNotification;
    cocos2d::MenuItemImage *mTutorial;
    cocos2d::MenuItemImage *miCloud;

    //---- friends
    cocos2d::Label *mCoin;
    cocos2d::Node *mShadeCode;

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
    bool onAssignCCBMemberVariable(cocos2d::Ref* pTarget, const char* pMemberVariableName, Node* pNode)
    {
      CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "mMusic", MenuItemImage*, mMusic);
      CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "mSfx", MenuItemImage*, mSfx);
      CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "mPushNotification", MenuItemImage*, mPushNotification);
      CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "mTutorial", MenuItemImage*, mTutorial);
      CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "miCloud", MenuItemImage*, miCloud);
      CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "mCoin", Label*, mCoin);
      CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "mShadeCode", Node*, mShadeCode);
      return false;
    }

    SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::Ref * pTarget, const char* pSelectorName)
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

    cocos2d::extension::Control::Handler   onResolveCCBCCControlSelector(cocos2d::Ref * pTarget, const char* pSelectorName)
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
  cocos2d::Node *node = pReader->readNodeGraphFromFile("menu-titleoption.ccbi", this);
  pReader->release();

  this->addChild(node);

  //sync button state
  //sychornize music & sfx
  if( SimpleAudioEngine::sharedEngine()->getBackgroundMusicVolume() == 0 )
  {
    mMusic->setNormalImage(cocos2d::Sprite::createWithSpriteFrameName("settingoff.png"));
    mMusic->setSelectedImage(cocos2d::Sprite::createWithSpriteFrameName("settingoff.png"));
  }
  else {
    mMusic->setNormalImage(cocos2d::Sprite::createWithSpriteFrameName("settingon.png"));
    mMusic->setSelectedImage(cocos2d::Sprite::createWithSpriteFrameName("settingon.png"));
  }
  if( SimpleAudioEngine::sharedEngine()->getEffectsVolume() == 0 )
  {
    mSfx->setNormalImage(cocos2d::Sprite::createWithSpriteFrameName("settingoff.png"));
    mSfx->setSelectedImage(cocos2d::Sprite::createWithSpriteFrameName("settingoff.png"));
  }
  else {
    mSfx->setNormalImage(cocos2d::Sprite::createWithSpriteFrameName("settingon.png"));
    mSfx->setSelectedImage(cocos2d::Sprite::createWithSpriteFrameName("settingon.png"));
  }
  //sync push notification
  if( GameRecord::sharedGameRecord()->game_notify < 0 )
  {
    mPushNotification->setNormalImage(cocos2d::Sprite::createWithSpriteFrameName("settingoff.png"));
    mPushNotification->setSelectedImage(cocos2d::Sprite::createWithSpriteFrameName("settingoff.png"));
  }
  else {
    mPushNotification->setNormalImage(cocos2d::Sprite::createWithSpriteFrameName("settingon.png"));
    mPushNotification->setSelectedImage(cocos2d::Sprite::createWithSpriteFrameName("settingon.png"));
  }
  //sync tutorial
  if( GameRecord::sharedGameRecord()->game_tutorial == 0 )
  {
    mTutorial->setNormalImage(cocos2d::Sprite::createWithSpriteFrameName("settingon.png"));
    mTutorial->setSelectedImage(cocos2d::Sprite::createWithSpriteFrameName("settingon.png"));
  }
  else {
    mTutorial->setNormalImage(cocos2d::Sprite::createWithSpriteFrameName("settingoff.png"));
    mTutorial->setSelectedImage(cocos2d::Sprite::createWithSpriteFrameName("settingoff.png"));
  }
  //sync icloud
  if( GameRecord::sharedGameRecord()->setting_icloud > 0 )
  {
    miCloud->setNormalImage(cocos2d::Sprite::createWithSpriteFrameName("settingon.png"));
    miCloud->setSelectedImage(cocos2d::Sprite::createWithSpriteFrameName("settingon.png"));
  }
  else {
    miCloud->setNormalImage(cocos2d::Sprite::createWithSpriteFrameName("settingoff.png"));
    miCloud->setSelectedImage(cocos2d::Sprite::createWithSpriteFrameName("settingoff.png"));
  }
}

void PopOption::loadFriendGain()
{
  //feature removed from android
  //    Node *node = [CCBReader nodeGraphFromFile:"menu-titlefriendgain.ccb" owner:this);
  //    this->addChild(node);
  //    std::string coin = std::string_format("%d", FriendList->sharedFriendList()->bounsCount());
  //    mCoin->setString(coin);
}

void PopOption::loadFriendAdd()
{
  //feature removed from android
  //    Node *node = [CCBReader nodeGraphFromFile:"menu-titlefriendset.ccb" owner:this);
  //    this->addChild(node);
  //    std::string coin = std::string_format("%d", FriendList->sharedFriendList()->bounsCount());
  //    mCoin->setString(coin);
  //    Label *label = [Label, ABSystem->queryUserCode(), "Helvetica", 24);
  //    label->setAnchorPoint(cocos2d::Vec2(0, 0.5f));
  //    mShadeCode->addChild(label);
}

void PopOption::loadDelivery()
{
  //feature removed from android
  //    Node *sp = [ABDelivery->sharedDelivery(), this, @selector(onOpenDelivery), @selector(onCloseOption));
  //    sp->setPosition(
  //    this->addChild(sp);

  //  sp->scale(0);
  //  float sq0Duration[] = {0.2f, 0.1f, 0.05f, 0.05f};
  //  float sq0Scale[] = {1.2f, 0.9f, 1.1f, 1};
  //  Sequence *sq0 = createScaleSequence(sq0Duration, sq0Scale, 4);
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
//                GameTool::PlaySound("getcoin.mp3");
//                mCoin->setString(std::string_format("%d", FriendList->sharedFriendList()->bounsCount()));
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
//  Application::getInstance()->openURL(ABDelivery->sharedDelivery()->deliveryURL());
//    master->hideOpt();
//}

void PopOption::onCloseOption(cocos2d::Ref*)
{
  master->hideOpt();
}

void PopOption::onToggleSfx(cocos2d::Ref*)
{
  GameTool::PlaySound("click.mp3");
  if( GameRecord::sharedGameRecord()->setting_sfx )
  {
    GameRecord::sharedGameRecord()->setting_sfx = 0;
    mSfx->setNormalImage(cocos2d::Sprite::createWithSpriteFrameName("settingoff.png"));
    mSfx->setSelectedImage(cocos2d::Sprite::createWithSpriteFrameName("settingoff.png"));
    SimpleAudioEngine::sharedEngine()->setEffectsVolume(0);
  }
  else{
    GameRecord::sharedGameRecord()->setting_sfx = 1;
    mSfx->setNormalImage(cocos2d::Sprite::createWithSpriteFrameName("settingon.png"));
    mSfx->setSelectedImage(cocos2d::Sprite::createWithSpriteFrameName("settingon.png"));
    SimpleAudioEngine::sharedEngine()->setEffectsVolume(1);
  }
}

void PopOption::onToggleMusic(cocos2d::Ref*)
{
  GameTool::PlaySound("click.mp3");
  if( GameRecord::sharedGameRecord()->setting_music )
  {
    GameRecord::sharedGameRecord()->setting_music = 0;
    mMusic->setNormalImage(cocos2d::Sprite::createWithSpriteFrameName("settingoff.png"));
    mMusic->setSelectedImage(cocos2d::Sprite::createWithSpriteFrameName("settingoff.png"));
    SimpleAudioEngine::sharedEngine()->setBackgroundMusicVolume(0);
  }
  else{
    GameRecord::sharedGameRecord()->setting_music = 1;
    mMusic->setNormalImage(cocos2d::Sprite::createWithSpriteFrameName("settingon.png"));
    mMusic->setSelectedImage(cocos2d::Sprite::createWithSpriteFrameName("settingon.png"));
    SimpleAudioEngine::sharedEngine()->setBackgroundMusicVolume(1);
  }
}

void PopOption::onTogglePushNotification(cocos2d::Ref*)
{
  //feature removed from android
  //    GameTool::PlaySound("click.mp3");
  //    if( GameRecord::sharedGameRecord()->game_notify < 0 )
  //    {
  //        GameRecord::sharedGameRecord()->game_notify = 1;
  //        mPushNotification->setNormalImage(cocos2d::Sprite::createWithSpriteFrameName("settingon.png"));
  //        mPushNotification->setSelectedImage(cocos2d::Sprite::createWithSpriteFrameName("settingon.png"));
  //    }
  //    else {
  //        GameRecord::sharedGameRecord()->game_notify = -1;
  //        mPushNotification->setNormalImage(cocos2d::Sprite::createWithSpriteFrameName("settingoff.png"));
  //        mPushNotification->setSelectedImage(cocos2d::Sprite::createWithSpriteFrameName("settingoff.png"));
  //    }
  //    AppController *delgate = (AppController*)UIApplication->sharedApplication().delegate;
  //    delgate->scheduleLocalNotifuication();
}

void PopOption::onToggleTutorial(cocos2d::Ref*)
{
  GameTool::PlaySound("click.mp3");
  if( GameRecord::sharedGameRecord()->game_tutorial == 0 )
  {
    GameRecord::sharedGameRecord()->game_tutorial = 1;
    mTutorial->setNormalImage(cocos2d::Sprite::createWithSpriteFrameName("settingoff.png"));
    mTutorial->setSelectedImage(cocos2d::Sprite::createWithSpriteFrameName("settingoff.png"));
  }
  else {
    GameRecord::sharedGameRecord()->game_tutorial = 0;
    mTutorial->setNormalImage(cocos2d::Sprite::createWithSpriteFrameName("settingon.png"));
    mTutorial->setSelectedImage(cocos2d::Sprite::createWithSpriteFrameName("settingon.png"));
  }
}

void PopOption::onToggleiCloud(cocos2d::Ref*)
{
  //feature removed from android
  //    GameTool::PlaySound("click.mp3");
  //    if( GameRecord::sharedGameRecord()->setting_icloud > 0 )
  //    {
  //        GameRecord::sharedGameRecord()->setting_icloud = 0;
  //        miCloud->setNormalImage(cocos2d::Sprite::createWithSpriteFrameName("settingoff.png"));
  //        miCloud->setSelectedImage(cocos2d::Sprite::createWithSpriteFrameName("settingoff.png"));
  //    }
  //    else {
  //        GameRecord::sharedGameRecord()->setting_icloud = 1;
  //        miCloud->setNormalImage(cocos2d::Sprite::createWithSpriteFrameName("settingon.png"));
  //        miCloud->setSelectedImage(cocos2d::Sprite::createWithSpriteFrameName("settingon.png"));
  //    }
}

void PopOption::onRateUs(cocos2d::Ref*)
{
  //feature removed from android
  std::string str = "itms-apps://ax.itunes.apple.com/WebObjects/MZStore.woa/wa/viewContentsUserReviews?type=Purple+Software&id=540290969";
  Application::getInstance()->openURL(str);
  //    
  //    GameRecord::sharedGameRecord()->rate_pop = -1;
}

class PopObj :
  public Node,
  public CCBMemberVariableAssigner,
  public CCBSelectorResolver
{
  public:
    CEClipedNode *mBoard;
    cocos2d::Node *mRode;
    float mTimer;
    bool mFlag;
    //-- auto assign --
    cocos2d::Label *mDailyObjective;
    cocos2d::Label *mWeeklyObjective;
    cocos2d::Label *mMonthlyObjective;
    cocos2d::Node *mDailyCrown;
    cocos2d::Node *mWeeklyCrown;
    cocos2d::Node *mMonthlyCrown;
    cocos2d::Sprite *mDailyIcon;
    cocos2d::Sprite *mWeeklyIcon;
    cocos2d::Sprite *mMonthlyIcon;
    cocos2d::Node *mClassic;
    cocos2d::Node *mArcade;
    cocos2d::Sprite *mGoldCoin;
    cocos2d::Sprite *mSilverCoin;
    cocos2d::Sprite *mBronzeCoin;
    cocos2d::Label *mGoldScore;
    cocos2d::Label *mGoldPrize;
    cocos2d::Label *mSilverScore;
    cocos2d::Label *mSilverPrize;
    cocos2d::Label *mBronzeScore;
    cocos2d::Label *mBronzePrize;
    cocos2d::CCMenu *mMenu;
    cocos2d::MenuItemImage *mSwitch;
    cocos2d::MenuItemImage *mSwitch2;

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
    bool onAssignCCBMemberVariable(cocos2d::Ref* pTarget, const char* pMemberVariableName, Node* pNode)
    {
      CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "mDailyObjective", Label*, mDailyObjective);
      CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "mWeeklyObjective", Label*, mWeeklyObjective);
      CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "mMonthlyObjective", Label*, mMonthlyObjective);
      CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "mDailyCrown", Node*, mDailyCrown);
      CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "mWeeklyCrown", Node*, mWeeklyCrown);
      CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "mMonthlyCrown", Node*, mMonthlyCrown);
      CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "mDailyIcon", Sprite*, mDailyIcon);
      CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "mWeeklyIcon", Sprite*, mWeeklyIcon);
      CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "mMonthlyIcon", Sprite*, mMonthlyIcon);
      CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "mClassic", Node*, mClassic);
      CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "mArcade", Node*, mArcade);
      CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "mGoldCoin", Sprite*, mGoldCoin);
      CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "mSilverCoin", Sprite*, mSilverCoin);
      CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "mBronzeCoin", Sprite*, mBronzeCoin);
      CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "mGoldScore", Label*, mGoldScore);
      CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "mGoldPrize", Label*, mGoldPrize);
      CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "mSilverScore", Label*, mSilverScore);
      CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "mSilverPrize", Label*, mSilverPrize);
      CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "mBronzeScore", Label*, mBronzeScore);
      CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "mBronzePrize", Label*, mBronzePrize);
      CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "mMenu", CCMenu*, mMenu);
      CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "mSwitch", MenuItemImage*, mSwitch);
      CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "mSwitch2", MenuItemImage*, mSwitch2);
      return false;
    }

    SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::Ref * pTarget, const char* pSelectorName)
    {
         CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "onClose", PopObj::onClose);
         CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "onChangeDisplay", PopObj::onChangeDisplay);
         return NULL;
    }

    cocos2d::extension::Control::Handler onResolveCCBCCControlSelector(cocos2d::Ref * pTarget, const char* pSelectorName)
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
  //update objectives TODO:refactor this
  cocos2d::Label* labels[] = { mDailyObjective, mWeeklyObjective, mMonthlyObjective };
  cocos2d::Sprite* icons[] = { mDailyIcon, mWeeklyIcon, mMonthlyIcon };
	cocos2d::Node* crowns[] = {mDailyCrown, mWeeklyCrown, mMonthlyCrown};
	GameTool::UpdateObjectives(labels,icons,crowns);
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
      mGoldScore->setColor(Color3B(128, 128, 128));
    }
    else
    {//not completed
      mGoldScore->setString(std::string_format("%d", GameRecord::sharedGameRecord()->task->goldPrize->score).c_str());
      mGoldPrize->setString(std::string_format("%d", GameRecord::sharedGameRecord()->task->goldPrize->prize).c_str());
    }
  }
  //silver
  {
    if( GameRecord::sharedGameRecord()->task->silverPrize->prize < 0 )
    {//completed
      mSilverCoin->setVisible(false);
      mSilverPrize->setVisible(false);
      mSilverScore->setString("已完成！");
      mSilverScore->setColor(Color3B(128, 128, 128));
    }
    else
    {//not completed
      mSilverScore->setString(std::string_format("%d", GameRecord::sharedGameRecord()->task->silverPrize->score).c_str());
      mSilverPrize->setString(std::string_format("%d", GameRecord::sharedGameRecord()->task->silverPrize->prize).c_str());
    }
  }
  //bronze
  {
    if( GameRecord::sharedGameRecord()->task->bronzePrize->prize < 0 )
    {//completed
      mBronzeCoin->setVisible(false);
      mBronzePrize->setVisible(false);
      mBronzeScore->setString("已完成！");
      mBronzeScore->setColor(Color3B(128, 128, 128));
    }
    else
    {//not completed
      mBronzeScore->setString(std::string_format("%d", GameRecord::sharedGameRecord()->task->bronzePrize->score).c_str());
      mBronzePrize->setString(std::string_format("%d", GameRecord::sharedGameRecord()->task->bronzePrize->prize).c_str());
    }
  } 
}

#define OBJECT_BOADER_OFFSET 90
#define OBJECT_BEGEN_X_POS -60
void PopObj::onCreate()
{
  
  mBoard = CEClipedNode::create();
  mBoard->setPosition(cocos2d::Vec2(-301+381, 58));
  cocos2d::CCRect rect = UniversalFit::sharedUniversalFit()->transformRect(cocos2d::Rect(OBJECT_BOADER_OFFSET, 0, SCREEN_WIDTH-OBJECT_BOADER_OFFSET, SCREEN_HEIGHT));
  //CCLog("CLIP = %fx%f %fx%f", rect.origin.x, rect.origin.y, rect.size.width, rect.size.height);
  mBoard->setClipRect(&rect);
  
  cocos2d::Node * node = createUIByCCBI("menu-titlepop", "TitleMenu", TitleMenuLayerLoader::loader(), this);
  mBoard->addChild(node);
  node->setPosition(cocos2d::Vec2(OBJECT_BEGEN_X_POS, 0));
  this->addChild(mBoard);

  mRode = cocos2d::Sprite::createWithSpriteFrameName("index_jz.png");
  mRode->setPosition(cocos2d::Vec2(70, 170));
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
  cocos2d::CCMoveTo *mt = cocos2d::CCMoveTo::create(0.2f, Vec2(0, 0));
  cocos2d::CCCallFunc *cf = cocos2d::CCCallFunc::create(this, callfunc_selector(PopObj::onDoneAnimation));
  cocos2d::Sequence *seq = cocos2d::Sequence::createWithTwoActions(mt, cf);

  node->runAction(seq);
  mSwitch->setVisible(false);
  mSwitch2->setVisible(false);
}

void PopObj::onDoneAnimation()
{
  mBoard->setPosition(cocos2d::Vec2( -301 + 381, 58));
  mMenu->setVisible(true);
  cocos2d::CCRect rect = UniversalFit::sharedUniversalFit()->transformRect(cocos2d::Rect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT));
  mBoard->setClipRect(&rect);
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
  //CCRect rect = cocos2d::Rect(60, 0, SCREEN_WIDTH-60, SCREEN_HEIGHT);
  cocos2d::CCRect rect = UniversalFit::sharedUniversalFit()->transformRect(cocos2d::Rect(OBJECT_BOADER_OFFSET, 0, SCREEN_WIDTH-OBJECT_BOADER_OFFSET, SCREEN_HEIGHT));
  mBoard->setClipRect(&rect);
  cocos2d::CCMoveBy *mt1 = cocos2d::MoveBy::create(0.2f, Vec2(-301,0));
  cocos2d::CCCallFunc *cf1 = cocos2d::CCCallFunc::create(this, (callfunc_selector(PopObj::onFlip)));
  cocos2d::CCMoveBy *mt2 = cocos2d::MoveBy::create(0.2f, Vec2(0,0));
  cocos2d::CCCallFunc *cf2 = cocos2d::CCCallFunc::create(this, (callfunc_selector(PopObj::onDoneAnimation)));
  Sequence *seq = cocos2d::Sequence::create(mt1, cf1, mt2, cf2, nullptr);

  mBoard->runAction(seq);

  mSwitch->setVisible(false);
  mSwitch2->setVisible(false);
  GameTool::PlaySound("flip.mp3");
}

#ifdef TITLESTYLE_SPRING
Sakura* Sakura::create(cocos2d::Node *parent)
{
  Sakura *ret = new Sakura;
  ret->init();
  ret->mSprite = cocos2d::Sprite::createWithSpriteFrameName("scene-snow.png");
  ret->mSprite->setPosition(cocos2d::Vec2((SCREEN_WIDTH+300)*CCRANDOM_0_1(), SCREEN_HEIGHT+50 ));
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
  if ( !Layer::init() )
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

  mUISwapper.onEnter();
  //enable keypad for back button
  this->setKeypadEnabled(true);
  cocos2d::Node * node = createUIByCCBI("menu-title", "TitleMenu", TitleMenuLayerLoader::loader(), this);
  if(node != NULL) {
    this->addChild(node);
  }
  //select SinaWeibo / Twitter
  if(UniversalFit::shouldUsingSinaWeibo())
  {
    mTwitter->setNormalSpriteFrame(GameTool::getSpriteFrameByName("contact3.png"));
    mTwitter->setSelectedSpriteFrame(GameTool::getSpriteFrameByName("contact3.png"));
  }

  for(int i=0; i<RUN; ++i)
  {
    mAni[i] = NULL;
  }

  mLogo->runAction(cocos2d::Sequence::create(
      cocos2d::ScaleTo::create(0.2f, 1.2f),
      cocos2d::ScaleTo::create(0.2f, 1.0f),
      nullptr
      ));

  mMainButton->runAction(cocos2d::Sequence::create(
        cocos2d::DelayTime::create(0.6f),
        cocos2d::CCShow::create(),
        nullptr
        ));

  mSakura = cocos2d::CCArray::create();
  mSakura->retain();
  mWind = 0;

  
  
    GameTool::PlayBackgroundMusic("menu.mp3");
  

     if(mUISwapper.isDone())
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
  mUISwapper.setSceneIntro(this);

  //TODO    [GameCenterController->sharedGameCenterController() loadFriendsLeaderboard);
  mNew->setVisible(false);
  if( GameRecord::sharedGameRecord()->task->newrefresh )
  {
    mNew->runAction(cocos2d::Sequence::create(
          cocos2d::DelayTime::create(1.0f),
          cocos2d::Show::create(),
          cocos2d::ScaleTo::create(0.2f, 1.5f),
          cocos2d::ScaleTo::create(0.1f, 1.0f),
          nullptr
          ));
  }

  //check new collections
  mCNew->setVisible(false);
  if( GameRecord::sharedGameRecord()->collection->newlist->count() > 0 )
  {
    mCNew->runAction(cocos2d::Sequence::create(
          cocos2d::DelayTime::create(1.5f),
          cocos2d::Show::create(),
          cocos2d::ScaleTo::create(0.2f, 1.5f),
          cocos2d::ScaleTo::create(0.1f, 1.0f),
          nullptr
          ));
  }

  for(int i=0; i<5; ++i)
  {
    mStarTimers[i] = 0;
  }

  //    //show friends
  //    if( FriendList->sharedFriendList()->canHaveBouns() )
  //    {
  //        [this showOpt:1);
  //        GameTool::PlaySound("getcoin.mp3");
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
  cocos2d::Layer::onEnter();
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
  GameTool::PlaySound("click.mp3");
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
  GameTool::PlaySound("click.mp3");
  mMask->setVisible(false);
  mMask->removeAllChildrenWithCleanup(true);

  mMainButton->setTouchEnabled(true);
  mMiniButton->setTouchEnabled(true);

  mOpt = NULL;
}
//
//Sprite* TitleMenu::int nid()
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
  for(int i=0; i<RUN; ++i) {
    if( mAni[i] == NULL ) {
      if( CCRANDOM_0_1() < 0.0005f ) {
        const char *enm = NULL;
        switch (randomInt(3)) {
          case 0: enm = "enemy"; break;
          case 1: enm = "mninja"; break;
          case 2: enm = "hninja"; break;
        }
        mAni[i] = GTAnimatedSprite::spriteWithGTAnimation(GTAnimation::loadedAnimationSet(enm));
        mAni[i]->setAnchorPoint(cocos2d::Vec2(0.4f, 0.0625f));
        mAni[i]->playGTAnimation(0, true);
        mAni[i]->setPosition(cocos2d::Vec2(-50, 64 + CCRANDOM_0_1()*36));
        mEnemies->addChild(mAni[i], 5);
        mSpeed[i] = 200 + CCRANDOM_0_1()*100;
      }
    } else {
      mAni[i]->updateGTAnimation(delta);
      cocos2d::Point np = mAni[i]->getPosition();
      np.x += delta*mSpeed[i];
      mAni[i]->setPosition(np);
      if( np.x > SCREEN_WIDTH + 50 ) {
        this->removeChild(mAni[i], true);
        mAni[i] = NULL;
      }
    }
  }

  //  if( mPop == NULL && mOpt == NULL )
  //  {
  //      if( !gPopFlag && ABDelivery->sharedDelivery()->isReadyForDelivery() )
  //      {
  //            mPopTimer += delta;
  //            if( mPopTimer > 2 )
  //            {
  //                mPopTimer = 0;
  //                if( randomInt(10) == 0 )
  //                {
  //                    [this showOpt:3);
  //                    gPopFlag = true;
  //                }
  //            }
  //            [this showOpt:3);
  //          gPopFlag = true;
  //      }
  //  }
}

void TitleMenu::onExit()
{
  removeAllChildrenWithCleanup(true);
  PublicLoad::menuTitle()->unloadAll();
  cocos2d::Layer::onExit();
}

void TitleMenu::onPlayClassic(cocos2d::Ref*)
{
  cocos2d::CCLog("Player");
  GamePlay::sharedGamePlay()->setGameMode(MODE_CLASSIC);
  GameTool::PlaySound("menu-change.mp3");
  mUISwapper.setSceneOutro(GameTool::scene<SelectMenu>(), this);
}

void TitleMenu::onPlayArcade(cocos2d::Ref*)
{
  GamePlay::sharedGamePlay()->setGameMode(MODE_ARCADE);
  GameTool::PlaySound("menu-change.mp3");
  mUISwapper.setSceneOutro(GameTool::scene<SelectMenu>(), this);
}

void TitleMenu::onCollections(cocos2d::Ref*)
{
  CollectionMenu::setNavBack(0);
  GameTool::PlaySound("menu-change.mp3");
  mUISwapper.setSceneOutro(GameTool::scene<CollectionMenu>(), this);
}

void TitleMenu::onStore(cocos2d::Ref*) {
  ShopMenu::setNavBack(0);
  GameTool::PlaySound("menu-change.mp3");
  mUISwapper.setSceneOutro(GameTool::scene<ShopMenu>(), this);
}

void TitleMenu::onFacebook(cocos2d::Ref*) {
  GameTool::PlaySound("menu-change.mp3");
  Application::getInstance()->openURL(std::string("http://www.facebook.com/pages/Little-Ninja-Rush/397668056947647"));
}

void TitleMenu::onTwitter(cocos2d::Ref*) {
  GameTool::PlaySound("menu-change.mp3");
  if( UniversalFit::sharedUniversalFit()->shouldUsingSinaWeibo() ) {
    Application::getInstance()->openURL(std::string("http://weibo.com/u/2810380794"));
  } else {
    Application::getInstance()->openURL(std::string("https://twitter.com/TrinGame"));
  }
}

void TitleMenu::onLeaderboard(cocos2d::Ref * node)
{
  GameTool::PlaySound("click.mp3");
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

void TitleMenu::onAchievement(cocos2d::Ref*)
{
  GameTool::PlaySound("click.mp3");
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

void TitleMenu::onExtra(cocos2d::Ref*)
{
  GameTool::PlaySound("menu-change.mp3");
  mUISwapper.setSceneOutro(GameTool::scene<ExtraSelect>(), this);
}

void TitleMenu::onShowObjectives(cocos2d::Ref *)
{
  GameTool::PlaySound("click.mp3");
  this->showObjs();
}

void TitleMenu::onOption(cocos2d::Ref*)
{
  GameTool::PlaySound("click.mp3");
  showOpt(0);
}

void TitleMenu::onMoreFun(cocos2d::Ref*)
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

void TitleMenu::onShareCode(cocos2d::Ref*)
{
  GameTool::PlaySound("click.mp3");
  showOpt(2);
}

SEL_MenuHandler TitleMenu::onResolveCCBCCMenuItemSelector(cocos2d::Ref * pTarget, const char* pSelectorName)
{
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
   return NULL;
}

cocos2d::extension::Control::Handler TitleMenu::onResolveCCBCCControlSelector(cocos2d::Ref * pTarget, const char* pSelectorName)
{
  return NULL;
}

bool TitleMenu::onAssignCCBMemberVariable(cocos2d::Ref* pTarget, const char* pMemberVariableName, Node* pNode)
{
  CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "mCloud1", Sprite*, mCloud1)
  CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "mCloud2", Sprite*, mCloud2)
  CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "mLogo", Sprite*, mLogo)
  CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "mStar1", Sprite*, mStar1) 
  CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "mStar2", Sprite*, mStar2) 
  CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "mStar3", Sprite*, mStar3) 
  CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "mStar4", Sprite*, mStar4) 
  CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "mStar5", Sprite*, mStar5) 
  CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "mCNew", Sprite*, mCNew) 
  CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "mNew", Sprite*, mNew) 
  CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "mSakuraNode", Node*, mSakuraNode)
  CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "mSakuraNode2", Node*, mSakuraNode2)
  CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "mEnemies", Node*, mEnemies)
  CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "mMiniButton", CCMenu*, mMiniButton) 
  CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "mMainButton", CCMenu*, mMainButton) 
  CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "mOption", MenuItemImage*, mOption) 
  CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "mTwitter", MenuItemImage*, mTwitter) 
  CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "mMask", LayerColor*, mMask)

  return true;
}
