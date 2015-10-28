#include "TileBrick.h"
#include "cocos2d.h"
#include "MapLayer.h"

USING_NS_CC;

bool TileBrick::init(CollisionDetector *detector, const int &tile_x, const int &tile_y, cocos2d::TMXLayer* mapLayer)
{
	if (!TileNode::init(detector, tile_x, tile_y, mapLayer))
	{
		return false;
	}
	_life = 2;
	return true;
}

COLL_RET TileBrick::onCollision(cocos2d::Node* other){
	if (other->getTag() == COLL_BULLET 
		|| other->getTag() == COLL_BULLET_ENEMIES
		|| other->getTag() == COLL_BULLET_IRON)
	{
		--_life;
		const Vec2 curTilePos = Vec2(_tile_x, _tile_y);
		if (_life == 1){
			_mapLayer->setTileGID(_mapLayer->getTileGIDAt(curTilePos) + 2, curTilePos);
			return RETAIN;
		}
		else if (_life == 0)
		{
			_mapLayer->removeTileAt(curTilePos);
			MapLayer *theMap = dynamic_cast<MapLayer *>(_mapLayer->getParent()->getParent());
			theMap->destroyTile(this);
			return DESTROY;
		}
		
		
	}
	return RETAIN;
};

