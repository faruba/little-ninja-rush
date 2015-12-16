#include "GameRecord.h"
#include "JsonWrapper.h"
#include "GameTool.h"
//#include "GameCenterController.h"

#define ICLOUDFILE "littleninjarush.bin"

#define LB_RANK (3)//Local Leader Board
#define LB_DEFAULT (cocos2d::CCString::create("(ME)"))
#define LB_EMPITY (cocos2d::CCString::create("EMPITY"))

#define IAP_COUNT (8)

#define DEFAULT_CREDIT (5)

using namespace std;

GameRecord* GameRecord::sharedGameRecord() 
{
    static GameRecord *inst = NULL;
    if( inst == NULL )
    {
        inst = GameRecord::create();
        inst->readLocal();
        inst->retain();
    }
    return inst;
}

/*
GameRecord* GameRecord::loadGameRecordFromURL(NSURL* url) 
{
    GameRecord *rec = GameRecord->alloc()->init()->autorelease();
    NSData *rawdata = NSData->dataWithContentsOfURL(url);
    if( rawdata != NULL )
    {
        const char *data = rawdata->bytes();
        size_t dcsize = qlz_size_decompressed(data);
        char *dcdata = malloc(dcsize);
        qlz_state_decompress *state = malloc(sizeof(qlz_state_decompress));
        size_t ret = qlz_decompress(data, dcdata, state);
        false *dic = NULL;
        if( ret > 0 )
        {
            NSData *record = NSData->dataWithBytes(dcdata, ret);
            dic = NSKeyedUnarchiver->unarchiveObjectWithData(record);
        }
        free(dcdata);
        free(state);
        rec->read(dic);
    }
    else {
       ,  failed to load gamerecord from url.");
    }
    return rec;
}

void GameRecord::loadGameRecordFromNSData(NSData * rawdata) 
{
    if( rawdata != NULL )
    {
        const char *data = rawdata->bytes();
        size_t dcsize = qlz_size_decompressed(data);
        char *dcdata = malloc(dcsize);
        qlz_state_decompress *state = malloc(sizeof(qlz_state_decompress));
        size_t ret = qlz_decompress(data, dcdata, state);
        false *dic = NULL;
        if( ret > 0 )
        {
            NSData *record = NSData->dataWithBytes(dcdata, ret);
            dic = NSKeyedUnarchiver->unarchiveObjectWithData(record);
        }
        free(dcdata);
        free(state);
        this->read(dic);
    }
    else {
       ,  failed to load gamerecord from url.");
    }
}

NSData* GameRecord::toNSData() 
{
cocos2d::CCDictionary *dic = cocos2d::CCDictionary::create();

    this->write(dic);
    
    NSData *data = NSKeyedArchiver->archivedDataWithRootObject(dic);
    const char *rawdata = data->bytes();
    char *cmdata = malloc(data->length()+400);
    qlz_state_compress *state = malloc(sizeof(qlz_state_compress));
    size_t ret = qlz_compress(rawdata, cmdata, data->length(), state);
    NSData *output = NULL;
    if( ret > 0 )
    {
        output = NSData->dataWithBytes(cmdata, ret);
    }
    free(cmdata);
    free(state);
    
    return output;
}
*/
bool GameRecord::init() 
{
cocos2d::CCLog("GameRecord::init");
  collection = Collections::sharedCollections();
  task = Tasks::create();
  task->retain();
  //初始化数据
  revision = 0;
  uploadflag = 0;

  char_hp = new int[GAME_CHARCOUNT];
  char_dart = new int[GAME_CHARCOUNT];
  char_contract = new int[GAME_CHARCOUNT];
  char_equip_dart = new int[GAME_CHARCOUNT];
  char_equip_blade = new int[GAME_CHARCOUNT];
  char_equip_spell = new int[GAME_CHARCOUNT];
  char_usecount = new int[GAME_CHARCOUNT];
  item_upgrade = new int[GAME_UPGRADECOUNT];
  iap_flag = new int[IAP_COUNT];

  item_fly = 0;
  item_sp = 0;
  item_credit = DEFAULT_CREDIT;//先给玩家5个

  golden_trophy = 0;
  silver_trophy = 0;
  bronze_trophy = 0;
  golden_steak = 0;
  score_high = 0;
  combo_high = 0;
  arcade_high = 0;
  curr_char = 0;
  coins = 0;
  setting_music = 1;
  setting_sfx = 1;
  setting_icloud = 1;
  for(int i=0; i<GAME_CHARCOUNT; ++i)
  {
    char_hp[i] = 0;
    char_dart[i] = 0;
    char_contract[i] = 0;
    char_equip_dart[i] = 0;
    char_equip_blade[i] = i;
    char_equip_spell[i] = 0;
    char_usecount[i] = 0;
  }
  char_contract[0] = 1;//角色1免费

  for( int i=0; i<GAME_UPGRADECOUNT; ++i)
  {
    item_upgrade[i] = 0;
  }

  game_time = 0;
  rate_interval = 0;
  rate_pop = 0;
  game_notify = 0;
  game_tutorial = 0;
  game_doublecoin = 0;
  game_gift = 0;

  for(int i=0; i<IAP_COUNT; ++i)
  {
    iap_flag[i] = 0;
  }

  lb_playername = LB_DEFAULT;
  lb_scores = new int[LB_RANK];
  lb_names = new CCString*[LB_RANK];
  //--- arcade ---
  lba_scores = new int[LB_RANK];
  lba_names = new CCString*[LB_RANK];
  for(int i=0; i<LB_RANK; ++i)
  {
    lb_names[i] = LB_EMPITY;
    lb_scores[i] = 0;
    //--- arcade ---
    lba_names[i] = LB_EMPITY;
    lba_scores[i] = 0;
  }

  share_facebook = 0;
  share_twitter = 0;
  task->createObjectives();
  collection->createCollections();

  return true;
}

