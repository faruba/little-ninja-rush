#ifndef ___CLASSES_CECLIPEDNODE_H_
#define ___CLASSES_CECLIPEDNODE_H_

#include "GameTool.h"
#include "GameConstants.h"
#include "cocos2d.h"
#include "cocos-ext.h"
#include "cocosbuilder/CocosBuilder.h"

;

class CEClipedNode : public CCNode
{
public:
CCB_STATIC_NEW_AUTORELEASE_OBJECT_METHOD(CEClipedNode, create);

  virtual void visit(Renderer *renderer, const Mat4& parentTransform, uint32_t parentFlags);
  void setClipRect(cocos2d::CCRect *rect);
private:
cocos2d::CCRect clipRectInPixel;
};



#endif
