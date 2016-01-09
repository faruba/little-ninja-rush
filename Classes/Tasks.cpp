#include "Tasks.h"
#include "JsonWrapper.h"
#include "GameConstants.h"
#include "GameRecord.h"
#include "Collections.h"
#include "GamePlay.h"
#include "patch.h"
#include "GameData.h"
//#include "GameCenterController.h"

///////////////////////// ObjectiveManager
void ObjectiveManager::complete (int oid) {
	const Achievement &obj = infoWithUiid(oid);
	if (mWorkType == ACH_DAILYWORK) mTasks->dispatchTask(ACH_DAYWORK, 1);
	if ( currentObjective.index == 2 ) {
		mTasks->dispatchTask(mWorkType, 1);
	}
	mTasks->dispatchTask(ACH_COMPLETEWORKS, 1);
	GamePlay::sharedGamePlay()->pushNotification("", std::string(obj.icon.c_str()), 0);
}

void ObjectiveManager::setCurrentObjectiveWithID (int id) {
	try {
		const Achievement &ach = infoWithUiid(id);

		mTasks->assignTask(ach.achieveCode, ach.achieveNumber, 0, mTaskType, ach.uiid);
		currentObjective.uiid = ach.uiid;
		currentObjective.index = currentObjective.index+1;
		currentObjective.count = 0;
		currentObjective.date = time(NULL);
	} catch (char* err) {
		cocos2d::log("failed to refresh objective. (%d)", id);
	}
}

void ObjectiveManager::initCurrentObjective (cocos2d::CCDictionary* dic) {
	currentObjective.uiid = gtReadInt(dic, mMark+"uiid", -1);
	currentObjective.count = gtReadInt(dic, mMark+"count", 0);
	currentObjective.index = gtReadInt(dic, mMark+"index", -1);
	currentObjective.date = gtReadInt(dic, mMark+"date", 0);

	if (currentObjective.uiid != -1) {
		const Achievement &ach = infoWithUiid(currentObjective.uiid);
		mTasks->assignTask(ach.achieveCode, ach.achieveCount, currentObjective.count, mTaskType, currentObjective.uiid);
	}
}
///////////////////////// ObjectiveManager

//装载数据
std::vector<Achievement> gAchievements;//成就
std::vector<Statistics> gStatistics;//统计
bool gLoadFlag = false;

Tasks* Tasks::sharedTasks() 
{
	static Tasks *inst = NULL;
	if( inst == NULL )
	{
		inst = Tasks::create();
		inst->retain();
	}
	return inst;
}


bool Tasks::init() 
{
	mTasks = cocos2d::CCArray::create();
	mTasks->retain();

	dailyObjective.init(this, ACH_DAILYWORK, "daily");
	weeklyObjective.init(this, ACH_WEEKLYWORK, "weekly");
	monthlyObjective.init(this, ACH_MONTHLYWORK, "monthly");

	mRecordRead = false;

	newrefresh = false;
	return true;
}

void Tasks::createObjectives() 
{
}

void Tasks::makeArcadePrize(ArcadePrize* p, int lev) 
{
	p->score = ARCADE_PRIZEBASE + ARCADE_PRIZEFACE*lev;
	p->prize = ARCADE_PRIZEPBASE + ARCADE_PRIZEPFACE*lev;
}