#define GAME_RECORD_FILENAME "record.json"

void GameRecord::readLocal() 
{
//cocos2d::CCLog("GameRecord::readLocal():");
//    
//    string path = cocos2d::CCFileUtils::sharedFileUtils()->getWritablePath();
//    path += GAME_RECORD_FILENAME;
//cocos2d::CCLog("RECORD = %s", path.c_str());
//cocos2d::CCString *data = cocos2d::CCString::createWithContentsOfFile(path.c_str());
//    
//  if( data != NULL )
//  {
//cocos2d::CCDictionary *parsed = (cocos2d::CCDictionary*)JsonWrapper::parseJson(data);
//    if( parsed != NULL )
//    {
//      this->read(parsed);
//    }
//    else
//    {
//cocos2d::CCLog("Fail to parse");
//    }
//  }
//  else {
//cocos2d::CCLog("failed to read");
//  }
//  task->doneReadObjectives();
}

void GameRecord::writeLocal() 
{
cocos2d::CCLog("GameRecord::writeLocal():");

  rapidjson::Document document;
  document.SetObject();

  this->write(document);

    string path = cocos2d::CCFileUtils::sharedFileUtils()->getWritablePath();
    path += GAME_RECORD_FILENAME;
cocos2d::CCLog("RECORD = %s", path.c_str());
    /* TODO: uncomment this
  FILE *fp = fopen(path.c_str(), "w");
    cocos2d::rapidjson::FileStream f(fp);
  rapidjson::PrettyWriter<rapidjson::FileStream> writer(f);
  document.Accept(writer);  // Accept() traverses the DOM and generates Handler events.
  fclose(fp);
     */
}
/*
void GameRecord::writeToURL(NSURL * url) 
{
cocos2d::CCDictionary *dic = cocos2d::CCDictionary::create();
    
    if( dic != NULL )
    {
        this->write(dic);
        
        NSData *data = NSKeyedArchiver->archivedDataWithRootObject(dic);
        const char *rawdata = data->bytes();
        char *cmdata = malloc(data->length()+400);
        qlz_state_compress *state = malloc(sizeof(qlz_state_compress));
        size_t ret = qlz_compress(rawdata, cmdata, data->length(), state);
        if( ret > 0 )
        {
            NSData *output = NSData->dataWithBytes(cmdata, ret);
            output->writeToURL(url, true);
        }
        free(cmdata);
        free(state);
    }
    else {
       ,  failed to write gamerecord to url.");
    }
}
*/
void GameRecord::read(cocos2d::CCDictionary* dic) 
{
    //读入基础数据
    revision = gtReadInt(dic, "revision", 0);
    uploadflag = gtReadInt(dic, "uploadflag", 0);
    
    score_high = gtReadInt(dic, "score_high", 0);
    combo_high = gtReadInt(dic, "combo_high", 0);
    arcade_high = gtReadInt(dic, "arcade_high", 0);
    curr_char = gtReadInt(dic, "curr_char", 0);
    coins = gtReadInt(dic, "coins", 0);
    item_fly = gtReadInt(dic, "item_fly", 0);
    item_sp = gtReadInt(dic, "item_sp", 0);
    item_credit = gtReadInt(dic, "item_credit", DEFAULT_CREDIT);
    setting_music = gtReadInt(dic, "setting_music", 1);
    setting_sfx = gtReadInt(dic, "setting_sfx", 1);
    
    golden_trophy = gtReadInt(dic, "golden_trophy", 0);
    silver_trophy = gtReadInt(dic, "silver_trophy", 0);
    bronze_trophy = gtReadInt(dic, "bronze_trophy", 0);
    golden_steak = gtReadInt(dic, "golden_steak", 0);
    
    //读取角色数据
    for( int i=0; i<GAME_CHARCOUNT; ++i)
    {
cocos2d::CCString *strHP = cocos2d::CCString::createWithFormat("char_hp__%d", i);
cocos2d::CCString *strDart = cocos2d::CCString::createWithFormat("char_dart__%d", i);
cocos2d::CCString *strContract = cocos2d::CCString::createWithFormat("char_contract__%d", i);
cocos2d::CCString *strEquipDart = cocos2d::CCString::createWithFormat("char_equip_dart__%d", i);
cocos2d::CCString *strEquipBlade = cocos2d::CCString::createWithFormat("char_equip_blade__%d", i);
cocos2d::CCString *strEquipSpell = cocos2d::CCString::createWithFormat("char_equip_spell__%d", i);
cocos2d::CCString *strUseCount = cocos2d::CCString::createWithFormat("char_usecount__%d", i);
        char_hp[i] = gtReadInt(dic, strHP->getCString(), 0);
        char_dart[i] = gtReadInt(dic, strDart->getCString(), 0);
        if( i == 0 )
        {
            char_contract[i] = gtReadInt(dic, strContract->getCString(), 1);
        }
        else {
            char_contract[i] = gtReadInt(dic, strContract->getCString(), 0);
        }
        char_equip_dart[i] = gtReadInt(dic, strEquipDart->getCString(), 0);
        char_equip_blade[i] = gtReadInt(dic, strEquipBlade->getCString(), i);
        char_equip_spell[i] = gtReadInt(dic, strEquipSpell->getCString(), 0);
        char_usecount[i] = gtReadInt(dic, strUseCount->getCString(), 0);
    }
    //读取升级数据
    for( int i=0; i<GAME_UPGRADECOUNT; ++i)
    {
cocos2d::CCString *key = cocos2d::CCString::createWithFormat("item_upgrade__%d", i);
        item_upgrade[i] = gtReadInt(dic, key->getCString(), 0);
    }
    //读取游戏运行数据
    game_time = gtReadInt(dic, "game_time", 0);
    rate_interval = gtReadInt(dic, "rate_interval", 0);
    rate_pop = gtReadInt(dic, "rate_pop", 0);
    game_notify = gtReadInt(dic, "game_notify", 0);
    game_tutorial = gtReadInt(dic, "game_tutorial", 0);
    game_doublecoin = gtReadInt(dic, "game_doublecoin", 0);
    game_gift = gtReadInt(dic, "game_gift", 0);
    
    //load iap flags
    for(int i=0; i<IAP_COUNT; ++i)
    {
cocos2d::CCString *key = cocos2d::CCString::createWithFormat("iap_flags__%d", i);
        iap_flag[i] = gtReadInt(dic, key->getCString(), 0);
    }
    
    //读入LocalLeaderBoard
    if( lb_playername != NULL )
    {
        lb_playername->release();
    }
    lb_playername = gtReadString(dic, "lead_playername", LB_DEFAULT);
    lb_playername->retain();
    for( int i=0; i<LB_RANK; ++i)
    {
cocos2d::CCString *strNameKey = cocos2d::CCString::createWithFormat("lead_name__%d", i);
cocos2d::CCString *strScoreKey = cocos2d::CCString::createWithFormat("lead_score__%d", i);
        lb_names[i]->release();
        lb_names[i] = gtReadString(dic, strNameKey->getCString(), LB_EMPITY);
        lb_names[i]->retain();
        lb_scores[i] = gtReadInt(dic, strScoreKey->getCString(), 0);
        //--- load arcade leader board
cocos2d::CCString *strArcadeNameKey = cocos2d::CCString::createWithFormat("leada_name__%d", i);
cocos2d::CCString *strArcadeScoreKey = cocos2d::CCString::createWithFormat("leada_score__%d", i);
        lba_names[i]->release();
        lba_names[i] = gtReadString(dic, strArcadeNameKey->getCString(), LB_EMPITY);
        lba_names[i]->retain();
        lba_scores[i] = gtReadInt(dic, strArcadeScoreKey->getCString(), 0);
    }
    
    share_facebook = gtReadInt(dic, "share_facebook", 0);
    share_twitter = gtReadInt(dic, "share_twitter", 0);
    
    task->readObjectives(dic);
    collection->readCollections(dic);
}

