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
  void setAutofit(CCSize size);

  static bool isRetinaGraphics();
  static bool isDuringHalloween();
  static bool isDuringChristmas(); 

  CCPoint restorePoint(CCPoint);
  static UniversalFit *sharedUniversalFit();
  static bool  shouldUsingSinaWeibo();
  CCRect transformRect(CCRect rect);
  CCPoint centralPoint;
  CCPoint sceneOffset;
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
