#ifndef ___CLASSES_GAMERECORD_H_
#define ___CLASSES_GAMERECORD_H_
#include "GameConstants.h"
//#include "IAPHelper.h"
#include "Tasks.h"
#include "Collections.h"

#include "cocos2d.h"
#include "cocos-ext.h"

using namespace cocos2d;

class GameRecord : public Ref
{
  public:
    static GameRecord* sharedGameRecord();

  public:
    virtual bool init();
CCB_STATIC_NEW_AUTORELEASE_OBJECT_WITH_INIT_METHOD(GameRecord, create);

    void read(cocos2d::CCDictionary* dic);
    void write(rapidjson::Document &document);
    void readLocal();
    void writeLocal();
    //falseT MERGE -----------------------------: 消耗性道具 及 单机设定值
    Collections *collection;


    int revision;//revision number
    int uploadflag;//icloud uploadflag

    int coins;//金币数
    int item_fly;//天狗木屐数量
    int item_sp;//满气槽数量
    int item_credit;//魔法硬币
    int curr_char;//当前角色
    //游戏设置
    int setting_music;
    int setting_sfx;
    int setting_icloud;
    //游戏运行信息
    double game_time;
    double rate_interval;
    int            rate_pop;
    int            game_notify;//-1=off 0=unset 1=on
    int            game_tutorial;//0=play 1=not play
    int            game_doublecoin;//0-off 1-on
    int            game_gift;

    int *char_usecount;
    int *char_equip_dart;
    int *char_equip_blade;
    int *char_equip_spell;

    //Local Leader Board 本地排行榜
cocos2d::CCString *lb_playername;
    int *lb_scores;
cocos2d::CCString **lb_names;
    //--- for arcade mode ---
    int *lba_scores;
cocos2d::CCString **lba_names;
    double share_facebook;
    double share_twitter;

  public:
    int score_high;//最高分
    int combo_high;//max combo

    int arcade_high;//arcade high score

    int golden_trophy;
    int silver_trophy;
    int bronze_trophy;
    int golden_steak;
    //MERGE ----------------------------------: 非消耗性道具 readonly

    //角色的配置
    int *char_hp;
    int *char_dart;
    int *char_contract;//IAP flag

    //升级数据
    int *item_upgrade;

    //IAP FLAGS
    int *iap_flag;
  public:
    Tasks *task;
    void setCharacterHp(int value, int roleid);
    void setCharacterDart(int value, int roleid);
    void setCharacterContract(int value, int roleid);
    void setItemUpgrade(int value, int itemid);
    void setIAPFlag(int value, int iapid);

    //GameRecord* loadGameRecordFromURL(NSURL* url);

    void makeCoins(int num);

    void checkPoint();

    void purchaseVerified(int pid) ;
    void purchaseCancelled(int pid) ;
    void purchaseFailed(int pid) ;
    void mergeWith(GameRecord * record) ;

    //--- local leader board ---
    void setLocalPlayerName(cocos2d::CCString* name);
    void submitScore(int score, CCString* name);
    void submitArcadeScore(int score, CCString* name);

};
#endif