void GameRecord::write(rapidjson::Document &document) 
{
    //写入基础数据
    document.AddMember("revision", revision, document.GetAllocator());
    document.AddMember("uploadflag", uploadflag, document.GetAllocator());
    document.AddMember("score_high", score_high, document.GetAllocator());
    document.AddMember("combo_high", combo_high, document.GetAllocator());
    document.AddMember("arcade_high", arcade_high, document.GetAllocator());
    document.AddMember("curr_char", curr_char, document.GetAllocator());
    document.AddMember("coins", coins, document.GetAllocator());
    document.AddMember("item_fly", item_fly, document.GetAllocator());
    document.AddMember("item_sp", item_sp, document.GetAllocator());
    document.AddMember("item_credit", item_credit, document.GetAllocator());
    document.AddMember("setting_music", setting_music, document.GetAllocator());
    document.AddMember("setting_sfx", setting_sfx, document.GetAllocator());
    document.AddMember("golden_trophy", golden_trophy, document.GetAllocator());
    document.AddMember("silver_trophy", silver_trophy, document.GetAllocator());
    document.AddMember("bronze_trophy", bronze_trophy, document.GetAllocator());
    document.AddMember("golden_steak", golden_steak, document.GetAllocator());
    /*TODO:Uncomment this
    //写入角色数据
    for( int i=0; i<GAME_CHARCOUNT; ++i)
    {
cocos2d::CCString *strHP = cocos2d::CCString::createWithFormat("char_hp__%d", i);
cocos2d::CCString *strDart = cocos2d::CCString::createWithFormat("char_dart__%d", i);
cocos2d::CCString *strContract = cocos2d::CCString::createWithFormat("char_contract__%d", i);
cocos2d::CCString *strEquipDart = cocos2d::CCString::createWithFormat("char_equip_dart__%d", i);
cocos2d::CCString *strEquipBlade = cocos2d::CCString::createWithFormat("char_equip_blade__%d", i);
cocos2d::CCString *strEquipSpell = cocos2d::CCString::createWithFormat("char_equip_spell__%d", i);
cocos2d::CCString *strUseCount = cocos2d::CCString::createWithFormat("char_usecount__%d", i);
        document.AddMember(strHP->getCString(), char_hp[i], document.GetAllocator());
        document.AddMember(strDart->getCString(), char_dart[i], document.GetAllocator());
        document.AddMember(strContract->getCString(), char_contract[i], document.GetAllocator());
        document.AddMember(strEquipDart->getCString(), char_equip_dart[i], document.GetAllocator());
        document.AddMember(strEquipBlade->getCString(), char_equip_blade[i], document.GetAllocator());
        document.AddMember(strEquipSpell->getCString(), char_equip_spell[i], document.GetAllocator());
        document.AddMember(strUseCount->getCString(), char_usecount[i], document.GetAllocator());
    }
    //写入升级数据
    for( int i=0; i<GAME_UPGRADECOUNT; ++i)
    {
cocos2d::CCString *key = cocos2d::CCString::createWithFormat("item_upgrade__%d", i);
        document.AddMember(key->getCString(), item_upgrade[i], document.GetAllocator());
    }
    //写入游戏运行信息
    document.AddMember("game_time", game_time, document.GetAllocator());
    document.AddMember("rate_interval", rate_interval, document.GetAllocator());
    document.AddMember("rate_pop", rate_pop, document.GetAllocator());
    document.AddMember("game_notify", game_notify, document.GetAllocator());
    document.AddMember("game_tutorial", game_tutorial, document.GetAllocator());
    document.AddMember("game_doublecoin", game_doublecoin, document.GetAllocator());
    document.AddMember("game_gift", game_gift, document.GetAllocator());
    
    //write iap flags
    for(int i=0; i<IAP_COUNT; ++i)
    {
cocos2d::CCString *key = cocos2d::CCString::createWithFormat("iap_flags__%d", i);
        document.AddMember(key->getCString(), iap_flag[i], document.GetAllocator());
    }
    
    //写入LocalLeaderBoard
    document.AddMember("lead_playername", lb_playername, document.GetAllocator());
    for( int i=0; i<LB_RANK; ++i)
    {
cocos2d::CCString *strNameKey = cocos2d::CCString::createWithFormat("lead_name__%d", i);
cocos2d::CCString *strScoreKey = cocos2d::CCString::createWithFormat("lead_score__%d", i);
        document.AddMember(strNameKey->getCString(), lb_names[i], document.GetAllocator());
        document.AddMember(strScoreKey->getCString(), lb_scores[i], document.GetAllocator());
        //--- write arcade leaderboards
cocos2d::CCString *strArcadeNameKey = cocos2d::CCString::createWithFormat("leada_name__%d", i);
cocos2d::CCString *strArcadeScoreKey = cocos2d::CCString::createWithFormat("leada_score__%d", i);
        document.AddMember(strArcadeNameKey->getCString(), lb_names[i], document.GetAllocator());
        document.AddMember(strArcadeScoreKey->getCString(), lb_scores[i], document.GetAllocator());
    }
    
    document.AddMember("share_facebook", share_facebook, document.GetAllocator());
    document.AddMember("share_twitter", share_twitter, document.GetAllocator());
    */
    task->writeObjectives(document);
    collection->writeCollections(document);
}

