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

void assetRole(const char* head, bool isload) {
  std::string role = std::string(head);
  if( isload ) {
    GTAnimation *ani = GTAnimation::startAnimationSet(10);
    addAnimation(ani, (role+"_run").c_str(), 3, 1);
    addAnimation(ani, (role+"_attack").c_str(), 5, 1);
    addAnimation(ani, (role+"_attack").c_str(), 5, 6);
    addAnimation(ani, (role+"_attack").c_str(), 7, 11);
    addAnimation(ani, (role+"_fall").c_str(), 7, 1);
    addAnimation(ani, (role+"_fall").c_str(), 7, 8);
    addAnimation(ani, (role+"_die").c_str(), 6, 1);
    addAnimation(ani, (role+"_die").c_str(), 2, 7);
    addAnimation(ani, (role+"_attack").c_str(), 1, 11);
    addAnimation(ani, (role+"_die").c_str(), 8, 8, true);
    ani->endAnimationSet();
    GTAnimation::loadAnimationSet(ani, head);
  } else {
    GTAnimation::unloadAnimationSet(head);
  }
}

void assetMainRole(const char* param, bool isload) { assetRole("main", isload); }

void assetMainGirl(const char* param, bool isload) { assetRole("girl", isload); }

void assetMainMusashi(const char* param, bool isload)
{
    std::string role = std::string("musashi");
    if( isload )
    {
        GTAnimation *ani = GTAnimation::startAnimationSet(11);
        addAnimation(ani, (role+"_run").c_str(), 8, 1);
        addAnimation(ani, (role+"_attack").c_str(), 5, 1);
        addAnimation(ani, (role+"_attack").c_str(), 5, 6);

        ani->startAnimation(6, 0.3f);
        ani->addFrame("musashi_attack11.png");
        ani->addFrame("musashi_attack12.png");
        ani->addFrame("musashi_attack13.png");
        ani->addFrame("musashi_attack14.png");
        ani->addFrame("musashi_attack15.png");
        ani->addFrame("musashi_attack12.png");
        ani->endAnimation();

        addAnimation(ani, (role+"_fall").c_str(), 7, 1);
        addAnimation(ani, (role+"_fall").c_str(), 7, 8);
        addAnimation(ani, (role+"_die").c_str(), 6, 1);
        addAnimation(ani, (role+"_die").c_str(), 2, 7);
        addAnimation(ani, (role+"_attack").c_str(), 1, 11);
        addAnimation(ani, (role+"_die").c_str(), 8, 8, true);

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
    } else {
      GTAnimation::unloadAnimationSet("musashi");
    }
}

void assetMainPussy(const char* param, bool isload)
{
    std::string role = std::string("cat");
    if( isload )
    {
        GTAnimation *ani = GTAnimation::startAnimationSet(13);
        addAnimation(ani, (role+"_run").c_str(), 8, 1);
        addAnimation(ani, (role+"_attack").c_str(), 5, 1);
        addAnimation(ani, (role+"_attack").c_str(), 5, 6);
        addAnimation(ani, (role+"_attack").c_str(), 7, 11);
        addAnimation(ani, (role+"_fall").c_str(), 7, 1);
        addAnimation(ani, (role+"_fall").c_str(), 7, 8);
        addAnimation(ani, (role+"_die").c_str(), 6, 1);
        addAnimation(ani, (role+"_die").c_str(), 2, 7);
        addAnimation(ani, (role+"_attack").c_str(), 1, 11);
        addAnimation(ani, (role+"_die").c_str(), 8, 8, true);
        //----- 猫咪四脚的动作 -------
        addAnimation(ani, (role+"_run").c_str(), 6, 4);
        addAnimation(ani, (role+"_attack0").c_str(), 5, 1);
        addAnimation(ani, (role+"_attack0").c_str(), 6, 6);
        ani->endAnimationSet();
        GTAnimation::loadAnimationSet(ani, "pussy");
    } else {
      GTAnimation::unloadAnimationSet("pussy");
    }
}


