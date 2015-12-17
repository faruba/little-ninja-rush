#include "GameTool.h"
#include "MainRole.h"
#include "GamePlay.h"
#include "GameConstants.h"
#include "Tasks.h"
#include "UniversalFit.h"
#include "GameRecord.h"
#include "GameData.h"
#include "Darts.h"
#include "FootPrint.h"
#include "GameScript.h"
#include "AnimatedParticle.h"
#include "StaticParticle.h"
#include "PEffect.h"
#include "TimedTrigger.h"
#include "TraceDart.h"
#include "ReflectDart.h"
#include "BombDart.h"

#include "PumpkinCircle.h"
MainRole* MainRole::role(cocos2d::Node* parent) 
{
  MainRole *mr = MainRole::create();
  mr->mRoleId = GameRecord::sharedGameRecord()->curr_char;
  mr->mParent = parent;
  mr->mAIPos = Vec2(UniversalFit::sharedUniversalFit()->playSize.width/2, PLAY_PLAYERLINE);
  return mr;
}

void MainRole::onCreate() 
{
  flag_dart = true;
  flag_blade = true;
  flag_spell = true;
  flag_move = true;

  mHiddenPower = false;

  //初始化角色属性
  mSprite = GTAnimatedSprite::spriteWithGTAnimation(GTAnimation::loadedAnimationSet(GameData::roleAnimation(mRoleId)->getCString()));

  HP = GameData::roleCurrHP(mRoleId);
  maxHP = HP;
  dart = GameData::roleCurrDart(mRoleId);
  maxDart = dart;
  runSpeed = GameData::roleRunSpeed(mRoleId);

  //dart
  int did = GameRecord::sharedGameRecord()->char_equip_dart[mRoleId];
  this->setEquipDart(did);

  //blade
  int bid = GameRecord::sharedGameRecord()->char_equip_blade[mRoleId];
  this->setEquipBlade(bid);

  //special
  int sid = GameRecord::sharedGameRecord()->char_equip_spell[mRoleId];
  this->setEquipSpell(sid);

  GamePlay *play = GamePlay::sharedGamePlay();
  if( play->mode == MODE_ARCADE )
  {
    dartCD *= 0.5f;
    bladeCD *= 0.5f;
  }

  mSprite->setAnchorPoint(cocos2d::Vec2(0.4f, 0.0625f));
  mSprite->setPosition(mAIPos);

  if( mRoleId == 3 && mShadowCounter > 0 )
  {//猫忍者
    mSprite->playGTAnimation(0+10, true);
  }
  else {
    mSprite->playGTAnimation(0, true);
  }

  mParent->addChild(mSprite, LAYER_MAINROLE);

  mGhost = GTGhostShadow::shadow(mSprite, 0.1f, 5);
  //TODO
  //mGhost->setOpacity(128);
  mGhost->toggleShadow(false);
  mParent->addChild(mGhost, LAYER_MAINROLE-1);

  mMoveFlag = 0;
  mAI = 0;

  //init timers
  mInvincible = false;
  mInvincibleTimer = 0;
  mLoadTimer = 0;
  mBladeTimer = 0;
  SPDrain = 0;
  mBladeTimer2 = 0;
  mBlinkTimer = -1;
  mRecoveryTimer = -1;

  mScheduleUnlimitedDarts = -1;
  mDashTimer = -1;
  mBladeAction = true;

  mVisibleCounter = 1;
  mShadowCounter = 0;
  mBlinkCounter = 0;

  mBlade = NULL;

  mStepTimer = 0;
  mFootPrintTimer = 0;
}

void MainRole::setEquipDart(int tid) 
{
//  Shuriken *sk = (Shuriken*)GameData::fetchShurikens()->objectAtIndex(tid);
//
//  dartCD = sk->reld;
//  dartSpeed = sk->flys;
//  dartShap = sk->shap;
//  dartEffect = sk->efft;
//  //special dart
//  if( sk->uiid == mRoleId + 39 )
//  {
//    dartCD -= dartCD*0.3f;
//  }
}

void MainRole::setEquipBlade(int tid) 
{
//  Katana *ka = (Katana*)GameData::fetchKatanas()->objectAtIndex(tid);
//
//  bladeCD = ka->reld;
//  bladeRange = ka->rang;
//  bladeAnim = ka->anim;
//  bladeEffect = ka->efft;
//  bladeSound = ka->sond;
//  //special power
//  if( ka->uiid == mRoleId + 43 )
//  {
//    mHiddenPower = true;
//  }
}

