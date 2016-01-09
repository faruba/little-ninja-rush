#ifndef ___CLASSES_GAMECONSTANTS_H_
#define ___CLASSES_GAMECONSTANTS_H_

//游戏常数
#define PI (3.14159f)

#if CC_TARGET_PLATFORM == CC_PLATFORM_MAC
#define GFONT_NAME ("Helvetica.ttf")
#else
#define GFONT_NAME ("fonts/Helvetica.ttf")
#endif
#define GFONT_SIZE_LARGE (24)
#define GFONT_SIZE_NORMAL (16)

#define SCREEN_WIDTH (480)         //屏幕宽度（点）
#define SCREEN_HEIGHT (320)        //屏幕高度（点）

//游戏模式
#define MODE_CLASSIC (0)
#define MODE_ARCADE (1)

//游戏状态
#define STATE_NORMAL (0)
#define STATE_CHANGESCENE (1)
#define STATE_DEAD (2)
#define STATE_MERCHANT (3)
#define STATE_TUTORIAL (4)
#define STATE_REVIVE (5)
#define STATE_RUSH (6)

#define SCENEINTRO_TIME (0.3f)
#define SCENEINTRO_DELAY (0.05f)
#define SCENEOUTRO_TIME (0.3f)
#define SCENEOUTRO_DELAY (0.05f)

#define GAME_CHARCOUNT (4)            //游戏中角色的数量
#define GAME_SPELLCOUNT (3)           //游戏中技能的数量
#define GAME_UPGRADECOUNT (9)         //游戏中的升级数量
#define GAME_UPGRADEOFFSET (-2)       //道具与升级id的修正

#define PLAY_MINISPEED (50.0f)        //移动初始速度
#define PLAY_SLIDEDART (0.298275f)    //丢飞镖的方向极限
#define PLAY_SLIDERELOAD (-0.7071f)   //重新装填的方向判断
#define PLAY_AUTOAIM (40.0f)          //辅助瞄准
#define PLAY_MINSHOOT (120.0f)        //人离敌人最短的高度距离
#define PLAY_PLAYERLINE (30.0f)       //玩家角色所在的线
#define PLAY_MAXDART (10)             //玩家可以拥有的最大飞镖数
#define PLAY_DISMETER (20.0f)         //距离换算因子
#define PLAY_SCENELEN (1000)          //切换场景的距离
#define PLAY_COMBOTIME (5)            //COMBO计时
#define PLAY_FEVERCOMBO (10)          //多少个Combo就变成fever
#define PLAY_GOSLIDETIME (1.7f)       //游戏结束时的滑行时间
#define PLAY_SCROLLPRICE (15)         //一个scroll的价格
#define PLAY_REVIVETIME (1.5f)        //复活时间

#define RESPAWN_YMIN (215)            //刷怪的区域下限
#define RESPAWN_Y (30)                //刷怪的区域上限

#define CONTROL_BALANCE (0.05f)       //重力感应平衡范围
#define CONTROL_MAXSPEED (0.3f)       //重力感应最高速度范围
#define CONTROL_MINSLIDE (50.0f)      //划屏幕的最小长度
#define CONTROL_MAXSLIDE (100.0f)     //划屏幕的最大长度

#define HIT_DART (0)                  //伤害类型：飞镖
#define HIT_BLADE (1)                 //伤害类型：刀刃
#define HIT_BOMB (2)                  //伤害类型： 炸弹
#define HIT_ARCHER (3)                //伤害类型：流矢
#define HIT_ICESPIKE (4)              //伤害类型：冰刺
#define HIT_MAGIC (5)                 //伤害类型：魔法

#define ENEMY_NNRUNSPEED (200.0f)     //敌人：下忍奔跑速度

#define DARTSPEED_HERO (700.0f)       //我方飞镖的飞行速度

#define EXP_HITSLOW (0.15f)           //攻击暂停

#define FOOTPRINT_TIME (5)
#define FOOTPRINT_INTERVAL (10)