void assetEnemy(const char* param, bool isload)
{
    std::string role = std::string("enemy");
    if( isload )
    {
        GTAnimation *ani = GTAnimation::startAnimationSet(7);
        addAnimation(ani, (role+"_run").c_str(), 3, 1);
        addAnimation(ani, (role+"_fall").c_str(), 13, 1);

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

        addAnimation(ani, (role+"_fall").c_str(), 4, 10);
        addAnimation(ani, (role+"_attack").c_str(), 5, 6);
        addAnimation(ani, (role+"_attack").c_str(), 3, 11);

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
    std::string role = std::string("mninja");
    if( isload )
    {
        GTAnimation *ani = GTAnimation::startAnimationSet(9);
        addAnimation(ani, (role+"_run").c_str(), 3, 1);
        addAnimation(ani, (role+"_fall").c_str(), 13, 1);

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

        addAnimation(ani, (role+"_fall").c_str(), 4, 10);
        addAnimation(ani, (role+"_attack").c_str(), 5, 6);
        addAnimation(ani, (role+"_attack").c_str(), 3, 11);

        addAnimation(ani, (role+"_run").c_str(), 1, 4);
        addAnimation(ani, (role+"_run").c_str(), 6, 5);

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
    std::string role = std::string("hninja");
    if( isload )
    {
        GTAnimation *ani = GTAnimation::startAnimationSet(7);
        addAnimation(ani, (role+"_run").c_str(), 3, 1);
        addAnimation(ani, (role+"_fall").c_str(), 13, 1);

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

        addAnimation(ani, (role+"_fall").c_str(), 4, 10);
        addAnimation(ani, (role+"_attack").c_str(), 5, 6);
        addAnimation(ani, (role+"_attack").c_str(), 3, 11);

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
    std::string role = std::string("samurai");
    if( isload )
    {
        GTAnimation *ani = GTAnimation::startAnimationSet(3);
        addAnimation(ani, (role+"_attack").c_str(), 1, 1);
        addAnimation(ani, (role+"_attack").c_str(), 10, 1);

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
    std::string role = std::string("lockon");
    if( isload )
    {
        GTAnimation *ani = GTAnimation::startAnimationSet(6);
        addAnimation(ani, role.c_str(), 5, 1);
        addAnimation(ani, role.c_str(), 2, 4);
        addAnimation(ani, role.c_str(), 4, 5);
        addAnimation(ani, role.c_str(), 5, 5, true);
        addAnimation(ani, "arrow", 1, 1);
        addAnimation(ani, "arrow", 1, 2);
        GTAnimation::loadAnimationSet(ani, "archer");
    } else {
      GTAnimation::unloadAnimationSet("archer");
    }
}

void assetMerchant(const char* param, bool isload)
{
    std::string role = std::string("oldman");
    if( isload )
    {
        GTAnimation *ani = GTAnimation::startAnimationSet(1);
        addAnimation(ani, role.c_str(), 6, 1);
        GTAnimation::loadAnimationSet(ani, "merchant");
    } else {
      GTAnimation::unloadAnimationSet("merchant");
    }
}

void assetMechanic(const char* param, bool isload)
{
    std::string role = std::string("srk");
    if( isload )
    {
        GTAnimation *ani = GTAnimation::startAnimationSet(4);
        addAnimation(ani, "srk_run", 3, 1);
        addAnimation(ani, "srk_alarm", 3, 1);
        
        ani->startAnimation(1, 0.05f);
        ani->addFrame("srk_attack.png");
        ani->endAnimation();

        addAnimation(ani, "srk_fall", 13, 1);
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

        addAnimation(ani, "hw-ng", 1, 2);

        addAnimation(ani, "hw-ngbz", 5, 1);

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
        addAnimation(ani, "santa", 3, 1);
        addAnimation(ani, "santa-fall", 13, 1);//4 后摔
        addAnimation(ani, "santa-fall", 13, 1);//5 前摔 TODO:是否有区别？
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
        addAnimation(ani, "dg", 5, 1);//0
        addAnimation(ani, "girlblade", 5, 1);//1
        addAnimation(ani, "musashiblade", 5, 1);//2
        addAnimation(ani, "catblade", 5, 1);//3
        addAnimation(ani, "fireblade", 6, 1);//4
        addAnimation(ani, "ayblade", 6, 1);//5
        addAnimation(ani, "lblade", 5, 1);//6
        addAnimation(ani, "chblade", 5, 1);//7
        addAnimation(ani, "swblade", 6, 1);//11
        addAnimation(ani, "hjblade", 5, 1);//12
        addAnimation(ani, "hdblade", 5, 1);//13
        addAnimation(ani, "bblade", 5, 1);//14
        addAnimation(ani, "mcblade1-", 5, 1);//15
        addAnimation(ani, "mcblade2-", 5, 1);//16
        addAnimation(ani, "mcblade3-", 5, 1);//17
        addAnimation(ani, "mcblade4-", 5, 1);//18
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
        addAnimation(ani, "tx_jb", 6, 1);//0
        addAnimation(ani, "tx_fb", 2, 1);//1
        addAnimation(ani, "tx_fby", 3, 1);//2
        addAnimation(ani, "tx_hx", 3, 1);//3
        addAnimation(ani, "dust", 5, 1);//4
        addAnimation(ani, "smoke", 8, 1);//5
        addAnimation(ani, "bstx", 8, 1);//6
        addAnimation(ani, "enemy_attack_tx", 6, 1);//7
        addAnimation(ani, "tx", 3, 1);//8
        addAnimation(ani, "coin", 6, 1);//9
        addAnimation(ani, "scrol", 1, 1);//10
        addAnimation(ani, "hfbtx", 6, 1);//11
        addAnimation(ani, "lfbtx", 6, 1);//12
        addAnimation(ani, "htx", 4, 1);//13
        addAnimation(ani, "ayfbtx", 6, 1);//14
        addAnimation(ani, "fbyh_tx", 4, 1);//15
        addAnimation(ani, "fbyh_tx", 2, 5);//16
        addAnimation(ani, "bffbtx", 6, 1);//17
        addAnimation(ani, "fbzk_tx", 4, 1);//18
        addAnimation(ani, "fbzk_tx", 2, 5);//19
        addAnimation(ani, "fbb_tx", 4, 1);//20
        addAnimation(ani, "fbb_tx", 2, 5);//21
        addAnimation(ani, "fbhd_tx", 4, 1);//22
        addAnimation(ani, "fbhd_tx", 2, 5);//23
        addAnimation(ani, "fbch_tx", 4, 1);//24
        addAnimation(ani, "fbch_tx", 2, 5);//25
        addAnimation(ani, "tx_yyz", 4, 1);//26
        addAnimation(ani, "criticaltx", 2, 1);//27
        addAnimation(ani, "ctctx", 2, 2);//28
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
        addAnimation(ani, "msg_run", 8, 1);//0 慢跑

        ani->startAnimation(3, 0.15f);//1 快跑
        ani->addFrame("msg_run1.png");
        ani->addFrame("msg_run2.png");
        ani->addFrame("msg_run4.png");
        ani->endAnimation();

        addAnimation(ani, "msg_surprised", 3, 9);//2 快跑2

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

        addAnimation(ani, "msg_fall1", 13, 14);//4 后摔
        addAnimation(ani, "msg_fall", 13, 1);//5 前摔

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
        addAnimation(ani, "coin", 6, 1);
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
        addAnimation(ani, "baozha", 14, 1);
        addAnimation(ani, "zhadan", 3, 1);
        addAnimation(ani, "bingzhu", 4, 1);
        addAnimation(ani, "bingzhu", 6, 5);
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
        addAnimation(ani, "fbl_tail", 5, 1);
        addAnimation(ani, "wsts", 2, 1);
        addAnimation(ani, "hd", 2, 1);
        addAnimation(ani, "hd", 2, 3);
        addAnimation(ani, "hd", 2, 5);
        addAnimation(ani, "hd", 2, 7);
        addAnimation(ani, "water", 7, 1);
        addAnimation(ani, "smoke", 8, 1);
        addAnimation(ani, "jc", 2, 2);
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

        ani->startAnimation(1, 1);
        ani->addFrame("xr_start0.png");
        ani->endAnimation();

        addAnimation(ani, "xr_start", 6, 0);

        ani->endAnimationSet();
        GTAnimation::loadAnimationSet(ani, "ui");
    }
    else {
      GTAnimation::unloadAnimationSet("ui");
    }
}
