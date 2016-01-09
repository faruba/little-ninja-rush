#ifndef ___CLASSES_GAMEDATA_H_
#define ___CLASSES_GAMEDATA_H_

#include "cocos2d.h"
#include "JsonWrapper.h"

class GameItemBase : public cocos2d::Ref {
	public:
		int uiid;         // Unique Item ID
		std::string titl; // TODO: change to title
		std::string name;
		std::string desc; // TODO: change to description
		std::string icon;
		int wght; // TODO: change to weight

		GameItemBase() {
		}

		GameItemBase(const GameItemBase &item) {
			uiid = item.uiid;
			titl = item.titl;
			name = item.name;
			desc = item.desc;
			icon = item.icon;
			wght = item.wght;
		}

	public:
		virtual void handleJsonValue(const ValueWrapper& value) {
			uiid = value.getInteger("uiid");
			titl = value.getString("titl");
			name = value.getString("name");
			desc = value.getString("desc");
			icon = value.getString("icon");
			wght = value.getInteger("wght");
		}
};

inline bool operator==(const GameItemBase& lhs, const int uiid) {
	return lhs.uiid == uiid;
}

class Shuriken: public GameItemBase {
	public:
		int flys;
		float reld;
		int efft;   // TODO: change to effect
		std::string shap; // TODO: change to shape

		Shuriken() { }

		Shuriken(const Shuriken& item) : GameItemBase(item) {
			flys = item.flys;
			reld = item.reld;
			efft = item.efft;
			shap = item.shap;
		}

		virtual void handleJsonValue(const ValueWrapper& value) {
			GameItemBase::handleJsonValue(value);
			flys = value.getInteger("flys");
			reld = value.getDouble("reld");
			efft = value.getInteger("efft");
			shap = value.getString("shap");
		}
};

class Katana:public GameItemBase {
	public:
		int anim;
		std::string sond;
		float rang;
		int efft;
		float reld;

		Katana() { }

		Katana(const Katana& item) : GameItemBase(item) {
			anim = item.anim;
			sond = item.sond;
			rang = item.rang;
			efft = item.efft;
			reld = item.reld;
		}

		virtual void handleJsonValue(const ValueWrapper& value) {
			GameItemBase::handleJsonValue(value);
			anim = value.getInteger("anim");
			sond = value.getString("sond");
			rang = value.getDouble("rang");
			efft = value.getInteger("efft");
			reld = value.getDouble("reld");
		}
};

//特技
class Special:public GameItemBase
{
	public:
		int spmx;
		int spac;
		int skid;
		std::string shap; // TODO: change to shape

		Special() { }

		Special(const Special& item) : GameItemBase(item) {
			spmx = item.spmx; 
			spac = item.spac; 
			skid = item.skid; 
			shap = item.shap; 
		}

		virtual void handleJsonValue(const ValueWrapper& value) {
			GameItemBase::handleJsonValue(value);
			spmx = value.getInteger("spmx");
			spac = value.getInteger("spac");
			skid = value.getInteger("skid");
			shap = value.getString("shap");
		}
};

class GameData : public cocos2d::Ref
{
	public:

		static int roleCurrDart(int rid);
		static int roleCurrHP(int rid);
		static int roleMaxDart(int rid);
		static int roleMaxHP(int rid);
		static float roleRunSpeed(int rid);
		static float roleArcadeBouns(int rid);
		static std::string roleAnimation(int rid);

		static int upgradeCost(int uid, int lev);

		static void loadData();
		static void releaseData();

		static std::vector<Shuriken>& fetchShurikens();
		static std::vector<Katana>& fetchKatanas();
		static std::vector<Special>& fetchSpecials();
		static std::vector<std::string>& fetchTips();

		static Shuriken& queryShuriken(int uiid);
		static Katana& queryKatana(int uiid);
		static Special& querySpecial(int uiid);
		static std::string& randomTip();
};

template <typename T>
void loadVectorFromJsonFile(const char *filename, std::vector<T> &vector) {
	CCLOG("Loading %s...", filename);
	try {
		JsonWrapper::parseJsonFileForVector(filename, vector);
	} catch ( const char *err ) {

		CCLOG("Error: failed to parse objectives data file %s.", filename);
	}
	CCLOG("%lu items loaded.", vector.size());
}


#endif
