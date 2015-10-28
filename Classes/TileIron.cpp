#include "TileIron.h"
#include "cocos2d.h"

USING_NS_CC;

COLL_RET TileIron::onCollision(cocos2d::Node* other)
{
	if (other->getTag() == COLL_BULLET_IRON)
	{
		const Vec2 curTilePos = Vec2(_tile_x, _tile_y);
		_mapLayer->removeTileAt(curTilePos);
		MapLayer *theMap = dynamic_cast<MapLayer *>(_mapLayer->getParent()->getParent());
		theMap->destroyTile(this);
		return DESTROY;
	}
	return RETAIN;
}