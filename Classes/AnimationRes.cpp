#include "AnimationRes.h"
#include "GTAnimation.h"

void addAnimation(GTAnimation* ani, const char name[], int count, int initialPos, bool reverse = false) {
  float durationPerFrame = 0.05f;
  const std::string head = std::string(name);
  ani->startAnimation(count, durationPerFrame*count);
  for (int i = 0; i < count; i++) {
    int id = initialPos + (reverse? -i : i);
    ani->addFrame((head+std::to_string(id)+".png").c_str());
  }
  ani->endAnimation();
}

void assetMainRole(const char* param, bool isload)
{
    if( isload )
    {
        GTAnimation *ani = GTAnimation::startAnimationSet(10);
        addAnimation(ani, "main_run", 3, 1);
        addAnimation(ani, "main_attack", 5, 1);
        addAnimation(ani, "main_attack", 5, 6);
        addAnimation(ani, "main_attack", 7, 11);
        addAnimation(ani, "main_fall", 7, 1);
        addAnimation(ani, "main_fall", 7, 8);
        addAnimation(ani, "main_die", 6, 1);
        addAnimation(ani, "main_die", 2, 7);
        addAnimation(ani, "main_attack", 1, 11);
        addAnimation(ani, "main_die", 8, 8, true);
        ani->endAnimationSet();
        GTAnimation::loadAnimationSet(ani, "main");
    }
    else
    {
      GTAnimation::unloadAnimationSet("main");
    }
}

void assetMainGirl(const char* param, bool isload)
{
    if( isload )
    {
        GTAnimation *ani = GTAnimation::startAnimationSet(10);
        ani->startAnimation(3, 0.15f);
        ani->addFrame("girl_run1.png");
        ani->addFrame("girl_run2.png");
        ani->addFrame("girl_run3.png");
        ani->endAnimation();
        ani->startAnimation(5, 0.25f);
        ani->addFrame("girl_attack1.png");
        ani->addFrame("girl_attack2.png");
        ani->addFrame("girl_attack3.png");
        ani->addFrame("girl_attack4.png");
        ani->addFrame("girl_attack5.png");
        ani->endAnimation();
        ani->startAnimation(5, 0.25f);
        ani->addFrame("girl_attack6.png");
        ani->addFrame("girl_attack7.png");
        ani->addFrame("girl_attack8.png");
        ani->addFrame("girl_attack9.png");
        ani->addFrame("girl_attack10.png");
        ani->endAnimation();
        ani->startAnimation(7, 0.35f);
        ani->addFrame("girl_attack11.png");
        ani->addFrame("girl_attack12.png");
        ani->addFrame("girl_attack13.png");
        ani->addFrame("girl_attack14.png");
        ani->addFrame("girl_attack15.png");
        ani->addFrame("girl_attack16.png");
        ani->addFrame("girl_attack17.png");
        ani->endAnimation();
        ani->startAnimation(7, 0.35f);
        ani->addFrame("girl_fall1.png");
        ani->addFrame("girl_fall2.png");
        ani->addFrame("girl_fall3.png");
        ani->addFrame("girl_fall4.png");
        ani->addFrame("girl_fall5.png");
        ani->addFrame("girl_fall6.png");
        ani->addFrame("girl_fall7.png");
        ani->endAnimation();
        ani->startAnimation(7, 0.35f);
        ani->addFrame("girl_fall8.png");
        ani->addFrame("girl_fall9.png");
        ani->addFrame("girl_fall10.png");
        ani->addFrame("girl_fall11.png");
        ani->addFrame("girl_fall12.png");
        ani->addFrame("girl_fall13.png");
        ani->addFrame("girl_fall14.png");
        ani->endAnimation();
        ani->startAnimation(6, 0.3f);
        ani->addFrame("girl_die1.png");
        ani->addFrame("girl_die2.png");
        ani->addFrame("girl_die3.png");
        ani->addFrame("girl_die4.png");
        ani->addFrame("girl_die5.png");
        ani->addFrame("girl_die6.png");
        ani->endAnimation();
        ani->startAnimation(2, 0.1f);
        ani->addFrame("girl_die7.png");
        ani->addFrame("girl_die8.png");
        ani->endAnimation();
        ani->startAnimation(1, 0.05f);
        ani->addFrame("girl_attack11.png");
        ani->endAnimation();
        ani->startAnimation(8, 0.5f);
        ani->addFrame("girl_die8.png");
        ani->addFrame("girl_die7.png");
        ani->addFrame("girl_die6.png");
        ani->addFrame("girl_die5.png");
        ani->addFrame("girl_die4.png");
        ani->addFrame("girl_die3.png");
        ani->addFrame("girl_die2.png");
        ani->addFrame("girl_die1.png");
        ani->endAnimation();
        ani->endAnimationSet();
        GTAnimation::loadAnimationSet(ani, "girl");
    }
    else
    {
      GTAnimation::unloadAnimationSet("girl");
    }
}

void assetMainMusashi(const char* param, bool isload)
{
    if( isload )
    {
        GTAnimation *ani = GTAnimation::startAnimationSet(11);
        ani->startAnimation(8, 0.4f);
        ani->addFrame("musashi_run1.png");
        ani->addFrame("musashi_run2.png");
        ani->addFrame("musashi_run3.png");
        ani->addFrame("musashi_run4.png");
        ani->addFrame("musashi_run5.png");
        ani->addFrame("musashi_run6.png");
        ani->addFrame("musashi_run7.png");
        ani->addFrame("musashi_run8.png");
        ani->endAnimation();
        ani->startAnimation(5, 0.25f);
        ani->addFrame("musashi_attack1.png");
        ani->addFrame("musashi_attack2.png");
        ani->addFrame("musashi_attack3.png");
        ani->addFrame("musashi_attack4.png");
        ani->addFrame("musashi_attack5.png");
        ani->endAnimation();
        ani->startAnimation(5, 0.25f);
        ani->addFrame("musashi_attack6.png");
        ani->addFrame("musashi_attack7.png");
        ani->addFrame("musashi_attack8.png");
        ani->addFrame("musashi_attack9.png");
        ani->addFrame("musashi_attack10.png");
        ani->endAnimation();
        ani->startAnimation(6, 0.3f);
        ani->addFrame("musashi_attack11.png");
        ani->addFrame("musashi_attack12.png");
        ani->addFrame("musashi_attack13.png");
        ani->addFrame("musashi_attack14.png");
        ani->addFrame("musashi_attack15.png");
        ani->addFrame("musashi_attack12.png");
        ani->endAnimation();
        ani->startAnimation(7, 0.35f);
        ani->addFrame("musashi_fall1.png");
        ani->addFrame("musashi_fall2.png");
        ani->addFrame("musashi_fall3.png");
        ani->addFrame("musashi_fall4.png");
        ani->addFrame("musashi_fall5.png");
        ani->addFrame("musashi_fall6.png");
        ani->addFrame("musashi_fall7.png");
        ani->endAnimation();
        ani->startAnimation(7, 0.35f);
        ani->addFrame("musashi_fall8.png");
        ani->addFrame("musashi_fall9.png");
        ani->addFrame("musashi_fall10.png");
        ani->addFrame("musashi_fall11.png");
        ani->addFrame("musashi_fall12.png");
        ani->addFrame("musashi_fall13.png");
        ani->addFrame("musashi_fall14.png");
        ani->endAnimation();
        ani->startAnimation(6, 0.3f);
        ani->addFrame("musashi_die1.png");
        ani->addFrame("musashi_die2.png");
        ani->addFrame("musashi_die3.png");
        ani->addFrame("musashi_die4.png");
        ani->addFrame("musashi_die5.png");
        ani->addFrame("musashi_die6.png");
        ani->endAnimation();
        ani->startAnimation(2, 0.1f);
        ani->addFrame("musashi_die7.png");
        ani->addFrame("musashi_die8.png");
        ani->endAnimation();
        ani->startAnimation(1, 0.05f);
        ani->addFrame("musashi_attack11.png");
        ani->endAnimation();
        ani->startAnimation(8, 0.5f);
        ani->addFrame("musashi_die8.png");
        ani->addFrame("musashi_die7.png");
        ani->addFrame("musashi_die6.png");
        ani->addFrame("musashi_die5.png");
        ani->addFrame("musashi_die4.png");
        ani->addFrame("musashi_die3.png");
        ani->addFrame("musashi_die2.png");
        ani->addFrame("musashi_die1.png");
        ani->endAnimation();
        //武藏的拔刀动作2 多出来的动作必须放在最后面
        ani->startAnimation(6, 0.3f);
        ani->addFrame("musashi_attack16.png");
        ani->addFrame("musashi_attack17.png");
        ani->addFrame("musashi_attack18.png");
        ani->addFrame("musashi_attack19.png");
        ani->addFrame("musashi_attack20.png");
        ani->addFrame("musashi_attack17.png");
        ani->endAnimation();
        ani->endAnimationSet();
        GTAnimation::loadAnimationSet(ani, "musashi");
    }
    else
    {
      GTAnimation::unloadAnimationSet("musashi");
    }
}

