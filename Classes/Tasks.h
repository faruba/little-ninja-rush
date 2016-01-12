#ifndef ___CLASSES_TASKS_H_
#define ___CLASSES_TASKS_H_
#include "GameTool.h"
#include "CCDate.h"
#include "GameConstants.h"
#include "json/rapidjson.h"
#include "json/document.h"
#include "json/filereadstream.h"
#include "JsonWrapper.h"

USING_NS_CC_EXT;

#define TASK_OBJECTIVEDAILY (0)
#define TASK_OBJECTIVEWEEKLY (1)
#define TASK_OBJECTIVEMONTHLY (2)
#define TASK_ACHIEVEMENT (3)
#define TASK_STATISTICS (4)

class Objective:public Ref
{
public:
  CCB_STATIC_NEW_AUTORELEASE_OBJECT_METHOD(Objective, create);
  virtual bool init(){return true;};
  int uiid;
  int count;
  int index;
  int date;
  
  bool operator==(int id) { return uiid == id; }
};


class Tasks;
class Record :public Ref
{
public:
  int achieveCount;
  int achieveCode;//完成代码
  int achieveNumber;//目标数据
  int uiid;
  
  void registTask(Tasks* task);
  void write(rapidjson::Document& document){
    std::string keyStr = key+"__"+std::to_string(uiid);
    rapidjson::Value index(key.c_str(), key.size(), document.GetAllocator());
    document.AddMember(index, achieveCount, document.GetAllocator());
  }
protected:
  std::string key;
  int taskCode;
  
  virtual bool isNeedAdd() = 0;
};
class Statistics : public Record
{
public:
  CCB_STATIC_NEW_AUTORELEASE_OBJECT_METHOD(Statistics, create);
  Statistics(){
    taskCode=TASK_STATISTICS;
    achieveNumber  = -1;
    key="statistics";
  }
  std::string name;
  std::string psfx;
  int achieveCode;
  void handleJsonValue(ValueWrapper& value) {
    uiid = value.getInteger("uiid");
    name = value.getString("name");
    psfx = value.getString("psfx");
    achieveCode = value.getInteger("code");
    achieveCount = 0;//从记录中读取
  }
  
protected:
  bool isNeedAdd(){ return achieveCode >= 0; }
  
};

