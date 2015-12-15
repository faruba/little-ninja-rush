#include "UniversalFit.h"
#include "GameConstants.h"

UniversalFit g_UniversalFit;

UniversalFit::UniversalFit()
{
}

UniversalFit::~UniversalFit()
{
}

UniversalFit *UniversalFit::sharedUniversalFit()
{ 
  return &g_UniversalFit;
}

void UniversalFit::init()
{
  CCDirector *pDirector = cocos2d::CCDirector::sharedDirector();
  if (NULL == pDirector) {
    CCLog("UniversalFit::UniversalFit() NULL == pDirector");
  }
  else
  {
    CCSize size = pDirector->getWinSize();
    centralPoint = ccp(size.width/2.0, size.height/2.0);
    sceneOffset = ccp(0, 0);
    fakeScale = false;
    scaleFactor = 1.0f;
  }
}
CCRect UniversalFit::transformRect(cocos2d::CCRect rect)
{
  return CCRectMake(rect.origin.x*transform.width + baseLeft, rect.origin.y*transform.height + baseHeight,
      rect.size.width*transform.width, rect.size.height*transform.height);
}

bool gRetinaGraphics = true;

bool UniversalFit::isRetinaGraphics() 
{
    return gRetinaGraphics;
}

bool UniversalFit::isDuringHalloween() 
{
  /*
    NSDate *today = NSDate->date();
    NSDateComponents *todaycomp = [NSCalendar->currentCalendar() components:NSDayCalendarUnit | NSMonthCalendarUnit | NSYearCalendarUnit | NSWeekdayCalendarUnit fromDate:today];
    bool afterhalloween = false;
    bool beforehalloween = false;
    //after 10.1
    if( todaycomp.month >= 9 )
    {
        afterhalloween = true;
    }
    //before
    if( todaycomp.month < 11 || ( todaycomp.month == 11 && todaycomp.day <= 15 ))
    {
        beforehalloween = true;
    }
    if( afterhalloween && beforehalloween )
    {
        return true;
    }
    */
    return false;
}

bool UniversalFit::isDuringChristmas() 
{
  /*
    NSDate *today = NSDate->date();
    NSDateComponents *todaycomp = [NSCalendar->currentCalendar() components:NSDayCalendarUnit | NSMonthCalendarUnit | NSYearCalendarUnit | NSWeekdayCalendarUnit fromDate:today];
    //after 12.1
    if( todaycomp.month >= 11 || todaycomp.month == 0 )
    {
        return true;
    }
    */
    return false;
}

bool UniversalFit::UniversalFit::shouldUsingSinaWeibo() 
{
  // TODO:
  return true;
}

void UniversalFit::setAutofit(cocos2d::CCSize size) 
{
  screenSize = size;
  scaleFactor = size.width/SCREEN_WIDTH < size.height/SCREEN_HEIGHT ? size.width/SCREEN_WIDTH : size.height/SCREEN_HEIGHT;//以宽度为基准 (maximum extend)
    CCSize scaleSize = cocos2d::CCSizeMake(SCREEN_WIDTH*scaleFactor, SCREEN_HEIGHT*scaleFactor);
    sceneOffset = ccp(0, 0);//ccp((size.width - SCREEN_WIDTH*scaleFactor)/2, (size.height - SCREEN_HEIGHT*scaleFactor)/2);
  //clipRect.setRect(0, (size.height - scaleSize.height)/2, size.width, scaleSize.height);
    clipRect.setRect(0, 0, SCREEN_WIDTH*4, SCREEN_HEIGHT*4);//hammer android titlemenu clip test
    baseHeight =0;//clipRect.origin.y;
    topHeight = SCREEN_HEIGHT;//baseHeight + clipRect.size.height;
    baseLeft = 0;//(size.width - scaleSize.width)/2;
    baseRight = SCREEN_WIDTH;//baseLeft + scaleSize.width;
  playSize = cocos2d::CCSizeMake(SCREEN_WIDTH + 2*baseLeft, SCREEN_HEIGHT);
//#ifdef DEBUG
//  //Debug output
//  CCLog("WindowSize = %dx%d", (int)size.width, (int)size.height); 
//  CCLog("ScaleFactor = %f", scaleFactor); 
//  CCLog("SceneOffset = %f %f", sceneOffset.x, sceneOffset.y); 
//  CCLog("ClipRect = %f %f %f %f", clipRect.origin.x, clipRect.origin.y, clipRect.size.width, clipRect.size.height); 
//  CCLog("PlaySize = %dx%d", (int)playSize.width, (int)playSize.height); 
//#endif

  if( size.width > 480 ) 
  {
    gRetinaGraphics = true;
  }
    
    CCDirector *pDirector = cocos2d::CCDirector::sharedDirector();
    CCSize surface = pDirector->getOpenGLView()->getFrameSize();
    CCSize need = cocos2d::CCSizeMake(SCREEN_WIDTH, SCREEN_HEIGHT);
    need.width *= pDirector->getContentScaleFactor();
    need.height *= pDirector->getContentScaleFactor();
    transform.width = surface.width/need.width;
    transform.height = surface.height/need.height;
    
    CCLog("- surface size = %fx%f", surface.width, surface.height);
    CCLog("- scale = %f", pDirector->getContentScaleFactor());
}

cocos2d::Point UniversalFit::restorePoint(cocos2d::Point pos)
{
    cocos2d::Point tp = ccpSub(pos, UniversalFit::sharedUniversalFit()->sceneOffset);
    //tp.x /= UniversalFit::sharedUniversalFit()->scaleFactor;
    //tp.y /= UniversalFit::sharedUniversalFit()->scaleFactor;
    return tp;
}