void Tasks::readObjectives(cocos2d::CCDictionary* dic) 
{
	if( !gLoadFlag )
	{
		dailyObjective.initCurrentObjective(dic);
		weeklyObjective.initCurrentObjective(dic);
		monthlyObjective.initCurrentObjective(dic);

		//载入ArcadePrize
		{
			prizeDate = gtReadInt(dic, "apz_date", 0);
			{
				//ArcadePrize *ap = ArcadePrize::create();
				goldPrize->score =gtReadInt(dic, "apz_gold_score", -1);
				goldPrize->prize =gtReadInt(dic, "apz_gold_prize", 0);
				//goldPrize->retain();
				//goldPrize = ap;
			}
			{
				//ArcadePrize *ap = ArcadePrize::create();
				silverPrize->score =gtReadInt(dic, "apz_silver_score", -1);
				silverPrize->prize =gtReadInt(dic, "apz_silver_prize", 0);
				//silverPrize->retain();
				//silverPrize = ap;
			}
			{
				//ArcadePrize *ap = ArcadePrize::create();
				bronzePrize->score =gtReadInt(dic, "apz_bronze_score", -1);
				bronzePrize->prize =gtReadInt(dic, "apz_bronze_prize", 0);
				//bronzePrize->retain();
				//bronzePrize = ap;
			}
		}
		//载入成就记录

		for(Achievement& ach : gAchievements)
		{
			std::string key = std::string_format("achievement__%d", ach.uiid);
			ach.achieveCount = gtReadInt(dic, key.c_str(), 0);
			//生成Task
			if( ach.achieveCount < ach.achieveNumber )
			{
				ach.registTask(this);
			}
			//gamecenter support
			/*{
				bool shouldInGameCenter = false;
				double percent = 0;
				if( ach.achieveCode > ACH_OVERLINE )
				{//done or not done
				if( ach.achieveCount >= ach.achieveNumber )
				{
				shouldInGameCenter = true;
				percent = 100;
				}
				}
				else
				{
				if( ach.achieveCount > 0 )
				{
				shouldInGameCenter = true;
				if( ach.achieveCount >= ach.achieveNumber )
				{
				percent = 100;
				}
				else
				{
				percent = 100.0*ach.achieveCount/ach.achieveNumber;
				}
				}
				}
				}*/
		}
		//载入统计记录
		for(Statistics& sta :gStatistics)
		{
			if( sta.achieveCode >= 0 )
			{
				std::string key = std::string_format("statistics__%d", sta.uiid);
				sta.achieveCount = gtReadInt(dic, key.c_str(), 0);
				//生成Task
				sta.registTask(this);
			}
		}
		gLoadFlag = true;
	}
	mRecordRead = true;
}

void Tasks::doneReadObjectives() 
{
	//TODO if I'm right, this code will never be excuted
	//if( !mRecordRead )
	//{
	//  //载入目标记录
	//  {
	//    prizeDate = 0;
	//    {
	//      ArcadePrize *pz = ArcadePrize::create();
	//      pz->score = -1;
	//      pz->prize = 0;
	//      goldPrize = pz;
	//    }
	//    {
	//      ArcadePrize *pz = ArcadePrize::create();
	//      pz->score = -1;
	//      pz->prize = 0;
	//      silverPrize = pz;
	//    }
	//    {
	//      ArcadePrize *pz = ArcadePrize::create();
	//      pz->score = -1;
	//      pz->prize = 0;
	//      bronzePrize = pz;
	//    }
	//    goldPrize->retain();
	//    silverPrize->retain();
	//    bronzePrize->retain();
	//  }
	//  //载入成就记录
	//  cocos2d::Ref *pObj = NULL;
	//  for(Achievement& ach : gAchievements)
	//  {
	//    //生成Task
	//    if( ach.achieveCount < ach.achieveNumber )
	//    {
	//      this->assignTask(ach.achieveCode, ach.achieveNumber, 0, TASK_ACHIEVEMENT, ach.uiid);
	//    }
	//  }
	//  //载入统计记录
	//  pObj = NULL;
	//  CCARRAY_FOREACH(gStatistics, pObj)
	//  {
	//    Statistics *sta = (Statistics*)pObj;
	//    if( sta->achieveCode >= 0 )
	//    {
	//      //生成Task
	//      this->assignTask(sta->achieveCode, -1, 0, TASK_STATISTICS, sta->uiid);
	//    }
	//  }
	//}

	//#ifdef DEBUG
	//    //PRINT TASKS
	// Tasks::for(Task* t in mTasks) 
	//    {
	// Tasks::CCLog("#TASK TYPE=%d CODE=%d CNT/ACH=%d/%d", t->taskType, t->achieveCode, t->achieved, t->achieveNumber); 
	//    }
	//#endif
}