void assetMainPussy(const char* param, bool isload)
{
    if( isload )
    {
        GTAnimation *ani = GTAnimation::startAnimationSet(13);
        ani->startAnimation(3, 0.15f);
        ani->addFrame("cat_run1.png");
        ani->addFrame("cat_run2.png");
        ani->addFrame("cat_run3.png");
        ani->endAnimation();
        ani->startAnimation(5, 0.25f);
        ani->addFrame("cat_attack1.png");
        ani->addFrame("cat_attack2.png");
        ani->addFrame("cat_attack3.png");
        ani->addFrame("cat_attack4.png");
        ani->addFrame("cat_attack5.png");
        ani->endAnimation();
        ani->startAnimation(5, 0.25f);
        ani->addFrame("cat_attack6.png");
        ani->addFrame("cat_attack7.png");
        ani->addFrame("cat_attack8.png");
        ani->addFrame("cat_attack9.png");
        ani->addFrame("cat_attack10.png");
        ani->endAnimation();
        ani->startAnimation(7, 0.35f);
        ani->addFrame("cat_attack11.png");
        ani->addFrame("cat_attack12.png");
        ani->addFrame("cat_attack13.png");
        ani->addFrame("cat_attack14.png");
        ani->addFrame("cat_attack15.png");
        ani->addFrame("cat_attack16.png");
        ani->addFrame("cat_attack17.png");
        ani->endAnimation();
        ani->startAnimation(7, 0.35f);
        ani->addFrame("cat_fall1.png");
        ani->addFrame("cat_fall2.png");
        ani->addFrame("cat_fall3.png");
        ani->addFrame("cat_fall4.png");
        ani->addFrame("cat_fall5.png");
        ani->addFrame("cat_fall6.png");
        ani->addFrame("cat_fall7.png");
        ani->endAnimation();
        ani->startAnimation(7, 0.35f);
        ani->addFrame("cat_fall8.png");
        ani->addFrame("cat_fall9.png");
        ani->addFrame("cat_fall10.png");
        ani->addFrame("cat_fall11.png");
        ani->addFrame("cat_fall12.png");
        ani->addFrame("cat_fall13.png");
        ani->addFrame("cat_fall14.png");
        ani->endAnimation();
        ani->startAnimation(6, 0.3f);
        ani->addFrame("cat_die1.png");
        ani->addFrame("cat_die2.png");
        ani->addFrame("cat_die3.png");
        ani->addFrame("cat_die4.png");
        ani->addFrame("cat_die5.png");
        ani->addFrame("cat_die6.png");
        ani->endAnimation();
        ani->startAnimation(2, 0.1f);
        ani->addFrame("cat_die7.png");
        ani->addFrame("cat_die8.png");
        ani->endAnimation();
        ani->startAnimation(1, 0.05f);
        ani->addFrame("cat_attack11.png");
        ani->endAnimation();
        ani->startAnimation(8, 0.5f);
        ani->addFrame("cat_die8.png");
        ani->addFrame("cat_die7.png");
        ani->addFrame("cat_die6.png");
        ani->addFrame("cat_die5.png");
        ani->addFrame("cat_die4.png");
        ani->addFrame("cat_die3.png");
        ani->addFrame("cat_die2.png");
        ani->addFrame("cat_die1.png");
        ani->endAnimation();
        //----- 猫咪四脚的动作 -------
        ani->startAnimation(6, 0.3f);
        ani->addFrame("cat_run4.png");
        ani->addFrame("cat_run5.png");
        ani->addFrame("cat_run6.png");
        ani->addFrame("cat_run7.png");
        ani->addFrame("cat_run8.png");
        ani->addFrame("cat_run9.png");
        ani->endAnimation();
        ani->startAnimation(5, 0.25f);
        ani->addFrame("cat_attack01.png");
        ani->addFrame("cat_attack02.png");
        ani->addFrame("cat_attack03.png");
        ani->addFrame("cat_attack04.png");
        ani->addFrame("cat_attack05.png");
        ani->endAnimation();
        ani->startAnimation(6, 0.3f);
        ani->addFrame("cat_attack06.png");
        ani->addFrame("cat_attack07.png");
        ani->addFrame("cat_attack08.png");
        ani->addFrame("cat_attack09.png");
        ani->addFrame("cat_attack010.png");
        ani->addFrame("cat_attack011.png");
        ani->endAnimation();
        ani->endAnimationSet();
        GTAnimation::loadAnimationSet(ani, "pussy");
    }
    else
    {
      GTAnimation::unloadAnimationSet("pussy");
    }
}


