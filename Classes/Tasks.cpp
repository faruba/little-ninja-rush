#include "Tasks.h"
#include "JsonWrapper.h"
#include "GameConstants.h"
#include "GameRecord.h"
#include "Collections.h"
#include "GamePlay.h"
//#include "GameCenterController.h"


//装载数据
cocos2d::CCArray *gDailyObjectives = NULL;//日常任务
cocos2d::CCArray *gWeeklyObjectives = NULL;//周常任务
cocos2d::CCArray *gMonthlyObjectives = NULL;//月常任务
cocos2d::CCArray *gAchievements = NULL;//成就
cocos2d::CCArray *gStatistics = NULL;//统计
bool gLoadFlag = false;
class UploadAchievement :
  public Ref
{
  public:
    cocos2d::CCString *gcid;
    double percent;
    UploadAchievement* upload(cocos2d::CCString* gid, double perc);
};
/*
   UploadAchievement* UploadAchievement::upload(cocos2d::CCString * gid, double perc) 
   {
   UploadAchievement *ua = UploadAchievement->alloc()->init()->autorelease();
   ua.gcid = gid;
   ua.percent = perc;
   return ua;
   }

   void UploadAchievement::encodeWithCoder(NSCoder * aCoder) 
   {
   aCoder->encodeObject(gcid, "gcid");
   aCoder->encodeDouble(percent, "percent");
   }
   id UploadAchievement::initWithCoder(NSCoder * aDecoder) 
   {
   this->init();
   gcid = aDecoder->decodeObjectForKey("gcid");
   percent = aDecoder->decodeDoubleForKey("percent");
   return this;
   }
   void UploadAchievement::dealloc() 
   {
   gcid->release();
   super->dealloc();
   }
   */





Tasks* Tasks::sharedTasks() 
{
  static Tasks *inst = NULL;
  if( inst == NULL )
  {
    inst = Tasks::create();
  }
  return inst;
}