void MainRole::setEquipSpell(int tid) 
{
  Special *sp = (Special*)GameData::fetchSpecials()->objectAtIndex(tid);

  spellType = sp->skid;
  SP = 0;
  maxSP = sp->spmx;
  activeSP = sp->spac;
}

void MainRole::setAI(int ai, cocos2d::Point pos) 
{
  mAI = ai;
  mAIPos = pos;
  if( mAI == 2 )
  {
    mAI = 0;
    mSprite->setPosition(pos);
  }
}

void MainRole::fire(cocos2d::Point dir) 
{
  if( flag_dart )
  {
    Vector2d *d = Vector2d::create();
    d->value = dir;
//    MessageManager::sharedMessageManager()->pushMsg(this, callfuncND_selector(MainRole::commitFire), d);
      this->commitFire(NULL, d);
  }
}

void MainRole::commitFire(cocos2d::Node* p, Node* pdata) 
{
  Vector2d* param = (Vector2d*)pdata;
  cocos2d::Point dir = param->value;
  GamePlay *play = GamePlay::sharedGamePlay();
  bool fake = false;
  if( play->mainrole2 != NULL && this == play->mainrole2 )
  {
    fake = true;
  }
  if( dart > 0 || fake )
  {//普通飞镖
    if( !fake && play->mainrole2 != NULL )
    {
      play->mainrole2->commitFire(NULL, param);
    }

    //achievement fired shurikens
    GameRecord::sharedGameRecord()->task->dispatchTask(ACH_FIREDSHURIKENS, 1);
    if( play->runwithoutfire == 0 )
    {
      play->runwithoutfire = play->distance/PLAY_DISMETER;
    }
    GameRecord::sharedGameRecord()->task->dispatchTask(ACH_ONEFIRE, play->firecount);
    if( play->roleId == 1 )
    {
      GameRecord::sharedGameRecord()->task->dispatchTask(ACH_ONESAKURAFIRE, play->firecount);
      GameRecord::sharedGameRecord()->task->dispatchTask(ACH_SAKURAFIRE, 1);
    }

    dir = play->autoAim(dir);//自动瞄准
    Dart *d = Dart::dart(dartShap, this->center(), dir, dartEffect, mParent);
    //d->traceRole(((GameObject*)[play->nearestEnemy(dir]) handler));
    play->darts->addObject(play->manager->addGameObject(d));
    if( dartEffect == 3 )
    {
      GameTool::PlaySound("sound/shuriken-laser.mp3");
    }
    else {
      GameTool::PlaySound("sound/throw.mp3");
    }
    if( dir.x < 0 )
    {
      if( mRoleId == 3 && mShadowCounter > 0 )
      {//猫忍者
        mSprite->playGTAnimation(1+10, false);
      }
      else {
        mSprite->playGTAnimation(1, false);
      }
    }
    else {
      if( mRoleId == 3 && mShadowCounter > 0 )
      {//猫忍者
        mSprite->playGTAnimation(2+10, false);
      }
      else {
        mSprite->playGTAnimation(2, false);
      }
    }
    if( mScheduleUnlimitedDarts < 0 )
    {
      dart--;
    }
  }
}

void MainRole::slice() 
{
  if( !flag_blade )
  {
    return;
  }

  bool blade2 = false;
  bool nocd = false;
  if( mRoleId == 2 )
  {
    if( mBladeTimer > 0 )
    {
      if( mBladeTimer2 > 0 )
      {
        nocd = true;
      }
      else {
        blade2 = true;
        mBladeTimer2 = bladeCD;
      }
    }
    else {
      mBladeTimer = bladeCD;
    }
  }
  else {
    if( mBladeTimer > 0 )
    {
      nocd = true;
    }
    else
    {
      mBladeTimer = bladeCD;
    }
  }
  if( nocd )
  {
    if( spellType == SPELL_GODHAND && SP >= activeSP )
    {
      GamePlay *play = GamePlay::sharedGamePlay();
      SP -= activeSP;
      mBladeTimer = bladeCD;
      play->gameSpell++;
      GameRecord::sharedGameRecord()->task->dispatchTask(ACH_ONESPELL, play->gameSpell);
    }
    else {
      return;
    }
  }
  if( mBladeAction )
  {
    mSprite->playGTAnimation(3, false);
  }
  else {
    mSprite->playGTAnimation(10, false);
  }

  if( mRoleId == 2 )
  {
    mBladeAction = !mBladeAction;
  }
  mLoadTimer = 0;//中断load
//  MessageManager::sharedMessageManager()->pushMsg(this, callfuncND_selector(MainRole::commitSlice), NULL);
    this->commitSlice();
}