#define TRY_READ_VALUE(NAME,FROM, TYPE) \
if(FROM.isKeyExist(#NAME)){ \
  NAME = FROM.get##TYPE(#NAME); \
}

class Achievement:public Record
{
public:
  CCB_STATIC_NEW_AUTORELEASE_OBJECT_METHOD(Achievement, create);
  Achievement(){
    taskCode=TASK_ACHIEVEMENT;
    key="achievement";
  }
  
  std::string text;
  std::string name;
  std::string icon;
  std::string desc;
  std::string gcid;
  int prizeType;
  int prizeCount;
  bool operator==(int id) { return uiid == id; }
  void handleJsonValue(ValueWrapper& value) {
    uiid = value.getInteger("uiid");
    TRY_READ_VALUE(gcid, value, String);
    TRY_READ_VALUE(text, value, String);
    TRY_READ_VALUE(name, value, String);
    icon = value.getString( "icon");
    desc = value.getString( "desc");
    achieveCode = value.getInteger( "code");
    achieveNumber = value.getInteger( "cont");
    prizeType = value.getInteger( "priz");
    prizeCount = value.getInteger( "pcnt");
    achieveCount = 0;//从记录中读取
  }
protected:
  bool isNeedAdd(){ return achieveCount < achieveNumber; }
};



class ObjectiveManager {
public:
  Objective currentObjective;
  Tasks *mTasks = nullptr;
  std::vector<Achievement> mObjectives;
  int mWorkType;
  int mTaskType;
  std::string mMark;
  
  void init (Tasks *task, int workType, std::string mark) {
    currentObjective.uiid = -1;
    mTasks = task;
    mWorkType = workType;
    mMark = std::string("obj_")+mark+"_";
    
    switch (mWorkType) {
      case ACH_DAILYWORK: mTaskType = TASK_OBJECTIVEDAILY; break;
      case ACH_WEEKLYWORK: mTaskType = TASK_OBJECTIVEWEEKLY; break;
      case ACH_MONTHLYWORK: mTaskType = TASK_OBJECTIVEMONTHLY; break;
    }
  }
  
  void write (rapidjson::Document &document) {
    GameTool::addMemberWithFormatToDocument(document, mMark, "uiid", currentObjective.uiid);
    GameTool::addMemberWithFormatToDocument(document, mMark, "count", currentObjective.count);
    GameTool::addMemberWithFormatToDocument(document, mMark, "index", currentObjective.index);
    GameTool::addMemberWithFormatToDocument(document, mMark, "date", currentObjective.date);
  }
  
  void initCurrentObjective (cocos2d::CCDictionary* dic);
  
  const Achievement &infoWithUiid (int uiid) {
//    auto result = std::find(std::begin(mObjectives), std::end(mObjectives), uiid);
//    auto result = std::find(mObjectives.begin(), mObjectives.end(), uiid);
//    if (result == std::end(mObjectives)) throw "Null";
//    return *result;
    for (Achievement& result : mObjectives){
      if (result.uiid == uiid){
        return result;
      }
    }
    throw "Null";
  }
  
  const Achievement* info () {
    try {
      return &(infoWithUiid(currentObjective.uiid));
    } catch (const char* err) {
      return NULL;
    }
  }
  
  bool isCompleted () {
    return hasObjective() &&  currentObjective.count >= infoWithUiid(currentObjective.uiid).achieveNumber;
  }
  
  bool hasObjective () {
    return currentObjective.uiid != -1;
  }
  
  void complete (int oid);
  
  void setCurrentObjectiveWithID (int id);
  
  void refresh () {
    setCurrentObjectiveWithID(randomInt(mObjectives.size()));
  }
  
  bool check () {
    bool needRefresh = false;
    if ( currentObjective.index < 0 ) {
      needRefresh = true;
    } else {
      CCDate *today = new CCDate();
      CCDate *date = new CCDate(currentObjective.date);
      
      if (mWorkType == ACH_DAILYWORK) {
        needRefresh = today->year() != date->year() || today->month() != date->month() || today->mday() != date->mday();
      } else if (mWorkType == ACH_WEEKLYWORK) {
        time_t t = date->date();
        t+=60*60*24*7;
        CCDate *after = new CCDate(t);
        needRefresh = this->isDay(date , after) || today->wday() < date->wday();
      } else {
        needRefresh = today->year() != date->year() || today->month() != date->month();
      }
      
      delete today;
      delete date;
    }
    if (needRefresh) {
      currentObjective.index = -1;
      refresh();
    }
    return needRefresh;
  }
  
  bool isDay(CCDate * a, CCDate * b) {
    if( a->year() != b->year() ) {
      return a->year() > b->year();
    } else if( a->month() != b->month() ) {
      return a->month() > b->month();
    } else {
      return a->mday() >= b->mday();
    }
  }
};


//arcade refreshes with daily task
class ArcadePrize
{
public:
  ArcadePrize(){
    reset();
  }
  void reset(){
    score = -1;
    prize = 0;
  }
  int score;
  int prize;//when prize <0 then the prize is completed
};


class Statistics;
class Tasks:public Ref
{
public:
  CCB_STATIC_NEW_AUTORELEASE_OBJECT_WITH_INIT_METHOD(Tasks, create);
  virtual bool init();
  cocos2d::CCArray *mTasks;
  bool mRecordRead;
  static Tasks* sharedTasks();
  
  void makeArcadePrize(ArcadePrize& p, int lev);
  
  bool newrefresh;
  
  ObjectiveManager dailyObjective;
  ObjectiveManager weeklyObjective;
  ObjectiveManager monthlyObjective;
  
  int prizeDate;
  ArcadePrize goldPrize;
  ArcadePrize silverPrize;
  ArcadePrize bronzePrize;
  
  void createObjectives();
  void readObjectives(cocos2d::CCDictionary* dic);
  void doneReadObjectives();
  void writeObjectives(rapidjson::Document &document);
  void mergeWith(Tasks* task);
  
  void assignTask(int achieveCode, int achieveNumber, int achieveCount, int type, int link);
  void dispatchTask(int achieveCode, int cnt);
  
  void completeAchievement(int tid);
  
  //loading list from file
  static void loadObjectivesFromFile();
  static void loadAchievementsFromFile();
  static void loadStatisticsFromFile();
  
  static Achievement* achievementWithUiid(int uiid);
  static Statistics* statisticsWithUiid(int uiid);
  
  static std::vector<Achievement>& getAchievements();
  static std::vector<Statistics>& getStatistics();
  
  //游戏启动时进行的测试
  void checkObjectives();
  bool isDay(CCDate* a, CCDate* b);
  void refreshArcadePrizes();
  static std::string stringForObjective(const std::string &desc, const int achcode, const int num, const int count);
};

class Task : public Ref
{
public:
  CCB_STATIC_NEW_AUTORELEASE_OBJECT_METHOD(Task, create);
  int achieveCode;//完成类型
  int achieved;//已完成数
  int achieveNumber;//需要完成的数量
  int taskType;//任务类型
  int taskLink;//任务编号
};


#endif
