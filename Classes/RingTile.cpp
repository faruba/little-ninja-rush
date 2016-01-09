#include "RingTile.h"
#include "GameTool.h"
RingTile* RingTile::tile(float TileSize, float WindowSize, int BufferSize) 
{
	RingTile *tile = RingTile::create();

	tile->mTileSize = TileSize;
	tile->mWindowSize = WindowSize;
	//calc mWindowTileCount
	int ts = TileSize;
	int ws = WindowSize;
	int wtc = ws/ts + 1;
	if (ws%ts!=0) {
		wtc++;
	}
	tile->mWindowTileCount = wtc;
	tile->mpWindowTiles = new cocos2d::Sprite*[wtc];
	for(int i=0; i<wtc; ++i)
	{
		tile->mpWindowTiles[i] = NULL;
	}
	tile->mBufferSize = BufferSize;
	tile->mpBuffer = new int[BufferSize];
	memset(tile->mpBuffer, -1, sizeof(int)*BufferSize);
	return tile;
}

bool RingTile::init() 
{
	mTileSize = 0;
	mWindowSize = 0;
	mpWindowTiles = NULL;
	mWindowTileCount = 0;
	mWindowSpriteIndex = 0;
	mWindowIndex = 0;
	mpBuffer = NULL;
	mBufferSize = 0;
	mPendingCount = 0;
	mAccmulator = 0;
	mOffset = 0;
	return true;
}

int RingTile::windowTileCount() 
{
	return mWindowTileCount;
}

int RingTile::pendingCount() 
{
	return mPendingCount;
}

int RingTile::freedCount() 
{
	return mBufferSize-mWindowTileCount-mPendingCount;
}

float RingTile::offset() 
{
	return mOffset;
}

void RingTile::reset() 
{
	//debug remains
	//CCLog("RingTile:RESET");

	mWindowSpriteIndex = 0;
	mWindowIndex = 0;
	mOffset = 0;
	mAccmulator = 0;
	mPendingCount = 0;
	this->removeAllChildrenWithCleanup(true);
	for(int i=0; i<mWindowTileCount; ++i)
	{
		if( mpWindowTiles[i] != NULL )
		{
			mpWindowTiles[i] = NULL;
		}
	}
	memset(mpBuffer, -1, sizeof(int)*mBufferSize);
}

void RingTile::initTiles(int * Tiles, int Count) 
{
	if(Count >= mWindowTileCount)
	{
		mWindowSpriteIndex = 0;
		cocos2d::SpriteFrameCache *cache = cocos2d::SpriteFrameCache::sharedSpriteFrameCache();
		for(int i=0; i<mWindowTileCount; ++i)
		{
			mpBuffer[i] = Tiles[i];
			//generate ccsprite
			if( mpWindowTiles[i] != NULL )
			{
				mpWindowTiles[i]->setDisplayFrame(cache->spriteFrameByName(std::string_format("tile_%d.png", Tiles[i]).c_str()));
			}
			else {
				mpWindowTiles[i] = cocos2d::Sprite::createWithSpriteFrameName(std::string_format("tile_%d.png", Tiles[i]).c_str());
				//mpWindowTiles[i]->retain();
				this->addChild(mpWindowTiles[i]);
			}
			mpWindowTiles[i]->getTexture()->setAliasTexParameters();//v1.0.4
			mpWindowTiles[i]->setPosition(cocos2d::Vec2( mTileSize*i, 0));
			mpWindowTiles[i]->setAnchorPoint(cocos2d::Vec2(0, 0));

		}
		int left = Count - mWindowTileCount;
		if(left > 0)
		{
			if(this->pushTiles(Tiles+mWindowTileCount, left) == false)
			{
				cocos2d::CCLog("GameWarning: RingTile::initTiles > buffer data overflow, only visual part were accepted.");
			}
		}
	}
	else {
		cocos2d::CCLog("GameWarning: RingTile::initTiles > Operation ignored for not enough data.");
	}
}

bool RingTile::pushTiles(int * Tiles, int Count) 
{
	//debug remains
	//CCLog("RingTile:PUSH %d", Count);

	bool ret = false;
	if(this->freedCount()>=Count)
	{
		int b = mWindowIndex+mWindowTileCount+mPendingCount;
		for(int i=0; i<Count; ++i)
		{
			b %= mBufferSize;
			mpBuffer[b] = Tiles[i];
			b++;
		}
		mPendingCount += Count;
		ret = true;
	}

	return ret;
}

float RingTile::calcViewPos(float pos) 
{
	return (pos - mOffset);
}

void RingTile::runTiles(float delta) 
{
	mOffset += delta;
	mAccmulator += delta;
	if(mAccmulator >= mTileSize)
	{
		int a = mAccmulator;
		int s = mTileSize;
		int n = a/s;

		mAccmulator -= mTileSize*n;
		for(int i=0; i<n; ++i)
		{
			//-----------------------------
			//释放前n个window精灵
			int si = (mWindowSpriteIndex + i) % mWindowTileCount;
			//加载前n个pending精灵
			int pi = (mWindowIndex + mWindowTileCount + i) % mBufferSize;
			mpWindowTiles[si]->setDisplayFrame(GameTool::getSpriteFrameByName(std::string_format("tile_%d.png", mpBuffer[pi]).c_str()));
			mpWindowTiles[si]->getTexture()->setAliasTexParameters();//1.0.4
		}
		mWindowIndex = (mWindowIndex + n) % mBufferSize;
		mWindowSpriteIndex = (mWindowSpriteIndex + n) % mWindowTileCount;
		mPendingCount -= n;
		if(mPendingCount<0)
		{
			cocos2d::CCLog("GameWarning: RingTile::runTiles > Not enough pending tiles.");
		}
	}
	//重新计算每个tile的坐标
	for(int i=0; i<mWindowTileCount; ++i) 
	{
		int si = (mWindowSpriteIndex + i) % mWindowTileCount;
		int off = mAccmulator;
		mpWindowTiles[si]->setPosition(cocos2d::Vec2( mTileSize*i - off, 0));
	}
}