void assetEnemy(const char* param, bool isload)
{
    if( isload )
    {
        GTAnimation *ani = GTAnimation::startAnimationSet(7);
        ani->startAnimation(3, 0.17f);
        ani->addFrame("enemy_run1.png");
        ani->addFrame("enemy_run2.png");
        ani->addFrame("enemy_run3.png");
        ani->endAnimation();
        ani->startAnimation(13, 0.65f);
        ani->addFrame("enemy_fall1.png");
        ani->addFrame("enemy_fall2.png");
        ani->addFrame("enemy_fall3.png");
        ani->addFrame("enemy_fall4.png");
        ani->addFrame("enemy_fall5.png");
        ani->addFrame("enemy_fall6.png");
        ani->addFrame("enemy_fall7.png");
        ani->addFrame("enemy_fall8.png");
        ani->addFrame("enemy_fall9.png");
        ani->addFrame("enemy_fall10.png");
        ani->addFrame("enemy_fall11.png");
        ani->addFrame("enemy_fall12.png");
        ani->addFrame("enemy_fall13.png");
        ani->endAnimation();
        ani->startAnimation(15, 0.75f);
        ani->addFrame("enemy_fall1.png");
        ani->addFrame("enemy_fall2.png");
        ani->addFrame("enemy_fall3.png");
        ani->addFrame("enemy_fall4.png");
        ani->addFrame("enemy_fall5.png");
        ani->addFrame("enemy_fall6.png");
        ani->addFrame("enemy_fall7.png");
        ani->addFrame("enemy_fall14.png");
        ani->addFrame("enemy_fall15.png");
        ani->addFrame("enemy_fall16.png");
        ani->addFrame("enemy_fall17.png");
        ani->addFrame("enemy_fall18.png");
        ani->addFrame("enemy_fall19.png");
        ani->addFrame("enemy_fall20.png");
        ani->addFrame("enemy_fall21.png");
        ani->endAnimation();
        ani->startAnimation(13, 0.65f);
        ani->addFrame("enemy_fall22.png");
        ani->addFrame("enemy_fall23.png");
        ani->addFrame("enemy_fall24.png");
        ani->addFrame("enemy_fall25.png");
        ani->addFrame("enemy_fall26.png");
        ani->addFrame("enemy_fall27.png");
        ani->addFrame("enemy_fall28.png");
        ani->addFrame("enemy_fall29.png");
        ani->addFrame("enemy_fall30.png");
        ani->addFrame("enemy_fall30.png");
        ani->addFrame("enemy_fall30.png");
        ani->addFrame("enemy_fall33.png");
        ani->addFrame("enemy_fall34.png");
        ani->endAnimation();
        ani->startAnimation(4, 0.2f);
        ani->addFrame("enemy_fall10.png");
        ani->addFrame("enemy_fall11.png");
        ani->addFrame("enemy_fall12.png");
        ani->addFrame("enemy_fall13.png");
        ani->endAnimation();
        ani->startAnimation(5, 0.5f);
        ani->addFrame("enemy_attack6.png");
        ani->addFrame("enemy_attack7.png");
        ani->addFrame("enemy_attack8.png");
        ani->addFrame("enemy_attack9.png");
        ani->addFrame("enemy_attack10.png");
        ani->endAnimation();
        ani->startAnimation(3, 0.15f);
        ani->addFrame("enemy_attack11.png");
        ani->addFrame("enemy_attack12.png");
        ani->addFrame("enemy_attack13.png");
        ani->endAnimation();
        ani->endAnimationSet();
        GTAnimation::loadAnimationSet(ani, "enemy");
    }
    else
    {
      GTAnimation::unloadAnimationSet("enemy");
    }
}

void assetMninja(const char* param, bool isload)
{
    if( isload )
    {
        GTAnimation *ani = GTAnimation::startAnimationSet(9);
        ani->startAnimation(3, 0.17f);
        ani->addFrame("mninja_run1.png");
        ani->addFrame("mninja_run2.png");
        ani->addFrame("mninja_run3.png");
        ani->endAnimation();
        ani->startAnimation(13, 0.65f);
        ani->addFrame("mninja_fall1.png");
        ani->addFrame("mninja_fall2.png");
        ani->addFrame("mninja_fall3.png");
        ani->addFrame("mninja_fall4.png");
        ani->addFrame("mninja_fall5.png");
        ani->addFrame("mninja_fall6.png");
        ani->addFrame("mninja_fall7.png");
        ani->addFrame("mninja_fall8.png");
        ani->addFrame("mninja_fall9.png");
        ani->addFrame("mninja_fall10.png");
        ani->addFrame("mninja_fall11.png");
        ani->addFrame("mninja_fall12.png");
        ani->addFrame("mninja_fall13.png");
        ani->endAnimation();
        ani->startAnimation(15, 0.75f);
        ani->addFrame("mninja_fall1.png");
        ani->addFrame("mninja_fall2.png");
        ani->addFrame("mninja_fall3.png");
        ani->addFrame("mninja_fall4.png");
        ani->addFrame("mninja_fall5.png");
        ani->addFrame("mninja_fall6.png");
        ani->addFrame("mninja_fall7.png");
        ani->addFrame("mninja_fall14.png");
        ani->addFrame("mninja_fall15.png");
        ani->addFrame("mninja_fall16.png");
        ani->addFrame("mninja_fall17.png");
        ani->addFrame("mninja_fall18.png");
        ani->addFrame("mninja_fall19.png");
        ani->addFrame("mninja_fall20.png");
        ani->addFrame("mninja_fall21.png");
        ani->endAnimation();
        ani->startAnimation(13, 0.65f);
        ani->addFrame("mninja_fall22.png");
        ani->addFrame("mninja_fall23.png");
        ani->addFrame("mninja_fall24.png");
        ani->addFrame("mninja_fall25.png");
        ani->addFrame("mninja_fall26.png");
        ani->addFrame("mninja_fall27.png");
        ani->addFrame("mninja_fall28.png");
        ani->addFrame("mninja_fall29.png");
        ani->addFrame("mninja_fall30.png");
        ani->addFrame("mninja_fall30.png");
        ani->addFrame("mninja_fall30.png");
        ani->addFrame("mninja_fall33.png");
        ani->addFrame("mninja_fall34.png");
        ani->endAnimation();
        ani->startAnimation(4, 0.2f);
        ani->addFrame("mninja_fall10.png");
        ani->addFrame("mninja_fall11.png");
        ani->addFrame("mninja_fall12.png");
        ani->addFrame("mninja_fall13.png");
        ani->endAnimation();
        ani->startAnimation(5, 0.5f);
        ani->addFrame("mninja_attack6.png");
        ani->addFrame("mninja_attack7.png");
        ani->addFrame("mninja_attack8.png");
        ani->addFrame("mninja_attack9.png");
        ani->addFrame("mninja_attack10.png");
        ani->endAnimation();
        ani->startAnimation(3, 0.15f);
        ani->addFrame("mninja_attack11.png");
        ani->addFrame("mninja_attack12.png");
        ani->addFrame("mninja_attack13.png");
        ani->endAnimation();
        ani->startAnimation(1, 0.05f);
        ani->addFrame("mninja_run4.png");
        ani->endAnimation();
        ani->startAnimation(6, 0.3f);
        ani->addFrame("mninja_run5.png");
        ani->addFrame("mninja_run6.png");
        ani->addFrame("mninja_run7.png");
        ani->addFrame("mninja_run8.png");
        ani->addFrame("mninja_run9.png");
        ani->addFrame("mninja_run10.png");
        ani->endAnimation();
        ani->endAnimationSet();
        GTAnimation::loadAnimationSet(ani, "mninja");
    }
    else
    {
      GTAnimation::unloadAnimationSet("mninja");
    }
}

