#include "GameData.h"
#include "GameConstants.h"
#include "GameTool.h"
#include "JsonWrapper.h"
#include "GameRecord.h"
#include "UniversalFit.h"
#include "CCNumber.h"

//角色的数据
int gHPBase[GAME_CHARCOUNT] = {3, 2, 4, 2};
int gHPMax[GAME_CHARCOUNT] = {5, 4, 6, 3};
int gDartBase[GAME_CHARCOUNT] = {5, 7, 4, 4};
int gDartMax[GAME_CHARCOUNT] = {7, 9, 6, 6};
int gSpeedBase[GAME_CHARCOUNT] = {300, 400, 250, 350};
float gArcadeBouns[GAME_CHARCOUNT] = {0.3f, 0.1f, 0.6f, 0.8f};

cocos2d::CCString *gAniSet[GAME_CHARCOUNT] = {
    new CCString("main"),
    new CCString("girl"),
    new CCString("musashi"),
    new CCString("pussy")
};

int gUpgradeCost[GAME_UPGRADECOUNT*3] = 
{
    500, 1500, 5000,//八方
    500, 1500, 5000,//隐身
    500, 1500, 5000,//子弹时间
    500, 1500, 5000,//加血
    500, 1500, 5000,//加气
    500, 1500, 5000,//一闪
    500, 1500, 5000,//流星雨
    500, 1500, 5000,//飞镖无限
    500, 1500, 5000,//全屏分身
};

//数据
std::vector<Shuriken*> gShurikens;
std::vector<Katana*> gKatanas;
std::vector<Special*> gSpecials;
std::vector<std::string*> gTips;

//--- character ---

int GameData::roleCurrDart(int rid)
{
    return gDartBase[rid] + GameRecord::sharedGameRecord()->char_dart[rid];
}

int GameData::roleMaxDart(int rid)
{
    return gDartMax[rid];
}
 
int GameData::roleCurrHP(int rid)
{
    return gHPBase[rid] + GameRecord::sharedGameRecord()->char_hp[rid];
}

int GameData::roleMaxHP(int rid)
{
    return gHPMax[rid];
}

float GameData::roleRunSpeed(int rid)
{
    return gSpeedBase[rid];
}

float GameData::roleArcadeBouns(int rid)
{
    return gArcadeBouns[rid];
}

cocos2d::CCString* GameData::roleAnimation(int rid)
{
    return gAniSet[rid];
}

int GameData::upgradeCost(int uid, int lev)
{
    if( lev > 2 )
    {
        return 0;
    }
    else {
        return gUpgradeCost[uid*3+lev];
    }
}

//--- data ---

void GameData::loadData() {
  loadVectorFromJsonFile("data/shuriken.json", gShurikens);
  loadVectorFromJsonFile("data/katana.json", gKatanas);
  loadVectorFromJsonFile("data/special.json", gSpecials);
  loadVectorFromJsonFile("data/tips.json", gTips);
}

void GameData::releaseData() {
  // TODO:Release memories
}

cocos2d::CCArray* GameData::fetchTips()
{
  return nullptr;
    //return gTips;
}

cocos2d::CCString* GameData::randomTip()
{
    //srand(time(NULL));
    //int n = rand()%gTips.size();
    //return (cocos2d::CCString*)gTips[n];
    return nullptr;
}

cocos2d::CCArray* GameData::fetchShurikens()
{
    return nullptr;
    //TODO:return gShurikens;
}

cocos2d::CCArray* GameData::fetchKatanas()
{
   // return gKatanas;
  return nullptr;
}

cocos2d::CCArray* GameData::fetchSpecials()
{
 //   return gSpecials;
  return nullptr;
}

Shuriken* GameData::queryShuriken(int uiid)
{
    return NULL;
}

Katana* GameData::queryKatana(int uiid)
{
//cocos2d::Ref *pObj = NULL;
//CCARRAY_FOREACH(gKatanas, pObj)
//    {
//        Katana *sh = (Katana*)pObj;
//        if( sh->uiid == uiid )
//        {
//            return sh;
//        }
//    }
    return NULL;
}

Special* GameData::querySpecial(int uiid)
{
//cocos2d::Ref *pObj = NULL;
//CCARRAY_FOREACH(gSpecials, pObj)
//    {
//        Special *sh = (Special*)pObj;
//        if( sh->uiid == uiid )
//        {
//            return sh;
//        }
//    }
    return NULL;
}