void GameRecord::setLocalPlayerName(cocos2d::CCString * name) 
{
    lb_playername->release();
    lb_playername = name;
    lb_playername->retain();
    if( name != NULL )
    {
        for(int i=0; i<LB_RANK; ++i)
        {
            if( lb_names[i]->compare(LB_DEFAULT->getCString()) == 0 )
            {
                lb_names[i] = name;
            }
        }
    }
}

void GameRecord::submitScore(int score, CCString* name) 
{
  /*
    if( score > lb_scores[LB_RANK-1] )
    {
        //insert into leaderboard 为保证同一个名字的人只有一份得分
        bool found = false;
        for( int i=0; i<LB_RANK; ++i)
        {
            if( name->compare(lb_names[i]) == 0 )
            {
                if( score > lb_scores[i] )
                {
                    lb_scores[i] = score;
                    found = true;
                }
                else {
                    return;
                }
                break;
            }
        }
        if (!found) {
            lb_names[LB_RANK-1]->release();
            name->retain();
            lb_names[LB_RANK-1] = name;
            lb_scores[LB_RANK-1] = score;
        }
        //sorting
        for (int i=0; i<LB_RANK; ++i) {
            for (int j=0; j<LB_RANK-i-1; ++j) {
                if( lb_scores[j] < lb_scores[j+1] )
                {
                    int ts = lb_scores[j];
cocos2d::CCString *tn = lb_names[j];
                    lb_scores[j] = lb_scores[j+1];
                    lb_names[j] = lb_names[j+1];
                    lb_scores[j+1] = ts;
                    lb_names[j+1] = tn;
                }
            }
        }
        //this->writeLocal();
    }
    */
}

