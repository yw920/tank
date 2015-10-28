#include "TileNode.h"
#include <math/CCGeometry.h>
#include <cocos2d.h>
#include "MapData.h"


USING_NS_CC;

bool TileNode::init(CollisionDetector *detector, const int &tile_x, const int &tile_y, TMXLayer* mapLayer)
{
	_tile_x = tile_x;
	_tile_y = tile_y;
	_mapLayer = mapLayer;
	Point centerPos = MAPDATA->getTileCenter(tile_x, tile_y);
	this->setPosition(centerPos.x, centerPos.y);
	if (!NodeWithCollision::init(detector))
	{
		return false;
	}
	_gid = _mapLayer->getTileGIDAt(Vec2(_tile_x, _tile_y));
	this->setTag(getCategory());
	return true;
}

b2Body * TileNode::createBody()
{
	return _collDetector->createBoxBody(this, b2_staticBody, getPositionX(), getPositionY(), MAPDATA->getTile_w_in_scene(), MAPDATA->getTile_h_in_scene());
}