void MainRole::commitSlice() 
{
  GamePlay *play = GamePlay::sharedGamePlay();

  if( mBlade == NULL )
  {
    mBlade = GTAnimatedSprite::spriteWithGTAnimation(GTAnimation::loadedAnimationSet("blade"));
    mBlade->setAnchorPoint(cocos2d::Vec2( 0.5f, 0.5f));
    mBlade->setPosition(cocos2d::Vec2( 35, 40 ));
    mSprite->addChild(mBlade, LAYER_MAINROLE+1);
  }
  mBlade->playGTAnimation(bladeAnim, false);

  //achievement
  if( play->runwithoutblade == 0 )
  {
    play->runwithoutblade = play->distance/PLAY_DISMETER;
  }

  //特效
  switch (bladeEffect) {
    case 1://樱花
      {
        for( int i=0; i<15; ++i)
        {
          play->manager->addGameObject(StaticParticle::particleSakura(this->center()));
        }
      }
      break;
    case 2://蝴蝶
      {
        for( int i=0; i<6; ++i)
        {
          play->manager->addGameObject(AnimatedParticle::particleButterfly(this->center()));
        }
      }
      break;
    case 3://冰雪
      {
        for( int i=0; i<6; ++i)
        {
          play->manager->addGameObject(StaticParticle::particleIce(this->center(), true));
          play->manager->addGameObject(StaticParticle::particleIceAir(this->center(), true));
        }
      }
      break;
    case 4://暗影刀
      {
        for( int i=0; i<12; ++i)
        {
          play->manager->addGameObject(StaticParticle::particleShadow(this->center()));
        }
      }
      break;
    case 5://黄金刀
      {
        for( int i=0; i<10; ++i)
        {
          play->manager->addGameObject(AnimatedParticle::particleCoin(this->center(), i));
        }
      }
      break;
  }

  //弹飞镖
  bool hit = false;
  int hitcount =0;
cocos2d::Ref* node = NULL;
CCARRAY_FOREACH(play->darts, node)
  {
    Darts * dr = (Darts*)node;
    if( dr->isEnemy() && exCollisionWithCircles(this->center(), 0, 0, bladeRange, dr->position(), 7) && dr->position().y > mSprite->getPosition().y )
    {
      dr->onHitback(this->center());
      hit = true;
      hitcount++;
    }
  }
  //achievement slice 5 darts
  if(hitcount>4)
  {
    GameRecord::sharedGameRecord()->task->dispatchTask(ACH_SLICE5DARTS, 1);
  }
  //arcade
  if( play->mode == MODE_ARCADE && hitcount > 0 )
  {
    int bouns = 0;
    for( int i=0; i< hitcount; ++i)
    {
      bouns += ARCADE_BOUNS_DART*(i+1);
    }
    cocos2d::Point pos = this->center();
    pos.y += 25;
    play->arcade->addScore(bouns, pos);
  }

  //切武士
  node = NULL;
CCARRAY_FOREACH(play->enemies, node)
  {
    Role* em = (Role*)node;
    if( em->collisionWithCircle(this->center(), bladeRange) )
    {
      em->deliverHit(HIT_BLADE, CCPointZero);
      hit = true;

      GTAnimatedEffect *hiteff = GTAnimatedEffect::create(GTAnimation::loadedAnimationSet("effect"), 28, false);
      hiteff->setAnchorPoint(cocos2d::Vec2(0.5f, 0.5f));
      hiteff->setPosition(em->center());
      hiteff->setRotation(90);
      play->addChild(hiteff, LAYER_MAINROLE+1);

      //achievement enemy burnt
      if( bladeAnim == 4 )
      {
        GameRecord::sharedGameRecord()->task->dispatchTask(ACH_ENEMYBURNT, 1);
      }
    }
  }

  SimpleAudioEngine::sharedEngine()->playEffect(bladeSound->getCString());

  if( hit )
  {
    GameTool::PlaySound("sound/reflect.mp3");
    play->scheduleSpeed(0, EXP_HITSLOW, 0);

    GTAnimatedEffect *eff = GTAnimatedEffect::create(GTAnimation::loadedAnimationSet("effect"), 3, false);
    eff->setAnchorPoint(cocos2d::Vec2( 0.5f, 0));
    eff->setPosition(center());
    mParent->addChild(eff, LAYER_MAINROLE+1);
  }
}

void MainRole::spell(cocos2d::Point param) 
{
  if( flag_spell )
  {
    Vector2d *d = Vector2d::create();
    d->value = param;
//    MessageManager::sharedMessageManager()->pushMsg(this, callfuncND_selector(MainRole::commitSpell), d);
      commitSpell(d);
  }
}

