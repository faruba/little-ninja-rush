//
//  GameScript.m
//  NinjiaRush
//
//  Created by 马 颂文 on 12-5-10.
//  Copyright (c) 2012年 TrinGame. All rights reserved.
//

#include "GameScript.h"
#include "GameConstants.h"
#include "GameRecord.h"
#include "GameData.h"
#include "Tasks.h"
#include "UniversalFit.h"
#include "ShadowKilling.h"
#include "Starfall.h"
#include "MoonWave.h"
#include "FeverBrust.h"
#include "PumpkinCircle.h"



GameScript* GameScript::sharedScript() 
{
    static GameScript *ret = NULL;
    if( ret == NULL )
    {
        ret = GameScript::create();
        ret->retain();
    }
    return ret;
}

//八方手里剑
void GameScript::invokeSpell0(cocos2d::CCInteger* mid) 
{
    int midn = mid->getValue();
    GamePlay *play = GamePlay::sharedGamePlay();
    int level = GameRecord::sharedGameRecord()->item_upgrade[0];
    int darts = 5 + level*2;
    float interval = 120/(darts+1);
    for(int i=0; i<darts; ++i)
    {
        float angle = CC_DEGREES_TO_RADIANS(30+(i+1)*interval);
        cocos2d::Point dir = ccpForAngle(angle);
        cocos2d::Point fp = play->mainrole->center();
        if( midn == 2 && play->mainrole2 != NULL )
        {
            fp = play->mainrole2->center();
            play->mainrole2->customAction(0);
        }
        else {
            play->mainrole->customAction(0);
        }
//        play->darts->addObject(play->manager->addGameObject(Dart::dart(cocos2d::CCString::create("dart.png"), fp, dir, -5, play)));
    }
    GameTool::PlaySound("duofa.mp3");
}

//分身术
void GameScript::invokeSpell1_0() 
{
    GameTool::PlaySound("smoke.mp3");
    GamePlay *play = GamePlay::sharedGamePlay();
    //制造两个分身
    float mpos = play->mainrole->position().x;
    if( mpos < SPECIAL_FENSHEN)
    {
        mpos = SPECIAL_FENSHEN;
    }
    if( mpos > UniversalFit::sharedUniversalFit()->playSize.width-SPECIAL_FENSHEN )
    {
        mpos = UniversalFit::sharedUniversalFit()->playSize.width-SPECIAL_FENSHEN;
    }
    cocos2d::Point np = play->mainrole->position();
    np.x = mpos;
    np.x += SPECIAL_FENSHEN/2.0f;
    //制造第一个分身
    play->mainrole->setAI(2, np);
    play->mainrole->attachEffect("effect", 5, false, true);
    play->mainrole2 = (MainRole*)play->manager->addGameObject(MainRole::role(play));
    np.x -= SPECIAL_FENSHEN;
    play->mainrole2->setAI(2, np);
    play->mainrole2->attachEffect("effect", 5, false, true);
    play->count_control--;
}

//隐身术
void GameScript::invokeSpell2(cocos2d::CCInteger*) 
{
    GamePlay *play = GamePlay::sharedGamePlay();
    int level = GameRecord::sharedGameRecord()->item_upgrade[1];
    int time = 5 + level;
    play->mainrole->attachEffect("effect", 5, false, true);
    play->mainrole->timedInvincible(time);
    if( play->mainrole2 != NULL )
    {
        play->mainrole2->attachEffect("effect", 5, false, true);
        play->mainrole2->timedInvincible(time);
    }
}

//子弹时间
void GameScript::invokeSpell3(cocos2d::CCInteger*) 
{
    GamePlay *play = GamePlay::sharedGamePlay();
    GameTool::PlaySound("spell.mp3");
    play->manager->addGameObject(FeverBrust::feverbrust(22, false));
    play->scheduleSpeed(0.1f, 3, 2);
    play->mainrole->timedUnlimitedDarts(3);
    if( play->mainrole2 != NULL )
    {
        play->mainrole2->timedUnlimitedDarts(3);
    }
}

//加血
void GameScript::invokeSpell4(cocos2d::Node*,void*) 
{
    GamePlay *play = GamePlay::sharedGamePlay();
    if( play->mainrole->HP < play->mainrole->maxHP )
    {
        play->mainrole->HP++;
    }
}

