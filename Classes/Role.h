#ifndef ___CLASSES_ROLE_H_
#define ___CLASSES_ROLE_H_
#include "GameObject.h"
#include "GameTool.h"

class Role: public GameObject {
  public:
    enum RoleState {
      Entering,
      Running,
      PreparingToShoot,
      Shooting,
      Fleeing,
      Hit,
      Dead,
      Dummy,
      Initializing
    };
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
    virtual void onCreate() {
      GameObject::onCreate();
      mSprite = GTAnimatedSprite::spriteWithGTAnimation(GTAnimation::loadedAnimationSet(animationSetName()));
      mSprite->setAnchorPoint(mAnchor);
      mState = Entering;
    }

    void drawCollision();

    bool handleGameOver (float delta) ;

    std::vector<Circle> mCollisionCircles;
    GTAnimatedSprite *mSprite;
    cocos2d::Vec2 mAnchor = cocos2d::Vec2(0.4f, 0.0625f);
    int mState = Initializing;
    //enum RoleState mState;

    cocos2d::Node *mParent;
    template<typename RoleType>
      static Role* CreateRole(cocos2d::Node *parent) {
        Role *role = RoleType::create();
        role->mParent = parent;
        return role;
      }
};

#endif