void assetHninja(const char* param, bool isload)
{
    if( isload )
    {
        GTAnimation *ani = GTAnimation::startAnimationSet(7);
        ani->startAnimation(3, 0.17f);
        ani->addFrame("hninja_run1.png");
        ani->addFrame("hninja_run2.png");
        ani->addFrame("hninja_run3.png");
        ani->endAnimation();
        ani->startAnimation(13, 0.65f);
        ani->addFrame("hninja_fall1.png");
        ani->addFrame("hninja_fall2.png");
        ani->addFrame("hninja_fall3.png");
        ani->addFrame("hninja_fall4.png");
        ani->addFrame("hninja_fall5.png");
        ani->addFrame("hninja_fall6.png");
        ani->addFrame("hninja_fall7.png");
        ani->addFrame("hninja_fall8.png");
        ani->addFrame("hninja_fall9.png");
        ani->addFrame("hninja_fall10.png");
        ani->addFrame("hninja_fall11.png");
        ani->addFrame("hninja_fall12.png");
        ani->addFrame("hninja_fall13.png");
        ani->endAnimation();
        ani->startAnimation(15, 0.75f);
        ani->addFrame("hninja_fall1.png");
        ani->addFrame("hninja_fall2.png");
        ani->addFrame("hninja_fall3.png");
        ani->addFrame("hninja_fall4.png");
        ani->addFrame("hninja_fall5.png");
        ani->addFrame("hninja_fall6.png");
        ani->addFrame("hninja_fall7.png");
        ani->addFrame("hninja_fall14.png");
        ani->addFrame("hninja_fall15.png");
        ani->addFrame("hninja_fall16.png");
        ani->addFrame("hninja_fall17.png");
        ani->addFrame("hninja_fall18.png");
        ani->addFrame("hninja_fall19.png");
        ani->addFrame("hninja_fall20.png");
        ani->addFrame("hninja_fall21.png");
        ani->endAnimation();
        ani->startAnimation(13, 0.65f);
        ani->addFrame("hninja_fall22.png");
        ani->addFrame("hninja_fall23.png");
        ani->addFrame("hninja_fall24.png");
        ani->addFrame("hninja_fall25.png");
        ani->addFrame("hninja_fall26.png");
        ani->addFrame("hninja_fall27.png");
        ani->addFrame("hninja_fall28.png");
        ani->addFrame("hninja_fall29.png");
        ani->addFrame("hninja_fall30.png");
        ani->addFrame("hninja_fall30.png");
        ani->addFrame("hninja_fall30.png");
        ani->addFrame("hninja_fall33.png");
        ani->addFrame("hninja_fall34.png");
        ani->endAnimation();
        ani->startAnimation(4, 0.2f);
        ani->addFrame("hninja_fall10.png");
        ani->addFrame("hninja_fall11.png");
        ani->addFrame("hninja_fall12.png");
        ani->addFrame("hninja_fall13.png");
        ani->endAnimation();
        ani->startAnimation(5, 0.5f);
        ani->addFrame("hninja_attack6.png");
        ani->addFrame("hninja_attack7.png");
        ani->addFrame("hninja_attack8.png");
        ani->addFrame("hninja_attack9.png");
        ani->addFrame("hninja_attack10.png");
        ani->endAnimation();
        ani->startAnimation(3, 0.15f);
        ani->addFrame("hninja_attack11.png");
        ani->addFrame("hninja_attack12.png");
        ani->addFrame("hninja_attack13.png");
        ani->endAnimation();
        ani->endAnimationSet();
        GTAnimation::loadAnimationSet(ani, "hninja");
    }
    else
    {
      GTAnimation::unloadAnimationSet("hninja");
    }
}

void assetSamurai(const char* param, bool isload)
{
    if( isload )
    {
        GTAnimation *ani = GTAnimation::startAnimationSet(3);
        ani->startAnimation(1, 0.05f);
        ani->addFrame("samurai_attack1.png");
        ani->endAnimation();
        ani->startAnimation(10, 0.5f);
        ani->addFrame("samurai_attack1.png");
        ani->addFrame("samurai_attack2.png");
        ani->addFrame("samurai_attack3.png");
        ani->addFrame("samurai_attack4.png");
        ani->addFrame("samurai_attack5.png");
        ani->addFrame("samurai_attack6.png");
        ani->addFrame("samurai_attack7.png");
        ani->addFrame("samurai_attack8.png");
        ani->addFrame("samurai_attack9.png");
        ani->addFrame("samurai_attack10.png");
        ani->endAnimation();
        ani->startAnimation(14, 0.7f);
        ani->addFrame("samurai_fall1.png");
        ani->addFrame("samurai_fall2.png");
        ani->addFrame("samurai_fall3.png");
        ani->addFrame("samurai_fall4.png");
        ani->addFrame("samurai_fall5.png");
        ani->addFrame("samurai_fall6.png");
        ani->addFrame("samurai_fall7.png");
        ani->addFrame("samurai_fall8.png");
        ani->addFrame("samurai_fall9.png");
        ani->addFrame("samurai_fall10.png");
        ani->addFrame("samurai_fall11.png");
        ani->addFrame("samurai_fall12.png");
        ani->addFrame("samurai_fall11.png");
        ani->addFrame("samurai_fall10.png");
        ani->endAnimation();
        ani->endAnimationSet();
        GTAnimation::loadAnimationSet(ani, "samurai");
    }
    else
    {
      GTAnimation::unloadAnimationSet("samurai");
    }
}

void assetArcher(const char *param, bool isload)
{
    if( isload )
    {
        GTAnimation *ani = GTAnimation::startAnimationSet(6);
        ani->startAnimation(5, 0.25f);
        ani->addFrame("lockon1.png");
        ani->addFrame("lockon2.png");
        ani->addFrame("lockon3.png");
        ani->addFrame("lockon4.png");
        ani->addFrame("lockon5.png");
        ani->endAnimation();
        ani->startAnimation(2, 0.2f);
        ani->addFrame("lockon4.png");
        ani->addFrame("lockon5.png");
        ani->endAnimation();
        ani->startAnimation(4, 0.2f);
        ani->addFrame("lockon5.png");
        ani->addFrame("lockon6.png");
        ani->addFrame("lockon7.png");
        ani->addFrame("lockon6.png");
        ani->endAnimation();
        ani->startAnimation(5, 0.25f);
        ani->addFrame("lockon5.png");
        ani->addFrame("lockon4.png");
        ani->addFrame("lockon3.png");
        ani->addFrame("lockon2.png");
        ani->addFrame("lockon1.png");
        ani->endAnimation();
        ani->startAnimation(1, 0.05f);
        ani->addFrame("arrow1.png");
        ani->endAnimation();
        ani->startAnimation(1, 0.05f);
        ani->addFrame("arrow2.png");
        ani->endAnimation();
        GTAnimation::loadAnimationSet(ani, "archer");
    }
    else {
      GTAnimation::unloadAnimationSet("archer");
    }
}

void assetMerchant(const char* param, bool isload)
{
    if( isload )
    {
        GTAnimation *ani = GTAnimation::startAnimationSet(1);
        ani->startAnimation(6, 0.3f);
        ani->addFrame("oldman1.png");
        ani->addFrame("oldman2.png");
        ani->addFrame("oldman3.png");
        ani->addFrame("oldman4.png");
        ani->addFrame("oldman5.png");
        ani->addFrame("oldman6.png");
        ani->endAnimation();
        ani->endAnimationSet();
        GTAnimation::loadAnimationSet(ani, "merchant");
    }
    else
    {
      GTAnimation::unloadAnimationSet("merchant");
    }
}