#define LAYER_SCENEBG (0)             //地图层
#define LAYER_ROLE (10)               //角色层次
#define LAYER_MAINROLE (200)          //主角层次
#define LAYER_SCENEFG (500)           //地图前景层
#define LAYER_UI (600)                //ui界面层次
#define LAYER_MASK (700)              //蒙版层

#define GAME_INITRUNSPEED (300)       //初始场景运动速度
#define GAME_MAXRUNSPEED  (400)       //最大运动速度
#define GAME_RUNSPEEDGROWTH (10)      //奔跑速度增长
#define GAME_INITMAXENEMY (4)         //初始最大敌人数量
#define GAME_MAXENEMYGROWTH (1)       //切换场景最大敌人增加数
#define GAME_DIFFGROWTH (0.07f)       //难度增长幅度
#define GAME_SCENEFADE (1)         //场景切换淡入淡出的时间
#define GAME_SCENECOUNT (5)

#define SPECIAL_FENSHEN (100.0f)         //分身与中间轴的距离

#define NNINJA_ACCURATE (20)          //精准
#define NNINJA_POLLTIME (1)           //活动周期
#define NNINJA_DARTSPEED (150)        //飞镖速度
#define NNINJA_AGGRISIVE (70)         //侵略性
#define NNINJA_RESPAWNCD (5)          //刷新cd
#define NNINJA_PREPARE (1.5f)         //准备时间
#define NNINJA_MAXDART (4)            //撤退标准
#define NNINJA_SCORE (10)              //下忍的分数

#define MNINJA_ACCURATE (15)          //精准
#define MNINJA_POLLTIME (1)           //活动周期
#define MNINJA_DARTSPEED (170)        //飞镖速度
#define MNINJA_AGGRISIVE (75)         //侵略性
#define MNINJA_RESPAWNCD (5)          //刷新cd
#define MNINJA_PREPARE (1.5f)            //准备时间
#define MNINJA_MAXDART (7)            //撤退标准
#define MNINJA_SCORE (15)              //中忍的分数

#define HNINJA_ACCURATE (10)          //精准
#define HNINJA_POLLTIME (1)           //活动周期
#define HNINJA_DARTSPEED (200)        //飞镖速度
#define HNINJA_AGGRISIVE (80)         //侵略性
#define HNINJA_RESPAWNCD (8)          //刷新cd
#define HNINJA_PREPARE (1.5f)            //准备时间
#define HNINJA_SCORE (20)              //上忍的分数

#define SAMURAI_RESPAWNCD (6)         //武士刷新cd
#define SAMURAI_WARNING (3)           //武士提醒时间
#define SAMURAI_SCORE (35)             //武士的分数

#define ARCHER_RESPAWNCD (7)          //弓箭手刷新cd
#define ARCHER_SCORE (50)             //切弓箭的分数

#define MECHANIC_POLLTIME (2)        //活动周期
#define MECHANIC_ATTACKCD (3)         //攻击cd
#define MECHANIC_PREPARE  (2)         //准备时间
#define MECHANIC_LAUNCH   (5)         //一次发射飞镖数
#define MECHANIC_LAUNCHCD (0.05f)         //飞镖发射cd
#define MECHANIC_ANGLE    (8)         //飞镖发射偏角
#define MECHANIC_RESPAWNCD (8)        //机械男cd
#define MECHANIC_SCORE (25)            //机械男

#define SANTA_LIFE (20)
#define SANTA_COIN (0.2f)

//游戏技能定义
#define SPELL_TRIDARTS (0)//三发飞镖
#define SPELL_REPLEACE (1)//替身术
#define SPELL_MOONBLADE (2)//月牙斩
#define SPELL_TWOBODIES (3)//分身术
#define SPELL_TRACEDART (4)//回旋手里剑
#define SPELL_REFLECTDART (5)//反弹手里剑
#define SPELL_POWERBRUST (6)//元气爆
#define SPELL_EVILBLADE (7)//妖刀
#define SPELL_GODHAND (8)//鬼影手
#define SPELL_BOMBDART (9)//爆弹
#define SPELL_DASH (10)//DASH
#define SPELL_MULTIDARTS (11)//八方手里剑
#define SPELL_INVISIBLE (12)//隐身术
#define SPELL_SLOWTIME (13)//子弹时间
#define SPELL_ADDHEALTH (14)//加血
#define SPELL_ADDPOWER (15)//加气
#define SPELL_BLINK (16)//一闪(全屏消除飞镖和炸弹)
#define SPELL_STARFALL (17)//流星雨
#define SPELL_DARTSUPPLY (18)//飞镖无限
#define SPELL_SHADOWKILL (19)//全屏分身
#define SPELL_FLY (20)//天狗木屐
#define SPELL_PUMPKINIMPS (21)//召唤南瓜小鬼