bool Tasks::init() 
{
  mTasks = cocos2d::CCArray::create();
  mTasks->retain();
  dailyObjective = NULL;
  weeklyObjective = NULL;
  monthlyObjective = NULL;

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
    //载入目标记录
    {
      cocos2d::CCLog("readObjectives");
      dailyObjective = Objective::create();
      dailyObjective->retain();
      dailyObjective->uiid = gtReadInt(dic, "obj_daily_uiid", -1);
      dailyObjective->count = gtReadInt(dic, "obj_daily_count", 0);
      dailyObjective->index = gtReadInt(dic, "obj_daily_index", -1);
      dailyObjective->date = gtReadInt(dic, "obj_daily_date", 0);
      //生成Task
      Achievement *ach = Tasks::dailyObjectiveWithUiid(dailyObjective->uiid);
      if( ach != NULL )
      {
        this->assignTask(ach->achieveCode, ach->achieveNumber, dailyObjective->count, TASK_OBJECTIVEDAILY, dailyObjective->uiid);
      }
    }
    {
      weeklyObjective = Objective::create();
      weeklyObjective->retain();
      weeklyObjective->uiid = gtReadInt(dic, "obj_weekly_uiid", -1);
      weeklyObjective->count = gtReadInt(dic, "obj_weekly_count", 0);
      weeklyObjective->index = gtReadInt(dic, "obj_weekly_index", -1);
      weeklyObjective->date = gtReadInt(dic, "obj_weekly_date", 0);
      //生成Task
      Achievement *ach = Tasks::weeklyObjectiveWithUiid(weeklyObjective->uiid);
      if( ach != NULL )
      {
        this->assignTask(ach->achieveCode, ach->achieveNumber, weeklyObjective->count, TASK_OBJECTIVEWEEKLY, weeklyObjective->uiid);
      }
    }
    {
      monthlyObjective = Objective::create();
      monthlyObjective->retain();
      monthlyObjective->uiid = gtReadInt(dic, "obj_monthly_uiid", -1);
      monthlyObjective->count = gtReadInt(dic, "obj_monthly_count", 0);
      monthlyObjective->index = gtReadInt(dic, "obj_monthly_index", -1);
      monthlyObjective->date = gtReadInt(dic, "obj_monthly_date", 0);
      //生成Task
      Achievement *ach = Tasks::monthlyObjectiveWithUiid(monthlyObjective->uiid);
      if( ach != NULL )
      {
        this->assignTask(ach->achieveCode, ach->achieveNumber, monthlyObjective->count, TASK_OBJECTIVEMONTHLY, monthlyObjective->uiid);
      }
    }
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
      dailyObjective = Objective::create();
      dailyObjective->retain();
      dailyObjective->uiid = -1;
      dailyObjective->count = 0;
      dailyObjective->index = -1;
      dailyObjective->date = 0;
      cocos2d::CCLog("doneReadObjectives");
    }
    {
      weeklyObjective = Objective::create();
      weeklyObjective->retain();
      weeklyObjective->uiid = -1;
      weeklyObjective->count = 0;
      weeklyObjective->index = -1;
      weeklyObjective->date = 0;
    }
    {
      monthlyObjective = Objective::create();
      monthlyObjective->retain();
      monthlyObjective->uiid = -1;
      monthlyObjective->count = 0;
      monthlyObjective->index = -1;
      monthlyObjective->date = 0;
    }
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
  {
    document.AddMember("obj_daily_uiid", dailyObjective->uiid, document.GetAllocator());
    document.AddMember("obj_daily_count", dailyObjective->count, document.GetAllocator());
    document.AddMember("obj_daily_index", dailyObjective->index, document.GetAllocator());
    document.AddMember("obj_daily_date", dailyObjective->date, document.GetAllocator());//best for long
  }
  {
    document.AddMember("obj_weekly_uiid", weeklyObjective->uiid, document.GetAllocator());
    document.AddMember("obj_weekly_count", weeklyObjective->count, document.GetAllocator());
    document.AddMember("obj_weekly_index", weeklyObjective->index, document.GetAllocator());
    document.AddMember("obj_weekly_date", weeklyObjective->date, document.GetAllocator());//best for long
  }
  {
    document.AddMember("obj_monthly_uiid", monthlyObjective->uiid, document.GetAllocator());
    document.AddMember("obj_monthly_count", monthlyObjective->count, document.GetAllocator());
    document.AddMember("obj_monthly_index", monthlyObjective->index, document.GetAllocator());
    document.AddMember("obj_monthly_date", monthlyObjective->date, document.GetAllocator());//best for long
  }
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
        dailyObjective->count = t->achieved;
      }
      if( t->taskType == TASK_OBJECTIVEWEEKLY )
      {
        weeklyObjective->count = t->achieved;
      }
      if( t->taskType == TASK_OBJECTIVEMONTHLY )
      {
        monthlyObjective->count = t->achieved;
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
              this->completeObjectiveDaily(t->taskLink);
            }
            break;
          case TASK_OBJECTIVEWEEKLY:
            {
              this->completeObjectiveWeekly(t->taskLink);
            }
            break;
          case TASK_OBJECTIVEMONTHLY:
            {
              this->completeObjectiveMonthly(t->taskLink);
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
  cocos2d::CCLog("!!! Achievement Completed!");
  Achievement *obj = Tasks::achievementWithUiid(tid);
  //TODO:GamePlay::sharedGamePlay()->pushNotification(obj->text, obj->icon, 3);
}

void Tasks::completeObjectiveDaily(int oid) 
{
  cocos2d::CCLog("!!! Daily Objective Completed! (Step%d)", dailyObjective->index+1);
  Achievement *obj = Tasks::dailyObjectiveWithUiid(oid);
  GameRecord::sharedGameRecord()->task->dispatchTask(ACH_DAYWORK, 1);
  if( dailyObjective->index == 2 )
  {
    GameRecord::sharedGameRecord()->task->dispatchTask(ACH_DAILYWORK, 1);
  }
  GameRecord::sharedGameRecord()->task->dispatchTask(ACH_COMPLETEWORKS, 1);
  //GamePlay::taskCompleted(NULL, obj->icon, 0);
  //TODO:GamePlay::sharedGamePlay()->pushNotification(NULL, obj->icon, 0);
}