void assetMechanic(const char* param, bool isload)
{
    if( isload )
    {
        GTAnimation *ani = GTAnimation::startAnimationSet(4);
        ani->startAnimation(3, 0.15f);
        ani->addFrame("srk_run1.png");
        ani->addFrame("srk_run2.png");
        ani->addFrame("srk_run3.png");
        ani->endAnimation();
        ani->startAnimation(3, 0.15f);
        ani->addFrame("srk_alarm1.png");
        ani->addFrame("srk_alarm2.png");
        ani->addFrame("srk_alarm3.png");
        ani->endAnimation();
        ani->startAnimation(1, 0.05f);
        ani->addFrame("srk_attack.png");
        ani->endAnimation();
        ani->startAnimation(13, 0.65f);
        ani->addFrame("srk_fall1.png");
        ani->addFrame("srk_fall2.png");
        ani->addFrame("srk_fall3.png");
        ani->addFrame("srk_fall4.png");
        ani->addFrame("srk_fall5.png");
        ani->addFrame("srk_fall6.png");
        ani->addFrame("srk_fall7.png");
        ani->addFrame("srk_fall8.png");
        ani->addFrame("srk_fall9.png");
        ani->addFrame("srk_fall10.png");
        ani->addFrame("srk_fall11.png");
        ani->addFrame("srk_fall12.png");
        ani->addFrame("srk_fall13.png");
        ani->endAnimation();
        ani->endAnimationSet();
        GTAnimation::loadAnimationSet(ani, "mechanic");
    }
    else {
      GTAnimation::unloadAnimationSet("mechanic");
    }
}

void assetPumpkin(const char* param, bool isload)
{
    if( isload )
    {
        GTAnimation *ani = GTAnimation::startAnimationSet(3);
        ani->startAnimation(28, 1.4f);
        ani->addFrame("hw-ng1.png");
        ani->addFrame("hw-ng1.png");
        ani->addFrame("hw-ng1.png");
        ani->addFrame("hw-ng1.png");
        ani->addFrame("hw-ng1.png");
        ani->addFrame("hw-ng1.png");
        ani->addFrame("hw-ng1.png");
        ani->addFrame("hw-ng1.png");
        ani->addFrame("hw-ng1.png");
        ani->addFrame("hw-ng2.png");
        ani->addFrame("hw-ng3.png");
        ani->addFrame("hw-ng4.png");
        ani->addFrame("hw-ng3.png");
        ani->addFrame("hw-ng2.png");
        ani->addFrame("hw-ng3.png");
        ani->addFrame("hw-ng4.png");
        ani->addFrame("hw-ng3.png");
        ani->addFrame("hw-ng2.png");
        ani->addFrame("hw-ng3.png");
        ani->addFrame("hw-ng4.png");
        ani->addFrame("hw-ng3.png");
        ani->addFrame("hw-ng2.png");
        ani->addFrame("hw-ng3.png");
        ani->addFrame("hw-ng4.png");
        ani->addFrame("hw-ng3.png");
        ani->addFrame("hw-ng2.png");
        ani->addFrame("hw-ng3.png");
        ani->addFrame("hw-ng4.png");
        ani->startAnimation(1, 0.2f);
        ani->addFrame("hw-ng2.png");
        ani->endAnimation();
        ani->startAnimation(5, 0.25f);
        ani->addFrame("hw-ngbz1.png");
        ani->addFrame("hw-ngbz2.png");
        ani->addFrame("hw-ngbz3.png");
        ani->addFrame("hw-ngbz4.png");
        ani->addFrame("hw-ngbz5.png");
        ani->endAnimation();
        ani->endAnimationSet();
        GTAnimation::loadAnimationSet(ani, "pumpkin");
    }
    else
    {
      GTAnimation::unloadAnimationSet("pumpkin");
    }
}

void assetSanta(const char* param, bool isload)
{
    if( isload )
    {
        GTAnimation *ani = GTAnimation::startAnimationSet(3);
        ani->startAnimation(3, 0.15f);
        ani->addFrame("santa1.png");
        ani->addFrame("santa2.png");
        ani->addFrame("santa3.png");
        ani->startAnimation(13, 0.65f);//4 后摔
        ani->addFrame("santa-fall1.png");
        ani->addFrame("santa-fall2.png");
        ani->addFrame("santa-fall3.png");
        ani->addFrame("santa-fall4.png");
        ani->addFrame("santa-fall5.png");
        ani->addFrame("santa-fall6.png");
        ani->addFrame("santa-fall7.png");
        ani->addFrame("santa-fall8.png");
        ani->addFrame("santa-fall9.png");
        ani->addFrame("santa-fall9.png");
        ani->addFrame("santa-fall9.png");
        ani->addFrame("santa-fall12.png");
        ani->addFrame("santa-fall13.png");
        ani->endAnimation();
        ani->startAnimation(13, 0.65f);//5 前摔
        ani->addFrame("santa-fall1.png");
        ani->addFrame("santa-fall2.png");
        ani->addFrame("santa-fall3.png");
        ani->addFrame("santa-fall4.png");
        ani->addFrame("santa-fall5.png");
        ani->addFrame("santa-fall6.png");
        ani->addFrame("santa-fall7.png");
        ani->addFrame("santa-fall8.png");
        ani->addFrame("santa-fall9.png");
        ani->addFrame("santa-fall9.png");
        ani->addFrame("santa-fall9.png");
        ani->addFrame("santa-fall12.png");
        ani->addFrame("santa-fall13.png");
        ani->endAnimation();
        ani->endAnimationSet();
        GTAnimation::loadAnimationSet(ani, "santa");
    }
    else
    {
      GTAnimation::unloadAnimationSet("santa");
    }
}

