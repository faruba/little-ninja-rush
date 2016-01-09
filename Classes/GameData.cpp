#include "GameData.h"
#include "GameConstants.h"
#include "GameTool.h"
#include "JsonWrapper.h"
#include "GameRecord.h"
#include "UniversalFit.h"
#include "CCNumber.h"

//角色的数据
int gHPBase[GAME_CHARCOUNT] = {3, 2, 4, 2};
int gHPMax[GAME_CHARCOUNT] = {5, 4, 6, 3};
int gDartBase[GAME_CHARCOUNT] = {5, 7, 4, 4};
int gDartMax[GAME_CHARCOUNT] = {7, 9, 6, 6};
int gSpeedBase[GAME_CHARCOUNT] = {300, 400, 250, 350};
float gArcadeBouns[GAME_CHARCOUNT] = {0.3f, 0.1f, 0.6f, 0.8f};

std::string gAniSet[GAME_CHARCOUNT] = {
	"main",
	"girl",
	"musashi",
	"pussy"
};

int gUpgradeCost[GAME_UPGRADECOUNT*3] = 
{
	500, 1500, 5000,//八方
	500, 1500, 5000,//隐身
	500, 1500, 5000,//子弹时间
	500, 1500, 5000,//加血
	500, 1500, 5000,//加气
	500, 1500, 5000,//一闪
	500, 1500, 5000,//流星雨
	500, 1500, 5000,//飞镖无限
	500, 1500, 5000,//全屏分身
};

//数据
std::vector<Shuriken> gShurikens;
std::vector<Katana> gKatanas;
std::vector<Special> gSpecials;
std::vector<std::string> gTips;

//--- character ---

int GameData::roleCurrDart(int rid)
{
	return gDartBase[rid] + GameRecord::sharedGameRecord()->char_dart[rid];
}

int GameData::roleMaxDart(int rid)
{
	return gDartMax[rid];
}

int GameData::roleCurrHP(int rid)
{
	return gHPBase[rid] + GameRecord::sharedGameRecord()->char_hp[rid];
}

int GameData::roleMaxHP(int rid)
{
	return gHPMax[rid];
}

float GameData::roleRunSpeed(int rid)
{
	return gSpeedBase[rid];
}

float GameData::roleArcadeBouns(int rid)
{
	return gArcadeBouns[rid];
}

std::string GameData::roleAnimation(int rid)
{
	return gAniSet[rid];
}

int GameData::upgradeCost(int uid, int lev)
{
	if( lev > 2 )
	{
		return 0;
	}
	else {
		return gUpgradeCost[uid*3+lev];
	}
}

//--- data ---

void GameData::loadData() {
	loadVectorFromJsonFile("data/shuriken.json", gShurikens);
	loadVectorFromJsonFile("data/katana.json", gKatanas);
	loadVectorFromJsonFile("data/special.json", gSpecials);
	loadVectorFromJsonFile("data/tips.json", gTips);
}

void GameData::releaseData() {
	// TODO:Release memories
}

std::vector<std::string> &GameData::fetchTips() {
	return gTips;
}

std::string &GameData::randomTip() {
	srand(time(NULL));
	int n = rand()%gTips.size();
	return gTips[n];
}

std::vector<Shuriken> &GameData::fetchShurikens() {
	return gShurikens;
}

std::vector<Katana> &GameData::fetchKatanas() {
	return gKatanas;
}

std::vector<Special> &GameData::fetchSpecials() {
	return gSpecials;
}

template <typename ItemType>
ItemType &queryByID(std::vector<ItemType>& vector, int uiid) {
	typename std::vector<ItemType>::iterator it;
	it = std::find (vector.begin(), vector.end(), uiid);
	if (it == vector.end()) {
		throw "Item Not Found"; //TODO:
	} else {
		return *it;
	}
}

Shuriken &GameData::queryShuriken(int uiid) {
	return queryByID(gShurikens, uiid);
}

Katana &GameData::queryKatana(int uiid) {
	return queryByID(gKatanas, uiid);
}

Special &GameData::querySpecial(int uiid) {
	return queryByID(gSpecials, uiid);
}
