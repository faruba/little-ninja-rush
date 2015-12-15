#include "GamePlay.h"
#include "SimpleAudioEngine.h"
#include "CEClipedNode.h"
#include "AnimationRes.h"
#include "GameRecord.h"
#include "PauseMenuClassic.h"
#include "Tasks.h"
#include "GameData.h"
#include "Loading.h"
#include "TitleMenu.h"
#include "SelectMenu.h"
#include "PublicLoad.h"
#include "GameScene.h"
#include "TutorialDirector.h"
#include "FeverBrust.h"
#include "AnimatedParticle.h"
#include "StaticParticle.h"
#include "UniversalFit.h"
#include "SpeedLine.h"

GamePlay* gPlay = NULL;
float gGravityFix = 1;
bool gArcadePlaying = false;

CCNode *gTaskComplete = NULL;

int gGameMode = MODE_CLASSIC;
/* PopQueue */
class PopQueue : 
  public CCObject
{
  public:
    CREATE_FUNC(PopQueue);

    bool init(){return true;};

    CCString *title;
    CCString *icon;
    int type;
};


CCArray *gPopQueues = NULL;
#define VIBRATE_PERIOD (0.04f)
#define VIBRATE_ZOOM (0.2f)
#define VIBRATE_MS (400.0f)
#define VIBRATE_MOVE (3.0f)

GamePlay::~GamePlay()
{
    gPlay = NULL;
}

CCScene* GamePlay::scene()
{
    CCScene *ret = CCScene::create();
    
    if( gPlay == NULL )
    {
        gPlay = GamePlay::create();
    }
    
    //universal
    ret->setScale(UniversalFit::sharedUniversalFit()->scaleFactor);
    ret->setAnchorPoint(ccp(0, 0));
    ret->setPosition(ccp(0, UniversalFit::sharedUniversalFit()->sceneOffset.y));
    
    CEClipedNode *clip = CEClipedNode::create();
    clip->setClipRect(&(UniversalFit::sharedUniversalFit()->clipRect));
    
    clip->addChild(gPlay);
    
    ret->addChild(clip);
    return ret;
}

GamePlay* GamePlay::sharedGamePlay()
{
    if( gPlay == NULL )
    {
        gPlay = GamePlay::create();
        gPlay->retain();
    }
    return gPlay;
}

bool GamePlay::isPlayingArcade()
{
    return gArcadePlaying;
}

void GamePlay::setGameMode(int mod)
{
    gGameMode = mod;
}

int GamePlay::getGameMode()
{
    return gGameMode;
}

bool GamePlay::init()
{
  if(CCLayer::init())
  {
    this->scheduleUpdate();
    this->setTouchEnabled(true);
    this->setAccelerometerEnabled(true);
    state = -1;

    //初始化方向 TODO:
    //        UIInterfaceOrientation orientation = UIApplication->sharedApplication()->statusBarOrientation();
    //        CCLog("Orientation = %d", orientation);
    //        if(orientation == UIInterfaceOrientationLandscapeLeft)
    //        {
    //            GamePlay::fixGravity(1);
    //        }
    //        else if (orientation == UIInterfaceOrientationLandscapeRight) {
    //            GamePlay::fixGravity(-1);
    //        }

    mSceneIntro = NULL;
    return true;
  }
  return false;
}

void GamePlay::onEnter()
{
  setAccelerometerEnabled(true);
  CCNode *taskcomplete = CCNode::create();
  taskcomplete->setPosition(ccp(UniversalFit::sharedUniversalFit()->playSize.width/2, SCREEN_HEIGHT));
  this->addChild(taskcomplete, LAYER_MASK+10);
  setTaskCompleteNode(taskcomplete);

  this->initGamePlay(gGameMode);

  this->setSceneIntro();

  CCLayer::onEnter();
}

void GamePlay::onExit()
{
  gArcadePlaying = false;
  setTaskCompleteNode(NULL);

  //demo -->
  gPlay->release();
  //demo <--

  CCLayer::onExit();
}

//初始化游戏
void GamePlay::initGamePlay(int mod)
{
  if( mod == MODE_CLASSIC )
  {
    gArcadePlaying = false;
  }
  else
  {
    gArcadePlaying = true;
  }

  //init manager
  manager = GameObjectManager::create();
  manager->retain();
  //enemies
  enemies = CCArray::create();
  darts = CCArray::create();
  enemies->retain();
  darts->retain();
  footprints = NULL;
  arcadeSpeed = 1;
  
  mask = CCLayerColor::create(ccc4(0, 0, 0, 255), UniversalFit::sharedUniversalFit()->screenSize.width + 20, UniversalFit::sharedUniversalFit()->screenSize.height + 20);
  mask->setPosition(ccp(-10, -10));
  mask->retain();
  this->addChild(mask, LAYER_MASK);
  
  mFeverMask = CCLayerColor::create(ccc4(239, 145, 3, 255));
  mFeverMask->setVisible(false);
  mFeverMask->setOpacity(0);
  mFeverMask->retain();
  this->addChild(mFeverMask, LAYER_UI-1);
  
  spellMask = CCLayerColor::create(ccc4(0, 0, 0, 200), UniversalFit::sharedUniversalFit()->playSize.width+VIBRATE_MOVE*3, SCREEN_HEIGHT+VIBRATE_MOVE*3);
  spellMask->retain();
  spellMask->setVisible(false);
  this->addChild(spellMask, LAYER_MAINROLE-2);
  
  paused = false;
  mUI = CCLayer::create();
  mUI->setTouchEnabled(true);
  this->addChild(mUI, LAYER_MASK+1);
  mode = mod;
  
  mScheduleSpeedOrigin = 1;
  mScheduleSpeedTarget = 1;
  mScheduleSpeedMode = -1;
  mScheduleSpeedTime = 0;
  mScheduleSpeedTimer = 0;
  
  mScheduleSpellRelease = -1;
  
  mScheduleMaskMode = -1;
  mScheduleMaskTime = 0;
  mScheduleMaskTimer = 0;
  
  mScheduleVibrate = -1;
  
  //增加菜单
  CCMenuItemImage *xpause = CCMenuItemImage::create();
  xpause->setNormalSpriteFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("pause.png"));
  xpause->setSelectedSpriteFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("pause.png"));
  xpause->setTarget(this, menu_selector(GamePlay::pause));
  xpause->setAnchorPoint(ccp(1, 1));
  xpause->setPosition(ccp(UniversalFit::sharedUniversalFit()->playSize.width, SCREEN_HEIGHT));
  this->mMenu = CCMenu::create();
  this->mMenu->addChild(xpause);
  mMenu->setPosition(ccp(-7, -7));
  mUI->addChild(mMenu, LAYER_UI);
  
  this->resetGame();
  
  operation = -1;
}

