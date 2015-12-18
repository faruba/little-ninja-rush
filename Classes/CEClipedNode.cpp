#include "CEClipedNode.h"
#include "UniversalFit.h"

void CEClipedNode::setClipRect(cocos2d::Rect *rect)
{
  if (NULL == rect) return;

  float factor = cocos2d::Director::getInstance()->getContentScaleFactor();
  clipRectInPixel = *rect;
  clipRectInPixel.origin.x *= factor;
  clipRectInPixel.origin.y *= factor;
  clipRectInPixel.size.width *= factor;
  clipRectInPixel.size.height *= factor;
}

void CEClipedNode::visit(cocos2d::Renderer *renderer, const  cocos2d::Mat4& parentTransform, uint32_t parentFlags)
{
    
  glEnable(GL_SCISSOR_TEST);

  glScissor(clipRectInPixel.origin.x, clipRectInPixel.origin.y,clipRectInPixel.size.width, clipRectInPixel.size.height);
    Node::visit(renderer, parentTransform, parentFlags);
  glDisable(GL_SCISSOR_TEST);
}