void Tasks::writeObjectives(rapidjson::Document &document) 
{
	//    //写入成就上传
	//    {
	//        dic->setObject(uploadedAchievements, "achievement_upload");
	//    }
	//写入目标记录
	dailyObjective.write(document);
	weeklyObjective.write(document);
	monthlyObjective.write(document);

	//写入奖杯信息
	{
		document.AddMember("apz_date", prizeDate, document.GetAllocator());//best for long
		document.AddMember("apz_gold_score", goldPrize->score, document.GetAllocator());
		document.AddMember("apz_gold_prize", goldPrize->prize, document.GetAllocator());
		document.AddMember("apz_silver_score", silverPrize->score, document.GetAllocator());
		document.AddMember("apz_silver_prize", silverPrize->prize, document.GetAllocator());
		document.AddMember("apz_bronze_score", bronzePrize->score, document.GetAllocator());
		document.AddMember("apz_bronze_prize", bronzePrize->prize, document.GetAllocator());
	}
	//写入成就记录
	for(Achievement& ach : gAchievements)
	{
		ach.write(document);
	}
	//写入统计记录
	for(Statistics& sta: gStatistics)
	{
		if( sta.achieveCode >= 0 )
		{
			sta.write(document);
		}
	}
}

void Tasks::assignTask(int achieveCode, int achieveNumber, int achieveCount, int type, int link) 
{
	Task *t = Task::create();
	t->achieveCode = achieveCode;
	t->achieved = achieveCount;
	t->achieveNumber = achieveNumber;
	t->taskType = type;
	t->taskLink = link;
	mTasks->addObject(t);
}

void Tasks::dispatchTask(int achieveCode, int cnt) 
{
	//tasks
	for(unsigned int i=0; i<mTasks->count(); ++i)
	{
		Task *t = (Task*)mTasks->objectAtIndex(i);
		if( t->achieveCode == achieveCode )
		{
			//1.2.0 arcade check
			if( GamePlay::sharedGamePlay()->isPlayingArcade &&
					(t->taskType == TASK_OBJECTIVEDAILY
					 || t->taskType == TASK_OBJECTIVEWEEKLY
					 || t->taskType == TASK_OBJECTIVEMONTHLY) )
			{
				//Arcade模式下，不完成常规任务
				continue;
			}

			bool completed = false;
			if( achieveCode > ACH_OVERLINE )//超越型任务
			{
				if( cnt >= t->achieveNumber )
				{
					t->achieved = cnt;
					completed = true;
				}
			}
			else {
				t->achieved += cnt;
				if( t->achieved >= t->achieveNumber && t->achieveNumber > 0 )//注意：统计项目的achieveNumber为-1
				{
					completed = true;
				}
			}
#ifdef DEBUG
			if( t->taskType <= TASK_OBJECTIVEMONTHLY )
			{
				cocos2d::CCLog("Objective%d (%d/%d)", t->taskType, t->achieved, t->achieveNumber);
			}
#endif
			//回溯保存
			if( t->taskType == TASK_STATISTICS )
			{
				Statistics *sta = Tasks::statisticsWithUiid(t->taskLink);
				sta->achieveCount+=cnt;
			}
			if( t->taskType == TASK_OBJECTIVEDAILY )
			{
				dailyObjective.currentObjective.count = t->achieved;
			}
			if( t->taskType == TASK_OBJECTIVEWEEKLY )
			{
				weeklyObjective.currentObjective.count = t->achieved;
			}
			if( t->taskType == TASK_OBJECTIVEMONTHLY )
			{
				monthlyObjective.currentObjective.count = t->achieved;
			}
			if( t->taskType == TASK_ACHIEVEMENT )
			{
				Achievement *ach = Tasks::achievementWithUiid(t->taskLink);
				if( achieveCode < ACH_OVERLINE )
				{
					ach->achieveCount +=cnt;
				}
				else
				{
					ach->achieveCount = cnt;
				}
				//gamecenter support
				{
					bool shouldInGameCenter = false;
					double percent = 0;
					if( ach->achieveCode > ACH_OVERLINE )
					{//done or not done
						if( ach->achieveCount >= ach->achieveNumber )
						{
							shouldInGameCenter = true;
							percent = 100;
						}
					}
					else
					{
						if( ach->achieveCount > 0 )
						{
							shouldInGameCenter = true;
							if( ach->achieveCount >= ach->achieveNumber )
							{
								percent = 100;
							}
							else
							{
								percent = 100.0*ach->achieveCount/ach->achieveNumber;
							}
						}
					}
				}
			}
			if( completed )
			{
				i--;
				switch (t->taskType) {
					case TASK_OBJECTIVEDAILY:
						{
							this->dailyObjective.complete(t->taskLink);
						}
						break;
					case TASK_OBJECTIVEWEEKLY:
						{
							this->weeklyObjective.complete(t->taskLink);
						}
						break;
					case TASK_OBJECTIVEMONTHLY:
						{
							this->monthlyObjective.complete(t->taskLink);
						}
						break;
					case TASK_ACHIEVEMENT:
						{
							this->completeAchievement(t->taskLink);
						}
						break;
				}
				mTasks->removeObject(t);
			}
		}
	}
}

