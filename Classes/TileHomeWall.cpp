#include "TileHomeWall.h"
#include <audio/include/SimpleAudioEngine.h>
#include <platform/CCFileUtils.h>
USING_NS_CC;


COLL_RET TileHomeWall::onCollision(cocos2d::Node* other)
{
	if (other->getTag() == COLL_BULLET_ENEMIES
		//|| other->getTag() == COLL_BULLET
		)
	{
		--_life;
		CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(FileUtils::getInstance()->fullPathForFilename("audio/blast.wav").c_str());
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
}

bool TileHomeWall::init(CollisionDetector *detector, const int &tile_x, const int &tile_y, cocos2d::TMXLayer* mapLayer)
{
	if (!TileNode::init(detector, tile_x, tile_y, mapLayer))
	{
		return false;
	}
	_life = 2;
	return true;
}

