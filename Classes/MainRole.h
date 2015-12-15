#ifndef ___CLASSES_MAINROLE_H_
#define ___CLASSES_MAINROLE_H_
#include "GameTool.h"
#include "Role.h"

class Vector2d:public CCNode
{
  public:
    virtual bool init() {return true;};
    CREATE_FUNC(Vector2d);
    cocos2d::Point value;
};

class MainRole:
  public Role
{
  public:
    virtual bool init() {return true;};
    CREATE_FUNC(MainRole);
    virtual void onCreate();
    virtual void onUpdate(float delta);
    virtual void onDestroy();

    void onHitClassic(int type);
    void onHitArcade(int type);
    CCSprite* sprite();

    CCNode *mParent;
    GTAnimatedSprite *mSprite;
    int     mRoleId;
    int     mMoveFlag;
    float   mMoveSpeed;
    float   mSpeed;
    float   mInvincibleTimer;
    float   mLoadTimer;
    float   mBladeTimer;
    float   mDrainAdder;
    int     mAI;
    cocos2d::Point mAIPos;
    bool    mInvincible;
    //计次开关
    int     mVisibleCounter;
    int     mShadowCounter;
    int     mBlinkCounter;
    float   mBlinkTimer;

    float   mDashTimer;

    float   mScheduleUnlimitedDarts;
    //武藏专用
    float mBladeTimer2;
    bool  mBladeAction;
    //Mr.Pussy专用
    float mRecoveryTimer;

    GTAnimatedSprite *mBlade;
    GTGhostShadow *mGhost;

    //CCLock mFireLock;


    float mStepTimer;
    float mFootPrintTimer;

    bool mHiddenPower;

    float SPDrain;
    int HP;//当前hp
    int maxHP;//最大hp
    int SP;//当前sp
    int maxSP;//最大sp
    int activeSP;//sp消耗
    int dart;//飞镖
    int maxDart;//最大飞镖数
    float runSpeed;//奔跑速度
    int bladeCount;//刀数
    int dartEffect;//飞镖特效
    int bladeAnim;//刀特效
    int bladeEffect;
    int spellType;//技能类型
    float bladeCD;
    float bladeRange;
    float dartCD;
    float dartSpeed;
    CCString *dartShap;
    CCString *bladeSound;
    bool flag_dart;
    bool flag_blade;
    bool flag_spell;
    bool flag_move;

    static MainRole* role(cocos2d::CCNode* parent);

    void setEquipDart(int tid);
    void setEquipBlade(int tid);
    void setEquipSpell(int tid);

    //设置角色AI
    void setAI(int ai, cocos2d::Point pos);
    //发射飞镖
    void fire(cocos2d::Point dir);
    //结算飞镖
    void commitFire(cocos2d::CCNode* param, CCNode* pdata);
    //拔刀
    void slice();
    //结算拔刀
    void commitSlice();
    //使用技能
    void spell(cocos2d::Point param);
    //结算技能
    void commitSpell(Vector2d* dir);
    //设置移动标记
    void setTilt(float dir);
    //开启或关闭阴影
    void toggleShadow(bool flag);

    void toggleInvincible(bool flag);
    //计时无敌
    void timedInvincible(float time);

    void timedUnlimitedDarts(float time);

    void toggleBlink(bool flag);

    void customAction(int key);

    void setShadowSpeed(float speed);

    void attachEffect(const char* aniset, int aniid, bool loop, bool toroot);

    void gainSP(int sp);

    //取消分身术
    bool cancelMain2();

    virtual bool collisionWithCircle(cocos2d::Point cc, float rad);
    virtual bool deliverHit(int type, cocos2d::Point dir);
    virtual cocos2d::Point position();
    virtual void setPosition(cocos2d::Point pos);
    virtual cocos2d::Point center();
    virtual bool supportAimAid();
    virtual void toggleVisible(bool flag);

};
#endif