//加气
void GameScript::invokeSpell5(cocos2d::Node*,void*)
{
    GamePlay *play = GamePlay::sharedGamePlay();
    play->mainrole->SP = play->mainrole->maxSP;
}

//一闪
void GameScript::invokeSpell6(cocos2d::CCInteger*) 
{
    GamePlay *play = GamePlay::sharedGamePlay();
    play->scheduleMask(Color3B(255, 255, 255), 0.5f, 1);
    GameTool::PlaySound("hit.mp3");
    unsigned int n = 0;
    while( n < play->darts->count() )
    {
        Darts *d = (Darts*)play->darts->objectAtIndex(n);
        if( d->isEnemy() )
        {
            GTAnimatedEffect *hiteff2 = GTAnimatedEffect::create(GTAnimation::loadedAnimationSet("effect"), 2, false);
            hiteff2->setAnchorPoint(cocos2d::Vec2(0.5f, 0.5f));
            hiteff2->setScale(1.5f);
            hiteff2->setPosition(d->position());
            play->addChild(hiteff2, LAYER_ROLE);
            
            play->manager->removeGameObject(d);
            play->darts->removeObjectAtIndex(n);
        }
        else {
            n++;
        }
    }
}

//流星雨
void GameScript::invokeSpell7(cocos2d::CCInteger*) 
{
    GamePlay *play = GamePlay::sharedGamePlay();
    play->manager->addGameObject(Starfall::starfall());
}

//飞镖无限
void GameScript::invokeSpell8(cocos2d::CCInteger*) 
{
    GamePlay *play = GamePlay::sharedGamePlay();
    int level = GameRecord::sharedGameRecord()->item_upgrade[7];
    int time = 5+level;
    play->mainrole->timedUnlimitedDarts(time);
    if( play->mainrole2 != NULL )
    {
        play->mainrole2->timedUnlimitedDarts(time);
    }
}

//全屏分身
void GameScript::invokeSpell9() 
{
    GamePlay *play = GamePlay::sharedGamePlay();
    play->manager->addGameObject(ShadowKilling::shadowKilling(play));
}

//--- 道具技能 ---
//手里剑旋风
void GameScript::invokeSpell15() 
{
  GamePlay *play = GamePlay::sharedGamePlay();
  int cr = GameRecord::sharedGameRecord()->curr_char;
  int did = GameRecord::sharedGameRecord()->char_equip_dart[cr];

  Shuriken &sk = GameData::fetchShurikens()[did];
cocos2d::Ref* node = NULL;
CCARRAY_FOREACH(play->enemies, node)
  {
    Role * em = (Role*)node;
    if( em->supportAimAid() && em->position().x > 0 && em->position().x < UniversalFit::sharedUniversalFit()->playSize.width )
    {
      if( play->mainrole->dart > 0 )
      {
        cocos2d::Point dir = ccpNormalize(ccpSub(em->center(), play->mainrole->center()));
        Dart *d = Dart::dart(sk.shap, play->mainrole->center(), dir, sk.efft, play);
        play->darts->addObject(play->manager->addGameObject(d));
        play->mainrole->dart--;
        //achievement fired shurikens
        GameRecord::sharedGameRecord()->task->dispatchTask(ACH_FIREDSHURIKENS, 1);
        play->firecount+=1;
        GameRecord::sharedGameRecord()->task->dispatchTask(ACH_ONEFIRE, play->firecount);
        if( play->roleId == 1 )
        {
          GameRecord::sharedGameRecord()->task->dispatchTask(ACH_ONESAKURAFIRE, play->firecount);
          GameRecord::sharedGameRecord()->task->dispatchTask(ACH_SAKURAFIRE, 1);
        }
      }
      else {
        break;
      }
    }
  }
  GameTool::PlaySound("duofa.mp3");
  play->mainrole->customAction(0);
}

//元气爆
void GameScript::invokeSpell16() 
{
  GamePlay *play = GamePlay::sharedGamePlay();
  play->manager->addGameObject(FeverBrust::feverbrust(20, false));
  cocos2d::Point mainpos = play->mainrole->position();
  cocos2d::Point maincen = play->mainrole->center();
cocos2d::Ref* node = NULL;
CCARRAY_FOREACH(play->darts, node)
  {
    Darts *dr = (Darts*)node;
    if( dr->isEnemy() && exCollisionWithCircles(maincen, 0, 0, 70*2, dr->position(), 7) && dr->position().y > mainpos.y )
    {
      dr->onHitback(maincen);
    }
  }
  GameTool::PlaySound("spell.mp3");
}