void Tasks::completeObjectiveWeekly(int oid) 
{
  cocos2d::CCLog("!!! Weekly Objective Completed! (Step%d)", weeklyObjective->index+1);
  Achievement *obj = Tasks::weeklyObjectiveWithUiid(oid);
  if( weeklyObjective->index == 2 )
  {
    GameRecord::sharedGameRecord()->task->dispatchTask(ACH_WEEKLYWORK, 1);
  }
  GameRecord::sharedGameRecord()->task->dispatchTask(ACH_COMPLETEWORKS, 1);
  //GamePlay::taskCompleted(NULL, obj->icon, 1);
  //TODO:GamePlay::sharedGamePlay()->pushNotification(NULL, obj->icon, 1);
}

void Tasks::completeObjectiveMonthly(int oid) 
{
  cocos2d::CCLog("!!! Monthly Objective Completed! (Step%d)", monthlyObjective->index+1);
  Achievement *obj = Tasks::monthlyObjectiveWithUiid(oid);
  if( monthlyObjective->index == 2 )
  {
    GameRecord::sharedGameRecord()->task->dispatchTask(ACH_MONTHLYWORK, 1);
  }
  GameRecord::sharedGameRecord()->task->dispatchTask(ACH_COMPLETEWORKS, 1);
  //GamePlay::taskCompleted(NULL, obj->icon, 2);
  //TODO:GamePlay::sharedGamePlay()->pushNotification(NULL, obj->icon, 2);
}

//给参数加了引用，为什么之前没有引用，有一部分也能功能，不科学
void loadObjectivesFromFile(const char* szJson, CCArray* &pArr)
{
  if( pArr == NULL )
  {
    pArr = cocos2d::CCArray::create();
    pArr->retain();
  }
  else
  {
    pArr->removeAllObjects();
  }
  cocos2d::CCString *path = cocos2d::CCString::createWithFormat("data/%s", szJson);
  cocos2d::CCString *str = cocos2d::CCString::createWithContentsOfFile(cocos2d::CCFileUtils::sharedFileUtils()->fullPathForFilename(path->getCString()).c_str());
  /*TODO:
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
    ach->icon = gtReadString(dic, "icon");
    ach->desc = gtReadString(dic, "desc");
    ach->achieveCode = gtReadInt(dic, "code");
    ach->achieveNumber = gtReadInt(dic, "cont");
    ach->prizeType = gtReadInt(dic, "priz");
    ach->prizeCount = gtReadInt(dic, "pcnt");
    ach->achieveCount = 0;//从记录中读取
    pArr->addObject(ach);
    }
    }
    }
    else {
    cocos2d::CCLog("Error: failed to parse objectives data file %s.", szJson);
    }
    */
}

