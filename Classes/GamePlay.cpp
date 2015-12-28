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

cocos2d::CCArray *gPopQueues = NULL;

#define VIBRATE_PERIOD (0.04f)
#define VIBRATE_ZOOM (0.2f)
#define VIBRATE_MS (400.0f)
#define VIBRATE_MOVE (3.0f)

GamePlay::~GamePlay()
{
  gPlay = NULL;
}

cocos2d::Scene* GamePlay::scene()
{
  return GameTool::scene(&gPlay);
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

void GamePlay::setGameMode(int mod)
{
  mGameMode = mod;
}

int GamePlay::getGameMode()
{
  return mGameMode;
}

bool GamePlay::init()
{
  if(cocos2d::Layer::init())
  {
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

    return true;
  }
  return false;
}

void GamePlay::onEnter()
{
  mUISwapper.onEnter();

  setAccelerometerEnabled(true);
  cocos2d::Node *taskcomplete = cocos2d::Node::create();
  taskcomplete->setPosition(cocos2d::Vec2(UniversalFit::sharedUniversalFit()->playSize.width/2, SCREEN_HEIGHT));
  this->addChild(taskcomplete, LAYER_MASK+10);
  setTaskCompleteNode(taskcomplete);

  this->initGamePlay(mGameMode);

  mUISwapper.setSceneIntro(this);
  cocos2d::Layer::onEnter();

  scheduleUpdate();

  auto listener = EventListenerTouchOneByOne::create();
  listener->onTouchBegan = CC_CALLBACK_2(GamePlay::onTouchBegan, this);
  listener->onTouchEnded = CC_CALLBACK_2(GamePlay::onTouchEnded, this);
  listener->onTouchMoved = CC_CALLBACK_2(GamePlay::onTouchMoved, this);
  listener->onTouchCancelled = CC_CALLBACK_2(GamePlay::onTouchEnded, this);
  _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
}

void GamePlay::onExit()
{
  isPlayingArcade = false;
  setTaskCompleteNode(NULL);

  gPlay->release();
  cocos2d::Layer::onExit();
}

//初始化游戏
void GamePlay::initGamePlay(int mod)
{
  isPlayingArcade = (mod != MODE_CLASSIC);

  //init manager
  manager = GameObjectManager::create();
  manager->retain();
  //enemies
  enemies = cocos2d::CCArray::create();
  darts = cocos2d::CCArray::create();
  enemies->retain();
  darts->retain();
  footprints = NULL;
  arcadeSpeed = 1;

  mask = cocos2d::LayerColor::create(Color4B(0, 0, 0, 255), UniversalFit::sharedUniversalFit()->screenSize.width + 20, UniversalFit::sharedUniversalFit()->screenSize.height + 20);
  mask->setPosition(cocos2d::Vec2(-10, -10));
  mask->retain();
  this->addChild(mask, LAYER_MASK);

  mFeverMask = cocos2d::LayerColor::create(Color4B(239, 145, 3, 255));
  mFeverMask->setVisible(false);
  mFeverMask->setOpacity(0);
  mFeverMask->retain();
  this->addChild(mFeverMask, LAYER_UI-1);

  spellMask = cocos2d::LayerColor::create(Color4B(0, 0, 0, 200), UniversalFit::sharedUniversalFit()->playSize.width+VIBRATE_MOVE*3, SCREEN_HEIGHT+VIBRATE_MOVE*3);
  spellMask->retain();
  spellMask->setVisible(false);
  this->addChild(spellMask, LAYER_MAINROLE-2);

  paused = false;
  mUI = cocos2d::Layer::create();
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
  cocos2d::MenuItemImage *xpause = cocos2d::MenuItemImage::create();
  xpause->setNormalSpriteFrame(GameTool::getSpriteFrameByName("pause.png"));
  xpause->setSelectedSpriteFrame(GameTool::getSpriteFrameByName("pause.png"));
  xpause->setTarget(this, menu_selector(GamePlay::pause));
  xpause->setAnchorPoint(cocos2d::Vec2(1, 1));
  xpause->setPosition(cocos2d::Vec2(UniversalFit::sharedUniversalFit()->playSize.width, SCREEN_HEIGHT));
  this->mMenu = cocos2d::Menu::create();
  this->mMenu->addChild(xpause);
  mMenu->setPosition(cocos2d::Vec2(-7, -7));
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
        cocos2d::CCCallFunc *callSelectorAction = cocos2d::CallFunc::create(mScheduleReleaseTarget, mScheduleReleaseSelector);
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
    cocos2d::Point np = this->getPosition();
    np = np + (mVibrateDir * st);
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
      this->setPosition(cocos2d::Vec2(0, 0));
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
  mGravityFix = val;
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

  this->scheduleMask(Color3B(0, 0, 0), 2, 1);

  mainrole->setAI(2, Vec2(-100, PLAY_PLAYERLINE));
  mainrole->setAI(1, Vec2(UniversalFit::sharedUniversalFit()->playSize.width/2, 0));
  GameTool::StopBackgroundMusic();

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

  GameTool::PlayBackgroundMusic("music-menu.mp3");

#ifdef DEBUG
  cocos2d::CCTextureCache::sharedTextureCache()->dumpCachedTextureInfo();
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

cocos2d::Layer* GamePlay::ui()
{
  return mUI;
}

//划屏幕
void GamePlay::slide(cocos2d::Point dir)
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
  cocos2d::Point lp = cocos2d::Vec2(mw, PLAY_MINSHOOT).getNormalized();
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
    mainrole->spell(cocos2d::CCPointZero);
  }
}

