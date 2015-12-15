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

CCString *gAniSet[GAME_CHARCOUNT] = {
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
CCArray *gShurikens = NULL;
CCArray *gKatanas = NULL;
CCArray *gSpecials = NULL;
CCArray *gPowerups = NULL;
CCArray *gTips = NULL;

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

CCString* GameData::roleAnimation(int rid)
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

void GameData::loadShurikens()
{
    CCLog("loading shurikens.");
    if( gShurikens != NULL )
    {
        gShurikens->release();
        gShurikens = NULL;
    }
    gShurikens = new CCArray();
    gShurikens->retain();
    CCString *data = cocos2d::CCString::createWithContentsOfFile(cocos2d::CCFileUtils::sharedFileUtils()->fullPathForFilename("data/shuriken.json").c_str());
    if( data != NULL )
    {
        CCArray *parsed = (cocos2d::CCArray*)JsonWrapper::parseJson(data);
        CCObject *pObj = NULL;
        CCARRAY_FOREACH(parsed, pObj)
        {
            CCDictionary *pDic = (cocos2d::CCDictionary*)pObj;
            Shuriken *sh = new Shuriken();
            sh->uiid = ((cocos2d::CCNumber*)pDic->objectForKey("uiid"))->getInt();
            sh->titl = (cocos2d::CCString*)pDic->objectForKey("titl");
            sh->name = (cocos2d::CCString*)pDic->objectForKey("name");
            sh->desc = (cocos2d::CCString*)pDic->objectForKey("desc");
            sh->icon = (cocos2d::CCString*)pDic->objectForKey("icon");
            sh->shap = (cocos2d::CCString*)pDic->objectForKey("shap");
            sh->flys = ((cocos2d::CCNumber*)pDic->objectForKey("flys"))->getFloat();
            sh->reld = ((cocos2d::CCNumber*)pDic->objectForKey("reld"))->getFloat();
            sh->efft = ((cocos2d::CCNumber*)pDic->objectForKey("efft"))->getInt();
            sh->wght = ((cocos2d::CCNumber*)pDic->objectForKey("wght"))->getFloat();
            gShurikens->addObject(sh);
        }
        
        data->release();
    }
    else{
        CCLog("GameData: Failed to load shurikens.");
    }
}

void GameData::loadKatanas()
{
    CCLog("loading katanas.");
    if( gKatanas != NULL )
    {
        gKatanas->release();
        gKatanas = NULL;
    }
    gKatanas = new CCArray();
    gKatanas->retain();
    CCString *data = cocos2d::CCString::createWithContentsOfFile(cocos2d::CCFileUtils::sharedFileUtils()->fullPathForFilename("data/katana.json").c_str());
    //CCString *data = cocos2d::CCString::createWithContentsOfFile(cocos2d::CCFileUtils::sharedFileUtils()->fullPathFromRelativePath("data/katana.json"));
    if( data != NULL )
    {
        CCArray *parsed = (cocos2d::CCArray*)JsonWrapper::parseJson(data);
        CCObject *pObj = NULL;
        CCARRAY_FOREACH(parsed, pObj)
        {
            CCDictionary *pDic = (cocos2d::CCDictionary*)pObj;
            Katana *sh = new Katana();
            sh->uiid = ((cocos2d::CCNumber*)pDic->objectForKey("uiid"))->getInt();
            sh->titl = (cocos2d::CCString*)pDic->objectForKey("titl");
            sh->name = (cocos2d::CCString*)pDic->objectForKey("name");
            sh->desc = (cocos2d::CCString*)pDic->objectForKey("desc");
            sh->icon = (cocos2d::CCString*)pDic->objectForKey("icon");
            sh->anim = ((cocos2d::CCNumber*)pDic->objectForKey("anim"))->getInt();
            sh->sond = (cocos2d::CCString*)pDic->objectForKey("sond");
            sh->rang = ((cocos2d::CCNumber*)pDic->objectForKey("rang"))->getFloat();
            sh->efft = ((cocos2d::CCNumber*)pDic->objectForKey("efft"))->getInt();
            sh->reld = ((cocos2d::CCNumber*)pDic->objectForKey("reld"))->getFloat();
            sh->wght = ((cocos2d::CCNumber*)pDic->objectForKey("wght"))->getFloat();
            gKatanas->addObject(sh);
        }
        
        data->release();
    }
    else{
        CCLog("GameData: Failed to load katanas.");
    }
}

void GameData::loadSpecials()
{
    CCLog("loading specials.");
    if( gSpecials != NULL )
    {
        gSpecials->release();
        gSpecials = NULL;
    }
    gSpecials = new CCArray();
    gSpecials->retain();
    CCString *data = cocos2d::CCString::createWithContentsOfFile(cocos2d::CCFileUtils::sharedFileUtils()->fullPathForFilename("data/special.json").c_str());
    if( data != NULL )
    {
        CCArray *parsed = (cocos2d::CCArray*)JsonWrapper::parseJson(data);
        CCObject *pObj = NULL;
        CCARRAY_FOREACH(parsed, pObj)
        {
            CCDictionary *pDic = (cocos2d::CCDictionary*)pObj;
            Special *sh = new Special();
            sh->uiid = ((cocos2d::CCNumber*)pDic->objectForKey("uiid"))->getInt();
            sh->titl = (cocos2d::CCString*)pDic->objectForKey("titl");
            sh->name = (cocos2d::CCString*)pDic->objectForKey("name");
            sh->desc = (cocos2d::CCString*)pDic->objectForKey("desc");
            sh->icon = (cocos2d::CCString*)pDic->objectForKey("icon");
            sh->shap = (cocos2d::CCString*)pDic->objectForKey("shap");
            sh->spmx = ((cocos2d::CCNumber*)pDic->objectForKey("spmx"))->getInt();
            sh->spac = ((cocos2d::CCNumber*)pDic->objectForKey("spac"))->getInt();
            sh->skid = ((cocos2d::CCNumber*)pDic->objectForKey("skid"))->getInt();
            sh->wght = ((cocos2d::CCNumber*)pDic->objectForKey("wght"))->getFloat();
            gSpecials->addObject(sh);
        }
        
        data->release();
    }
    else{
        CCLog("GameData: Failed to load specials.");
    }
}

void GameData::loadTips()
{
  if( gTips != NULL )
  {
    gTips->release();
    gTips = NULL;
  }
  gTips = new CCArray();
  gTips->retain();
  CCString *data = NULL;

  const char *szFileName = NULL;
  if(true)
  {
    szFileName = "tipschs.json";
    CCLog("loading tipschs.json");
  }
  else
  {
    szFileName = "tips.json";
    CCLog("loading tips.json");
  }
  data = cocos2d::CCString::createWithContentsOfFile(cocos2d::CCFileUtils::sharedFileUtils()->fullPathForFilename("data/tipschs.json").c_str());
  if( data != NULL )
  {
    CCArray *parsed = (cocos2d::CCArray*)JsonWrapper::parseJson(data);
    CCObject *pObj = NULL;
    CCARRAY_FOREACH(parsed, pObj)
    {
      CCString *pTip = (cocos2d::CCString*)pObj;
      gTips->addObject(pTip);
    }
  }
}

CCArray* GameData::fetchTips()
{
    return gTips;
}

CCString* GameData::randomTip()
{
    srand(time(NULL));
    int n = rand()%gTips->count();
    return (cocos2d::CCString*)gTips->objectAtIndex(n);
}

CCArray* GameData::fetchShurikens()
{
    return gShurikens;
}

CCArray* GameData::fetchKatanas()
{
    return gKatanas;
}

CCArray* GameData::fetchSpecials()
{
    return gSpecials;
}

Shuriken* GameData::queryShuriken(int uiid)
{
    CCObject *pObj = NULL;
    CCARRAY_FOREACH(gShurikens, pObj)
    {
        Shuriken *sh = (Shuriken*)pObj;
        if( sh->uiid == uiid )
        {
            return sh;
        }
    }
    return NULL;
}

Katana* GameData::queryKatana(int uiid)
{
    CCObject *pObj = NULL;
    CCARRAY_FOREACH(gKatanas, pObj)
    {
        Katana *sh = (Katana*)pObj;
        if( sh->uiid == uiid )
        {
            return sh;
        }
    }
    return NULL;
}

Special* GameData::querySpecial(int uiid)
{
    CCObject *pObj = NULL;
    CCARRAY_FOREACH(gSpecials, pObj)
    {
        Special *sh = (Special*)pObj;
        if( sh->uiid == uiid )
        {
            return sh;
        }
    }
    return NULL;
}