void GamePlay::update(float delta)
{
  deltaTime = delta;
  if(paused)
  {
    delta = 0;
  }

  //schedule spellmask
  if( mScheduleSpellRelease >= 0 )
  {
    mScheduleSpellRelease -= delta;
    delta = 0;
    if( mScheduleSpellRelease < 0 )
    {
      spelling = false;
      spellMask->setVisible(false);
      if( mScheduleReleaseTarget != NULL )
      {
        CCCallFunc *callSelectorAction = CCCallFunc::create(mScheduleReleaseTarget, mScheduleReleaseSelector);
        runAction(callSelectorAction);
        mScheduleReleaseTarget = NULL;
      }
    }
  }
  //schedule mask
  if( mScheduleMaskMode >= 0 )
  {
    mScheduleMaskTimer += delta;
    if( mScheduleMaskTimer >= mScheduleMaskTime )
    {
      this->unscheduleMask();
    }
    else {
      if( mScheduleMaskMode == 1 )
      {
        float k = 1.0f - mScheduleMaskTimer/mScheduleMaskTime;
        mask->setOpacity(255*k);
      }
      else if( mScheduleMaskMode == 2 )
      {
        float k = mScheduleMaskTimer/mScheduleMaskTime;
        mask->setOpacity(255*k);
      }
    }
  }
  //fever mask
  if( mFeverMaskTimer >= 0 )
  {
    mFeverMaskTimer+= delta;
    float circle = 0.5f;
    float min = 0;
    float max = 40;
    int n = mFeverMaskTimer/circle;
    float left = mFeverMaskTimer - circle*n;
    if( left < circle/2 )
    {
      mFeverMask->setOpacity(min + left/(circle/2)*max);
    }
    else {
      mFeverMask->setOpacity(min + (circle - left)/(circle/2)*max);
    }
  }
  //scheduled vibrate
  if( mScheduleVibrate >= 0 )
  {
    mScheduleVibrate -= deltaTime;
    //do scale
    mVibrateTimer += deltaTime;
    int n = mVibrateTimer/VIBRATE_PERIOD;
    float sc = mVibrateTimer - VIBRATE_PERIOD*n;
    float k = 1;
    if( sc < VIBRATE_PERIOD/2 )
    {
      k += VIBRATE_ZOOM*sc/VIBRATE_PERIOD/2;
    }
    else {
      k += VIBRATE_ZOOM*(VIBRATE_PERIOD - sc)/VIBRATE_PERIOD/2;
    }
    this->setScale(k);
    //do roll
    float st = deltaTime*VIBRATE_MS;
    CCPoint np = this->getPosition();
    np = ccpAdd(np, ccpMult(mVibrateDir, st));
    if( np.x < -VIBRATE_MOVE )
    {
      np.x = -VIBRATE_MOVE;
      mVibrateDir.x *= -1;
    }
    if( np.x > VIBRATE_MOVE )
    {
      np.x = VIBRATE_MOVE;
      mVibrateDir.x *= -1;
    }
    if( np.y < -VIBRATE_MOVE )
    {
      np.y = -VIBRATE_MOVE;
      mVibrateDir.y *= -1;
    }
    if( np.y > VIBRATE_MOVE )
    {
      np.y = VIBRATE_MOVE;
      mVibrateDir.y *= -1;
    }
    this->setPosition(np);

    if( mScheduleVibrate < 0 )
    {
      //clean up
      this->setScale(1);
      this->setPosition(ccp(0, 0));
    }
  }
  //scheduled slow
  if( mScheduleSpeedMode >= 0 )
  {
    mScheduleSpeedTimer += delta;
    if( mScheduleSpeedTimer >= mScheduleSpeedTime && mScheduleSpeedTime > 0 )
    {
      this->unscheduleSpeed();
    }
    else {
      switch (mScheduleSpeedMode) {
        case 0:
          {
            gamespeed = mScheduleSpeedOrigin*mScheduleSpeedTimer/mScheduleSpeedTime;
          }
          break;
        case 2:
          {
            if( gamespeed != mScheduleSpeedTarget )
            {
              float ds = mScheduleSpeedTarget - gamespeed;
              float dr = ds/fabsf(ds);
              float mv = delta*dr;
              if( fabsf(mv) >= fabsf(ds) )
              {
                gamespeed = mScheduleSpeedTarget;
              }
              else {
                gamespeed += mv;
              }
            }
          }
          break;
      }
    }
  }
  delta *= gamespeed;
  delta *= arcadeSpeed;
  if( gameOverTimer >= 0 )
  {
    gameOverTimer += delta;
  }
  //--------
  if( count_runscene <= 0 || (gameOverTimer>=0 && gameOverTimer<PLAY_GOSLIDETIME))
  {
    deltaDistance = delta*runspeed;
  }
  else
  {
    deltaDistance = 0;
  }
  //--------
  manager->update(delta);
  this->operate();

  //update speed line
  if( mFeverLevel > 0 )
  {
    mSpeedlineTimer+=delta;
    if( mSpeedlineTimer > 0.05f )
    {
      manager->addGameObject(SpeedLine::create());
      mSpeedlineTimer = 0;
    }
  }
}