//妖刀
void GameScript::invokeSpell17() 
{
  GamePlay *play = GamePlay::sharedGamePlay();
  play->scheduleMask(Color3B(255, 0, 0), 0.1f, 1);
  GameTool::PlaySound("blade-clay.mp3");
cocos2d::Ref* node = NULL;
CCARRAY_FOREACH(play->enemies, node)
  {
    Role *em = (Role*)node;
    GTAnimatedEffect *hiteff = GTAnimatedEffect::create(GTAnimation::loadedAnimationSet("effect"), 1, false);
    hiteff->setAnchorPoint(cocos2d::Vec2(0.5f, 0.5f));
    hiteff->setPosition(em->center());
    hiteff->setRotation(360*CCRANDOM_0_1());
    play->addChild(hiteff, LAYER_MAINROLE+1);

    GTAnimatedEffect *hiteff2 = GTAnimatedEffect::create(GTAnimation::loadedAnimationSet("effect"), 13, false);
    hiteff2->setAnchorPoint(cocos2d::Vec2(0.5f, 0.5f));
    hiteff2->setPosition(em->center());
    play->addChild(hiteff2, LAYER_ROLE);

    cocos2d::Point dir = ccpNormalize(ccpSub(em->center(), play->mainrole->center()));
    em->deliverHit(HIT_BLADE, dir);
  }
}

//大招前兆
void GameScript::invokeSpellRelease(cocos2d::CCInteger * sid) 
{
    GamePlay *play = GamePlay::sharedGamePlay();
    int num = sid->getValue();
    GTAnimatedEffect *eff = GTAnimatedEffect::create(GTAnimation::loadedAnimationSet("effect"), 6, false);
    eff->setAnchorPoint(cocos2d::Vec2(0.5f, 0.5f));
    eff->setPosition(play->mainrole->center());
    play->addChild(eff, LAYER_MAINROLE+1);
    GameTool::PlaySound("spell.mp3");
    switch (num) {
        case SPELL_TRIDARTS:
        {
            play->scheduleSpellRelease(this, callfunc_selector(GameScript::invokeSpell15));
        }
            break;
        case SPELL_EVILBLADE:
        {
            play->scheduleSpellRelease(this, callfunc_selector(GameScript::invokeSpell17));
        }
            break;
        case SPELL_SHADOWKILL:
        {
            play->scheduleSpellRelease(this, callfunc_selector(GameScript::invokeSpell9));
        }
            break;
        case SPELL_FLY:
        {
            play->scheduleSpellRelease(this, callfunc_selector(GameScript::invokeFly));
        }
            break;
        case SPELL_MOONBLADE:
        {
            play->scheduleSpellRelease(this, callfunc_selector(GameScript::invokeMoonBlade));
        }
            break;
        case SPELL_PUMPKINIMPS:
        {
            play->scheduleSpellRelease(this, callfunc_selector(GameScript::invokePumpkinImps));
        }
            break;
    }
}

void GameScript::invokeFly() 
{
    GamePlay *play = GamePlay::sharedGamePlay();
    play->state = STATE_RUSH;
    //-------------
    play->runspeed = 2*play->levelspeed;
    play->count_attack++;
    play->count_control++;
    play->mainrole->toggleShadow(true);
    play->mainrole->setShadowSpeed(-100);
    //mMenu->removeChild(mFly, true);
    play->mainrole->toggleInvincible(true);
}

void GameScript::invokeSP() 
{
    GamePlay *play = GamePlay::sharedGamePlay();
    play->mainrole->SP = play->mainrole->maxSP;
    //mMenu->removeChild(mSP, true);
}

void GameScript::invokeMoonBlade() 
{
    GamePlay *play = GamePlay::sharedGamePlay();
    GameTool::PlaySound("moonblade.mp3");
    play->manager->addGameObject(MoonWave::wave(play->mainrole->position()));
}

void GameScript::invokePumpkinImps() 
{
  PumpkinCircle::summonPumpkin();
}
