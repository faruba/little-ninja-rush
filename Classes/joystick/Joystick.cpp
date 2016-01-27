//
//  Joystick.c
//  little-ninja-rush
//
//  Created by Jovi Du on 1/26/16.
//
//

#include <stdio.h>
#include "Joystick.h"

using namespace cocos2d;
bool Joystick::init()
{
    
    
    if (!cocos2d::Sprite::initWithSpriteFrameName("htx4.png"))
    return false;
  
  _lastPos = Vec2::ZERO;
  
    _frame = cocos2d::Sprite::createWithSpriteFrameName("htx4.png");
  _frame->setPosition(WIDTH(this) / 2, HEIGHT(this) / 2);
  addChild(_frame);
  
  _stick = Sprite::createWithSpriteFrameName("bxtx.png");
  _stick->setPosition(WIDTH(_frame) / 2, HEIGHT(_frame) / 2);
  _frame->addChild(_stick);
  
  _controlDiameter = WIDTH(_frame) - WIDTH(_stick);
  
  _touchEventListener = EventListenerTouchOneByOne::create();
  _touchEventListener->onTouchBegan = CC_CALLBACK_2(Joystick::onTouchBegan, this);
  _touchEventListener->onTouchMoved = CC_CALLBACK_2(Joystick::onTouchMoved, this);
  _touchEventListener->onTouchEnded = CC_CALLBACK_2(Joystick::onTouchEnded, this);
  _touchEventListener->onTouchCancelled = CC_CALLBACK_2(Joystick::onTouchCancelled, this);
  _touchEventListener->setSwallowTouches(true);
  
  Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(_touchEventListener, this);
  
  return true;
}

void Joystick::setJoystickPositionChangeHandler(JoystickHandler handler)
{
  _handler = handler;
}

bool Joystick::onTouchBegan(Touch* touch, Event* event)
{
  if (this->getBoundingBox().containsPoint(touch->getLocation()))
  {
    handleTouchChange(touch);
    return true;
  }
  
  return false;
}

void Joystick::onTouchMoved(Touch* touch, Event* event)
{
  if (this->getBoundingBox().containsPoint(touch->getLocation()))
    handleTouchChange(touch);
}

void Joystick::onTouchEnded(Touch* touch, Event* event)
{
  static Vec2 origin = Vec2(WIDTH(_frame) / 2, HEIGHT(_frame) / 2);
  
  _stick->setPosition(origin);
  
  if (_handler)
    _handler(Vec2::ZERO, Vec2::ZERO);
  
  _lastPos = Vec2::ZERO;
}

void Joystick::onTouchCancelled(Touch* touch, Event* event)
{
  onTouchEnded(touch, event);
}

void Joystick::handleTouchChange(Touch* touch)
{
  static Vec2 origin = Vec2(WIDTH(_frame) / 2, HEIGHT(_frame) / 2);
  static float bigR = WIDTH(_frame) / 2;
  static float smallR = WIDTH(_stick) / 2;
  
  Vec2 hit = this->_frame->convertToNodeSpaceAR(touch->getLocation());
  
  if (hit.getDistance(Vec2::ZERO) + smallR > bigR)
  {
    float x = (bigR - smallR) / sqrt(1 + hit.y * hit.y / hit.x / hit.x);
    float y = abs(hit.y / hit.x * x);
    
    if (hit.x > 0)
    {
      if (hit.y > 0)
      {
        hit.x = x;
        hit.y = y;
      }
      else
      {
        hit.x = x;
        hit.y = -y;
      }
    }
    else
    {
      if (hit.y > 0)
      {
        hit.x = -x;
        hit.y = y;
      }
      else
      {
        hit.x = -x;
        hit.y = -y;
      }
    }
  }
  
  _stick->setPosition(hit + origin);
  
  if (_handler)
    _handler(hit, _lastPos);
  
  _lastPos = hit;
}


bool Button::init()
{
  
  _touchEventListener = EventListenerTouchOneByOne::create();
  _touchEventListener->onTouchEnded = CC_CALLBACK_2(Button::onTouchEnded, this);
  _touchEventListener->onTouchCancelled = CC_CALLBACK_2(Button::onTouchCancelled, this);
  _touchEventListener->setSwallowTouches(true);
  
  Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(_touchEventListener, this);
  
  return true;
}

void Button::setJoystickPositionChangeHandler(ButtonHandler handler)
{
  _handler = handler;
}

void Button::setImg(const char* name){
  button= cocos2d::Sprite::createWithSpriteFrameName(name);
  button->setPosition(WIDTH(this) / 2, HEIGHT(this) / 2);
  addChild(button);
}
void Button::onTouchEnded(Touch* touch, Event* event)
{
  
  if (_handler)
    _handler();
  
}

void Button::onTouchCancelled(Touch* touch, Event* event)
{
  onTouchEnded(touch, event);
}