void GamePlay::fixGravity(float val)
{
    gGravityFix = val;
}

void GamePlay::resetGame()
{
  GameScene::initSceneSequence();
  this->togglePauseButton(true);
  //清除所有游戏对象
  manager->clear();
  enemies->removeAllObjects();
  darts->removeAllObjects();
  state = STATE_NORMAL;
  roleId = GameRecord::sharedGameRecord()->curr_char;

  //重新创建游戏对象
  if( mode == MODE_CLASSIC )
  {
    classic = ClassicMode::mode();
    manager->addGameObject(classic);//压入普通模式
  }
  else {
    //Arcade Mode
    arcade = ArcadeMode::mode();
    manager->addGameObject(arcade);//压入街机模式
  }
  //add foot prints
  manager->addGameObject(FootPrint::footprint());
  //是否开启教程
  bool tutorial = false;

  //manager->addGameObject(MessageManager::sharedMessageManager());
  tiles = (GameScene*)manager->addGameObject(GameScene::scene(this));//创建界面
  mainrole = (MainRole*)manager->addGameObject(MainRole::role(this));//创建主角
  //addChild(mainrole);

  if( GameRecord::sharedGameRecord()->game_tutorial == 0 && mode == MODE_CLASSIC )
  {
    tutorial = true;
    manager->addGameObject(TutorialDirector::create());//压入教程模式
    GameRecord::sharedGameRecord()->char_equip_spell[roleId] = 0;//强行使用第一个技能
  }

  mainrole2 = NULL;
  if( tutorial )
  {
    mainrole->flag_dart = false;
    mainrole->flag_blade = false;
    mainrole->flag_spell = false;
    mainrole->flag_move = false;
  }

  interface = (GameInterface*)manager->addGameObject(GameInterface::interface(this));//创建界面
  //load new scene
  tiles->setScene(GameScene::getCurrentSceneId());

  substate = 0;
  stateTimer = 0;

  festivalPieces = 0;
  mFeverBegin = -1;
  gamespeed = 1.0f;
  fixedgamespeed = gamespeed;
  runspeed = GAME_INITRUNSPEED;
  levelspeed = runspeed;
  distance = 0;
  scenecount = 1;
  difficulty = 1.0f;
  maxenemy = GAME_INITMAXENEMY;
  combo = 0;
  comboTimer = 0;
  mFeverAdd = 0;
  coins = 0;
  scrolls = 0;
  gameOverTimer = -1;
  canPause = true;
    CCLOG("canPause = %d", canPause);
  doubleCoin = false;
  doubleCoinIAP = false;
  mFeverMaskTimer = -1;
  mFeverLevel = 0;
  mSpeedlineTimer = 0;
  spelling = false;
  reviveCount = 0;
  if( GameRecord::sharedGameRecord()->game_doublecoin > 0 )
  {
    GameRecord::sharedGameRecord()->game_doublecoin = 0;
    doubleCoin = true;
  }
  if( GameRecord::sharedGameRecord()->iap_flag[0] > 0 )
  {
    doubleCoinIAP = true;
  }

  count_control = 0;
  count_respawn = 0;
  count_attack = 0;
  count_runscene = 0;

  flag_samurai = false;

  mainrole->HP = mainrole->maxHP;
  mainrole->SP = 0;
  mainrole->dart = mainrole->maxDart;
  //计算切换场景的阈值
  changeflag = scenecount*PLAY_SCENELEN*PLAY_DISMETER;

  this->scheduleMask(ccc3(0, 0, 0), 2, 1);

  mainrole->setAI(2, ccp(-100, PLAY_PLAYERLINE));
  mainrole->setAI(1, ccp(UniversalFit::sharedUniversalFit()->playSize.width/2, 0));
  SimpleAudioEngine::sharedEngine()->stopBackgroundMusic();

  //achievement play game
  GameRecord::sharedGameRecord()->task->dispatchTask(ACH_STARTGAME, 1);
  sceneSpell = 0;
  gameSpell = 0;
  killingCount = 0;
  totalScorll = 0;
  reflectKill = 0;
  killJoninCri = 0;
  wounded = 0;
  getitem = false;
  missshoot = false;
  runwithoutfire = 0;
  runwithoutblade = 0;
  runwithoutkill = 0;
  maxfever = 0;
  killsamurai = 0;
  firecount = 0;
  meetchristmas = 0;
  killpumpkins = 0;
  pumpkinspell = 0;

  if( GameRecord::sharedGameRecord()->curr_char == 1 )
  {
    if( GameRecord::sharedGameRecord()->char_equip_dart[1] == 1
        && GameRecord::sharedGameRecord()->char_equip_blade[1] == 4 )
    {
      GameRecord::sharedGameRecord()->task->dispatchTask(ACH_SAKURA, 1);
    }
  }
  if( GameRecord::sharedGameRecord()->curr_char == 2 )
  {
    if( GameRecord::sharedGameRecord()->char_equip_blade[2] == 11 )
    {
      GameRecord::sharedGameRecord()->task->dispatchTask(ACH_TRUEMANWITHBLADES, 1);
    }
  }
  if( GameRecord::sharedGameRecord()->curr_char == 3 )
  {
    if( GameRecord::sharedGameRecord()->char_equip_dart[3] == 11
        && GameRecord::sharedGameRecord()->char_equip_blade[3] == 14 )
    {
      GameRecord::sharedGameRecord()->task->dispatchTask(ACH_ALIENPUSSY, 1);
    }
  }

  GameRecord::sharedGameRecord()->char_usecount[roleId]++;
  bool allcharacter = true;
  for( int i=0; i<GAME_CHARCOUNT; ++i)
  {
    if( GameRecord::sharedGameRecord()->char_usecount[i] == 0 )
    {
      allcharacter = false;
      break;
    }
  }
  if( allcharacter )
  {
    GameRecord::sharedGameRecord()->task->dispatchTask(ACH_USEALLCHARACTER, 1);
  }
  GameRecord::sharedGameRecord()->task->dispatchTask(ACH_USECHARACTER0+roleId, 1);

    SimpleAudioEngine::sharedEngine()->playBackgroundMusic(CCFileUtils::sharedFileUtils()->fullPathForFilename("sound/music-menu.mp3").c_str(), true);
    
  CCLog("GamePlay:done resetGame");

#ifdef DEBUG
  CCTextureCache::sharedTextureCache()->dumpCachedTextureInfo();
#endif
}

