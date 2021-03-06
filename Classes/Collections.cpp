#include "Collections.h"
#include "GameTool.h"
#include "GameRecord.h"
#include "GameConstants.h"
#include "json/filestream.h"
#include "json/prettywriter.h"

//常量定义
#define PIECE_COUNT (47)

using namespace rapidjson;

int gPiecesNumber[PIECE_COUNT] = 
{
	//- 飞镖 -
	0, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9,
	//- 刀刃 -
	0, 0, 0, 0, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9,
	//- 技能 -
	0, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9, 9
};
//test code remains
//int gPiecesNumber[PIECE_COUNT] = 
//{
//    //- 飞镖 -
//    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//    //- 刀刃 -
//    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
//    //- 技能 -
//    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
//};


Collections* Collections::sharedCollections() 
{
	static Collections *inst = NULL;
	if( inst == NULL )
	{
		inst = Collections::create();
		inst->retain();
	}
	return inst;
}


void Collections::createCollections() 
{
	pieces = new int[PIECE_COUNT];
	for( int i=0; i<PIECE_COUNT; ++i)
	{
		pieces[i] = 0;
	}
	newlist = cocos2d::CCArray::create();
	newlist->retain();
	life_piece = 0;
	dart_piece = 0;
}

void Collections::readCollections(cocos2d::CCDictionary* dic) 
{
	magic_piece = gtReadInt(dic, "magic_piece", 0);
	for( int i=0; i<PIECE_COUNT; ++i)
	{
		pieces[i] = gtReadInt(dic, std::string_format("pieces__%d", i).c_str(), 0);
	}
	//read CCArray
	cocos2d::Ref* nli = dic->objectForKey("collection_newlist");
	if( nli != NULL )
	{
		//TODO:Fix this, this crashes
		//        if( newlist != NULL )
		//        {
		//            newlist->release();
		//            newlist = NULL;
		//        }
		//        newlist = (cocos2d::CCArray*)nli;
		//        newlist->retain();
	}
	life_piece = gtReadInt(dic, "life_piece", 0);
	dart_piece = gtReadInt(dic, "dart_piece", 0);
}

void Collections::writeCollections(rapidjson::Document &document) 
{
	document.AddMember("magic_piece", magic_piece, document.GetAllocator());
	for( int i=0; i<PIECE_COUNT; ++i)
	{
		const std::string key = std::string("pieces__")+std::to_string(i);
		rapidjson::Value index(key.c_str(), key.size(), document.GetAllocator());
		document.AddMember(index, pieces[i], document.GetAllocator());
	}
	//write CCArray
	{
		rapidjson::Value lst(kArrayType);
		if( newlist != NULL )
		{
			cocos2d::Ref *pObj = NULL;
			CCARRAY_FOREACH(newlist, pObj)
			{
				cocos2d::CCInteger *pInt = (cocos2d::CCInteger*)pObj;
				lst.PushBack(pInt->getValue(), document.GetAllocator());
			}
		}
		document.AddMember("collection_newlist", lst, document.GetAllocator());
	}
	//document.AddMember("collection_newlist", newlist, document.GetAllocator());

	document.AddMember("life_piece", life_piece, document.GetAllocator());
	document.AddMember("dart_piece", dart_piece, document.GetAllocator());
}

bool Collections::isItemCompleted(int pid) 
{
	if( pid < PIECE_COUNT )
	{
		if( pieces[pid] >= gPiecesNumber[pid] )
		{
			return true;
		}
		else {
			return false;
		}
	}
	cocos2d::CCLog("Logic,  Invalid Piece ID (%d)", pid);
	return false;
}

int Collections::itemTotalPiece(int pid) 
{
	if( pid < PIECE_COUNT )
	{
		return gPiecesNumber[pid];
	}
	cocos2d::CCLog("Logic,  Invalid Piece ID (%d)", pid);
	return -1;
}

int Collections::itemLostPiece(int pid) 
{
	if( pid < PIECE_COUNT )
	{
		return gPiecesNumber[pid] - pieces[pid];
	}
	cocos2d::CCLog("Logic,  Invalid Piece ID (%d)", pid);
	return -1;
}

void Collections::gainItemPiece(int pid) 
{
	if( pid < PIECE_COUNT )
	{
		if( pieces[pid] < gPiecesNumber[pid] )
		{
			pieces[pid]++;
			addToNewList(pid);
			//achievement collect
			if( pieces[pid] >= gPiecesNumber[pid] )
			{
				if( pid < ITEM_SHURIKENEND )
				{
					GameRecord::sharedGameRecord()->task->dispatchTask(ACH_COLLECTSHURIKEN, 1);
				}
				if( pid >= ITEM_KATANASTART && pid < ITEM_KATANAEND )
				{
					GameRecord::sharedGameRecord()->task->dispatchTask(ACH_COLLECTKATANA, 1);
				}
				if( pid >= ITEM_SPECIALSTART )
				{
					GameRecord::sharedGameRecord()->task->dispatchTask(ACH_COLLECTSPELL, 1);
				}
			}
			GameRecord::sharedGameRecord()->uploadflag++;
		}
	}
	else {
		cocos2d::CCLog("Logic,  Invalid Piece ID (%d)", pid);
	}
}

//new list operate
void Collections::addToNewList(int uiid) 
{
	if( !isContainedInNewList(uiid) )
	{
		cocos2d::CCInteger *n = cocos2d::CCInteger::create(uiid);
		newlist->addObject(n);
	}
}

void Collections::removeFromNewList(int uiid) 
{
	for( unsigned int i=0; i<newlist->count(); ++i)
	{
		cocos2d::CCInteger *del = (cocos2d::CCInteger*)(newlist->objectAtIndex(i));
		int n = del->getValue();
		if( n == uiid)
		{
			newlist->removeObject(del);
			break;
		}
	}
}

void Collections::cleanNewList() 
{
	newlist->removeAllObjects();
}

bool Collections::isContainedInNewList(int uiid) 
{
	for( unsigned int i=0; i<newlist->count(); ++i)
	{
		cocos2d::CCInteger *del = (cocos2d::CCInteger*)(newlist->objectAtIndex(i));
		int n = del->getValue();
		if( n == uiid)
		{
			return true;
		}
	}
	return false;
}

void Collections::mergeWith(Collections* collection) 
{
	//only merge normal piece
	for( int i=0; i<PIECE_COUNT; ++i)
	{
		pieces[i] = MAX2(pieces[i], collection->pieces[i]);
	}
}