void Tasks::loadObjectivesFromFile() 
{
  //load daily
  ::loadObjectivesFromFile("dailyobj.json", gDailyObjectives);
  //load weekly
  ::loadObjectivesFromFile("weeklyobj.json", gWeeklyObjectives);
  //load monthly
  ::loadObjectivesFromFile("monthlyobj.json", gMonthlyObjectives);
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

Achievement* Tasks::dailyObjectiveWithUiid(int uiid) 
{
  if( uiid >= 0 )
  {
    cocos2d::Ref *pObj = NULL;
    CCARRAY_FOREACH(gDailyObjectives, pObj){
      Achievement *obj = (Achievement*)pObj;
      if( obj->uiid == uiid )
      {
        return obj;
      }
    }
  }
  return NULL;
}

Achievement* Tasks::weeklyObjectiveWithUiid(int uiid) 
{
  if( uiid >= 0 )
  {
    cocos2d::Ref *pObj = NULL;
    CCARRAY_FOREACH(gWeeklyObjectives, pObj){
      Achievement *obj = (Achievement*)pObj;
      if( obj->uiid == uiid )
      {
        return obj;
      }
    }
  }
  return NULL;
}

Achievement* Tasks::monthlyObjectiveWithUiid(int uiid) 
{
  if( uiid >= 0 )
  {
    cocos2d::Ref *pObj = NULL;
    CCARRAY_FOREACH(gMonthlyObjectives, pObj){
      Achievement *obj = (Achievement*)pObj;
      if( obj->uiid == uiid )
      {
        return obj;
      }
    }
  }
  return NULL;
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
  if( uiid >= 0 )
  {
    cocos2d::Ref *pObj = NULL;
    CCARRAY_FOREACH(gDailyObjectives, pObj){
      Statistics *obj = (Statistics*)pObj;
      if( obj->uiid == uiid )
      {
        return obj;
      }
    }
  }
  return NULL;
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
{/*TODO:
   cocos2d::CCDate *today = new CCDate();
  //refresh arcade prize
  if( prizeDate == 0 )
  {
  this->refreshArcadePrizes();
  newrefresh = true;
  }
  else {
  cocos2d::CCDate *date = new CCDate(prizeDate);
  cocos2d::CCLOG("ARCADE PRIZE = %d-%d-%d %d:%d", date->year(), date->month(), date->mday(), date->hour(), date->min());
  if( today->year() != date->year() || today->month() != date->month()
  || today->mday() != date->mday() )
  {
  this->refreshArcadePrizes();
  newrefresh = true;
  }
  }
  //判断日任务是否需要刷新
  if( dailyObjective->index < 0 )
  {
  this->refreshDailyObjective();
  newrefresh = true;
  }
  else {
  cocos2d::CCDate *date = new CCDate(dailyObjective->date);
  cocos2d::CCLOG("DAILY PRIZE = %d-%d-%d %d:%d", date->year(), date->month(), date->mday(), date->hour(), date->min());
  if( today->year() != date->year() || today->month() != date->month()
  || today->mday() != date->mday() )
  {
  //remove previous task
  for(int i=0; i<mTasks->count(); ++i)
  {
  Task *t = (Task*)mTasks->objectAtIndex(i);
  if( t->taskType == TASK_OBJECTIVEDAILY )
  {
  mTasks->removeObjectAtIndex(i);
  i--;
  }
  }

  dailyObjective->index = -1;
  this->refreshDailyObjective();
  newrefresh = true;
  }
  delete date;
  date = NULL;
  }
  //判断周任务是否需要刷新
  if( weeklyObjective->index < 0 )
  {
  this->refreshWeeklyObjective();
  newrefresh = true;
  }
  else {
  cocos2d::CCDate *date = new CCDate(weeklyObjective->date);
  cocos2d::CCLOG("WEEKLY PRIZE = %d-%d-%d %d:%d", date->year(), date->month(), date->mday(), date->hour(), date->min());
  time_t t = date->date();
  t+=60*60*24*7;
  cocos2d::CCDate *after = new CCDate(t);
  if( this->isDay(date , after) || today->wday() < date->wday() )
  {
  //remove previous task
  for(int i=0; i<mTasks->count(); ++i)
  {
  Task *t = (Task*)mTasks->objectAtIndex(i);
  if( t->taskType == TASK_OBJECTIVEWEEKLY )
  {
  mTasks->removeObjectAtIndex(i);
  i--;
  }
}

weeklyObjective->index = -1;
this->refreshWeeklyObjective();
newrefresh = true;
}
delete date;
delete after;
date = NULL;
after = NULL;
}
//判断月任务是否需要刷新
if( monthlyObjective->index < 0 )
{
  this->refreshMonthlyObjective();
  newrefresh = true;
}
else {
  cocos2d::CCDate *date = new CCDate(monthlyObjective->date);
  cocos2d::CCLOG("MONTHLY PRIZE = %d-%d-%d %d:%d", date->year(), date->month(), date->mday(), date->hour(), date->min());
  if( today->year() != date->year() || today->month() != date->month() )
  {
    //remove previous task
    for(int i=0; i<mTasks->count(); ++i)
    {
      Task *t = (Task*)mTasks->objectAtIndex(i);
      if( t->taskType == TASK_OBJECTIVEMONTHLY )
      {
        mTasks->removeObjectAtIndex(i);
        i--;
      }
    }

    monthlyObjective->index = -1;
    this->refreshMonthlyObjective();
    newrefresh = true;
  }
  delete date;
  date = NULL;
}

delete today;
today = NULL;*/
}

bool Tasks::isDay(CCDate * a, CCDate * b)
{
  if( a->year() != b->year() )
  {
    if( a->year() > b->year() )
    {
      return true;
    }
    else {
      return false;
    }
  }
  else if( a->month() != b->month() )
  {
    if( a->month() > b->month() )
    {
      return true;
    }
    else {
      return false;
    }
  }
  else {
    if( a->mday() >= b->mday() )
    {
      return true;
    }
    else {
      return false;
    }
  }
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

void Tasks::refreshDailyObjective() 
{
  cocos2d::CCLog("refreshing daily objective...");
  cocos2d::CCLog("CURR TASK = %d", dailyObjective->uiid);
  int uiid = randomInt(gDailyObjectives->count());
  cocos2d::CCLog("NEW TASK = %d", uiid);
  //生成Task
  Achievement *ach = Tasks::dailyObjectiveWithUiid(uiid);
  if( ach != NULL )
  {
    this->assignTask(ach->achieveCode, ach->achieveNumber, 0, TASK_OBJECTIVEDAILY, uiid);
    dailyObjective->uiid = ach->uiid;
    dailyObjective->index = dailyObjective->index+1;
    dailyObjective->count = 0;
    dailyObjective->date = time(NULL);
  }
  else {
    cocos2d::CCLog("failed to refresh daily objective. (%d)", uiid);
  }
}

void Tasks::refreshWeeklyObjective() 
{
  cocos2d::CCLog("refreshing weekly objective...");
  int uiid = randomInt(gWeeklyObjectives->count());
  //生成Task
  Achievement *ach = Tasks::weeklyObjectiveWithUiid(uiid);
  if( ach != NULL )
  {
    this->assignTask(ach->achieveCode, ach->achieveNumber, 0, TASK_OBJECTIVEWEEKLY, uiid);
    weeklyObjective->uiid = ach->uiid;
    weeklyObjective->index = weeklyObjective->index+1;
    weeklyObjective->count = 0;
    weeklyObjective->date = time(NULL);
  }
  else {
    cocos2d::CCLog("failed to refresh weekly objective. (%d)", uiid);
  }
}

void Tasks::refreshMonthlyObjective() 
{
  cocos2d::CCLog("refreshing monthly objective...");
  int uiid = randomInt(gMonthlyObjectives->count());
  //生成Task
  Achievement *ach = Tasks::monthlyObjectiveWithUiid(uiid);
  if( ach != NULL )
  {
    this->assignTask(ach->achieveCode, ach->achieveNumber, 0, TASK_OBJECTIVEMONTHLY, uiid);
    monthlyObjective->uiid = ach->uiid;
    monthlyObjective->index = monthlyObjective->index+1;
    monthlyObjective->count = 0;
    monthlyObjective->date = time(NULL);
  }
  else {
    cocos2d::CCLog("failed to refresh monthly objective. (%d)", uiid);
  }
}

void Tasks::mergeWith(Tasks* task) 
{
  //do not merge task
}
/*
   void Tasks::dealloc() 
   {
   mTasks->removeAllObjects();
   mTasks->release();
   dailyObjective->release();
   dailyObjective = NULL;
   weeklyObjective->release();
   weeklyObjective = NULL;
   monthlyObjective->release();
   monthlyObjective = NULL;
   goldPrize->release();
   silverPrize->release();
   bronzePrize->release();
   uploadedAchievements->release();
   uploadingAchievements->release();
   super->dealloc();
   }
   */

cocos2d::CCString* Tasks::stringForObjective(cocos2d::CCString * desc, int achcode, int num, int count)
{
  cocos2d::CCString *all = desc;
  if( achcode < ACH_OVERLINE && num > 1 && num-count > 0 )
  {
    all = cocos2d::CCString::createWithFormat("%s，还剩%d。", desc->getCString(), num-count);
  }
  return all;
}
