#ifndef ___CLASSES_ROLE_H_
#define ___CLASSES_ROLE_H_
#include "GameObject.h"
#include "GameTool.h"

class Role;

#define INITIAL_OFFSET 100
#define LIMITE_OFFSET 101
class RoleStateDelegate {
public:
  Role *mRole = nullptr;
  bool mAnimationIsOver = false;
  virtual void init () {};
  virtual void reset () {};

  virtual void onEnter () {};
  virtual void pre_update (float delta) ;
  virtual void update (float delta) {} ;
  virtual void after_update (float delta) ;
  virtual void onLeave () {};

  template<typename DelegateType>
    static DelegateType* CreateDelegate() {
      DelegateType* delegate = new DelegateType();
      delegate->init();
      return delegate;
    }
};

class BasicEnteringStateDelegate : public RoleStateDelegate {
public:
  virtual void onEnter ();
  virtual void update (float delta);
};

class MechanicEnteringStateDelegate : public RoleStateDelegate {
public:
  virtual void onEnter ();
  virtual void update (float delta);
};

class HighNinjaEnteringStateDelegate : public RoleStateDelegate {
public:
  virtual void onEnter ();
  virtual void update (float delta);
public:
  float mTimer;
};

class MiddleNinjaEnteringStateDelegate : public RoleStateDelegate {
public:
  virtual void onEnter ();
  virtual void update (float delta);
public:
  cocos2d::Point mTargetPos;
  float mTimer;
};

class RepositioningStateDelegate : public RoleStateDelegate {
public:
  virtual void init () { onEnter(); };

  virtual void onEnter ();
  virtual void update (float delta);

  float mTargetPos;
private:
};

class MechanicRunningStateDelegate : public RoleStateDelegate {
public:
  virtual void onEnter ();
  virtual void update (float delta);
private:
  float mTimer = 0;
};

class NinjaRunningStateDelegate : public RoleStateDelegate {
public:
  virtual void onEnter ();
  virtual void update (float delta);
  float mPollTime;
  float mAggressive;
private:
  float mTimer = 0;
};

class MPreparingStateDelegate : public RoleStateDelegate {
public:
  virtual void onEnter ();
  virtual void update (float delta);
private:
  float mTimer = 0;
};

class PreparingStateDelegate : public RoleStateDelegate {
public:
  virtual void onEnter ();
  virtual void update (float delta);
  float mPreparingTime;
private:
  float mTimer = 0;
};

class ShootStateDelegate : public RoleStateDelegate {
public:
  virtual void onEnter ();
  virtual void update (float delta);
private:
};

class HShootStateDelegate : public RoleStateDelegate {
public:
  virtual void onEnter ();
  virtual void update (float delta);
private:
  float mTimer;
  bool mFlag;
};

class MechanicShootStateDelegate : public RoleStateDelegate {
public:
  virtual void onEnter ();
  virtual void update (float delta);
private:
  float mTimer;
  int mCount = 0;
};

class MShootStateDelegate : public RoleStateDelegate {
public:
  virtual void onEnter ();
  virtual void update (float delta);
private:
  float mTimer;
  bool mFlag;
};

class FleeStateDelegate : public RoleStateDelegate {
public:
  virtual void onEnter ();
  virtual void update (float delta);
private:
};

class HDeadStateDelegate : public RoleStateDelegate {
public:
  virtual void onEnter ();
  virtual void update (float delta);
private:
  float mTimer = 0;
  bool  mFlag;
};

class DummyStateDelegate : public RoleStateDelegate {
public:
  virtual void onEnter ();
  virtual void update (float delta);
};

class DeadStateDelegate : public RoleStateDelegate {
public:
  virtual void onEnter ();
  virtual void update (float delta);
private:
  float mTimer = 0;
  bool  mFlag;
};

class Role: public GameObject {
  public:
    enum RoleState {
      Repositioning,
      Entering,
      Running,
      PreparingToShoot,
      Shooting,
      Fleeing,
      Hit,
      Dead,
      Dummy,
      Initializing,
      StateCount
    };

    RoleStateDelegate *mStateDelegate[RoleState::StateCount] = {nullptr};
    void switchStateDelegate(int state, RoleStateDelegate *delegate);
    void switchToState(RoleState state) {
      if (mStateDelegate[state] != nullptr) mStateDelegate[state]->onEnter();
      mState = state;
    }
  public:
    //与圆做碰撞检测
    virtual bool collisionWithCircle(cocos2d::Point cc, float rad) {
      if ( mState == Dead || mState == Initializing || mState == Dummy ) {
        return false;
      } else {
        for (auto & circle : mCollisionCircles) {
          Circle me = Circle(position()+circle.center, circle.radius);
          if (collide(me, Circle(cc, rad))) {
            return true;
          }
        }
      }  
      return false;
    }

    //投递伤害 伤害类型 返回伤害是否被接受
    virtual bool deliverHit(int type, cocos2d::Point dir) = 0;

    //获得坐标
    virtual cocos2d::Point position() { return mSprite->getPosition(); }
    virtual void setPosition(cocos2d::Point pos) { mSprite->setPosition(pos); }

    //获得人物中心坐标
    virtual cocos2d::Point center() = 0;

    //是否支持辅助瞄准
    virtual bool supportAimAid() = 0;

    virtual void toggleVisible(bool flag) { mSprite->setVisible(flag); }
    virtual bool isEnemy() {return false;};

    virtual const char* animationSetName() { return ""; }
    virtual void onCreate();
    virtual void onDestroy();
    virtual void onUpdate(float delta) {
      mLifeSpan += delta;
    }

    bool handleGameOver (float delta) ;

    std::vector<Circle> mCollisionCircles;
    GTAnimatedSprite *mSprite;
    cocos2d::Vec2 mAnchor = cocos2d::Vec2(0.4f, 0.0625f);
    int mState = Initializing;
    float mSpeed;
    float mLifeSpan = 0;
    int mDartCount = 0;
    int mDartCapacity = 0;
    bool mIsCoward = true;

    bool haveDart () { return mDartCount < mDartCapacity; }

    cocos2d::Node *mParent;
    template<typename RoleType>
      static Role* CreateRole(cocos2d::Node *parent) {
        Role *role = RoleType::create();
        role->mParent = parent;
        return role;
      }
};

#endif
