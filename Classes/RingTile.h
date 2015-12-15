#ifndef ___CLASSES_RINGTILE_H_
#define ___CLASSES_RINGTILE_H_
#include "cocos2d.h"

class RingTile:public cocos2d::CCNode
{
  public:
    virtual bool init();
    CREATE_FUNC(RingTile);

    float   mTileSize;//砖块尺寸
    float   mWindowSize;//窗口的尺寸
    cocos2d::CCSprite **mpWindowTiles;//背景贴图
    int     mWindowTileCount;//mpWindowTiles的尺寸
    int     mWindowSpriteIndex;//mpWindowTiles的起始index
    int     mWindowIndex;//Window在Buffer中起始index
    int     *mpBuffer;//缓冲区的数据
    int     mBufferSize;//缓冲区的尺寸（一定大于mWindowTileCount）
    int     mPendingCount;//待处理的tile计数
    float   mAccmulator;//累加器
    float   mOffset;//窗口最左边的偏移

    static RingTile* tile(float TileSize, float WindowSize, int BufferSize);

    int windowTileCount();
    int pendingCount();
    int freedCount();
    float offset();
    void reset();
    void initTiles(int* Tiles, int Count);
    bool pushTiles(int* Tiles, int Count);

    float calcViewPos(float pos);
    void runTiles(float delta);

};
#endif
