#ifndef ___CLASSES_GAMEDATA_H_
#define ___CLASSES_GAMEDATA_H_

#include "cocos2d.h"

using namespace cocos2d;

//飞镖
class Shuriken:public CCObject
{
public:
 int uiid;
cocos2d::CCString *titl;
cocos2d::CCString *name;
cocos2d::CCString *desc;
cocos2d::CCString *icon;
cocos2d::CCString *shap;
 float flys;
 float reld;
 int efft;
 float wght;
};

//刀刃
class Katana:public CCObject
{
public:
 int uiid;
cocos2d::CCString *titl;
cocos2d::CCString *name;
cocos2d::CCString *desc;
cocos2d::CCString *icon;
 int anim;
cocos2d::CCString *sond;
 float rang;
 int efft;
 float reld;
 float wght;
};

//特技
class Special:public CCObject
{
public:
 int uiid;
cocos2d::CCString *titl;
cocos2d::CCString *name;
cocos2d::CCString *desc;
cocos2d::CCString *icon;
cocos2d::CCString *shap;
 int spmx;
 int spac;
 int skid;
 float wght;
};

class GameData : public CCObject
{
  public:

    static int roleCurrDart(int rid);
    static int roleCurrHP(int rid);
    static int roleMaxDart(int rid);
    static int roleMaxHP(int rid);
    static float roleRunSpeed(int rid);
    static float roleArcadeBouns(int rid);
    static CCString* roleAnimation(int rid);
    
    static int upgradeCost(int uid, int lev);

    static void loadShurikens();
    static void loadKatanas();
    static void loadSpecials();
    static void loadTips();

    static CCArray* fetchShurikens();
    static CCArray* fetchKatanas();
    static CCArray* fetchSpecials();
    static CCArray* fetchTips();
    
    static Shuriken* queryShuriken(int uiid);
    static Katana* queryKatana(int uiid);
    static Special* querySpecial(int uiid);
    static CCString* randomTip();
};


#endif