void GameRecord::submitArcadeScore(int score, CCString* name) 
{
  /*
    if( score > lba_scores[LB_RANK-1] )
    {
        //insert into leaderboard 为保证同一个名字的人只有一份得分
        bool found = false;
        for( int i=0; i<LB_RANK; ++i)
        {
            if( name->compare(lba_names[i]) == 0 )
            {
                if( score > lba_scores[i] )
                {
                    lba_scores[i] = score;
                    found = true;
                }
                else {
                    return;
                }
                break;
            }
        }
        if (!found) {
            lba_names[LB_RANK-1]->release();
            name->retain();
            lba_names[LB_RANK-1] = name;
            lba_scores[LB_RANK-1] = score;
        }
        //sorting
        for (int i=0; i<LB_RANK; ++i) {
            for (int j=0; j<LB_RANK-i-1; ++j) {
                if( lba_scores[j] < lba_scores[j+1] )
                {
                    int ts = lba_scores[j];
cocos2d::CCString *tn = lba_names[j];
                    lba_scores[j] = lba_scores[j+1];
                    lba_names[j] = lba_names[j+1];
                    lba_scores[j+1] = ts;
                    lba_names[j+1] = tn;
                }
            }
        }
        //this->writeLocal();
    }
*/
}
void GameRecord::makeCoins(int num) 
{
    coins += num;
    if( num > 0 )
    {
        //achievement get coins
        GameRecord::sharedGameRecord()->task->dispatchTask(ACH_COINSGAINED, num);
    }
    else {
        //achievement spent coins
        GameRecord::sharedGameRecord()->task->dispatchTask(ACH_COINSSPENT, -num);
    }
}