void MainRole::commitSpell(Vector2d* dir) 
{
  cocos2d::Point param = dir->value;
  GamePlay *play = GamePlay::sharedGamePlay();
  if( SP >= activeSP && !play->spelling )
  {
    bool spelled = false;
    switch (spellType) {
      case SPELL_TRIDARTS:
        {//三发飞镖
          bool t = false;
cocos2d::Ref *node = NULL;
CCARRAY_FOREACH(play->enemies, node)
          {
            Role* em = (Role*)node;
            if( em->supportAimAid() && em->position().x > 0 && em->position().x < UniversalFit::sharedUniversalFit()->playSize.width )
            {
              t = true;
              break;
            }
          }
          if( play->mainrole->dart > 0 && t )
          {
            spelled = true;
            SP -= activeSP;
            GameScript::sharedScript()->invokeSpellRelease(cocos2d::CCInteger::create(SPELL_TRIDARTS));
          }
        }
        break;
      case SPELL_REPLEACE:
        {//替身术
          this->attachEffect("effect", 5, false, true);
          GameTool::PlaySound("sound/smoke.mp3");
          //-------
          mSprite->setVisible(false);
          play->count_control++;
          play->manager->addGameObject(Wood::wood(this->center(), param, mParent, this->handler(), mSprite->getAnchorPoint()));
          SP -= activeSP;
          spelled = true;
        }
        break;
      case SPELL_MOONBLADE:
        {//月牙斩
          mSprite->playGTAnimation(3, false);
          GameScript::sharedScript()->invokeSpellRelease(cocos2d::CCInteger::create(SPELL_MOONBLADE));
          SP -= activeSP;
          spelled = true;
        }
        break;
      case SPELL_TWOBODIES:
        {//分身术
          if( play->mainrole2 == NULL )
          {
            SPDrain = 2;
            mDrainAdder = 0;
            this->attachEffect("effect", 5, false, true);
            //------
            mInvincibleTimer = 1;
            play->manager->addGameObject(TimedTrigger::triggerWithDelay(GameScript::sharedScript(), callfunc_selector(GameScript::invokeSpell1_0), 0.2f));
            play->count_control++;
            spelled = true;
          }
        }
        break;
      case SPELL_TRACEDART:
        {//回旋手里剑
          SP -= activeSP;
          float angle = 30 + CCRANDOM_0_1()*120;
          cocos2d::Point dir = ccpForAngle(CC_DEGREES_TO_RADIANS(angle));
          play->manager->addGameObject(TraceDart::dart(this->center(), dir));
          GameTool::PlaySound("sound/duofa.mp3");
          spelled = true;
        }
        break;
      case SPELL_REFLECTDART:
        {//反弹手里剑
          SP -= activeSP;
          float angle = 30 + CCRANDOM_0_1()*120;
          cocos2d::Point dir = ccpForAngle(CC_DEGREES_TO_RADIANS(angle));
          play->manager->addGameObject(ReflectDart::dart(this->center(), dir));
          GameTool::PlaySound("sound/duofa.mp3");
          spelled = true;
        }
        break;
      case SPELL_POWERBRUST:
        {//元气爆
          SP -= activeSP;
          GameScript::sharedScript()->invokeSpell16();
          spelled = true;
        }
        break;
      case SPELL_EVILBLADE:
        {//妖刀
          SP -= activeSP;
          GameScript::sharedScript()->invokeSpellRelease(cocos2d::CCInteger::create(SPELL_EVILBLADE));
          spelled = true;
        }
        break;
      case SPELL_BOMBDART:
        {//爆弹
          SP -= activeSP;
          //选择优目标点
          int hs = 0;
          cocos2d::Point pos = Vec2(UniversalFit::sharedUniversalFit()->playSize.width/2, RESPAWN_YMIN + RESPAWN_Y/2);
cocos2d::Ref *node = NULL;
CCARRAY_FOREACH(play->enemies, node)
          {
            Role* em = (Role*)node;
            int s = 0;
            float xx = em->position().x;
cocos2d::Ref* node = NULL;
CCARRAY_FOREACH(play->enemies, node)
            {
              Role * im = (Role*)node;
              float dy = fabsf(im->position().x - xx);
              if( dy < 100 )
              {
                s++;
              }
            }
            if( s > hs )
            {
              pos.x = em->position().x;
              hs = s;
            }
          }
          cocos2d::Point dir = ccpNormalize(ccpSub(pos, this->center()));
          play->manager->addGameObject(BombDart::dart(this->center(), dir));
          spelled = true;
        }
        break;
      case SPELL_DASH:
        {//dash
          if( mDashTimer < 0 )
          {
            SP -= activeSP;
            //lock speed
            if( mSpeed >= 0 )
            {
              mSpeed = runSpeed*1.5f;
            }
            else {
              mSpeed = -runSpeed*1.5f;
            }
            mDashTimer = DASH_TIME;
            this->toggleShadow(true);
            this->timedInvincible(DASH_TIME);
            spelled = true;

            GameTool::PlaySound("sound/dash.mp3");
          }
        }
        break;
      case SPELL_PUMPKINIMPS:
        {//pumpkin imps
          SP -= activeSP;
          GameScript::sharedScript()->invokeSpellRelease(cocos2d::CCInteger::create(SPELL_PUMPKINIMPS));
          spelled = true;
        }
        break;
    }
    //achievement spell
    if( spelled )
    {
      GameRecord::sharedGameRecord()->task->dispatchTask(ACH_SPELLSPECIAL, 1);
      play->sceneSpell++;
      play->gameSpell++;
      GameRecord::sharedGameRecord()->task->dispatchTask(ACH_ONESPELL, play->gameSpell);
    }
  }
}