void GamePlay::triggerDeath()
{
    //achievement one
    GameRecord::sharedGameRecord()->task->dispatchTask(ACH_ONEKILLING, killingCount);
    GameRecord::sharedGameRecord()->task->dispatchTask(ACH_ONESCROLL, totalScorll);
    GameRecord::sharedGameRecord()->task->dispatchTask(ACH_ONEREFLECTKILL, reflectKill);
    GameRecord::sharedGameRecord()->task->dispatchTask(ACH_ONEKILLJONINCRI, killJoninCri);
    GameRecord::sharedGameRecord()->task->dispatchTask(ACH_RUNWITHOUTKILL, runwithoutkill);
    GameRecord::sharedGameRecord()->task->dispatchTask(ACH_RUNWITHOUTFIRE, runwithoutfire);
    GameRecord::sharedGameRecord()->task->dispatchTask(ACH_RUNWIHTOUTBLADE, runwithoutblade);
    
    if( mode == MODE_CLASSIC )
    {
        //save high score
        int score = distance/PLAY_DISMETER;
        
        if( runwithoutkill == 0 )
        {
            runwithoutkill = score;
        }
        if( runwithoutfire == 0 )
        {
            runwithoutfire = score;
        }
        if( runwithoutblade == 0 )
        {
            runwithoutblade = score;
        }
        
        //achievement distance
        GameRecord::sharedGameRecord()->task->dispatchTask(ACH_DISTANCE, score);
        
        GameRecord::sharedGameRecord()->task->dispatchTask(ACH_ONECOINS, coins);
        GameRecord::sharedGameRecord()->task->dispatchTask(ACH_ONEDISTANCE, score);
        
        if( roleId == 1 )
        {
            GameRecord::sharedGameRecord()->task->dispatchTask(ACH_ONESAKURADISTANCE, score);
            GameRecord::sharedGameRecord()->task->dispatchTask(ACH_SAKURADISTANCE, score);
            GameRecord::sharedGameRecord()->task->dispatchTask(ACH_ONESAKURACOINS, coins);
            GameRecord::sharedGameRecord()->task->dispatchTask(ACH_SAKURACOINS, coins);
        }
        if( roleId == 2 )
        {
            GameRecord::sharedGameRecord()->task->dispatchTask(ACH_ONEMUSASHIDISTANCE, score);
            GameRecord::sharedGameRecord()->task->dispatchTask(ACH_MUSASHIDISTANCE, score);
        }
        if( roleId == 3 )
        {
            GameRecord::sharedGameRecord()->task->dispatchTask(ACH_MAROONDISTANCE, score);
            GameRecord::sharedGameRecord()->task->dispatchTask(ACH_ONEMAROONDISTANCE, score);
        }
    }
    else
    {//ARCADE
        if( wounded == 0 )
        {
            GameRecord::sharedGameRecord()->task->dispatchTask(ACH_ARCADEfalseHIT, 1);
        }
    }
}

CCLayer* GamePlay::ui()
{
    return mUI;
}

//划屏幕
void GamePlay::slide(CCPoint dir)
{
    //#bugfix: dead stand
    if( mainrole->HP <= 0 )
    {
        return;
    }
    
    float mpx = mainrole->position().x;//为修正分身术的发射飞镖的方向极限
    if( mainrole2 != NULL )
    {
        mpx = (mainrole->position().x + mainrole2->position().x)/2;
    }
    float mw;
    if( dir.x > 0 )
    {
        mw = /*UniversalFit::sharedUniversalFit()->playSize.width*/SCREEN_WIDTH - mpx;
    }
    else {
        mw = mpx;
    }
    CCPoint lp = ccpNormalize(ccp(mw, PLAY_MINSHOOT));
    float slidedart = PLAY_SLIDEDART > lp.y ? PLAY_SLIDEDART : lp.y;
    //slidedart 暂时还没有考虑到分身术
    if( dir.y >= slidedart )
    {//丢飞镖
        mainrole->fire(dir);
        return;
    }
    if( dir.y >= PLAY_SLIDERELOAD )
    {
        mainrole->slice();
        if( mainrole2 != NULL )
        {
            mainrole2->slice();
        }
        return;
    }
    //发动技能
    if( mainrole->spellType != SPELL_REPLEACE
       && mainrole->spellType != SPELL_GODHAND)
    {
        mainrole->spell(CCPointZero);
    }
}

CCPoint GamePlay::autoAim(CCPoint dir)
{
  CCPoint mainpos = mainrole->center();
  float mindis = PLAY_AUTOAIM*PLAY_AUTOAIM;
  CCPoint rdir = dir;
  CCObject* node = NULL;
  CCARRAY_FOREACH(enemies, node)
  {
    Role *en = (Role*)node;
    if( en->supportAimAid() )
    {
      CCPoint pen = en->center();
      pen = ccpSub(pen, mainpos);
      float len1 = ccpDot(pen, dir);
      float len2SQ = ccpLengthSQ(pen);
      float len = len2SQ - len1*len1;
      if( len < mindis )
      {
        mindis = len;
        rdir = ccpNormalize(pen);
      }
    }
  }
  return rdir;
}

bool GamePlay::onTouchBegan(Touch * touch, Event * event)
{
  if( paused )
  {
    return false;
  }
  LNR_GET_TOUCH_POS;

  if( count_control <= 0 )
  {
    mTouchBegin = pos;
    mTouchProcessed = false;
  }
  return true;
}