#define ITEM_SHURIKENSTART (0)
#define ITEM_SHURIKENEND (12)
#define ITEM_KATANASTART (12)
#define ITEM_KATANAEND (27)
#define ITEM_SPECIALSTART (27)
#define ITEM_SPECIALEND (38)

#define COINS_VALUE1 (2000)
#define COINS_VALUE2 (10000)
#define COINS_VALUE3 (50000)
#define COINS_VALUE4 (200000)

//*** ARCADE MODE ***
#define ARCADE_TIMER (90)
#define ARCADE_DISRATE (50)
#define ARCADE_COMBOBGN (5)
#define ARCADE_COMBOAdd (5)
#define ARCADE_INITSPEED (1000)
#define ARCADE_MINSPEED (980)
#define ARCADE_MAXSPEED (1500)
#define ARCADE_FEVER1 (1100)
#define ARCADE_FEVER2 (1400)
#define ARCADE_FEVERGAP (50)
#define ARCADE_HURTPUBLISH (50)
#define ARCADE_SHRINKSA (0.05f)
#define ARCADE_SHRINKSB (1)
#define ARCADE_PRIZEBASE (6000)
#define ARCADE_PRIZEFACE (600)
#define ARCADE_PRIZELEVEL (45)
#define ARCADE_PRIZEMINUS (3)
#define ARCADE_PRIZEPBASE (200)
#define ARCADE_PRIZEPFACE (20)
#define ARCADE_ENEMY_A (0.01f)
#define ARCADE_PREPARE_BASE (2)
#define ARCADE_PREPARE_A (-0.002668f)
//-bouns-
#define ARCADE_BOUNS_DART (3)
#define ARCADE_BOUNS_ICEPIKE (5)

#define FRIENDS_BOUNSINIT (100)
#define FRIENDS_BOUNSONE  (20)
#define FRIENDS_BOUNSMAX  (2000)

//技能参数
#define DASH_TIME (0.2f)

#define TWITTER_URL @"http://itunes.apple.com/us/app/little-ninja-rush/id540290969?mt=8"//分享到twitter上的url
#define TIP_FONTNAME @"MarkerFelt-Thin"