void MainRole::setTilt(float dir) 
{
  if( mDashTimer >= 0 || !flag_move )
  {
    return;
  }

  float val = fabsf(dir);

  if( val > CONTROL_MAXSPEED )
  {
    val = CONTROL_MAXSPEED;
  }
  if( val < CONTROL_BALANCE )
  {
    mSpeed = 0;
  }
  else {
    mSpeed = PLAY_MINISPEED + (val-CONTROL_BALANCE)/(CONTROL_MAXSPEED-CONTROL_BALANCE)*(runSpeed-PLAY_MINISPEED);
  }
  if( dir < 0 )
  {
    mSpeed *= -1.0f;
  }
}

void MainRole::onUpdate(float delta) 
{
  GamePlay *play = GamePlay::sharedGamePlay();
  bool dust = true;
  //消耗法力值
  if( play->count_control <= 0 && SPDrain > 0 )
  {
    mDrainAdder += delta*SPDrain;
    int d = mDrainAdder;
    mDrainAdder -= d;
    SP -= d;
    if( SP < 0 ){
      SP = 0;
      //取消技能
      if( spellType == SPELL_TWOBODIES )
      {
        this->cancelMain2();
      }
    }
  }
  //消除计时器
  if(mInvincibleTimer>0 && !mInvincible)
  {
    mInvincibleTimer -= delta;
    if( mInvincibleTimer <= 0 )
    {
      this->toggleBlink(false);
    }
  }
  //无限飞镖计时
  if( mScheduleUnlimitedDarts >= 0 )
  {
    mScheduleUnlimitedDarts -= delta;
    if( mScheduleUnlimitedDarts < 0 )
    {
      play->interface->toggleUnlimtedDarts(false);
    }
  }
  //回血
  if( mRecoveryTimer >= 0 )
  {
    mRecoveryTimer -= delta;
    if( mRecoveryTimer < 0 && HP < maxHP && HP>0 )
    {
      HP++;
      if( HP < maxHP )
      {
        mRecoveryTimer = 10;
      }
      //effect
      for( int i=0; i<5; ++i)
      {
        play->manager->addGameObject(StaticParticle::particleAddHP(this->center()));
      }
    }
  }
  //闪烁效果
  if( mBlinkTimer >= 0 )
  {
    mBlinkTimer+=delta;
    int n = mBlinkTimer/0.1f;
    float v = mBlinkTimer-0.1f*n;
    if( v < 0.05f )
    {
      v = 0.1f-v;
    }
    v -= 0.05f;
    float k = v/0.05f;
    mSprite->setOpacity(255*k);
  }
  //装载飞镖
  if(mLoadTimer>0 && HP>0)
  {
    mLoadTimer-=play->deltaTime;
    if(mLoadTimer<=0)
    {
      if( dart < maxDart )
      {
        dart++;
      }
      mLoadTimer = 0;
    }
  }
  else {
    if( dart < maxDart )
    {
      mLoadTimer = dartCD;
    }
  }
  if(mBladeTimer>0)
  {
    mBladeTimer-=delta;
    if( mBladeTimer <= 0 )
    {
      //play effect
      GTAnimatedEffect *eff = GTAnimatedEffect::create(GTAnimation::loadedAnimationSet("effect"), 8, false);
      eff->setPosition(cocos2d::Vec2( 31, 18));
      mSprite->addChild(eff);
      GameTool::PlaySound("sound/blade_ready.mp3");
    }
  }
  //武藏专用
  if(mBladeTimer2>0)
  {
    mBladeTimer2-=delta;
    if( mBladeTimer2 <= 0 )
    {
      //play effect
      GTAnimatedEffect *eff = GTAnimatedEffect::create(GTAnimation::loadedAnimationSet("effect"), 8, false);
      eff->setPosition(cocos2d::Vec2( 31, 18));
      mSprite->addChild(eff);
      GameTool::PlaySound("sound/blade_ready.mp3");
    }
  }

  bool playend = mSprite->updateGTAnimation(delta);
  //阴影
  mGhost->syncShadow(delta);

  if( playend && HP>0 )
  {
    if( mRoleId == 3 && mShadowCounter > 0 )
    {//猫忍者
      mSprite->playGTAnimation(0+10, true);
    }
    else {
      mSprite->playGTAnimation(0, true);
    }
  }
  //dash
  if( mDashTimer >= 0 )
  {
    mDashTimer-=delta;
    if( mDashTimer < 0 )
    {
      this->toggleShadow(false);
    }
    if( randomInt(2) == 0 )
    {
      play->manager->addGameObject(StaticParticle::particleDash(this->center()));
    }
  }
  //move control
  float runmin = 10;
  float runmax = UniversalFit::sharedUniversalFit()->playSize.width-10;
  if( play->mainrole2 != NULL )
  {
    if( this == play->mainrole )
    {
      runmin += SPECIAL_FENSHEN;
    }
    else {
      runmax -= SPECIAL_FENSHEN;
    }
  }
  if( mSpeed != 0 )
  {
    cocos2d::Point np = mSprite->getPosition();
    np.x += mSpeed*delta;
    if( np.x > runmax )
    {
      np.x = runmax;
    }
    if( np.x < runmin )
    {
      np.x = runmin;
    }
    mSprite->setPosition(np);
  }

  if( mBlade != NULL )
  {
    if( mBlade->updateGTAnimation(delta) )
    {
      mSprite->removeChild(mBlade, true);
      mBlade = NULL;
    }
  }
  //处理 AI
  if( mAI == 1 )
  {
    float ds = delta*runSpeed;
    float dis = mAIPos.x - mSprite->getPosition().x;
    cocos2d::Point np = mSprite->getPosition();
    if( fabsf(dis) > ds )
    {
      if( dis > 0 )
      {
        np.x += ds;
      }
      else {
        np.x -= ds;
      }
      mSprite->setPosition(np);
    }
    else {
      np.x = mAIPos.x;
      mAI = 0;
    }
  }
  //死亡动画
  if( HP <= 0 )
  {
    if( play->gameOverTimer <= PLAY_GOSLIDETIME )
    {
      //            float k = (PLAY_GOSLIDETIME - play->gameOverTimer)/PLAY_GOSLIDETIME;
      //            float ds = k*runSpeed*delta/2;
      //            cocos2d::Point np = mSprite->getPosition();
      //            np.x += ds;
      // MainRole::mSprite->setPosition( np);
      if( playend && randomInt(100) < 40 && play->tiles->currentScene() != 4 )
      {
        cocos2d::Point np = this->center();
        np.y = PLAY_PLAYERLINE + 5;
        play->manager->addGameObject(AnimatedParticle::particleDeadSlide(np));
      }
    }
    else {
      dust = false;
      if( mSprite->animationId() != 7 )
      {
        mSprite->playGTAnimation(7, false);
      }
    }
  }
  else
  {
    FootPrint::goFootPrint(&mFootPrintTimer, mSprite->getPosition());
  }

  //step dust
  mStepTimer += delta;
  if( mStepTimer > 0.12f && dust )
  {
    play->stepDust(mSprite->getPosition());
    mStepTimer = 0;
  }
}

