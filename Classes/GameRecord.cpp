#include "GameRecord.h"
#include "JsonWrapper.h"
#include "GameTool.h"
#include "json/filestream.h"
#include "json/prettywriter.h"
#include "json/filereadstream.h"
//#include "GameCenterController.h"

#define ICLOUDFILE "littleninjarush.bin"

#define LB_RANK (3)//Local Leader Board
#define LB_DEFAULT (std::string("(ME)"))
#define LB_EMPITY (std::string("EMPITY"))

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

bool GameRecord::init() 
{
	cocos2d::CCLog("GameRecord::init");
	collection = Collections::sharedCollections();
	task = Tasks::sharedTasks();
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
	lb_names = new std::string[LB_RANK];
	//--- arcade ---
	lba_scores = new int[LB_RANK];
	lba_names = new std::string[LB_RANK];
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
	cocos2d::CCLog("GameRecord::readLocal():");

	std::string path = cocos2d::CCFileUtils::sharedFileUtils()->getWritablePath();
	path += GAME_RECORD_FILENAME;
	cocos2d::CCLog("RECORD = %s", path.c_str());
	std::string data = cocos2d::CCString::createWithContentsOfFile(path.c_str())->getCString();

	cocos2d::CCDictionary *parsed = (cocos2d::CCDictionary*)JsonWrapper::parseJson(data);
	if( parsed != NULL )
	{
		this->read(parsed);
	}
	else
	{
		cocos2d::CCLog("Fail to parse");
		writeLocal();
		readLocal();
		return;
	}
	task->doneReadObjectives();
}

void GameRecord::writeLocal() 
{
	cocos2d::CCLog("GameRecord::writeLocal():");

	rapidjson::Document document;
	document.SetObject();

	this->write(document);

	string path = cocos2d::CCFileUtils::sharedFileUtils()->getWritablePath();
	path += GAME_RECORD_FILENAME;
	cocos2d::CCLog("write RECORD = %s", path.c_str());
	FILE *fp = fopen(path.c_str(), "w");
	rapidjson::FileStream f(fp);
	rapidjson::PrettyWriter<rapidjson::FileStream> writer(f);
	document.Accept(writer);  // Accept() traverses the DOM and generates Handler events.
	fclose(fp);
}

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
		std::string strHP = std::string_format("char_hp__%d", i);
		std::string strDart = std::string_format("char_dart__%d", i);
		std::string strContract = std::string_format("char_contract__%d", i);
		std::string strEquipDart = std::string_format("char_equip_dart__%d", i);
		std::string strEquipBlade = std::string_format("char_equip_blade__%d", i);
		std::string strEquipSpell = std::string_format("char_equip_spell__%d", i);
		std::string strUseCount = std::string_format("char_usecount__%d", i);
		char_hp[i] = gtReadInt(dic, strHP.c_str(), 0);
		char_dart[i] = gtReadInt(dic, strDart.c_str(), 0);
		if( i == 0 )
		{
			char_contract[i] = gtReadInt(dic, strContract.c_str(), 1);
		}
		else {
			char_contract[i] = gtReadInt(dic, strContract.c_str(), 0);
		}
		char_equip_dart[i] = gtReadInt(dic, strEquipDart.c_str(), 0);
		char_equip_blade[i] = gtReadInt(dic, strEquipBlade.c_str(), i);
		char_equip_spell[i] = gtReadInt(dic, strEquipSpell.c_str(), 0);
		char_usecount[i] = gtReadInt(dic, strUseCount.c_str(), 0);
	}
	//读取升级数据
	for( int i=0; i<GAME_UPGRADECOUNT; ++i)
	{
		std::string key = std::string_format("item_upgrade__%d", i);
		item_upgrade[i] = gtReadInt(dic, key.c_str(), 0);
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
		std::string key = std::string_format("iap_flags__%d", i);
		iap_flag[i] = gtReadInt(dic, key.c_str(), 0);
	}

	//读入LocalLeaderBoard
	lb_playername = gtReadString(dic, "lead_playername", LB_DEFAULT);
	for( int i=0; i<LB_RANK; ++i)
	{
		std::string strNameKey = std::string_format("lead_name__%d", i);
		std::string strScoreKey = std::string_format("lead_score__%d", i);
		lb_names[i] = gtReadString(dic, strNameKey.c_str(), LB_EMPITY);
		lb_scores[i] = gtReadInt(dic, strScoreKey.c_str(), 0);
		//--- load arcade leader board
		std::string strArcadeNameKey = std::string_format("leada_name__%d", i);
		std::string strArcadeScoreKey = std::string_format("leada_score__%d", i);
		lba_names[i] = gtReadString(dic, strArcadeNameKey.c_str(), LB_EMPITY);
		lba_scores[i] = gtReadInt(dic, strArcadeScoreKey.c_str(), 0);
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
	//写入角色数据
	for( int i=0; i<GAME_CHARCOUNT; ++i)
	{
		GameTool::addMemberWithFormatToDocument(document, "char_hp__", i, char_hp[i]);
		GameTool::addMemberWithFormatToDocument(document, "char_dart__", i, char_dart[i]);
		GameTool::addMemberWithFormatToDocument(document, "char_contract__", i, char_contract[i]);
		GameTool::addMemberWithFormatToDocument(document, "char_equip_dart__", i, char_equip_dart[i]);
		GameTool::addMemberWithFormatToDocument(document, "char_equip_blade__", i, char_equip_blade[i]);
		GameTool::addMemberWithFormatToDocument(document, "char_equip_spell__", i, char_equip_spell[i]);
		GameTool::addMemberWithFormatToDocument(document, "char_usecount__", i, char_usecount[i]);
	}
	//写入升级数据
	for( int i=0; i<GAME_UPGRADECOUNT; ++i)
	{
		GameTool::addMemberWithFormatToDocument(document, "item_upgrade__", i,  item_upgrade[i]);
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
		GameTool::addMemberWithFormatToDocument(document, "iap_flags__", i, iap_flag[i]);
	}

	//写入LocalLeaderBoard
	rapidjson::Value v(lb_playername.c_str(), lb_playername.size(), document.GetAllocator());
	document.AddMember("lead_playername", v, document.GetAllocator());
	for( int i=0; i<LB_RANK; ++i)
	{
		GameTool::addMemberWithFormatToDocument(document, "lead_name__", i, lb_names[i]);
		GameTool::addMemberWithFormatToDocument(document, "lead_score__", i, lb_scores[i]);
		//--- write arcade leaderboards
		GameTool::addMemberWithFormatToDocument(document, "leada_name__", i, lb_names[i]);
		GameTool::addMemberWithFormatToDocument(document, "leada_score__", i, lb_scores[i]);
	}

	document.AddMember("share_facebook", share_facebook, document.GetAllocator());
	document.AddMember("share_twitter", share_twitter, document.GetAllocator());
	task->writeObjectives(document);
	collection->writeCollections(document);
}

void GameRecord::setLocalPlayerName(std::string& name) 
{
	lb_playername = name;
	for(int i=0; i<LB_RANK; ++i)
	{
		if( lb_names[i] == LB_DEFAULT)
		{
			lb_names[i] = name;
		}
	}
}

void GameRecord::submitScore(int score, std::string& name) 
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
	std::string tn = lb_names[j];
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

void GameRecord::submitArcadeScore(int score, std::string& name) 
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
	std::string tn = lba_names[j];
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
	 std::string documentsDirectory = paths->objectAtIndex(0);
	 std::string filename = documentsDirectory->stringByAppendingPathComponent(ICLOUDFILE);
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