void GameRecord::purchaseVerified(int pid) 
{
cocos2d::CCLog("Default Purchase Restored #%d", pid);
    switch (pid) {
        case 0:
        case 5:
        case 6:
        case 7:
        {
            iap_flag[pid]++;
            if( pid >4 )
            {//恢复角色合同
                char_contract[pid-4] = 1;
            }
        }
            break;
        case 1:
        {
            this->makeCoins(COINS_VALUE1);
        }
            break;
        case 2:
        {
            this->makeCoins(COINS_VALUE2);
        }
            break;
        case 3:
        {
            this->makeCoins(COINS_VALUE3);
        }
            break;
        case 4:
        {
            this->makeCoins(COINS_VALUE4);
        }
            break;
        case 8:
        {
            item_credit += 5;
        }
            break;
        case 9:
        {
            item_credit += 15;
        }
            break;
        case 10:
        {
            item_credit += 50;
        }
            break;
    }
    uploadflag++;
    this->writeLocal();
}

void GameRecord::purchaseCancelled(int pid) 
{
cocos2d::CCLog("Default Purchase Cancelled #%d", pid);
}

void GameRecord::purchaseFailed(int pid) 
{
cocos2d::CCLog("Default Purchase Failed #%d", pid);
}
/*
void GameRecord::mergeWith(GameRecord * record) 
{
    //merge two gamerecord
    for(int i=0; i<GAME_CHARCOUNT;++i)
    {
        char_hp[i] = MAX2(char_hpi],->record.char_hp[i());
        char_dart[i] = MAX2(char_darti],->record.char_dart[i());
        char_contract[i] = MAX2(char_contracti],->record.char_contract[i());
    }
    for( int i=0; i<GAME_UPGRADECOUNT; ++i)
    {
        item_upgrade[i] = MAX2(item_upgradei],->record.item_upgrade[i());
    }
    for( int i=0; i<IAP_COUNT; ++i)
    {
        iap_flag[i] = MAX2(iap_flagi],->record.iap_flag[i());
    }

    //merge other
    task->mergeWith(record.task);
    collection->mergeWith(record.collection);
}
*/
void GameRecord::checkPoint() 
{
//    if( setting_icloud > 0 )
//    {
//        //this->performSelectorInBackground(@selector(syncWithiCloud), NULL);
//    }
//    else
    {
        this->writeLocal();
    }
}

