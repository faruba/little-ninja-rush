#include "Tasks.h"
#include "JsonWrapper.h"
#include "GameConstants.h"
#include "GameRecord.h"
#include "Collections.h"
#include "GamePlay.h"
#include "patch.h"
//#include "GameCenterController.h"

///////////////////////// ObjectiveManager
void ObjectiveManager::complete (int oid) {
  const Achievement &obj = infoWithUiid(oid);
  if (mWorkType == ACH_DAILYWORK) mTasks->dispatchTask(ACH_DAYWORK, 1);
  if ( currentObjective.index == 2 ) {
    mTasks->dispatchTask(mWorkType, 1);
  }
  mTasks->dispatchTask(ACH_COMPLETEWORKS, 1);
  GamePlay::sharedGamePlay()->pushNotification(NULL, std::string(obj.icon.c_str()), 0);
}

void ObjectiveManager::setCurrentObjectiveWithID (int id) {
  try {
    const Achievement &ach = infoWithUiid(id);

    mTasks->assignTask(ach.achieveCode, ach.achieveNumber, 0, TASK_OBJECTIVEDAILY, ach.uiid);
    currentObjective.uiid = ach.uiid;
    currentObjective.index = currentObjective.index+1;
    currentObjective.count = 0;
    currentObjective.date = time(NULL);
  } catch (char* err) {
    cocos2d::log("failed to refresh objective. (%d)", id);
  }
}
///////////////////////// ObjectiveManager

//装载数据
cocos2d::CCArray *gAchievements = NULL;//成就
cocos2d::CCArray *gStatistics = NULL;//统计
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
        ArcadePrize *ap = ArcadePrize::create();
        ap->score =gtReadInt(dic, "apz_gold_score", -1);
        ap->prize =gtReadInt(dic, "apz_gold_prize", 0);
        ap->retain();
        goldPrize = ap;
      }
      {
        ArcadePrize *ap = ArcadePrize::create();
        ap->score =gtReadInt(dic, "apz_silver_score", -1);
        ap->prize =gtReadInt(dic, "apz_silver_prize", 0);
        ap->retain();
        silverPrize = ap;
      }
      {
        ArcadePrize *ap = ArcadePrize::create();
        ap->score =gtReadInt(dic, "apz_bronze_score", -1);
        ap->prize =gtReadInt(dic, "apz_bronze_prize", 0);
        ap->retain();
        bronzePrize = ap;
      }
    }
    //载入成就记录
    cocos2d::Ref *obj = NULL;
    CCARRAY_FOREACH (gAchievements, obj)
    {
      Achievement *ach = (Achievement*)obj;
      cocos2d::CCString *key = cocos2d::CCString::createWithFormat("achievement__%d", ach->uiid);
      ach->achieveCount = gtReadInt(dic, key->getCString(), 0);
      //生成Task
      if( ach->achieveCount < ach->achieveNumber )
      {
        this->assignTask(ach->achieveCode, ach->achieveNumber, ach->achieveCount, TASK_ACHIEVEMENT, ach->uiid);
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
    //载入统计记录
    cocos2d::Ref *pObj = NULL;
    CCARRAY_FOREACH(gStatistics, pObj)
    {
      Statistics *sta = (Statistics*)pObj;
      if( sta->achieveCode >= 0 )
      {
        cocos2d::CCString *key = cocos2d::CCString::createWithFormat("statistics__%d", sta->uiid);
        sta->achieveCount = gtReadInt(dic, key->getCString(), 0);
        //生成Task
        this->assignTask(sta->achieveCode, -1, sta->achieveCount, TASK_STATISTICS, sta->uiid);
      }
    }
    gLoadFlag = true;
  }
  mRecordRead = true;
}

