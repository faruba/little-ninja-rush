#ifndef ___CLASSES_GAMEDATA_H_
#define ___CLASSES_GAMEDATA_H_

#include "cocos2d.h"
#include "JsonWrapper.h"

class GameItemBase : public cocos2d::Ref {
  public:
    int uiid;         // Unique Item ID
    std::string titl; // TODO: change to title
    std::string name;
    std::string desc; // TODO: change to description
    std::string icon;
    float wght; // TODO: change to weight

  public:
    virtual void handleJsonValue(const ValueWrapper& value) {
      uiid = value.getInteger("uiid");
      titl = value.getString("titl");
      name = value.getString("name");
      desc = value.getString("desc");
      icon = value.getString("icon");
      wght = value.getDouble("wght");
    }
};

class Shuriken: public GameItemBase {
  public:
    float flys;
    float reld;
    int efft;   // TODO: change to effect
    std::string shap; // TODO: change to shape

    virtual void handleJsonValue(const ValueWrapper& value) {
      GameItemBase::handleJsonValue(value);
      flys = value.getDouble("wght");
      reld = value.getDouble("wght");
      efft = value.getInteger("uiid");
      shap = value.getString("shap");
    }
};

class Katana:public GameItemBase {
  public:
    int anim;
    std::string sond;
    float rang;
    int efft;
    float reld;

    virtual void handleJsonValue(const ValueWrapper& value) {
      GameItemBase::handleJsonValue(value);
      anim = value.getInteger("anim");
      sond = value.getString("sond");
      rang = value.getDouble("rang");
      efft = value.getInteger("efft");
      reld = value.getDouble("reld");
    }
};

//特技
class Special:public GameItemBase
{
  public:
    int spmx;
    int spac;
    int skid;
    std::string shap; // TODO: change to shape

    virtual void handleJsonValue(const ValueWrapper& value) {
      GameItemBase::handleJsonValue(value);
      spmx = value.getInteger("spmx");
      spac = value.getInteger("spac");
      skid = value.getInteger("skid");
      shap = value.getString("shap");
    }
};

class GameData : public cocos2d::Ref
{
  public:

    static int roleCurrDart(int rid);
    static int roleCurrHP(int rid);
    static int roleMaxDart(int rid);
    static int roleMaxHP(int rid);
    static float roleRunSpeed(int rid);
    static float roleArcadeBouns(int rid);
    static cocos2d::CCString* roleAnimation(int rid);
    
    static int upgradeCost(int uid, int lev);

    static void loadData();
    static void releaseData();

    static cocos2d::CCArray* fetchShurikens();
    static cocos2d::CCArray* fetchKatanas();
    static cocos2d::CCArray* fetchSpecials();
    static cocos2d::CCArray* fetchTips();
    
    static Shuriken* queryShuriken(int uiid);
    static Katana* queryKatana(int uiid);
    static Special* querySpecial(int uiid);
    static cocos2d::CCString* randomTip();
};

template <typename T>
void loadVectorFromJsonFile(const char *filename, std::vector<T*> &vector) {
  CCLOG("Loading %s...", filename);
  try {
    JsonWrapper::parseJsonFileForVector(filename, vector);
  } catch ( const char *err ) {
  }
  CCLOG("%lu items loaded.", vector.size());
}


#endif