cocos2d::Point GamePlay::autoAim(cocos2d::Point dir)
{
  cocos2d::Point mainpos = mainrole->center();
  float mindis = PLAY_AUTOAIM*PLAY_AUTOAIM;
  cocos2d::Point rdir = dir;
  cocos2d::Ref* node = NULL;
  CCARRAY_FOREACH(enemies, node)
  {
    Role *en = (Role*)node;
    if( en->supportAimAid() )
    {
      cocos2d::Point pen = en->center();
      pen = pen - mainpos;
      float len1 = pen.dot(dir);
      float len2SQ = pen.getLengthSq();
      float len = len2SQ - len1*len1;
      if( len < mindis )
      {
        mindis = len;
        rdir = pen.getNormalized();
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

  if( count_control <= 0 )
  {
    LNR_GET_TOUCH_POS;
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

      cocos2d::Point dir = pos - mTouchBegin;
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

void GamePlay::onAcceleration(Acceleration* pAccelerationValue, Event*)
{
  pAccelerationValue->y = pAccelerationValue->x;
  if( count_control <= 0 )
  {
    mainrole->setTilt(pAccelerationValue->y*mGravityFix);
    if( mainrole2 != NULL )
    {
      mainrole2->setTilt(pAccelerationValue->y*mGravityFix);
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

Role* GamePlay::nearestEnemy(cocos2d::Point dir)
{
  Role *ret = NULL;
  cocos2d::Point mainpos = mainrole->center();
  float mindis = 9999*9999;
  cocos2d::Ref* node = NULL;
  CCARRAY_FOREACH(enemies, node)
  {
    Role *en = (Role*)node;
    if( en->supportAimAid() )
    {
      cocos2d::Point pen = en->center();
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
      GameTool::PlaySound("fever.mp3");
      manager->addGameObject(FeverBrust::feverbrust(27, true));

      //clean darts
      unsigned int n = 0;
      while( n < darts->count() )
      {
        Darts* d = (Darts*)darts->objectAtIndex(n);
        if( d->isEnemy() )
        {
          GTAnimatedEffect *hiteff2 = GTAnimatedEffect::create(GTAnimation::loadedAnimationSet("effect"), 2, false);
          hiteff2->setAnchorPoint(cocos2d::Vec2(0.5f, 0.5f));
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
      GameTool::PlaySound("feverbgmend.mp3");
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
    cocos2d::CCString *filename = cocos2d::CCString::createWithFormat("combo%d.mp3", cn);
    GameTool::PlaySound(filename->getCString());
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
        cocos2d::CCString *filename = cocos2d::CCString::createWithFormat("fever%d.mp3", fn);
          GameTool::PlaySound(filename->getCString());
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
void GamePlay::pause(cocos2d::Ref* ref)
{
  GameTool::PlaySound("click.mp3");
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
void GamePlay::resume()
{
  CCLOG("canPause = %d", canPause);
  if( canPause && paused )
  {
    paused = false;
    mMenu->setVisible(true);
    mask->setOpacity(0);
  }
}

void GamePlay::restart()
{
  this->resume();
  this->resetGame();
}

void GamePlay::exit()
{
  this->resume();
  GameTool::StopBackgroundMusic();
  mUISwapper.setSceneOutro(Loading::loadTo(GameTool::scene<TitleMenu>(), PublicLoad::menuLoadingList(), PublicLoad::gameLoadingList(), false), this);
}

void GamePlay::change()
{
  this->resume();
  GameTool::StopBackgroundMusic();
  mUISwapper.setSceneOutro(Loading::loadTo(GameTool::scene<SelectMenu>(), PublicLoad::menuLoadingList(), PublicLoad::gameLoadingList(), false), this);
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

void GamePlay::scheduleMask(Color3B target, float time, int maskmode)
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
    mask->setColor(Color3B(0, 0, 0));
    mScheduleMaskMode = -1;
  }
}

void GamePlay::scheduleSpellRelease(cocos2d::Node* target, SEL_CallFunc sel)
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

void GamePlay::setTaskCompleteNode(cocos2d::Node * node)
{
  mTaskComplete = node;
}

bool GamePlay::completeSomeObjectives() {
  Tasks *manager = GameRecord::sharedGameRecord()->task;
  return manager->dailyObjective.isCompleted() ||
         manager->weeklyObjective.isCompleted() ||
         manager->monthlyObjective.isCompleted();
}

void GamePlay::popNotification(PopQueue *pop) {
  cocos2d::Sprite *board = cocos2d::Sprite::createWithSpriteFrameName("task-complete.png");
  board->setAnchorPoint(cocos2d::Vec2(0.5f, 0));
  mTaskComplete->addChild(board);
  int type = pop->type;
  std::string title = pop->title;
  std::string icon = pop->icon;

  if (type == 5) {
    cocos2d::Label *label = cocos2d::Label::createWithBMFont("ab34.fnt", title.c_str());
    label->setAnchorPoint(cocos2d::Vec2(0.5f, 0.5f));
    label->setPosition(cocos2d::Vec2(90, 20));
    board->addChild(label);
  } else if (type == 4) {
    cocos2d::Sprite *item = cocos2d::Sprite::create(icon.c_str());
    item->setPosition(cocos2d::Vec2(26, 19));
    item->setScale(0.7f);
    board->addChild(item);

    cocos2d::Sprite *sname = cocos2d::Sprite::create(title.c_str());
    sname->setPosition(cocos2d::Vec2(117, 30));
    board->addChild(sname);
    cocos2d::Sprite *scoll = cocos2d::Sprite::create("collected.png");
    scoll->setPosition(cocos2d::Vec2(117, 13));
    board->addChild(scoll);
  } else {
    std::string config[4][2] = {
      {"ms_bg1.png", "日常任务"},
      {"ms_bg3.png", "周常任务"},
      {"ms_bg2.png", "月常任务"},
      {"ms_bg4.png", title}
    };
    cocos2d::Sprite *ibg = cocos2d::Sprite::createWithSpriteFrameName(config[type][0]);
    cocos2d::Label *label = cocos2d::Label::createWithSystemFont(config[type][1]+"\n已完成！", GFONT_NAME, GFONT_SIZE_NORMAL);

    ibg->setPosition(cocos2d::Vec2(22, 22));
    board->addChild(ibg);
    cocos2d::Sprite *iconsp = cocos2d::Sprite::createWithSpriteFrameName(icon.c_str());
    iconsp->setPosition(ibg->getPosition());
    if( type == 3 )
    {
      iconsp->setScale(0.625f);
    }
    board->addChild(iconsp);

    label->setAnchorPoint(cocos2d::Vec2(0.5f, 0.5f));
    label->setPosition(cocos2d::Vec2(100, 20));
    board->addChild(label);
  }

  board->runAction(cocos2d::Sequence::create(
        cocos2d::MoveBy::create(0.5f, Vec2(0, -board->getContentSize().height)),
        cocos2d::DelayTime::create(2),
        cocos2d::MoveBy::create(0.5f, Vec2(0, board->getContentSize().height)),
        cocos2d::Hide::create(),
        cocos2d::CallFunc::create([this] () -> void {
            this->processNotificationQueue();
        }),
        nullptr));
  GameTool::PlaySound("objective-complete.mp3");
}

void GamePlay::pushNotification(std::string name, std::string icon, int type)
{
  if( mTaskComplete == NULL )
  {
    return;
  }

  if( gPopQueues == NULL )
  {
    gPopQueues = cocos2d::CCArray::create();
    gPopQueues->retain();
  }
  PopQueue *pop = PopQueue::create();
  pop->title = name;
  pop->icon = icon;
  pop->type = type;
  gPopQueues->addObject(pop);

  if( mTaskComplete->getChildrenCount() <= 0 )
  {
    GamePlay::sharedGamePlay()->processNotificationQueue();
  }
}

void GamePlay::processNotificationQueue()
{
  mTaskComplete->removeAllChildrenWithCleanup(true);
  if( gPopQueues != NULL && gPopQueues->count() > 0)
  {
    popNotification((PopQueue*)gPopQueues->objectAtIndex(0));
    gPopQueues->removeObjectAtIndex(0);
  }
}

void GamePlay::stepDust(cocos2d::Point pos)
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