void Tasks::doneReadObjectives() 
{
  if( !mRecordRead )
  {
    //载入目标记录
    {
      prizeDate = 0;
      {
        ArcadePrize *pz = ArcadePrize::create();
        pz->score = -1;
        pz->prize = 0;
        goldPrize = pz;
      }
      {
        ArcadePrize *pz = ArcadePrize::create();
        pz->score = -1;
        pz->prize = 0;
        silverPrize = pz;
      }
      {
        ArcadePrize *pz = ArcadePrize::create();
        pz->score = -1;
        pz->prize = 0;
        bronzePrize = pz;
      }
      goldPrize->retain();
      silverPrize->retain();
      bronzePrize->retain();
    }
    //载入成就记录
    cocos2d::Ref *pObj = NULL;
    CCARRAY_FOREACH(gAchievements, pObj)
    {
      Achievement  *ach = (Achievement *)pObj;
      //生成Task
      if( ach->achieveCount < ach->achieveNumber )
      {
        this->assignTask(ach->achieveCode, ach->achieveNumber, 0, TASK_ACHIEVEMENT, ach->uiid);
      }
    }
    //载入统计记录
    pObj = NULL;
    CCARRAY_FOREACH(gStatistics, pObj)
    {
      Statistics *sta = (Statistics*)pObj;
      if( sta->achieveCode >= 0 )
      {
        //生成Task
        this->assignTask(sta->achieveCode, -1, 0, TASK_STATISTICS, sta->uiid);
      }
    }
  }

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
  cocos2d::Ref *obj = NULL;
  CCARRAY_FOREACH (gAchievements, obj)
  {
    Achievement *ach = (Achievement*)obj;
    const std::string key = std::string("achievement__")+std::to_string(ach->uiid);
    rapidjson::Value index(key.c_str(), key.size(), document.GetAllocator());
    document.AddMember(index, ach->achieveCount, document.GetAllocator());
  }
  //写入统计记录
  cocos2d::Ref *pObj = NULL;
  CCARRAY_FOREACH(gStatistics, pObj)
  {
    Statistics *sta = (Statistics*)pObj;
    if( sta->achieveCode >= 0 )
    {
      const std::string key = std::string("statistics__")+std::to_string(sta->uiid);
      rapidjson::Value index(key.c_str(), key.size(), document.GetAllocator());
      document.AddMember(index, sta->achieveCount, document.GetAllocator());
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
  //TODO:GamePlay::sharedGamePlay()->pushNotification(obj->text, obj->icon, 3);
}

//给参数加了引用，为什么之前没有引用，有一部分也能功能，不科学
void loadObjectivesFromFile(const char* szJson, ObjectiveManager &manager)
{
  cocos2d::CCString *path = cocos2d::CCString::createWithFormat("data/%s", szJson);
  cocos2d::CCString *str = cocos2d::CCString::createWithContentsOfFile(cocos2d::CCFileUtils::sharedFileUtils()->fullPathForFilename(path->getCString()).c_str());
  std::string data(str->getCString());
  cocos2d::CCArray *parsed = (cocos2d::CCArray*)JsonWrapper::parseJson(data);
  if (parsed) {
    cocos2d::Ref *pObj = NULL;
    CCARRAY_FOREACH(parsed, pObj)
    {
      cocos2d::CCDictionary *dic = (cocos2d::CCDictionary*)pObj; {
        Achievement ach;
        ach.uiid = gtReadInt(dic, "uiid");
        ach.icon = gtReadString(dic, "icon", "");
        ach.desc = gtReadString(dic, "desc", "");
        ach.achieveCode = gtReadInt(dic, "code");
        ach.achieveNumber = gtReadInt(dic, "cont");
        ach.prizeType = gtReadInt(dic, "priz");
        ach.prizeCount = gtReadInt(dic, "pcnt");
        ach.achieveCount = 0;//从记录中读取
        manager.mObjectives.push_back(ach);
      }
    }
  }
  else {
    cocos2d::CCLog("Error: failed to parse objectives data file %s.", szJson);
  }
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
{/*TODO:
   if( gAchievements == NULL )
   {
   gAchievements = cocos2d::CCArray::create();
   gAchievements->retain();
   }
   else
   {
   gAchievements->removeAllObjects();
   }
   cocos2d::CCString *str = cocos2d::CCString::createWithContentsOfFile(cocos2d::CCFileUtils::sharedFileUtils()->fullPathForFilename("data/achievements.json").c_str());
   cocos2d::CCArray *parsed = (cocos2d::CCArray*)JsonWrapper::parseJson(str);
   if (parsed)
   {
   cocos2d::Ref *pObj = NULL;
   CCARRAY_FOREACH(parsed, pObj)
   {
   cocos2d::CCDictionary *dic = (cocos2d::CCDictionary*)pObj;
   {
   Achievement *ach = Achievement::create();
   ach->uiid = gtReadInt(dic, "uiid");
   ach->gcid = gtReadString(dic, "gcid");
   ach->text = gtReadString(dic, "text");
   ach->name = gtReadString(dic, "name");
   ach->icon = gtReadString(dic, "icon");
   ach->desc = gtReadString(dic, "desc");
   ach->achieveCode = gtReadInt(dic, "code");
   ach->achieveNumber = gtReadInt(dic, "cont");
   ach->prizeType = gtReadInt(dic, "priz");
   ach->prizeCount = gtReadInt(dic, "pcnt");
   ach->achieveCount = 0;//从记录中读取
   gAchievements->addObject(ach);
   }
   }
   }
   else {
   cocos2d::CCLog("Error: failed to parse achievements data file %s.");
   }*/
}

void Tasks::loadStatisticsFromFile() 
{
  if( gStatistics == NULL )
  {
    gStatistics = cocos2d::CCArray::create();
    gStatistics->retain();
  }
  else
  {
    gStatistics->removeAllObjects();
  }
  /*TODO:
  //CCString *str = cocos2d::CCString::createWithContentsOfFile(cocos2d::CCFileUtils::sharedFileUtils()->fullPathFromRelativeFile("statistics.json", "data"));
  cocos2d::CCString *str = cocos2d::CCString::createWithContentsOfFile(cocos2d::CCFileUtils::sharedFileUtils()->fullPathForFilename("data/statistics.json").c_str());
  cocos2d::CCArray *parsed = (cocos2d::CCArray*)JsonWrapper::parseJson(str);
  if (parsed)
  {
  cocos2d::Ref *pObj = NULL;
  CCARRAY_FOREACH(parsed, pObj)
  {
  cocos2d::CCDictionary *dic = (cocos2d::CCDictionary*)pObj;
  {
  Statistics *sta = Statistics::create();
  sta->uiid = gtReadInt(dic, "uiid");
  sta->name = gtReadString(dic, "name");
  sta->psfx = gtReadString(dic, "psfx");
  sta->achieveCode = gtReadInt(dic, "code");
  sta->achieveCount = 0;//从记录中读取
  gStatistics->addObject(sta);
  }
  }
  }
  else {
  cocos2d::CCLog("Error: failed to load statistics file.");
  }
  */
}

Achievement* Tasks::achievementWithUiid(int uiid) 
{
  if( uiid >= 0 )
  {
    cocos2d::Ref *pObj = NULL;
    CCARRAY_FOREACH(gAchievements, pObj){
      Achievement *obj = (Achievement*)pObj;
      if( obj->uiid == uiid )
      {
        return obj;
      }
    }
  }
  return NULL;
}

Statistics* Tasks::statisticsWithUiid(int uiid) 
{
  //TODO:
//  if( uiid >= 0 )
//  {
//    cocos2d::Ref *pObj = NULL;
//    CCARRAY_FOREACH(gDailyObjectives, pObj){
//      Statistics *obj = (Statistics*)pObj;
//      if( obj->uiid == uiid )
//      {
//        return obj;
//      }
//    }
//  }
//  return NULL;
}

cocos2d::CCArray* Tasks::getStatistics() 
{
  return gStatistics;
}

cocos2d::CCArray* Tasks::getAchievements() 
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
  cocos2d::CCLog("refreshing arcade prize...");
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

cocos2d::CCString* Tasks::stringForObjective(const std::string &descr, const int achcode, const int num, const int count)
{
  cocos2d::CCString *all ;
  if( achcode < ACH_OVERLINE && num > 1 && num-count > 0 ) {
    all = cocos2d::CCString::createWithFormat("%s，还剩%d。", descr.c_str(), num-count);
  } else {
    all = cocos2d::CCString::createWithFormat(descr.c_str());
  }
  return all;
}
