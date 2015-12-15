#include "CEClipedNode.h"
#include "UniversalFit.h"

void CEClipedNode::setClipRect(CCRect *rect)
{
  if (NULL == rect) return;
    
  float factor = CCDirector::sharedDirector()->getContentScaleFactor();
  clipRectInPixel = *rect;
  clipRectInPixel.origin.x *= factor;
  clipRectInPixel.origin.y *= factor;
  clipRectInPixel.size.width *= factor;
  clipRectInPixel.size.height *= factor;
    CCLog("setClip(%f, %f, %f, %f)",clipRectInPixel.origin.x, clipRectInPixel.origin.y, clipRectInPixel.size.width, clipRectInPixel.size.height);
}

void CEClipedNode::visit(Renderer *renderer, const Mat4& parentTransform, uint32_t parentFlags)
{
  glEnable(GL_SCISSOR_TEST);

  glScissor(clipRectInPixel.origin.x, clipRectInPixel.origin.y,
      clipRectInPixel.size.width, clipRectInPixel.size.height);

  CCNode::visit(Renderer *renderer, const Mat4& parentTransform, uint32_t parentFlags);

  glDisable(GL_SCISSOR_TEST);
}
