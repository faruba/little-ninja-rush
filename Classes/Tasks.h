#ifndef ___CLASSES_TASKS_H_
#define ___CLASSES_TASKS_H_
#include "GameTool.h"
#include "CCDate.h"
#include "GameConstants.h"
#include "json/rapidjson.h"
#include "json/document.h"
#include "json/filereadstream.h"

USING_NS_CC_EXT;

#define TASK_OBJECTIVEDAILY (0)
#define TASK_OBJECTIVEWEEKLY (1)
#define TASK_OBJECTIVEMONTHLY (2)
#define TASK_ACHIEVEMENT (3)
#define TASK_STATISTICS (4)
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

class Statistics : public Ref
{
  public:
    CCB_STATIC_NEW_AUTORELEASE_OBJECT_METHOD(Statistics, create);
    int uiid;
    cocos2d::CCString *name;
    cocos2d::CCString *psfx;
    int achieveCode;
    int achieveCount;//已完成数量
};

class Achievement:public Ref
{
  public:
    CCB_STATIC_NEW_AUTORELEASE_OBJECT_METHOD(Achievement, create);
    int uiid;
    cocos2d::CCString *text;
    cocos2d::CCString *name;
    std::string icon;
    std::string desc;
    cocos2d::CCString *gcid;
    int achieveCode;//完成代码
    int achieveNumber;//目标数据
    int prizeType;
    int prizeCount;
    int achieveCount;//已完成数量
    bool operator==(int id) { return uiid == id; }
};

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


//arcade refreshes with daily task
class ArcadePrize:public Ref
{
  public:
    CREATE_FUNC(ArcadePrize);
    virtual bool init(){return true;};
    int score;
    int prize;//when prize <0 then the prize is completed
};

class Tasks ;
class ObjectiveManager {
  public:
    Objective currentObjective;
    Tasks *mTasks = nullptr;
    std::vector<Achievement> mObjectives;
    int mWorkType;
    std::string mMark;

    void init (Tasks *task, int workType, std::string mark) {
      currentObjective.uiid = -1;
      mTasks = task;
      workType = workType;
      mMark = std::string("obj_")+mark+"_";
    }

    void write (rapidjson::Document &document) {
      GameTool::addMemberWithFormatToDocument(document, mMark, "uiid", currentObjective.uiid);
      GameTool::addMemberWithFormatToDocument(document, mMark, "count", currentObjective.count);
      GameTool::addMemberWithFormatToDocument(document, mMark, "index", currentObjective.index);
      GameTool::addMemberWithFormatToDocument(document, mMark, "date", currentObjective.date);
    }

    void initCurrentObjective (cocos2d::CCDictionary* dic) {
      currentObjective.uiid = gtReadInt(dic, mMark+"uiid", -1);
      currentObjective.count = gtReadInt(dic, mMark+"count", 0);
      currentObjective.index = gtReadInt(dic, mMark+"index", -1);
      currentObjective.date = gtReadInt(dic, mMark+"date", 0);

      if (currentObjective.uiid != -1) {
        setCurrentObjectiveWithID(currentObjective.uiid);
      }
    }

    const Achievement &infoWithUiid (int uiid) {
      auto result = std::find(std::begin(mObjectives), std::end(mObjectives), uiid);
      if (result == std::end(mObjectives)) throw "Null";
      return *result;
    }

    const Achievement &info () {
      return infoWithUiid(currentObjective.uiid);
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
      if (needRefresh) refresh();
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

class Tasks:public Ref
{
  public:
    CCB_STATIC_NEW_AUTORELEASE_OBJECT_WITH_INIT_METHOD(Tasks, create);
    virtual bool init();
    cocos2d::CCArray *mTasks;
    bool mRecordRead;
    static Tasks* sharedTasks();

    void makeArcadePrize(ArcadePrize* p, int lev);

    bool newrefresh;

    ObjectiveManager dailyObjective;
    ObjectiveManager weeklyObjective;
    ObjectiveManager monthlyObjective;

    int prizeDate;
    ArcadePrize *goldPrize;
    ArcadePrize *silverPrize;
    ArcadePrize *bronzePrize;

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

    static CCArray* getStatistics();
    static CCArray* getAchievements();

    //游戏启动时进行的测试
    void checkObjectives();
    bool isDay(CCDate* a, CCDate* b);
    void refreshArcadePrizes();
    static CCString* stringForObjective(const std::string &desc, const int achcode, const int num, const int count);
};

#endif