void GamePlay::onTouchMoved(Touch * touch, Event * event)
{
  if( count_control <= 0 )
  {
    if( mTouchProcessed == false )
    {
      LNR_GET_TOUCH_POS;

      CCPoint dir = ccpSub(pos, mTouchBegin);
      float len = ccpLength(dir);
      if( len > CONTROL_MAXSLIDE )
      {
        mTouchProcessed = true;
        this->slide(ccpNormalize(dir));
      }
    }
  }
  else {
    mTouchProcessed = true;
  }
}

void GamePlay::onTouchEnded(Touch * touch, Event * event)
{
  if( count_control <= 0 )
  {
    if( mTouchProcessed == false )
    {
      LNR_GET_TOUCH_POS;

      CCPoint dir = ccpSub(pos, mTouchBegin);
      float len = ccpLength(dir);
      if( len > CONTROL_MINSLIDE )
      {
        this->slide(ccpNormalize(dir));
      }
    }
  }
  else {
    mTouchProcessed = true;
  }
  
}

void GamePlay::onTouchCancelled(Touch * touch, Event *unused)
{//同TouchEnded
    this->onTouchEnded(touch, unused);
}
void GamePlay::onAcceleration(Acceleration* pAccelerationValue, Event*)
{
    pAccelerationValue->y = pAccelerationValue->x;
    if( count_control <= 0 )
    {
        mainrole->setTilt(pAccelerationValue->y*gGravityFix);
        if( mainrole2 != NULL )
        {
            mainrole2->setTilt(pAccelerationValue->y*gGravityFix);
        }
    }
    else {
        mainrole->setTilt(0);
        if( mainrole2 != NULL )
        {
            mainrole2->setTilt(0);
        }
    }
}

Role* GamePlay::nearestEnemy(CCPoint dir)
{
  Role *ret = NULL;
  CCPoint mainpos = mainrole->center();
  float mindis = 9999*9999;
  CCObject* node = NULL;
  CCARRAY_FOREACH(enemies, node)
  {
    Role *en = (Role*)node;
    if( en->supportAimAid() )
    {
      CCPoint pen = en->center();
      pen = ccpSub(pen, mainpos);
      float len1 = ccpDot(pen, dir);
      float len2SQ = ccpLengthSQ(pen);
      float len = len2SQ - len1*len1;
      if( len < mindis )
      {
        mindis = len;
        ret = en;
      }
    }
  }
  return ret;
}

//set fever effect
void GamePlay::setFeverLevel(int lv)
{
    if( lv != mFeverLevel )
    {
        if( lv == 2 )
        {
            //enable shadow
            mainrole->toggleShadow(true);
            if( mainrole2 != NULL )
            {
                mainrole2->toggleShadow(true);
            }
            
            //enable sound
            this->startLoopFever();
            
            //enable mask
            //fever mask
            mFeverMaskTimer = 0;
            mFeverMask->setVisible(true);
            mFeverMask->setOpacity(0);
        }
        else
        {
            if( mFeverLevel == 2 )
            {
                //disable shadow
                mainrole->toggleShadow(false);
                if( mainrole2 != NULL )
                {
                    mainrole2->toggleShadow(false);
                }
            }
            
            //disable sound
            this->stopLoopFever();
            
            //disable mask
            mFeverMaskTimer = -1;
            mFeverMask->setVisible(false);
        }
        
        //setup for fever effect
        if( lv > mFeverLevel )
        {
            SimpleAudioEngine::sharedEngine()->playEffect(CCFileUtils::sharedFileUtils()->fullPathForFilename("sound/fever.mp3").c_str());
            manager->addGameObject(FeverBrust::feverbrust(27, true));
            
            //clean darts
            unsigned int n = 0;
            while( n < darts->count() )
            {
                Darts* d = (Darts*)darts->objectAtIndex(n);
                if( d->isEnemy() )
                {
                    GTAnimatedEffect *hiteff2 = GTAnimatedEffect::create(GTAnimation::loadedAnimationSet("effect"), 2, false);
                    hiteff2->setAnchorPoint(ccp(0.5f, 0.5f));
                    hiteff2->setScale(1.5f);
                    hiteff2->setPosition(d->position());
                    this->addChild(hiteff2, LAYER_ROLE);
                    manager->removeGameObject(d);
                    darts->removeObjectAtIndex(n);
                }
                else {
                    n++;
                }
            }
        }
        //play combo close sound
        if( mFeverLevel == 2 )
        {
            SimpleAudioEngine::sharedEngine()->playEffect(CCFileUtils::sharedFileUtils()->fullPathForFilename("sound/feverbgmend.mp3").c_str());
        }
        mFeverLevel = lv;
    }
}

void GamePlay::makeCombo()
{
    combo++;
    //refresh combo high
    if(combo > GameRecord::sharedGameRecord()->combo_high )
    {
        GameRecord::sharedGameRecord()->combo_high = combo;
    }
    
    comboTimer = PLAY_COMBOTIME;
    
    //play music
    {//combo sfx
        int cn = 1;
        if( combo < PLAY_FEVERCOMBO )
        {
            cn = ((combo-1)/3)+1;
        }
        else {
            cn = ((combo-PLAY_FEVERCOMBO)/5)+4;
            if( cn > 9 )
            {
                cn = 9;
            }
        }
        CCString *filename = CCString::createWithFormat("combo%d.mp3", cn);
        SimpleAudioEngine::sharedEngine()->playEffect(filename->getCString());
    }
    
    if( mode == MODE_CLASSIC )
    {
        //fever check
        if( combo >= PLAY_FEVERCOMBO )
        {//fever
            int lev = (combo-PLAY_FEVERCOMBO)/5;
            float add = 0.15f + 0.05f*lev;
            if( add > 0.35f )
            {
                add = 0.35f;//fever 最高涨50%
            }
            gamespeed -= mFeverAdd;
            gamespeed += add;
            mFeverAdd = add;
        }
        
        {//fever sfx
            if( combo >= PLAY_FEVERCOMBO && combo%5==0 )
            {
                int fn = (combo-PLAY_FEVERCOMBO)/5+1;
                int earn = (fn-1)*5;
                coins += earn;
                if( earn > 0 )
                {
                    interface->popFeverCoins(earn);
                }
                if( fn > 5 )
                {
                    fn = 5;
                }
                CCString *filename = CCString::createWithFormat("fever%d.mp3", fn);
                SimpleAudioEngine::sharedEngine()->playEffect(filename->getCString());
            }
        }
        if( combo == PLAY_FEVERCOMBO )
        {
            this->increaseScheduleCounter();//increase for speed up
            
            //1.2.0 fever change
            this->setFeverLevel(2);
    
            //achievement fever
            GameRecord::sharedGameRecord()->task->dispatchTask(ACH_FEVERCOUNT, 1);
            mFeverBegin = distance/PLAY_DISMETER;
        }
    }
    
    if( combo > 1 )
    {
        interface->popHits(combo);
    }
    
    //achievement critical
    GameRecord::sharedGameRecord()->task->dispatchTask(ACH_CRITICAL, 1);
    //achievement reach combo
    GameRecord::sharedGameRecord()->task->dispatchTask(ACH_REACHCOMBO, combo);
}

