//
//  Joystick.h
//  little-ninja-rush
//
//  Created by Jovi Du on 1/26/16.
//
//

#ifndef Joystick_h
#define Joystick_h

#include "cocos2d.h"
using namespace cocos2d;

#define WIDTH(obj) (obj)->getContentSize().width
#define HEIGHT(obj) (obj)->getContentSize().height
class Joystick : public cocos2d::Sprite
{
public:
  CREATE_FUNC(Joystick);
  virtual bool init();
  
  typedef std::function<void(const Vec2& newPos, const Vec2& lastPos)> JoystickHandler;
  void setJoystickPositionChangeHandler(JoystickHandler);
  
  
  CC_SYNTHESIZE(float, _controlDiameter, ControlDiameter);
  
  
private:
  Sprite* _stick;
  Sprite* _frame;
  Vec2 _lastPos;
  JoystickHandler _handler;
  EventListenerTouchOneByOne* _touchEventListener;
  
  
  bool onTouchBegan(Touch*, Event*);
  void onTouchMoved(Touch*, Event*);
  void onTouchEnded(Touch*, Event*);
  void onTouchCancelled(Touch*, Event*);
  
  
  void handleTouchChange(Touch*);
};

class JoyButton : public cocos2d::Sprite
{
 public:
  CREATE_FUNC(JoyButton);
  virtual bool init();
  void setImg(const std::string& name);
  typedef std::function<void()> ButtonHandler;
  void setButtonHandler( ButtonHandler);
  
private:
  Sprite* button;
  ButtonHandler _handler;
  EventListenerTouchOneByOne* _touchEventListener;
  
  bool onTouchBegan(Touch*, Event*);
  void onTouchMoved(Touch*, Event*){}
  void onTouchEnded(Touch*, Event*);
  void onTouchCancelled(Touch*, Event*);
  
};
#endif /* Joystick_h */