void MainRole::toggleShadow(bool flag) 
{
  if( flag )
  {
    mShadowCounter++;
  }
  else {
    mShadowCounter--;
  }
  if( mShadowCounter>0 )
  {
    mGhost->toggleShadow(true);
    if( mRoleId == 3 && mSprite->animationId()<3 )
    {
      mSprite->playGTAnimation(mSprite->animationId()+10, mSprite->looping());
    }
  }
  else {
    mGhost->toggleShadow(false);
    if( mRoleId == 3 && mSprite->animationId()>=10 )
    {
      mSprite->playGTAnimation(mSprite->animationId()-10, mSprite->looping());
    }
  }
}

void MainRole::toggleVisible(bool flag) 
{
  if( flag )
  {
    mVisibleCounter++;
  }
  else {
    mVisibleCounter--;
  }
  if( mVisibleCounter>0 )
  {
    mSprite->setVisible(true);
  }
  else {
    mSprite->setVisible(false);
  }
}

void MainRole::toggleInvincible(bool flag) 
{
  mInvincible = flag;
}

void MainRole::timedInvincible(float time) 
{
  if( mInvincibleTimer > 0 )
  {
    if( mInvincibleTimer < time )
    {
      mInvincibleTimer = time;
    }
  }
  else {
    mInvincibleTimer = time;
    this->toggleBlink(true);
  }
}