void assetBlade(const char* param, bool isload)
{
    if( isload )
    {
        GTAnimation *ani = GTAnimation::startAnimationSet(19);
        ani->startAnimation(5, 0.25f);//0
        ani->addFrame("dg1.png");
        ani->addFrame("dg2.png");
        ani->addFrame("dg3.png");
        ani->addFrame("dg4.png");
        ani->addFrame("dg5.png");
        ani->endAnimation();
        ani->startAnimation(5, 0.25f);//1
        ani->addFrame("girlblade1.png");
        ani->addFrame("girlblade2.png");
        ani->addFrame("girlblade3.png");
        ani->addFrame("girlblade4.png");
        ani->addFrame("girlblade5.png");
        ani->endAnimation();
        ani->startAnimation(5, 0.25f);//2
        ani->addFrame("musashiblade1.png");
        ani->addFrame("musashiblade2.png");
        ani->addFrame("musashiblade3.png");
        ani->addFrame("musashiblade4.png");
        ani->addFrame("musashiblade5.png");
        ani->endAnimation();
        ani->startAnimation(5, 0.25f);//3
        ani->addFrame("catblade1.png");
        ani->addFrame("catblade2.png");
        ani->addFrame("catblade3.png");
        ani->addFrame("catblade4.png");
        ani->addFrame("catblade5.png");
        ani->endAnimation();
        ani->startAnimation(6, 0.3f);//4
        ani->addFrame("fireblade1.png");
        ani->addFrame("fireblade2.png");
        ani->addFrame("fireblade3.png");
        ani->addFrame("fireblade4.png");
        ani->addFrame("fireblade5.png");
        ani->addFrame("fireblade6.png");
        ani->endAnimation();
        ani->startAnimation(6, 0.3f);//5
        ani->addFrame("ayblade1.png");
        ani->addFrame("ayblade2.png");
        ani->addFrame("ayblade3.png");
        ani->addFrame("ayblade4.png");
        ani->addFrame("ayblade5.png");
        ani->addFrame("ayblade6.png");
        ani->endAnimation();
        ani->startAnimation(5, 0.25f);//6
        ani->addFrame("lblade1.png");
        ani->addFrame("lblade2.png");
        ani->addFrame("lblade3.png");
        ani->addFrame("lblade4.png");
        ani->addFrame("lblade5.png");
        ani->addFrame("lblade6.png");
        ani->endAnimation();
        ani->startAnimation(5, 0.25f);//7
        ani->addFrame("chblade1.png");
        ani->addFrame("chblade2.png");
        ani->addFrame("chblade3.png");
        ani->addFrame("chblade4.png");
        ani->addFrame("chblade5.png");
        ani->addFrame("chblade6.png");
        ani->endAnimation();
        ani->startAnimation(5, 0.25f);//8
        ani->addFrame("zkblade1.png");
        ani->addFrame("zkblade2.png");
        ani->addFrame("zkblade3.png");
        ani->addFrame("zkblade4.png");
        ani->addFrame("zkblade5.png");
        ani->endAnimation();
        ani->startAnimation(5, 0.25f);//9
        ani->addFrame("jgblade1.png");
        ani->addFrame("jgblade2.png");
        ani->addFrame("jgblade3.png");
        ani->addFrame("jgblade4.png");
        ani->addFrame("jgblade5.png");
        ani->endAnimation();
        ani->startAnimation(5, 0.25f);//10
        ani->addFrame("skblade1.png");
        ani->addFrame("skblade2.png");
        ani->addFrame("skblade3.png");
        ani->addFrame("skblade4.png");
        ani->addFrame("skblade5.png");
        ani->endAnimation();
        ani->startAnimation(6, 0.3f);//11
        ani->addFrame("swblade1.png");
        ani->addFrame("swblade2.png");
        ani->addFrame("swblade3.png");
        ani->addFrame("swblade4.png");
        ani->addFrame("swblade5.png");
        ani->addFrame("swblade6.png");
        ani->endAnimation();
        ani->startAnimation(5, 0.25f);//12
        ani->addFrame("hjblade1.png");
        ani->addFrame("hjblade2.png");
        ani->addFrame("hjblade3.png");
        ani->addFrame("hjblade4.png");
        ani->addFrame("hjblade5.png");
        ani->endAnimation();
        ani->startAnimation(5, 0.25f);//13
        ani->addFrame("hdblade1.png");
        ani->addFrame("hdblade2.png");
        ani->addFrame("hdblade3.png");
        ani->addFrame("hdblade4.png");
        ani->addFrame("hdblade5.png");
        ani->endAnimation();
        ani->startAnimation(5, 0.25f);//14
        ani->addFrame("bblade1.png");
        ani->addFrame("bblade2.png");
        ani->addFrame("bblade3.png");
        ani->addFrame("bblade4.png");
        ani->addFrame("bblade5.png");
        ani->endAnimation();
        ani->startAnimation(5, 0.25f);//15
        ani->addFrame("mcblade1-1.png");
        ani->addFrame("mcblade1-2.png");
        ani->addFrame("mcblade1-3.png");
        ani->addFrame("mcblade1-4.png");
        ani->addFrame("mcblade1-5.png");
        ani->endAnimation();
        ani->startAnimation(5, 0.25f);//16
        ani->addFrame("mcblade2-1.png");
        ani->addFrame("mcblade2-2.png");
        ani->addFrame("mcblade2-3.png");
        ani->addFrame("mcblade2-4.png");
        ani->addFrame("mcblade2-5.png");
        ani->endAnimation();
        ani->startAnimation(5, 0.25f);//17
        ani->addFrame("mcblade3-1.png");
        ani->addFrame("mcblade3-2.png");
        ani->addFrame("mcblade3-3.png");
        ani->addFrame("mcblade3-4.png");
        ani->addFrame("mcblade3-5.png");
        ani->endAnimation();
        ani->startAnimation(5, 0.25f);//18
        ani->addFrame("mcblade4-1.png");
        ani->addFrame("mcblade4-2.png");
        ani->addFrame("mcblade4-3.png");
        ani->addFrame("mcblade4-4.png");
        ani->addFrame("mcblade4-5.png");
        ani->endAnimation();
        ani->endAnimationSet();
        GTAnimation::loadAnimationSet(ani, "blade");
    }
    else
    {
      GTAnimation::unloadAnimationSet("blade");
    }
}

