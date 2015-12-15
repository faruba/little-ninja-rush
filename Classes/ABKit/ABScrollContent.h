#ifndef ___CLASSES_ABKIT_ABSCROLLCONTENT_H_
#define ___CLASSES_ABKIT_ABSCROLLCONTENT_H_
#include "cocos2d.h"
#include "cocos-ext.h"
#include "CEClipedNode.h"

USING_NS_CC_EXT;

class ABScrollContent : public CCLayer{
public:
  virtual bool init();
  virtual void update(float);

  virtual bool onTouchBegan(Touch * touch, Event * event);
  virtual void onTouchMoved(Touch * touch, Event * event);
  virtual void onTouchEnded(Touch * touch, Event * event);
  virtual void onTouchCancelled(Touch * touch, Event * event);

  CCB_STATIC_NEW_AUTORELEASE_OBJECT_WITH_INIT_METHOD(ABScrollContent, create);
  void setClipRect(cocos2d::CCRect clipRect);

  void resetContentPosition();
  void addContent(cocos2d::CCNode* content);
  void removeContent(cocos2d::CCNode* content);
  void removeAllContent();

  CEClipedNode *mClipNode;
  CCNode *contentNode;
  CCObject *clickTarget;
  SEL_CallFunc clickMethod;
  cocos2d::Point clickPoint;

private:
  cocos2d::Point mTouchBegin;
  float mBeginPressY;
  float mBeginNodeY;
  bool mFly;
  float mLastY;
  float mFlySpeed;
  //CFAbsoluteTime mLastTime;
    struct cc_timeval mLastTime;

  CCRect clipRect;
  

};
#endif
