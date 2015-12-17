#include "CEClipedNode.h"
#include "UniversalFit.h"

void CEClipedNode::setClipRect(cocos2d::CCRect *rect)
{
  if (NULL == rect) return;
    return;
  float factor = cocos2d::CCDirector::sharedDirector()->getContentScaleFactor();
  clipRectInPixel = *rect;
  clipRectInPixel.origin.x *= factor;
  clipRectInPixel.origin.y *= factor;
  clipRectInPixel.size.width *= factor;
  clipRectInPixel.size.height *= factor;
cocos2d::CCLog("setClip(%f, %f, %f, %f)",clipRectInPixel.origin.x, clipRectInPixel.origin.y, clipRectInPixel.size.width, clipRectInPixel.size.height);
}

void CEClipedNode::visit(Renderer *renderer, const Mat4& parentTransform, uint32_t parentFlags)
{
  glEnable(GL_SCISSOR_TEST);

  glScissor(clipRectInPixel.origin.x, clipRectInPixel.origin.y,
      clipRectInPixel.size.width, clipRectInPixel.size.height);
    /* TODO: uncomment this
cocos2d::Node::visit(Renderer *renderer, const Mat4& parentTransform, uint32_t parentFlags);
*/
  glDisable(GL_SCISSOR_TEST);
}