void assetEffect(const char *param, bool isload)
{
    if( isload )
    {
        GTAnimation *ani = GTAnimation::startAnimationSet(29);
        ani->startAnimation(6, 0.3f);//0
        ani->addFrame("tx_jb1.png");
        ani->addFrame("tx_jb2.png");
        ani->addFrame("tx_jb3.png");
        ani->addFrame("tx_jb4.png");
        ani->addFrame("tx_jb5.png");
        ani->addFrame("tx_jb6.png");
        ani->endAnimation();
        ani->startAnimation(2, 0.1f);//1
        ani->addFrame("tx_fb1.png");
        ani->addFrame("tx_fb2.png");
        ani->endAnimation();
        ani->startAnimation(3, 0.15f);//2
        ani->addFrame("tx_fby1.png");
        ani->addFrame("tx_fby2.png");
        ani->addFrame("tx_fby3.png");
        ani->endAnimation();
        ani->startAnimation(3, 0.15f);//3
        ani->addFrame("tx_hx1.png");
        ani->addFrame("tx_hx2.png");
        ani->addFrame("tx_hx3.png");
        ani->endAnimation();
        ani->startAnimation(5, 0.25f);//4
        ani->addFrame("dust1.png");
        ani->addFrame("dust2.png");
        ani->addFrame("dust3.png");
        ani->addFrame("dust4.png");
        ani->addFrame("dust5.png");
        ani->endAnimation();
        ani->startAnimation(8, 0.4f);//5
        ani->addFrame("smoke1.png");
        ani->addFrame("smoke2.png");
        ani->addFrame("smoke3.png");
        ani->addFrame("smoke4.png");
        ani->addFrame("smoke5.png");
        ani->addFrame("smoke6.png");
        ani->addFrame("smoke7.png");
        ani->addFrame("smoke8.png");
        ani->endAnimation();
        ani->startAnimation(8, 0.4f);//6
        ani->addFrame("bstx1.png");
        ani->addFrame("bstx2.png");
        ani->addFrame("bstx3.png");
        ani->addFrame("bstx4.png");
        ani->addFrame("bstx5.png");
        ani->addFrame("bstx6.png");
        ani->addFrame("bstx7.png");
        ani->addFrame("bstx8.png");
        ani->endAnimation();
        ani->startAnimation(6, 0.3f);//7
        ani->addFrame("enemy_attack_tx1.png");
        ani->addFrame("enemy_attack_tx2.png");
        ani->addFrame("enemy_attack_tx3.png");
        ani->addFrame("enemy_attack_tx4.png");
        ani->addFrame("enemy_attack_tx5.png");
        ani->addFrame("enemy_attack_tx6.png");
        ani->endAnimation();
        ani->startAnimation(3, 0.15f);//8
        ani->addFrame("tx4.png");
        ani->addFrame("tx5.png");
        ani->addFrame("tx6.png");
        ani->endAnimation();
        ani->startAnimation(6, 0.3f);//9
        ani->addFrame("coin1.png");
        ani->addFrame("coin2.png");
        ani->addFrame("coin3.png");
        ani->addFrame("coin4.png");
        ani->addFrame("coin5.png");
        ani->addFrame("coin6.png");
        ani->endAnimation();
        ani->startAnimation(1, 0.05f);//10
        ani->addFrame("scroll.png");
        ani->endAnimation();
        ani->startAnimation(6, 0.3f);//11
        ani->addFrame("hfbtx0.png");
        ani->addFrame("hfbtx1.png");
        ani->addFrame("hfbtx2.png");
        ani->addFrame("hfbtx3.png");
        ani->addFrame("hfbtx4.png");
        ani->addFrame("hfbtx5.png");
        ani->endAnimation();
        ani->startAnimation(6, 0.3f);//12
        ani->addFrame("lfbtx0.png");
        ani->addFrame("lfbtx1.png");
        ani->addFrame("lfbtx2.png");
        ani->addFrame("lfbtx3.png");
        ani->addFrame("lfbtx4.png");
        ani->addFrame("lfbtx5.png");
        ani->endAnimation();
        ani->startAnimation(4, 0.2f);//13
        ani->addFrame("htx1.png");
        ani->addFrame("htx2.png");
        ani->addFrame("htx3.png");
        ani->addFrame("htx4.png");
        ani->endAnimation();
        ani->startAnimation(6, 0.3f);//14
        ani->addFrame("ayfbtx0.png");
        ani->addFrame("ayfbtx1.png");
        ani->addFrame("ayfbtx2.png");
        ani->addFrame("ayfbtx3.png");
        ani->addFrame("ayfbtx4.png");
        ani->addFrame("ayfbtx5.png");
        ani->endAnimation();
        ani->startAnimation(4, 0.2f);//15
        ani->addFrame("fbyh_tx1.png");
        ani->addFrame("fbyh_tx2.png");
        ani->addFrame("fbyh_tx3.png");
        ani->addFrame("fbyh_tx4.png");
        ani->endAnimation();
        ani->startAnimation(2, 0.1f);//16
        ani->addFrame("fbyh_tx5.png");
        ani->addFrame("fbyh_tx6.png");
        ani->endAnimation();
        ani->startAnimation(6, 0.3f);//17
        ani->addFrame("bffbtx0.png");
        ani->addFrame("bffbtx1.png");
        ani->addFrame("bffbtx2.png");
        ani->addFrame("bffbtx3.png");
        ani->addFrame("bffbtx4.png");
        ani->addFrame("bffbtx5.png");
        ani->endAnimation();
        ani->startAnimation(4, 0.2f);//18
        ani->addFrame("fbzk_tx1.png");
        ani->addFrame("fbzk_tx2.png");
        ani->addFrame("fbzk_tx3.png");
        ani->addFrame("fbzk_tx4.png");
        ani->endAnimation();
        ani->startAnimation(2, 0.1f);//19
        ani->addFrame("fbzk_tx5.png");
        ani->addFrame("fbzk_tx6.png");
        ani->endAnimation();
        ani->startAnimation(4, 0.2f);//20
        ani->addFrame("fbb_tx1.png");
        ani->addFrame("fbb_tx2.png");
        ani->addFrame("fbb_tx3.png");
        ani->addFrame("fbb_tx4.png");
        ani->endAnimation();
        ani->startAnimation(2, 0.1f);//21
        ani->addFrame("fbb_tx5.png");
        ani->addFrame("fbb_tx6.png");
        ani->endAnimation();
        ani->startAnimation(4, 0.2f);//22
        ani->addFrame("fbhd_tx1.png");
        ani->addFrame("fbhd_tx2.png");
        ani->addFrame("fbhd_tx3.png");
        ani->addFrame("fbhd_tx4.png");
        ani->endAnimation();
        ani->startAnimation(2, 0.1f);//23
        ani->addFrame("fbhd_tx5.png");
        ani->addFrame("fbhd_tx6.png");
        ani->endAnimation();
        ani->startAnimation(4, 0.2f);//24
        ani->addFrame("fbch_tx1.png");
        ani->addFrame("fbch_tx2.png");
        ani->addFrame("fbch_tx3.png");
        ani->addFrame("fbch_tx4.png");
        ani->endAnimation();
        ani->startAnimation(2, 0.1f);//25
        ani->addFrame("fbch_tx5.png");
        ani->addFrame("fbch_tx6.png");
        ani->endAnimation();
        ani->startAnimation(4, 0.2f);//26
        ani->addFrame("tx_yyz1.png");
        ani->addFrame("tx_yyz2.png");
        ani->addFrame("tx_yyz3.png");
        ani->addFrame("tx_yyz4.png");
        ani->endAnimation();
        ani->startAnimation(2, 0.1f);//27
        ani->addFrame("criticaltx1.png");
        ani->addFrame("criticaltx2.png");
        ani->endAnimation();
        ani->startAnimation(2, 0.1f);//28
        ani->addFrame("ctctx2.png");
        ani->addFrame("ctctx3.png");
        ani->endAnimation();
        ani->endAnimationSet();
        GTAnimation::loadAnimationSet(ani, "effect");
    }
    else {
      GTAnimation::unloadAnimationSet("effect");
    }
}

void assetMsg(const char* param, bool isload)
{
    if( isload )
    {
        GTAnimation *ani = GTAnimation::startAnimationSet(6);
        ani->startAnimation(8, 0.4f);//0 慢跑
        ani->addFrame("msg_run1.png");
        ani->addFrame("msg_run2.png");
        ani->addFrame("msg_run3.png");
        ani->addFrame("msg_run4.png");
        ani->addFrame("msg_run5.png");
        ani->addFrame("msg_run6.png");
        ani->addFrame("msg_run7.png");
        ani->addFrame("msg_run8.png");
        ani->endAnimation();
        ani->startAnimation(3, 0.15f);//1 快跑
        ani->addFrame("msg_run1.png");
        ani->addFrame("msg_run2.png");
        ani->addFrame("msg_run4.png");
        ani->endAnimation();
        ani->startAnimation(3, 0.15f);//2 快跑2
        ani->addFrame("msg_surprised9.png");
        ani->addFrame("msg_surprised10.png");
        ani->addFrame("msg_surprised11.png");
        ani->endAnimation();
        ani->startAnimation(16, 0.8f);//3 察觉
        ani->addFrame("msg_surprised1.png");
        ani->addFrame("msg_surprised2.png");
        ani->addFrame("msg_surprised3.png");
        ani->addFrame("msg_surprised4.png");
        ani->addFrame("msg_surprised5.png");
        ani->addFrame("msg_surprised6.png");
        ani->addFrame("msg_surprised7.png");
        ani->addFrame("msg_surprised8.png");
        ani->addFrame("msg_surprised1.png");
        ani->addFrame("msg_surprised2.png");
        ani->addFrame("msg_surprised3.png");
        ani->addFrame("msg_surprised4.png");
        ani->addFrame("msg_surprised5.png");
        ani->addFrame("msg_surprised6.png");
        ani->addFrame("msg_surprised7.png");
        ani->addFrame("msg_surprised8.png");
        ani->endAnimation();
        ani->startAnimation(13, 0.65f);//4 后摔
        ani->addFrame("msg_fall14.png");
        ani->addFrame("msg_fall15.png");
        ani->addFrame("msg_fall16.png");
        ani->addFrame("msg_fall17.png");
        ani->addFrame("msg_fall18.png");
        ani->addFrame("msg_fall19.png");
        ani->addFrame("msg_fall20.png");
        ani->addFrame("msg_fall21.png");
        ani->addFrame("msg_fall22.png");
        ani->addFrame("msg_fall23.png");
        ani->addFrame("msg_fall24.png");
        ani->addFrame("msg_fall25.png");
        ani->addFrame("msg_fall26.png");
        ani->endAnimation();
        ani->startAnimation(13, 0.65f);//5 前摔
        ani->addFrame("msg_fall1.png");
        ani->addFrame("msg_fall2.png");
        ani->addFrame("msg_fall3.png");
        ani->addFrame("msg_fall4.png");
        ani->addFrame("msg_fall5.png");
        ani->addFrame("msg_fall6.png");
        ani->addFrame("msg_fall7.png");
        ani->addFrame("msg_fall8.png");
        ani->addFrame("msg_fall9.png");
        ani->addFrame("msg_fall9.png");
        ani->addFrame("msg_fall9.png");
        ani->addFrame("msg_fall12.png");
        ani->addFrame("msg_fall13.png");
        ani->endAnimation();
        ani->endAnimationSet();
        GTAnimation::loadAnimationSet(ani, "messager");
    }
    else
    {
      GTAnimation::unloadAnimationSet("messager");
    }
}

