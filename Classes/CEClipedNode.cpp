#include "CEClipedNode.h"
#include "UniversalFit.h"
int _globalZOrder = 0;
void DrawRect(Node* pNode,cocos2d::CCRect& rect){

	auto rectNode = DrawNode::create();
	Vec2 rectangle[4];
	rectangle[0] = Vec2(rect.origin.x,rect.origin.y);
	rectangle[1] = Vec2(rect.origin.x+rect.size.width,rect.origin.y);
	rectangle[2] = Vec2(rect.origin.x+rect.size.width,rect.origin.y+rect.size.height);
	rectangle[3] = Vec2(rect.origin.x,rect.origin.y+rect.size.height);
	rectNode->drawPolygon(rectangle,4,Color4F::WHITE,1,Color4F::RED);
	pNode->addChild(rectNode);
}
void CEClipedNode::setClipRect(cocos2d::Rect *rect)
{
  if (NULL == rect) return;

  float factor = cocos2d::Director::getInstance()->getContentScaleFactor();
  clipRectInPixel = *rect;
  clipRectInPixel.origin.x *= factor;
  clipRectInPixel.origin.y *= factor;
  clipRectInPixel.size.width *= factor;
  clipRectInPixel.size.height *= factor;

//	DrawRect(this,clipRectInPixel);
}

//void CEClipedNode::visit(cocos2d::Renderer *renderer, const  cocos2d::Mat4& parentTransform, uint32_t parentFlags)
//{
//    
//	glPushMatrix();
//  glEnable(GL_SCISSOR_TEST);
//		glScissor(clipRectInPixel.origin.x, clipRectInPixel.origin.y,clipRectInPixel.size.width, clipRectInPixel.size.height);
//		Node::visit(renderer, parentTransform, parentFlags);
//  glDisable(GL_SCISSOR_TEST);
//	glPopMatrix();
//}


void CEClipedNode::onBeforeVisitScissor()
{
	glEnable(GL_SCISSOR_TEST);
	auto glview = Director::getInstance()->getOpenGLView();
	glview->setScissorInPoints(clipRectInPixel.origin.x, clipRectInPixel.origin.y,clipRectInPixel.size.width, clipRectInPixel.size.height);
}

void CEClipedNode::onAfterVisitScissor()
{
	glDisable(GL_SCISSOR_TEST);
}

void CEClipedNode::visit(cocos2d::Renderer *renderer, const  cocos2d::Mat4& parentTransform, uint32_t parentFlags)
{
	_beforeVisitCmdScissor.init(_globalZOrder);
	_beforeVisitCmdScissor.func = CC_CALLBACK_0(CEClipedNode::onBeforeVisitScissor, this);
	renderer->addCommand(&_beforeVisitCmdScissor);

	Node::visit(renderer, parentTransform, parentFlags);

	_afterVisitCmdScissor.init(_globalZOrder);
	_afterVisitCmdScissor.func = CC_CALLBACK_0(CEClipedNode::onAfterVisitScissor, this);
	renderer->addCommand(&_afterVisitCmdScissor);
}
