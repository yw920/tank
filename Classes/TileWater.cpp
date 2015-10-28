#include "TileWater.h"

USING_NS_CC;

bool TileWater::init(CollisionDetector *detector, const int &tile_x, const int &tile_y, cocos2d::TMXLayer* mapLayer)
{
	if (!TileNode::init(detector, tile_x, tile_y, mapLayer))
	{
		return false;
	}
	_curTid = _gid;
	schedule(schedule_selector(TileWater::changeTileId), 0.1f);
	return true;
}

void TileWater::changeTileId(float dt)
{
	if (_curTid == _gid)
	{
		_curTid = _gid + 2;
	}
	else{
		_curTid = _gid;
	}
	_mapLayer->setTileGID(_curTid, Vec2(_tile_x, _tile_y));
}
