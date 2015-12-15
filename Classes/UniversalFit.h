#ifndef ___CLASSES_UNIVERSALFIT_H_
#define ___CLASSES_UNIVERSALFIT_H_

#include "cocos2d.h"

using namespace cocos2d;

class UniversalFit
{
public:
  UniversalFit();
  ~UniversalFit();

  void init();
  void setAutofit(cocos2d::CCSize size);

  static bool isRetinaGraphics();
  static bool isDuringHalloween();
  static bool isDuringChristmas(); 

  cocos2d::Point restorePoint(cocos2d::Point);
  static UniversalFit *sharedUniversalFit();
  static bool  shouldUsingSinaWeibo();
  CCRect transformRect(cocos2d::CCRect rect);
  cocos2d::Point centralPoint;
  cocos2d::Point sceneOffset;
  bool fakeScale;
  float scaleFactor;
  int baseLeft;
  int baseRight;
  int baseHeight;
  int topHeight;
  CCRect clipRect;
  CCSize playSize;
  CCSize screenSize;
    
    CCSize transform;
};

#endif
