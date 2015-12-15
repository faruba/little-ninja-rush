#ifndef ___CLASSES_GAMEPLAY_H_
#define ___CLASSES_GAMEPLAY_H_
#include "cocos2d.h"
#include "GameTool.h"
#include "GameConstants.h"
#include "ArcadeMode.h"
#include "ClassicMode.h"
#include "GameScene.h"
#include "MainRole.h"
#include "GameInterface.h"
#include "Dart.h"
#include "FootPrint.h"

#define OP_RESET (0)

class GamePlay : 
  public cocos2d::CCLayer
{
  public:
    ~GamePlay();
    virtual bool init();
cocos2d::CCScene* scene();
CCB_STATIC_NEW_AUTORELEASE_OBJECT_WITH_INIT_METHOD(GamePlay, create);

    virtual void onEnter();
    virtual void update(float);
    virtual void onExit();

    virtual bool onTouchBegan(Touch * touch, Event * event);
    virtual void onTouchMoved(Touch * touch, Event * event);
    virtual void onTouchEnded(Touch * touch, Event * event);
    virtual void onTouchCancelled(Touch * touch, Event * event);
    virtual void onAcceleration(Acceleration* acc, Event* unsedEvent);
    static GamePlay *sharedGamePlay();

    void callBack() {GamePlay::sharedGamePlay()->processNotificationQueue();};
    void startLoopFever();
    void stopLoopFever();

    void taskCompleted(cocos2d::CCString * tile, CCString * icon, int type); 
    void pieceComplete(cocos2d::CCString * title, CCString * icon);
    void popText(cocos2d::CCString* text);
    void scheduleSpellRelease(cocos2d::CCNode* target, SEL_CallFunc sel);
    bool isPlayingArcade();
    void setGameMode(int mod);
    int getGameMode();

    void initGamePlay(int mod);
    void resetGame();
cocos2d::CCLayer* ui();

    void fixGravity(float val);

    void processNotificationQueue();


    //游戏控制
    void pause(cocos2d::Ref*);
    void resume();
    void restart();
    void exit();
    void change();

    void clearFootPrints();

    //死亡处理
    void triggerDeath();

    void togglePauseButton(bool flag);

    //模式：0-线性恢复 1-突变 2-渐变
    void scheduleSpeed(float target, float time, int slowmode);
    void unscheduleSpeed();
    void increaseScheduleCounter();
    void decreaseScheduleCounter();

    //模式：0-突变 1-突变淡出 2-淡入
    void scheduleMask(ccColor3B target, float time, int maskmode);
    void unscheduleMask();


    void scheduleVibrate(float dur);

    void slide(cocos2d::Point dir);
    cocos2d::Point autoAim(cocos2d::Point dir);
    Role *nearestEnemy(cocos2d::Point dir);

    void operate();

    //combo系统
    void makeCombo();
    void refreshCombo();
    void stopCombo();

    void setFeverLevel(int lv);

    //intro&outro
    void setSceneIntro();
    void setSceneOutro(cocos2d::CCScene* newscene);
    void doneOutro();
    void doneIntro();

    static void setTaskCompleteNode(cocos2d::CCNode* node);
    bool completeSomeObjectives();

    //0-daily 1-weekly 2-monthly 3-achievement 4-collection 5-text
    static void pushNotification(cocos2d::CCString* name, CCString* icon, int type);

    void stepDust(cocos2d::Point pos);
    cocos2d::Point mTouchBegin;
    bool  mTouchProcessed;
cocos2d::CCLayer *mUI;
cocos2d::CCMenu *mMenu;//菜单项目

    float mFeverAdd;//因为fever游戏加速的程度
    int mFeverBegin;
cocos2d::CCLayerColor *mFeverMask;
    float mFeverMaskTimer;

    float mScheduleSpeedOrigin;
    float mScheduleSpeedTarget;
    float mScheduleSpeedTime;
    float mScheduleSpeedTimer;
    int   mScheduleSpeedMode;
    int   mScheduleSpeedCounter;

    float mScheduleSpellRelease;
cocos2d::CCNode*      mScheduleReleaseTarget;
    SEL_CallFunc mScheduleReleaseSelector;

    float mScheduleMaskTime;
    float mScheduleMaskTimer;
    int mScheduleMaskMode;

    float mScheduleVibrate;
    float mVibrateTimer;
    cocos2d::Point mVibrateDir;

    //intro&outro
cocos2d::CCNode *mSceneIntro;
cocos2d::CCScene *mNewScene;
    bool mIntroFlag;

    //CDSoundSource *mFeverLoop;

    float mSpeedlineTimer;
    int mFeverLevel;
    GameObjectManager *manager;
    GameScene *tiles;
cocos2d::CCArray *enemies;
cocos2d::CCArray *darts;
    MainRole *mainrole;
    MainRole *mainrole2;//分身
    GameInterface *interface;
    int state;//游戏状态

    int mode;//游戏模式
cocos2d::CCLayerColor *mask;
cocos2d::CCLayerColor *spellMask;
cocos2d::CCSpriteBatchNode *footprints;

    //Arcade Mode
    ArcadeMode *arcade;
    ClassicMode *classic;

    //运行时参数
    float levelspeed;//当前关卡速度
    float runspeed;//奔跑速度
    float gamespeed;//游戏整体速度
    float fixedgamespeed;//原本游戏的速度
    float distance;//奔跑距离
    float difficulty;//难度
    int   scenecount;//场景计数
    int   maxenemy;//最大敌人数量
    int   combo;//连击
    float comboTimer;//连击计时
    int   coins;//金币计数
    int   scrolls;//卷轴数计数
    bool paused;
    float changeflag;
    float deltaTime;
    float deltaDistance;
    int substate;
    float stateTimer;
    int operation;
    float gameOverTimer;
    bool canPause;
    bool doubleCoin;
    bool doubleCoinIAP;
    int roleId;
    int festivalPieces;
    bool spelling;
    int reviveCount;
    float arcadeSpeed;

    //achievement counter
    int sceneSpell;
    int gameSpell;
    int killingCount;
    int totalScorll;
    int reflectKill;
    int killJoninCri;
    int wounded;
    bool getitem;
    bool missshoot;
    int runwithoutkill;
    int runwithoutfire;
    int maxfever;
    int runwithoutblade;
    int meetchristmas;
    int killpumpkins;
    int killsamurai;
    int firecount;
    int pumpkinspell;

    //游戏状态标记
    bool flag_samurai;//是否有武士

    int count_control;//是否可以控制
    int count_attack;//是否攻击玩家
    int count_respawn;//是否刷怪
    int count_runscene;//是否滚动场景
};

#endif