void MainRole::timedUnlimitedDarts(float time) 
{
  if( time > mScheduleUnlimitedDarts )
  {
    mScheduleUnlimitedDarts = time;
    GamePlay::sharedGamePlay()->interface->toggleUnlimtedDarts(true);
  }
}

void MainRole::toggleBlink(bool flag) 
{
  if( flag )
  {
    mBlinkCounter++;
  }
  else {
    mBlinkCounter--;
  }
  if( mBlinkCounter>0 )
  {
    if( mBlinkTimer < 0 )
    {
      mBlinkTimer = 0;
    }
  }
  else {
    mBlinkTimer = -1;
    mSprite->setOpacity(255);
  }
}

void MainRole::customAction(int key) 
{
  switch (key) {
    case 0:
      {//播放发射飞镖的动作
        if( mRoleId == 3 && mShadowCounter > 0 )
        {//猫忍者
          mSprite->playGTAnimation(1+10, false);
        }
        else {
          mSprite->playGTAnimation(1, false);
        }
      }
      break;
    case 1:
      {//播放复活的动作
        mSprite->playGTAnimation(9, false);
      }
      break;
  }
}

//碰撞检测
bool MainRole::collisionWithCircle(cocos2d::Point cc, float rad) 
{
  if( HP < 0 )
  {
    return false;
  }
  if( mRoleId == 2 )
  {
      if( mSprite != NULL )
      {
        if( exCollisionWithCircles(mSprite->getPosition(), 4.5, 12, 9, cc, rad) ||
            exCollisionWithCircles(mSprite->getPosition(), 17, 30, 14, cc, rad) )
        {
          return true;
        }
      }
  }
  else {
      if( mSprite != NULL )
      {
        if( exCollisionWithCircles(mSprite->getPosition(), 6, 12, 9, cc, rad) ||
            exCollisionWithCircles(mSprite->getPosition(), 17, 27, 13, cc, rad) )
        {
          return true;
        }
      }
  }
  return false;
}

//on hit process in classic mode
void MainRole::onHitClassic(int type) 
{
  GamePlay *play = GamePlay::sharedGamePlay();

  HP--;
  if( HP > 0 )
  {
    this->timedInvincible(1.5f);

    if( type == HIT_DART )
    {
      mSprite->playGTAnimation(5, false);
    }
    else {
      mSprite->playGTAnimation(4, false);
    }

    //Mr.Pussy 回血
    if( mRoleId == 3 )
    {
      mRecoveryTimer = 10;
    }

    //sound
cocos2d::CCString *hit = cocos2d::CCString::createWithFormat("hit-%d%d.mp3", mRoleId, (randomInt(2)+1));
    SimpleAudioEngine::sharedEngine()->playEffect(hit->getCString());

    //achievement wounded
    GameRecord::sharedGameRecord()->task->dispatchTask(ACH_WOUNDED, 1);
    play->wounded++;
    GameRecord::sharedGameRecord()->task->dispatchTask(ACH_ONEWOUNDED, play->wounded);
    if( play->roleId == 3 )
    {
      GameRecord::sharedGameRecord()->task->dispatchTask(ACH_ONEMAROONWOUND, play->wounded);
      GameRecord::sharedGameRecord()->task->dispatchTask(ACH_MAROONWOUND, 1);
    }
  }
  else {
    HP = 0;
    mSprite->playGTAnimation(6, false);

    //sound
cocos2d::CCString *hit = cocos2d::CCString::createWithFormat("die-%d.mp3", mRoleId);
    SimpleAudioEngine::sharedEngine()->playEffect(hit->getCString());

    //achievement death
    GameRecord::sharedGameRecord()->task->dispatchTask(ACH_DEATH, 1);
    switch (type) {
      case HIT_DART:
        {
          GameRecord::sharedGameRecord()->task->dispatchTask(ACH_DEADBYSHURIKEN, 1);
        }
        break;
      case HIT_BLADE:
        {
          GameRecord::sharedGameRecord()->task->dispatchTask(ACH_DEADBYSAMURAI, 1);
        }
        break;
      case HIT_ARCHER:
        {
          GameRecord::sharedGameRecord()->task->dispatchTask(ACH_DEADBYARCHER, 1);
        }
        break;
      case HIT_BOMB:
        {
          GameRecord::sharedGameRecord()->task->dispatchTask(ACH_DEADBYBOMB, 1);
        }
        break;
      case HIT_ICESPIKE:
        {
          GameRecord::sharedGameRecord()->task->dispatchTask(ACH_DEADBYICESPIKE, 1);
        }
        break;
    }

    //dismiss pumpkins
    PumpkinCircle::dismissPumpkins();
  }
}

