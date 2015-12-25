#ifndef ___CLASSES_CECLIPEDNODE_H_
#define ___CLASSES_CECLIPEDNODE_H_

#include "cocos2d.h"
#include "cocos-ext.h"
#include "cocosbuilder/CocosBuilder.h"
#include "renderer/CCCustomCommand.h"

class CEClipedNode : public cocos2d::Node
{
public:
CCB_STATIC_NEW_AUTORELEASE_OBJECT_METHOD(CEClipedNode, create);

  virtual void visit(cocos2d::Renderer *renderer, const cocos2d::Mat4& parentTransform, uint32_t parentFlags);
  void setClipRect(cocos2d::Rect *rect);
	void onBeforeVisitScissor();
	void onAfterVisitScissor();
private:
	cocos2d::CustomCommand _beforeVisitCmdScissor;
	cocos2d::CustomCommand _afterVisitCmdScissor;

	cocos2d::Rect clipRectInPixel;
};



#endif