//Achieve Code Define
#define ACH_DISTANCE (0)
#define ACH_FIREDSHURIKENS (1)
#define ACH_COINSGAINED (2)
#define ACH_COINSSPENT (3)
#define ACH_CASHEDIN (4)
#define ACH_MERCHANTMET (5)
#define ACH_EMPITYSCROLL (6)
#define ACH_STARTGAME (7)
#define ACH_SELECTBUTTON (8)
#define ACH_OPENSTORE (9)
#define ACH_CHANGEEQUIPMENT (10)
#define ACH_SCENEPASSED (11)
#define ACH_CHUNINBYBOMB (12)
#define ACH_JONINBYONESTRIKE (13)
#define ACH_CRITICAL (14)
#define ACH_KILLING (15)
#define ACH_KILLGENIN (16)
#define ACH_KILLCHUNIN (17)
#define ACH_KILLJONIN (18)
#define ACH_KILLSAMURAI (19)
#define ACH_KILLMESSAGER (20)
#define ACH_KILLMECHANICS (21)
#define ACH_DEATH (22)
#define ACH_WOUNDED (23)
#define ACH_DEADBYSHURIKEN (24)
#define ACH_DEADBYSAMURAI (25)
#define ACH_DEADBYARCHER (26)
#define ACH_DEADBYBOMB (27)
#define ACH_DEADBYICESPIKE (28)
#define ACH_COLLECTSHURIKEN (29)
#define ACH_COLLECTKATANA (30)
#define ACH_COLLECTSPELL (31)
#define ACH_COLLECTSCROLL (32)
#define ACH_SLICEARROW (33)
#define ACH_SPELLSPECIAL (34)
#define ACH_FEVERCOUNT (35)
#define ACH_SCENESWITHOUTSPELL (36)
#define ACH_REFLECTTODEATH (37)
#define ACH_SLICEICEDART (38)
#define ACH_SLICEICESPIKE (39)
#define ACH_ENEMYBURNT (40)
#define ACH_DIEINSCENE1 (41)
#define ACH_DIEINSCENE3 (42)
#define ACH_DARTONGROUND (43)
#define ACH_SLICE5DARTS (44)
#define ACH_SAKURA (45)
#define ACH_ALIENPUSSY (46)
#define ACH_TRUEMANWITHBLADES (47)
#define ACH_falseITEM (48)
#define ACH_NEVERMISS (49)
#define ACH_DAYWORK (50)
#define ACH_DAILYWORK (51)
#define ACH_WEEKLYWORK (52)
#define ACH_MONTHLYWORK (53)
#define ACH_COMPLETEWORKS (54)
#define ACH_USEPOTION (55)
#define ACH_USEGAGET (56)
#define ACH_USEALLCHARACTER (57)
#define ACH_USECHARACTER0 (58)
#define ACH_USECHARACTER1 (59)
#define ACH_USECHARACTER2 (60)
#define ACH_USECHARACTER3 (61)
#define ACH_USECHARACTER4 (62)//reserved
#define ACH_USECHARACTER5 (63)//reserved
#define ACH_MUSASHIKILLSAMURAI (64)
#define ACH_MUSASHIDISTANCE (65)
#define ACH_MAROONDISTANCE (66)
#define ACH_MAROONWOUND (67)
#define ACH_SAKURADISTANCE (68)
#define ACH_SAKURAFIRE (69)
#define ACH_SAKURACOINS (70)
#define ACH_MEETCHRISTMAS (71)
#define ACH_KILLPUMPKINS (72)
#define ACH_SPELLPUMPKIN (73)
#define ACH_ARCADEfalseHIT (74)
#define ACH_ARCADEGOLD (75)
#define ACH_ARCADETROPHY (76)

#define ACH_OVERLINE (999)

#define ACH_ONEDISTANCE (1000)
#define ACH_ONEKILLING (1001)
#define ACH_ONESCROLL (1002)
#define ACH_ONECOINS (1003)
#define ACH_ONESPELL (1004)
#define ACH_ONEREFLECTKILL (1005)
#define ACH_ONEKILLJONINCRI (1006)
#define ACH_ONEWOUNDED (1007)
#define ACH_RUNWITHOUTKILL (1008)
#define ACH_RUNWITHOUTFIRE (1009)
#define ACH_MAXFEVER (1010)
#define ACH_RUNWIHTOUTBLADE (1011)
#define ACH_REACHCOMBO (1012)
#define ACH_ONEKILLSAMURAI (1013)
#define ACH_ONEMUSASHIKILLSAMURAI (1014)
#define ACH_ONEMUSASHIDISTANCE (1015)
#define ACH_ONEMAROONDISTANCE (1016)
#define ACH_ONEMAROONWOUND (1017)
#define ACH_ONESAKURADISTANCE (1018)
#define ACH_ONESAKURAFIRE (1019)
#define ACH_ONEFIRE (1020)
#define ACH_ONESAKURACOINS (1021)
#define ACH_ONEMEETCHRISTMAS (1022)
#define ACH_ONEKILLPUMPKINS (1023)
#define ACH_ONESPELLPUMPKIN (1024)
#define ACH_ONEARCADEHIT (1025)
#define ACH_GOLDSTEAK (1026)
#define ACH_SUMMONPUMPKIN (1027)
#endif
