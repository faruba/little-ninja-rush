#ifndef __LittleNinjaRushAP__JsonWrapper__
#define __LittleNinjaRushAP__JsonWrapper__

#include "cocos2d.h"
#include "json/rapidjson.h"
#include "json/document.h"
#include "json/filereadstream.h"

typedef bool Predicate (const rapidjson::Value &arg);

class ValueWrapper {
  public:
    ValueWrapper(rapidjson::Value &arg) {
      value = arg;
    }

    void applyToItem(std::string &item) {
      item = getString(nullptr);
    }

    template <typename T>
    void applyToItem(T &item) {
      item.handleJsonValue(*this);
    }

    std::string getString(const char* key) const {
      const rapidjson::Value &v = getValue(key, [](const rapidjson::Value &arg) -> bool { return arg.IsString();});
      return std::string(v.GetString());
    }

    int getInteger(const char* key) const {
      const rapidjson::Value &v = getValue(key, [](const rapidjson::Value &arg) -> bool { return arg.IsInt();});
      return v.GetInt();
    }

    double getDouble(const char* key) const {
      const rapidjson::Value &v = getValue(key, [](const rapidjson::Value &arg) -> bool { return arg.IsDouble() || arg.IsInt() ;});
      return v.GetDouble();
    }

  private:
    rapidjson::Value value;

    const rapidjson::Value& getValue(const char* key, Predicate p, const bool required = true) const {
      bool hasKey = (key != nullptr);
      rapidjson::Value::ConstMemberIterator itr;
      if (hasKey) {
        itr = value.FindMember(key);
        if (itr == value.MemberEnd()) {
          throw "Field '`key`' missing."; // TODO: Good Exception
        }
      }

      const rapidjson::Value &v = hasKey? itr->value : value;

      if (!p(v)) {
        throw "Type Miss Match for `key`"; // TODO: Good Exception
      }
      return v;
    }
};

class JsonWrapper
{
public:
    template <typename T>
    static void parseJsonFileForVector(const char filename[], std::vector<T> &vector);
    static cocos2d::Ref* parseJson(std::string *pStr);
    static const char* dumpJson(cocos2d::ValueMap *pDic);
};

template <typename T>
void JsonWrapper::parseJsonFileForVector(const char filename[], std::vector<T> &vector) {
  std::string path = cocos2d::FileUtils::getInstance()->fullPathForFilename(filename);
  FILE* pFile = fopen(path.c_str(), "rb");
  char buffer[65535];
  rapidjson::FileReadStream is(pFile, buffer, sizeof(buffer));
  rapidjson::Document doc;
  doc.ParseStream<0, rapidjson::UTF8<>, rapidjson::FileReadStream>(is);

  if (doc.HasParseError()) {
    throw "JsonWrapper: parse failed."; // TODO: Better exception handling
  }

  if (!doc.IsArray() ) {
    throw "Type miss match"; // TODO:
  }

  for(rapidjson::SizeType i=0; i<doc.Size(); ++i) {
      ValueWrapper vw(doc[i]);
      T item;
      vw.applyToItem(item);
      vector.push_back(item);
  }
}

#endif /* defined(__LittleNinjaRushAP__JsonWrapper__) */
