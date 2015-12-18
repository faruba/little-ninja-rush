#ifndef ___CLASSES_ABKIT_ABSCROLLCONTENT_H_
#define ___CLASSES_ABKIT_ABSCROLLCONTENT_H_
#include "cocos2d.h"
#include "cocos-ext.h"
#include "CEClipedNode.h"

USING_NS_CC_EXT;

class ABScrollContent : public cocos2d::Layer{
public:
  virtual bool init();
  virtual void update(float);

  virtual bool onTouchBegan(cocos2d::Touch * touch, cocos2d::Event * event);
  virtual void onTouchMoved(cocos2d::Touch * touch, cocos2d::Event * event);
  virtual void onTouchEnded(cocos2d::Touch * touch, cocos2d::Event * event);

CCB_STATIC_NEW_AUTORELEASE_OBJECT_WITH_INIT_METHOD(ABScrollContent, create);
  void setClipRect(cocos2d::CCRect clipRect);

  void resetContentPosition();
  void addContent(cocos2d::Node* content);
  void removeContent(cocos2d::Node* content);
  void removeAllContent();

  CEClipedNode *mClipNode;
cocos2d::Node *contentNode;
cocos2d::Ref *clickTarget;
  cocos2d::SEL_CallFunc clickMethod;
  cocos2d::Point clickPoint;

private:
  cocos2d::Point mTouchBegin;
  float mBeginPressY;
  float mBeginNodeY;
  bool mFly;
  float mLastY;
  float mFlySpeed;
  //CFAbsoluteTime mLastTime;
    time_t mLastTime;
cocos2d::CCRect clipRect;
  

};
#endif