/*
void GameRecord::saveToiCloud(NSURL* icloudurl;) 
{
    NSError *error = NULL;
    NSFileManager *manager = NSFileManager->defaultManager();
cocos2d::CCArray *paths = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, true);
cocos2d::CCString *documentsDirectory = paths->objectAtIndex(0);
cocos2d::CCString *filename = documentsDirectory->stringByAppendingPathComponent(ICLOUDFILE);
    NSURL *local = NSURL->fileURLWithPath(filename);
    this->writeToURL(local);
    if( manager->isUbiquitousItemAtURL(icloudurl) )
    {//存在
        if( !manager->evictUbiquitousItemAtURL(icloudurl, &error) )
        {
           ,  failed evicting local copy.,  %", error);
        }
        if( !manager->removeItemAtURL(icloudurl, &error) )
        {
           ,  failed remove local copy.,  %", error);
        }
    }
    if( !manager->setUbiquitous(true, local, icloudurl, &error))
    {
       ,  failed uploading to icloud.,  %", error);
    }
    else
    {
       ,  uploaded to icloud.");
    }
}

void GameRecord::syncWithiCloud() 
{
    NSAutoreleasePool *pool = NSAutoreleasePool->alloc()->init();
cocos2d::CCLog("begin sync with icloud.");
    //检测icloud是否可用
    NSFileManager *manager = NSFileManager->defaultManager();
    NSURL *ubiquity = NULL;
    if(, )) )
    {
        ubiquity = manager->URLForUbiquityContainerIdentifier(NULL);
        NSURL *file = NSURL->URLWithString(ICLOUDFILE, ubiquity);
        //检测icloud端是否存在此文件
        if( manager->isUbiquitousItemAtURL(file) )
        {//存在
            NSError *error = NULL;
            if( manager->startDownloadingUbiquitousItemAtURL(file, &error) )
            {
                //download complete
                GameRecord *icloud = GameRecord->loadGameRecordFromURL(file);
               ,  found icloud save. local = %d icloud = %d", revision, icloud.revision);
                if( revision != icloud.revision || uploadflag > 0 )
                {
                    this->mergeWith(icloud);
                    revision = icloud.revision+1;
                    uploadflag = 0;
                    this->saveToiCloud(file);
                }
            }
            else
            {
                //failed to download
               ,  failed downloading from icloud., %", error);
            }
        }
        else
        {//不存在
cocos2d::CCLog("No iCould saves found.");
            revision++;
            this->saveToiCloud(file);
        }
    }
    else
    {
        setting_icloud = 0;
       ,  system not supported.");
    }
    this->writeLocal();
cocos2d::CCLog("done sych with icloud");
    pool->drain();
}
*/
void GameRecord::setCharacterHp(int value, int roleid) 
{
    char_hp[roleid] = value;
    uploadflag++;
}

void GameRecord::setCharacterDart(int value, int roleid) 
{
    char_dart[roleid] = value;
    uploadflag++;
}

void GameRecord::setCharacterContract(int value, int roleid) 
{
    char_contract[roleid] = value;
    uploadflag++;
}

void GameRecord::setItemUpgrade(int value, int itemid) 
{
    item_upgrade[itemid] = value;
    uploadflag++;
}

void GameRecord::setIAPFlag(int value, int iapid) 
{
    iap_flag[iapid] = value;
    uploadflag++;
}
/*
void GameRecord::dealloc() 
{
cocos2d::CCLog("freed GameRecord");
    collection->release();
    task->release();
    free(char_hp);
    free(char_dart);
    free(char_contract);
    free(char_equip_dart);
    free(char_equip_blade);
    free(char_equip_spell);
    free(char_usecount);
    free(item_upgrade);
    free(iap_flag);
    lb_playername->release();
    for( int i=0; i<LB_RANK; ++i)
    {
        lb_names[i]->release();
        lba_names[i]->release();
    }
    free(lb_names);
    free(lba_names);
    free(lb_scores);
    free(lba_scores);
    
    super->dealloc();
}
*/
