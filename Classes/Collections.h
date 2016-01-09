#ifndef ___CLASSES_COLLECTIONS_H_
#define ___CLASSES_COLLECTIONS_H_

#include <cocos2d.h>
#include "cocos-ext.h"
#include "GameObject.h"
;
USING_NS_CC_EXT;

class Collections:public GameObject
{
	public:
		CREATE_FUNC(Collections);
		//万能碎片
		int magic_piece;
		int *pieces;//only merge
		cocos2d::CCArray *newlist;
		int life_piece;
		int dart_piece;

		static Collections* sharedCollections();

		void createCollections();
		void readCollections(cocos2d::CCDictionary* dic);
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