void GamePlay::refreshCombo()
{
    comboTimer = PLAY_COMBOTIME;
}

void GamePlay::stopCombo()
{
    if( combo >= PLAY_FEVERCOMBO )
    {
        //1.2.0 fever change
        this->setFeverLevel(0);
        if( mode == MODE_CLASSIC )
        {
            gamespeed -= mFeverAdd;
            mFeverAdd = 0;
            this->decreaseScheduleCounter();
        }
    }
    comboTimer = 0;
    combo = 0;
    interface->fadeHits();
    if( mFeverBegin >= 0 )
    {
        int feverdis = distance/PLAY_DISMETER - mFeverBegin;
        if( feverdis > maxfever )
        {
            maxfever = feverdis;
            GameRecord::sharedGameRecord()->task->dispatchTask(ACH_MAXFEVER, maxfever);
        }
    }
    mFeverBegin = -1;
}

//暂停
void GamePlay::pause(Ref* ref)
{
    SimpleAudioEngine::sharedEngine()->playEffect(CCFileUtils::sharedFileUtils()->fullPathForFilename("sound/click.mp3").c_str());
    CCLOG("canPause = %d paused = %d", canPause, paused);
    if( this->canPause && !this->paused )
    {
        this->paused = true;
        this->mMenu->setVisible(false);
        this->mask->setOpacity(128);
        //添加菜单
        this->manager->addGameObject(PauseMenuClassic::create());
    }
}

//恢复
void GamePlay::resume(Ref* ref)
{
    CCLOG("canPause = %d", canPause);
    if( canPause && paused )
    {
        paused = false;
        mMenu->setVisible(true);
        mask->setOpacity(0);
    }
}

void GamePlay::restart(Ref* ref)
{
    this->resume();
    this->resetGame();
}

void GamePlay::exit(Ref* ref)
{
    this->resume();
    SimpleAudioEngine::sharedEngine()->stopBackgroundMusic();
    this->setSceneOutro(Loading::loadTo(TitleMenu::scene(), PublicLoad::menuLoadingList(), PublicLoad::gameLoadingList(), false));
}

void GamePlay::change(Ref* ref)
{
    this->resume();
    SimpleAudioEngine::sharedEngine()->stopBackgroundMusic();
    this->setSceneOutro(Loading::loadTo(SelectMenu::scene(), PublicLoad::menuLoadingList(), PublicLoad::gameLoadingList(), false));
}

void GamePlay::clearFootPrints()
{
    this->removeChild(footprints);
    footprints = NULL;
}

void GamePlay::togglePauseButton(bool flag)
{
    if( flag )
    {
        mMenu->setVisible(true);
    }
    else {
        mMenu->setVisible(false);
    }
}

void GamePlay::scheduleSpeed(float target, float time, int slowmode)
{
    this->unscheduleSpeed();
    mScheduleSpeedOrigin = gamespeed;
    mScheduleSpeedMode = slowmode;
    mScheduleSpeedTarget = target;
    mScheduleSpeedTime = time;
    mScheduleSpeedTimer = 0;
    if( mScheduleSpeedMode == 1 )
    {
        gamespeed = mScheduleSpeedTarget;
    }
    this->increaseScheduleCounter();
}

void GamePlay::unscheduleSpeed()
{
    if( mScheduleSpeedMode >= 0 )
    {
        gamespeed = mScheduleSpeedOrigin;
        mScheduleSpeedMode = -1;
        
        this->decreaseScheduleCounter();
    }
}

void GamePlay::increaseScheduleCounter()
{
    mScheduleSpeedCounter++;
}

void GamePlay::decreaseScheduleCounter()
{
    mScheduleSpeedCounter--;
    if( mScheduleSpeedCounter <= 0 )
    {
        mScheduleSpeedCounter = 0;
        gamespeed = fixedgamespeed;
    }
}

void GamePlay::scheduleMask(ccColor3B target, float time, int maskmode)
{
    this->unscheduleMask();
    mScheduleMaskMode = maskmode;
    mask->setColor(target);
    mask->setVisible(true);
    mScheduleMaskTime = time;
    mScheduleMaskTimer = 0;
    switch (mScheduleMaskMode) {
        case 0:
        {
            mask->setOpacity((int)time);
        }
            break;
        case 1:
        {
            mask->setOpacity(255);
        }
            break;
        case 2:
        {
            mask->setOpacity(0);
        }
            break;
    }
}

void GamePlay::unscheduleMask()
{
    if( mScheduleMaskMode >= 0 )
    {
        mask->setVisible(false);
        mask->setColor(ccc3(0, 0, 0));
        mScheduleMaskMode = -1;
    }
}

