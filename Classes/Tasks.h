#ifndef ___CLASSES_TASKS_H_
#define ___CLASSES_TASKS_H_
#include "GameTool.h"
#include "CCDate.h"

;
USING_NS_CC_EXT;

#define TASK_OBJECTIVEDAILY (0)
#define TASK_OBJECTIVEWEEKLY (1)
#define TASK_OBJECTIVEMONTHLY (2)
#define TASK_ACHIEVEMENT (3)
#define TASK_STATISTICS (4)
class Task : public CCObject
{
  public:
CCB_STATIC_NEW_AUTORELEASE_OBJECT_METHOD(Task, create);
    int achieveCode;//完成类型
    int achieved;//已完成数
    int achieveNumber;//需要完成的数量
    int taskType;//任务类型
    int taskLink;//任务编号
};

class Statistics : public CCObject
{
  public:
CCB_STATIC_NEW_AUTORELEASE_OBJECT_METHOD(Statistics, create);
    int uiid;
cocos2d::CCString *name;
cocos2d::CCString *psfx;
    int achieveCode;
    int achieveCount;//已完成数量
};

class Achievement:public CCObject
{
  public:
CCB_STATIC_NEW_AUTORELEASE_OBJECT_METHOD(Achievement, create);
    int uiid;
cocos2d::CCString *text;
cocos2d::CCString *name;
cocos2d::CCString *icon;
cocos2d::CCString *desc;
cocos2d::CCString *gcid;
    int achieveCode;//完成代码
    int achieveNumber;//目标数据
    int prizeType;
    int prizeCount;
    int achieveCount;//已完成数量
};

class Objective:public CCObject
{
  public:
CCB_STATIC_NEW_AUTORELEASE_OBJECT_METHOD(Objective, create);
    virtual bool init(){return true;};
    int uiid;
    int count;
    int index;
    //time_t date;
    int date;
};

//arcade refreshes with daily task
class ArcadePrize:public CCObject
{
  public:
    CREATE_FUNC(ArcadePrize);
    virtual bool init(){return true;};
    int score;
    int prize;//when prize <0 then the prize is completed
};

class Tasks:public CCObject
{
  public:
CCB_STATIC_NEW_AUTORELEASE_OBJECT_WITH_INIT_METHOD(Tasks, create);
    virtual bool init();
cocos2d::CCArray *mTasks;
    bool mRecordRead;
    static Tasks* sharedTasks();

    void makeArcadePrize(ArcadePrize* p, int lev);

    bool newrefresh;

    Objective *dailyObjective;
    Objective *weeklyObjective;
    Objective *monthlyObjective;
    //time_t prizeDate;
    int prizeDate;
    ArcadePrize *goldPrize;
    ArcadePrize *silverPrize;
    ArcadePrize *bronzePrize;
    //CCArray *uploadedAchievements;
    //CCArray *uploadingAchievements;

    void createObjectives();
    void readObjectives(cocos2d::CCDictionary* dic);
    void doneReadObjectives();
    void writeObjectives(rapidjson::Document &document);
    void mergeWith(Tasks* task);

    void assignTask(int achieveCode, int achieveNumber, int achieveCount, int type, int link);
    void dispatchTask(int achieveCode, int cnt);

    void completeAchievement(int tid);
    void completeObjectiveDaily(int oid);
    void completeObjectiveWeekly(int oid);
    void completeObjectiveMonthly(int oid);

    //loading list from file
    static void loadObjectivesFromFile();
    static void loadAchievementsFromFile();
    static void loadStatisticsFromFile();


    static Achievement* dailyObjectiveWithUiid(int uiid);
    static Achievement* weeklyObjectiveWithUiid(int uiid);
    static Achievement* monthlyObjectiveWithUiid(int uiid);
    static Achievement* achievementWithUiid(int uiid);
    static Statistics* statisticsWithUiid(int uiid);

    static CCArray* getStatistics();
    static CCArray* getAchievements();

    //游戏启动时进行的测试
    void checkObjectives();
    bool isDay(CCDate* a, CCDate* b);
    void refreshArcadePrizes();
    void refreshDailyObjective();
    void refreshWeeklyObjective();
    void refreshMonthlyObjective();
    static CCString* stringForObjective(cocos2d::CCString* desc, int achcode, int num, int count);
};



#endif