void Tasks::completeAchievement(int tid) 
{
	Achievement *obj = Tasks::achievementWithUiid(tid);
	GamePlay::sharedGamePlay()->pushNotification(obj->text, obj->icon, 3);
}

//给参数加了引用，为什么之前没有引用，有一部分也能功能，不科学
void loadObjectivesFromFile(const char* szJson, ObjectiveManager &manager)
{

	std::string path = std::string_format("data/%s", szJson);
	loadVectorFromJsonFile(path.c_str(), manager.mObjectives);

}

void Tasks::loadObjectivesFromFile() 
{
	//load daily
	::loadObjectivesFromFile("dailyobj.json", sharedTasks()->dailyObjective);
	//load weekly
	::loadObjectivesFromFile("weeklyobj.json", sharedTasks()->weeklyObjective);
	//load monthly
	::loadObjectivesFromFile("monthlyobj.json", sharedTasks()->monthlyObjective);
}

void Tasks::loadAchievementsFromFile() 
{
	gAchievements.clear();
	loadVectorFromJsonFile("data/achievements.json",gAchievements);
}

void Tasks::loadStatisticsFromFile() 
{
	gStatistics.clear();
	loadVectorFromJsonFile("data/statistics.json",gStatistics);
}

template <typename T>
T* findByUiid(int uiid, std::vector<T>& vec) {
	if( uiid >= 0 )
	{
		typename std::vector<T>::iterator it = std::find_if(vec.begin(),vec.end(),[&](T& t){
				return t.uiid == uiid;
				});
		if(it != vec.end()){
			return it.operator->();
		}
	}
	return NULL;
}
Achievement* Tasks::achievementWithUiid(int uiid)
{
	return findByUiid(uiid, gAchievements);
}

Statistics* Tasks::statisticsWithUiid(int uiid) 
{
	return findByUiid(uiid, gStatistics);
}

std::vector<Statistics>& Tasks::getStatistics()
{
	return gStatistics;
}

std::vector<Achievement>& Tasks::getAchievements()
{
	return gAchievements;
}

//启动
void Tasks::checkObjectives() 
{
	CCDate *today = new CCDate();
	//refresh arcade prize
	if( prizeDate == 0 )
	{
		this->refreshArcadePrizes();
		newrefresh = true;
	}
	else {
		CCDate *date = new CCDate(prizeDate);
		if( today->year() != date->year() || today->month() != date->month()
				|| today->mday() != date->mday() )
		{
			this->refreshArcadePrizes();
			newrefresh = true;
		}
	}
	// 任务是否需要刷新
	newrefresh = dailyObjective.check() ? true: newrefresh;
	newrefresh = weeklyObjective.check() ? true: newrefresh;
	newrefresh = monthlyObjective.check() ? true: newrefresh;

	delete today;
	today = NULL;
}

void Tasks::refreshArcadePrizes() 
{
	if (goldPrize == NULL){
		return;
	}
	if( goldPrize->prize >= 0 )
	{
		//stop golden steak
		GameRecord::sharedGameRecord()->golden_steak = 0;
	}
	int level = randomInt(ARCADE_PRIZELEVEL);
	this->makeArcadePrize(goldPrize, level);
	this->makeArcadePrize(silverPrize, level-ARCADE_PRIZEMINUS);
	this->makeArcadePrize(bronzePrize, level-ARCADE_PRIZEMINUS*2);
	prizeDate = time(NULL);
}


void Tasks::mergeWith(Tasks* task) 
{
	//do not merge task
}

std::string Tasks::stringForObjective(const std::string &descr, const int achcode, const int num, const int count)
{
	std::string all ;
	if( achcode < ACH_OVERLINE && num > 1 && num-count > 0 ) {
		all = std::string_format("%s还剩%d。", descr.c_str(), num-count);
	} else {
		all = std::string_format(descr.c_str());
	}
	return all;
}

void Record::registTask(Tasks *task){
	if(isNeedAdd()){
		task->assignTask(achieveCode, achieveNumber, 0, taskCode, uiid);
	}
}