void GamePlay::scheduleSpellRelease(CCNode* target, SEL_CallFunc sel)
{
    mScheduleSpellRelease = 0.4f;
    spellMask->setVisible(true);
    spelling = true;

    this->scheduleVibrate(0.2f);
    
    if( target != NULL )
    {
        mScheduleReleaseTarget = target;
        mScheduleReleaseSelector = sel;
    }
    else {
        mScheduleReleaseTarget = NULL;
        mScheduleReleaseSelector = NULL;
    }
}

void GamePlay::scheduleVibrate(float dur)
{
    mScheduleVibrate = dur;
    mVibrateTimer = 0;
    mVibrateDir = ccpForAngle(CC_DEGREES_TO_RADIANS(360.0f*CCRANDOM_0_1()));
}


void GamePlay::operate()
{
    if( operation >= 0 )
    {
        switch (operation) {
            case OP_RESET:
            {
                this->resetGame();
            }
                break;
        }
        operation = -1;
    }
}

void GamePlay::setSceneIntro()
{
  //doSceneIntro(mSceneIntro, this);
    if( mSceneIntro == NULL )
    {
        mSceneIntro = CCNode::create();
        this->addChild(mSceneIntro, LAYER_MASK+99);
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
    left->setPosition(ccp(UniversalFit::sharedUniversalFit()->playSize.width/2, 0));
    CCSprite *right = (CCSprite*)(mSceneIntro->getChildByTag(1));
    right->setVisible(true);
    right->setPosition(ccp(UniversalFit::sharedUniversalFit()->playSize.width/2, 0));
    
    CCDelayTime *dt1 = CCDelayTime::create(SCENEINTRO_DELAY);
    CCMoveBy *mb1 = CCMoveBy::create(SCENEINTRO_TIME, ccp(-UniversalFit::sharedUniversalFit()->playSize.width/2, 0));
    CCHide *hd1 = CCHide::create();
    CCCallFunc *fn1 = CCCallFunc::create(this, (SEL_CallFunc)(&GamePlay::doneIntro));
    CCSequence *sq1 = CCSequence::create(dt1, mb1, hd1, fn1, NULL);
    left->runAction(sq1);
    CCDelayTime *dt2 = CCDelayTime::create(SCENEINTRO_DELAY);
    CCMoveBy *mb2 = CCMoveBy::create(SCENEINTRO_TIME, ccp(UniversalFit::sharedUniversalFit()->playSize.width/2, 0));
    CCHide *hd2 = CCHide::create();
    CCSequence *sq2 = CCSequence::create(dt2, mb2, hd2, NULL);
    right->runAction(sq2);
    
    SimpleAudioEngine::sharedEngine()->playEffect(CCFileUtils::sharedFileUtils()->fullPathForFilename("sound/open.mp3").c_str());
}

void GamePlay::doneIntro()
{
    this->removeChild(mSceneIntro, true);
    CCTextureCache::sharedTextureCache()->removeTextureForKey("door.png");
    mSceneIntro = NULL;
}

void GamePlay::setSceneOutro(CCScene* newscene)
{
  if( mIntroFlag )
  {
    return;
  }

  mIntroFlag = true;

  mNewScene = doSceneOutro(newscene, mSceneIntro, (SEL_CallFunc)(&GamePlay::doneOutro), this);
}

void GamePlay::doneOutro()
{
    CCDirector::sharedDirector()->replaceScene(mNewScene);
    mNewScene->release();
}

void GamePlay::setTaskCompleteNode(CCNode * node)
{
    gTaskComplete = node;
}

bool GamePlay::completeSomeObjectives()
{
    {
        Objective *obj = GameRecord::sharedGameRecord()->task->dailyObjective;
        Achievement *ach = Tasks::dailyObjectiveWithUiid(obj->uiid);
        if( ach != NULL && obj->count >= ach->achieveNumber )
        {
            return true;
        }
    }
    {
        Objective *obj = GameRecord::sharedGameRecord()->task->weeklyObjective;
        Achievement *ach = Tasks::weeklyObjectiveWithUiid(obj->uiid);
        if( ach != NULL && obj->count >= ach->achieveNumber )
        {
            return true;
        }
    }
    {
        Objective *obj = GameRecord::sharedGameRecord()->task->monthlyObjective;
        Achievement *ach = Tasks::monthlyObjectiveWithUiid(obj->uiid);
        if( ach != NULL && obj->count >= ach->achieveNumber )
        {
            return true;
        }
    }

    return false;
}

void GamePlay::taskCompleted(CCString * tile, CCString * icon, int type)
{
    CCSprite *board = CCSprite::createWithSpriteFrameName("task-complete.png");
    board->setAnchorPoint(ccp(0.5f, 0));
    gTaskComplete->addChild(board);
    CCSprite *ibg = NULL;
    CCLabelTTF *label = NULL;
    switch (type) {
        case 0:
        {
            ibg = CCSprite::createWithSpriteFrameName("ms_bg1.png");
            label = CCLabelTTF::create("日常任务\n已完成！", GFONT_NAME, GFONT_SIZE_NORMAL);
        }
            break;
        case 1:
        {
            ibg = CCSprite::createWithSpriteFrameName("ms_bg3.png");
            label = CCLabelTTF::create("周常\n已完成！", GFONT_NAME, GFONT_SIZE_NORMAL);
        }
            break;
        case 2:
        {
            ibg = CCSprite::createWithSpriteFrameName("ms_bg2.png");
            label = CCLabelTTF::create("月常\n已完成！", GFONT_NAME, GFONT_SIZE_NORMAL);
        }
            break;
        case 3:
        {
            ibg = CCSprite::createWithSpriteFrameName("ms_bg4.png");
            label = CCLabelTTF::create(CCString::createWithFormat("%s\n已完成！", tile->getCString())->getCString(), GFONT_NAME, GFONT_SIZE_NORMAL);
        }
            break;
    }
    ibg->setPosition(ccp(22, 22));
    board->addChild(ibg);
    CCSprite *iconsp = CCSprite::createWithSpriteFrameName(icon->getCString());
    iconsp->setPosition(ibg->getPosition());
    if( type == 3 )
    {
        iconsp->setScale(0.625f);
    }
    board->addChild(iconsp);
    label->setAnchorPoint(ccp(0.5f, 0.5f));
    label->setPosition(ccp(100, 20));
    board->addChild(label);
    
    CCMoveBy *a1 = CCMoveBy::create(0.5f, ccp(0, -board->getContentSize().height));
    CCDelayTime *a2 = CCDelayTime::create(2);
    CCMoveBy *a3 = CCMoveBy::create(0.5f, ccp(0, board->getContentSize().height));
    CCHide *a4 = CCHide::create();
    CCCallFunc *cb = CCCallFunc::create(GamePlay::sharedGamePlay(), callfunc_selector(GamePlay::callBack));
    CCSequence *sq = CCSequence::create(a1, a2, a3, a4, cb, NULL);
    board->runAction(sq);
    SimpleAudioEngine::sharedEngine()->playEffect(CCFileUtils::sharedFileUtils()->fullPathForFilename("sound/objective-complete.mp3").c_str());
}

void GamePlay::pieceComplete(CCString * title, CCString * icon)
{
    CCSprite *board = CCSprite::createWithSpriteFrameName("task-complete.png");
    board->setAnchorPoint(ccp(0.5f, 0));
    gTaskComplete->addChild(board);
    CCSprite *item = CCSprite::create(icon->getCString());
    item->setPosition(ccp(26, 19));
    item->setScale(0.7f);
    board->addChild(item);
    
    CCSprite *sname = CCSprite::create(title->getCString());
    sname->setPosition(ccp(117, 30));
    board->addChild(sname);
    
    CCSprite *scoll = CCSprite::create("collected.png");
    scoll->setPosition(ccp(117, 13));
    board->addChild(scoll);
    
    CCMoveBy *a1 = CCMoveBy::create(0.5f, ccp(0, -board->getContentSize().height));
    CCDelayTime *a2 = CCDelayTime::create(2);
    CCMoveBy *a3 = CCMoveBy::create(0.5f, ccp(0, board->getContentSize().height));
    CCHide *a4 = CCHide::create();
    CCCallFunc *cb = CCCallFunc::create(GamePlay::sharedGamePlay(), callfunc_selector(GamePlay::callBack));
    CCSequence *sq = CCSequence::create(a1, a2, a3, a4, cb, NULL);
    board->runAction(sq);
    SimpleAudioEngine::sharedEngine()->playEffect(CCFileUtils::sharedFileUtils()->fullPathForFilename("sound/objective-complete.mp3").c_str());
}

void GamePlay::popText(CCString* text)
{
    CCSprite *board = CCSprite::createWithSpriteFrameName("task-complete.png");
    board->setAnchorPoint(ccp(0.5f, 0));
    gTaskComplete->addChild(board);
    
    CCLabelBMFont *label = CCLabelBMFont::create(text->getCString(), "ab34.fnt");
    label->setAnchorPoint(ccp(0.5f, 0.5f));
    label->setPosition(ccp(90, 20));
    board->addChild(label);
    
    CCMoveBy *a1 = CCMoveBy::create(0.5f, ccp(0, -board->getContentSize().height));
    CCDelayTime *a2 = CCDelayTime::create(2);
    CCMoveBy *a3 = CCMoveBy::create(0.5f, ccp(0, board->getContentSize().height));
    CCHide *a4 = CCHide::create();
    CCCallFunc *cb = CCCallFunc::create(GamePlay::sharedGamePlay(), callfunc_selector(GamePlay::callBack));
    CCSequence *sq = CCSequence::create(a1, a2, a3, a4, cb, NULL);
    board->runAction(sq);
    SimpleAudioEngine::sharedEngine()->playEffect(CCFileUtils::sharedFileUtils()->fullPathForFilename("sound/objective-complete.mp3").c_str());
}

void GamePlay::pushNotification(CCString * name, CCString * icon, int type)
{
    if( gTaskComplete == NULL )
    {
        return;
    }
    
    if( gPopQueues == NULL )
    {
        gPopQueues = CCArray::create();
        gPopQueues->retain();
    }
    PopQueue *pop = PopQueue::create();
    pop->title = name;
    pop->icon = icon;
    pop->type = type;
    gPopQueues->addObject(pop);
    
    if( gTaskComplete->getChildrenCount() <= 0 )
    {
        GamePlay::sharedGamePlay()->processNotificationQueue();
    }
}

void GamePlay::processNotificationQueue()
{
    gTaskComplete->removeAllChildrenWithCleanup(true);
    if( gPopQueues != NULL && gPopQueues->count() > 0)
    {
        PopQueue *pop = (PopQueue*)gPopQueues->objectAtIndex(0);
        switch (pop->type) {
            case 0:
            case 1:
            case 2:
            case 3:
            {
                GamePlay::taskCompleted(pop->title, pop->icon, pop->type);
            }
                break;
            case 4:
            {
                GamePlay::pieceComplete(pop->title, pop->icon);
            }
                break;
            case 5:
            {
                GamePlay::popText(pop->title);
            }
                break;
        }
        gPopQueues->removeObjectAtIndex(0);
    }
}

void GamePlay::stepDust(CCPoint pos)
{
    if( tiles->currentScene() == 4 )
    {
        manager->addGameObject(AnimatedParticle::particleStepWater(pos));
    }
    else if( tiles->currentScene() != 0 && tiles->currentScene() != 6 )
    {
        manager->addGameObject(StaticParticle::particleStepDust(pos));
    }
}

void GamePlay::startLoopFever()
{
  /*
    this->stopLoopFever();
    mFeverLoop = SimpleAudioEngine::sharedEngine()->soundSourceForFile("feverbgm.mp3");
    mFeverLoop->retain();
    mFeverLoop.looping = true;
    mFeverLoop->play();
    */
}

void GamePlay::stopLoopFever()
{
  /*
    if( mFeverLoop != NULL )
    {
        mFeverLoop->stop();
        mFeverLoop->release();
        mFeverLoop = NULL;
    }
    */
}
