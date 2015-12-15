#ifndef ___CLASSES_COLLECTIONS_H_
#define ___CLASSES_COLLECTIONS_H_

#include <cocos2d.h>
#include "cocos-ext.h"
#include "rapidjson/rapidjson.h"
#include "rapidjson/document.h"
#include "rapidjson/prettywriter.h"
#include "rapidjson/filestream.h"

USING_NS_CC;
USING_NS_CC_EXT;

class Collections:public CCObject
{
public:
  CCB_STATIC_NEW_AUTORELEASE_OBJECT_METHOD(Collections, create);
  //万能碎片
  int magic_piece;
  int *pieces;//only merge
  CCArray *newlist;
  int life_piece;
  int dart_piece;

  static Collections* sharedCollections();

  void createCollections();
  void readCollections(CCDictionary* dic);
  void writeCollections(rapidjson::Document &document);
  void mergeWith(Collections* collection);

  //道具操作
  bool isItemCompleted(int pid);
  int itemTotalPiece(int pid);
  int itemLostPiece(int pid);
  void gainItemPiece(int pid);

  //NEWLIST
  void addToNewList(int uiid);
  void removeFromNewList(int uiid);
  void cleanNewList();
  bool isContainedInNewList(int uiid);

};
#endif
