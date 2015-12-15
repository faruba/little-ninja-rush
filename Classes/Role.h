#ifndef ___CLASSES_ROLE_H_
#define ___CLASSES_ROLE_H_
#include "GameObject.h"
class Role:
  public GameObject
{
  public:
    //与圆做碰撞检测
    virtual bool collisionWithCircle(CCPoint cc, float rad) = 0;

    //投递伤害 伤害类型 返回伤害是否被接受
    virtual bool deliverHit(int type, CCPoint dir) = 0;

    //获得坐标
    virtual CCPoint position() = 0;
    virtual void setPosition(CCPoint pos) = 0;

    //获得人物中心坐标
    virtual CCPoint center() = 0;

    //是否支持辅助瞄准
    virtual bool supportAimAid() = 0;

    virtual void toggleVisible(bool flag) = 0;
    virtual bool isEnemy() {return false;};
};
#endif
