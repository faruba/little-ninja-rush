#ifndef ___CLASSES_ROLE_H_
#define ___CLASSES_ROLE_H_
#include "GameObject.h"
#include "GameTool.h"

class Role;

class RoleStateDelegate {
public:
  Role *mRole = nullptr;
  virtual void update (float delta) {};
  virtual void init () {};
  virtual void reset () {};

  template<typename DelegateType>
    static DelegateType* CreateDelegate() {
      DelegateType* delegate = new DelegateType();
      delegate->init();
      return delegate;
    }
};

class EnteringStateDelegate : public RoleStateDelegate {
public:
  virtual void update (float delta) {};
};

class RepositioningStateDelegate : public RoleStateDelegate {
public:
  virtual void update (float delta);
  virtual void init () { reset(); };
  virtual void reset ();

  float mTargetPos;
private:
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
    void switchToState(RoleState state) { mState = state; }
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
    float  mTimer;
    float mLifeSpan = 0;

    cocos2d::Node *mParent;
    template<typename RoleType>
      static Role* CreateRole(cocos2d::Node *parent) {
        Role *role = RoleType::create();
        role->mParent = parent;
        return role;
      }
};

#endif