//on hit process in arcade mode
void MainRole::onHitArcade(int type) 
{
  GamePlay *play = GamePlay::sharedGamePlay();

  play->arcade->onMainHit();//hit callback

  this->timedInvincible(1.5f);

  if( type == HIT_DART )
  {
    mSprite->playGTAnimation(5, false);
  }
  else {
    mSprite->playGTAnimation(4, false);
  }

  //sound
cocos2d::CCString *hit = cocos2d::CCString::createWithFormat("hit-%d%d.mp3", mRoleId, (randomInt(2)+1));
  SimpleAudioEngine::sharedEngine()->playEffect(hit->getCString());

  //achievement wounded
  play->wounded++;
  GameRecord::sharedGameRecord()->task->dispatchTask(ACH_ONEARCADEHIT, play->wounded);
  if( play->roleId == 3 )
  {
    GameRecord::sharedGameRecord()->task->dispatchTask(ACH_ONEMAROONWOUND, play->wounded);
    GameRecord::sharedGameRecord()->task->dispatchTask(ACH_MAROONWOUND, 1);
  }
}

//受到伤害
bool MainRole::deliverHit(int type, cocos2d::Point dir) 
{
  GamePlay *play = GamePlay::sharedGamePlay();
  if( HP <= 0 )
  {
    return false;
  }
  if( mInvincibleTimer > 0 || mInvincible )
  {
    return false;
  }
  //分身术
  if( this->cancelMain2() )
  {
    return false;
  }
  //替身术
  if( spellType == SPELL_REPLEACE && SP >= activeSP )
  {
    this->spell(dir);
  }
  else {
    if( play->mode == MODE_CLASSIC )
    {//Classic Mode
      this->onHitClassic(type);
    }
    else
    {//Arcade Mode
      this->onHitArcade(type);
    }
  }

  return true;
}

cocos2d::Point MainRole::position() 
{
  if( mSprite == NULL )
  {
    return mAIPos;
  }
  else {
    return mSprite->getPosition();
  }
}

void MainRole::setPosition(cocos2d::Point pos) 
{
  mSprite->setPosition(pos);
}

void MainRole::setShadowSpeed(float speed) 
{
  mGhost->setMoveSpeed(speed);
}

cocos2d::Point MainRole::center() 
{
  if( mRoleId == 2 )
  {
    return ccpAdd(mSprite->getPosition(), Vec2(12, 21));
  }
  else {
    return ccpAdd(mSprite->getPosition(), Vec2(9, 20));
  }
}

bool MainRole::supportAimAid() 
{
  return false;
}

cocos2d::Sprite* MainRole::sprite() 
{
  return mSprite;
}

void MainRole::attachEffect(const char * aniset, int aniid, bool loop, bool toroot) 
{
  GTAnimatedEffect *eff = GTAnimatedEffect::create(GTAnimation::loadedAnimationSet(aniset), aniid, loop);
  if( toroot )
  {
    eff->setAnchorPoint(cocos2d::Vec2(0.4f, 0.0625f));
    eff->setPosition(position());
    mParent->addChild(eff, LAYER_MAINROLE+1);
  }
  else {
    eff->setPosition(cocos2d::Vec2( 9, 20));
    mSprite->addChild(eff);
  }
}

void MainRole::gainSP(int sp) 
{
  if( SPDrain <= 0 )
  {
    if( mHiddenPower )
    {
      sp *= 1.5f;
    }
    SP+=sp;
    if( SP > maxSP )
    {
      SP = maxSP;
    }
  }
}

bool MainRole::cancelMain2() 
{
  GamePlay *play = GamePlay::sharedGamePlay();
  if( play->mainrole2 != NULL )
  {
    this->attachEffect("effect", 5, false, true);
    if( this == play->mainrole )
    {
      cocos2d::Point np = play->mainrole2->position();
      play->mainrole->setAI(2, np);
    }
    //删除第二个角色
    play->manager->removeGameObject(play->mainrole2);
    play->mainrole2 = NULL;
    mDrainAdder = 0;
    play->mainrole->SPDrain = 0;
    play->mainrole->SP = 0;
    return true;
  }
  return false;
}

void MainRole::onDestroy() 
{
  //mFireLock.release();
  mParent->removeChild(mSprite, false);
  mParent->removeChild(mGhost, false);
}
