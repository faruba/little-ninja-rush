//
//  JsonWrapper.cpp
//  LittleNinjaRushAP
//
//  Created by 马 颂文 on 13-3-10.
//  Copyright (c) 2013年 Trin Game. All rights reserved.
//

#include "JsonWrapper.h"
#include "json/rapidjson.h"
#include "json/document.h"
#include "CCNumber.h"

using namespace std;
using namespace cocos2d;
using namespace rapidjson;




cocos2d::Ref* processNode(const rapidjson::Value& value) {
	cocos2d::Ref *pRet = NULL;

	if ( value.IsArray() ) {
		cocos2d::CCArray *pArray = new CCArray();
		pArray->init();

		for(SizeType i=0; i<value.Size(); ++i) {
			const rapidjson::Value& sub = value[i];
			pArray->addObject(processNode(sub));
		}
		pRet = pArray;
	}
	else if( value.IsObject() )
	{
		cocos2d::CCDictionary *pDictionary = new CCDictionary();
		for(rapidjson::Value::ConstMemberIterator iter = value.MemberBegin(); iter != value.MemberEnd(); ++iter)
		{
			pDictionary->setObject(processNode(iter->value), string(iter->name.GetString()));
		}
		pRet = pDictionary;
	}
	else if( value.IsString() )
	{
		cocos2d::CCString *pString = new CCString(value.GetString());
		pRet = pString;
	}
	else if( value.IsBool() )
	{
		cocos2d::CCBool *pBool = new CCBool(value.GetBool());
		pRet = pBool;
	}
	else if( value.IsNumber() )
	{
		if( value.IsDouble() )
		{
			CCNumber *pNumber = new CCNumber((float)value.GetDouble());
			pRet = pNumber;
		}
		else if( value.IsInt() )
		{
			CCNumber *pNumber = new CCNumber((int)value.GetDouble());
			pRet = pNumber;
		}
		else
		{
			printf("JsonWrapper: Unknown type of number.\n");
		}
	}
	else
	{
		printf("JsonWrapper: Unknown type.\n");
	}

	return pRet;
}

cocos2d::Ref* JsonWrapper::parseJson(std::string &pStr)
{
	cocos2d::Ref *pRet = NULL;

	Document doc;
	doc.Parse<0>(pStr.c_str());
	if( !doc.HasParseError() )
	{
		pRet = processNode(doc);
	}
	else
	{
		printf("JsonWrapper: parse failed.\n");
	}

	return pRet;
}

const char* JsonWrapper::dumpJson(cocos2d::ValueMap *pDic)
{
	char *pRet = NULL;

	Document doc;
	//  doc.Parse<0>(pStr->getCString());
	//  if( !doc.HasParseError() )
	//  {
	//      pRet = processNode(doc);
	//  }
	//  else
	//  {
	//      printf("JsonWrapper: parse failed.\n");
	//  }

	return pRet;
}