void assetItem(const char *param, bool isload)
{
    if( isload )
    {
        GTAnimation *ani = GTAnimation::startAnimationSet(1);
        ani->startAnimation(6, 0.3f);
        ani->addFrame("coin1.png");
        ani->addFrame("coin2.png");
        ani->addFrame("coin3.png");
        ani->addFrame("coin4.png");
        ani->addFrame("coin5.png");
        ani->addFrame("coin6.png");
        ani->endAnimation();
        ani->endAnimationSet();
        GTAnimation::loadAnimationSet(ani, "item");
    }
    else {
      GTAnimation::unloadAnimationSet("item");
    }
}

void assetBullets(const char* param, bool isload)
{
    if( isload )
    {
        GTAnimation *ani = GTAnimation::startAnimationSet(4);
        ani->startAnimation(14, 0.7f);
        ani->addFrame("baozha1.png");
        ani->addFrame("baozha2.png");
        ani->addFrame("baozha3.png");
        ani->addFrame("baozha4.png");
        ani->addFrame("baozha5.png");
        ani->addFrame("baozha6.png");
        ani->addFrame("baozha7.png");
        ani->addFrame("baozha8.png");
        ani->addFrame("baozha9.png");
        ani->addFrame("baozha10.png");
        ani->addFrame("baozha11.png");
        ani->addFrame("baozha12.png");
        ani->addFrame("baozha13.png");
        ani->addFrame("baozha14.png");
        ani->endAnimation();
        ani->startAnimation(3, 0.15f);
        ani->addFrame("zhadan1.png");
        ani->addFrame("zhadan2.png");
        ani->addFrame("zhadan3.png");
        ani->endAnimation();
        ani->startAnimation(4, 0.2f);
        ani->addFrame("bingzhu1.png");
        ani->addFrame("bingzhu2.png");
        ani->addFrame("bingzhu3.png");
        ani->addFrame("bingzhu4.png");
        ani->endAnimation();
        ani->startAnimation(6, 0.3f);
        ani->addFrame("bingzhu5.png");
        ani->addFrame("bingzhu6.png");
        ani->addFrame("bingzhu7.png");
        ani->addFrame("bingzhu8.png");
        ani->addFrame("bingzhu9.png");
        ani->addFrame("bingzhu10.png");
        ani->endAnimation();
        ani->endAnimationSet();
        GTAnimation::loadAnimationSet(ani, "bullets");
    }
    else {
      GTAnimation::unloadAnimationSet("bullets");
    }
}

void assetMisc(const char *param, bool isload)
{
    if( isload )
    {
        GTAnimation *ani = GTAnimation::startAnimationSet(9);
        ani->startAnimation(2, 0.1f);
        ani->addFrame("fbl_tail1.png");
        ani->addFrame("fbl_tail2.png");
        ani->addFrame("fbl_tail3.png");
        ani->addFrame("fbl_tail4.png");
        ani->addFrame("fbl_tail5.png");
        ani->endAnimation();
        ani->startAnimation(2, 0.3f);
        ani->addFrame("wsts1.png");
        ani->addFrame("wsts2.png");
        ani->endAnimation();
        ani->startAnimation(3, 0.2f);
        ani->addFrame("hd1.png");
        ani->addFrame("hd2.png");
        ani->endAnimation();
        ani->startAnimation(3, 0.2f);
        ani->addFrame("hd3.png");
        ani->addFrame("hd4.png");
        ani->endAnimation();
        ani->startAnimation(3, 0.2f);
        ani->addFrame("hd5.png");
        ani->addFrame("hd6.png");
        ani->endAnimation();
        ani->startAnimation(3, 0.2f);
        ani->addFrame("hd7.png");
        ani->addFrame("hd8.png");
        ani->endAnimation();
        ani->startAnimation(7, 0.35f);
        ani->addFrame("water1.png");
        ani->addFrame("water2.png");
        ani->addFrame("water3.png");
        ani->addFrame("water4.png");
        ani->addFrame("water5.png");
        ani->addFrame("water6.png");
        ani->addFrame("water7.png");
        ani->endAnimation();
        ani->startAnimation(8, 0.4f);
        ani->addFrame("smoke1.png");
        ani->addFrame("smoke2.png");
        ani->addFrame("smoke3.png");
        ani->addFrame("smoke4.png");
        ani->addFrame("smoke5.png");
        ani->addFrame("smoke6.png");
        ani->addFrame("smoke7.png");
        ani->addFrame("smoke8.png");
        ani->endAnimation();
        ani->startAnimation(2, 1);
        ani->addFrame("jc2.png");
        ani->addFrame("jc3.png");
        ani->endAnimation();
        ani->endAnimationSet();
        GTAnimation::loadAnimationSet(ani, "misc");
    }
    else {
      GTAnimation::unloadAnimationSet("misc");
    }
}

void assetUI(const char *param, bool isload)
{
    if( isload )
    {
        GTAnimation *ani = GTAnimation::startAnimationSet(4);
        ani->startAnimation(2, 0.5f);
        ani->addFrame("heart.png");
        ani->addFrame("pulife2.png");
        ani->endAnimation();
        ani->startAnimation(2, 0.5f);
        ani->addFrame("ui-dart.png");
        ani->addFrame("pushuriken2.png");
        ani->endAnimation();
        ani->endAnimationSet();
        ani->startAnimation(1, 1);
        ani->addFrame("xr_start0.png");
        ani->endAnimation();
        ani->startAnimation(6, 0.3f);
        ani->addFrame("xr_start0.png");
        ani->addFrame("xr_start1.png");
        ani->addFrame("xr_start2.png");
        ani->addFrame("xr_start3.png");
        ani->addFrame("xr_start4.png");
        ani->addFrame("xr_start5.png");
        ani->endAnimation();
        ani->endAnimationSet();
        GTAnimation::loadAnimationSet(ani, "ui");
    }
    else {
      GTAnimation::unloadAnimationSet("ui");
    }
}
