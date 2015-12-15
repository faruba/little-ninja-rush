#ifndef ___CLASSES_ROLE_H_
#define ___CLASSES_ROLE_H_
#include "GameObject.h"
class Role:
  public GameObject
{
  public:
    //与圆做碰撞检测
    virtual bool collisionWithCircle(cocos2d::Point cc, float rad) = 0;

    //投递伤害 伤害类型 返回伤害是否被接受
    virtual bool deliverHit(int type, cocos2d::Point dir) = 0;

    //获得坐标
    virtual cocos2d::Point position() = 0;
    virtual void setPosition(cocos2d::Point pos) = 0;

    //获得人物中心坐标
    virtual cocos2d::Point center() = 0;

    //是否支持辅助瞄准
    virtual bool supportAimAid() = 0;

    virtual void toggleVisible(bool flag) = 0;
    virtual bool isEnemy() {return false;};
};
#endif
