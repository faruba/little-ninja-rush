#ifndef ___CLASSES_GAMESCRIPT_H_
#define ___CLASSES_GAMESCRIPT_H_
#include "GamePlay.h"

class GameScript:public CCNode
{
  public:

    virtual bool init(){return true;};
    static GameScript* sharedScript();
    CREATE_FUNC(GameScript);

    //四面八方手里剑
    void invokeSpell0(CCInteger* mid);

    //分身术
    void invokeSpell1_0();

    //隐身术
    void invokeSpell2(CCInteger*);

    //子弹时间
    void invokeSpell3(CCInteger*);

    //加血
    void invokeSpell4(CCNode*,void*);

    //加气
    void invokeSpell5(CCNode*,void*);

    //一闪
    void invokeSpell6(CCInteger*);

    //流星雨
    void invokeSpell7(CCInteger*);

    //飞镖无限
    void invokeSpell8(CCInteger*);

    //全屏分身
    void invokeSpell9();

    //--- 道具技能 ---

    //手里剑旋风
    void invokeSpell15();

    //元气爆
    void invokeSpell16();

    //妖刀
    void invokeSpell17();

    //大招效果
    void invokeSpellRelease(CCInteger* sid);

    void invokeFly();

    void invokeSP();

    void invokeMoonBlade();

    void invokePumpkinImps();

};
#endif
