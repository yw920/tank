#include "TileHome.h"
#include "MapData.h"
#include "SceneGame.h"
#include "ScenePause.h"
#include <audio/include/SimpleAudioEngine.h>
#include <platform/CCFileUtils.h>

USING_NS_CC;


bool TileHome::init(CollisionDetector *detector, const int &tile_x, const int &tile_y, cocos2d::TMXLayer* mapLayer)
{
	_tile_x = tile_x;
	_tile_y = tile_y;
	_mapLayer = mapLayer;
	_life = 2;
	Point centerPos = MAPDATA->tilePos2worldPos(tile_x, tile_y);
	setPosition(centerPos.x, centerPos.y);
	if (!NodeWithCollision::init(detector))
	{
		return false;
	}
	this->setTag(COLL_TILE_HOME);
	return true;
}


COLL_RET TileHome::onCollision(cocos2d::Node* other)
{
	//gameOver
	if (other->getTag() == COLL_BULLET_ENEMIES)
	{
		--_life;
		CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(FileUtils::getInstance()->fullPathForFilename("audio/tankbomb.wav").c_str());
		if (_life == 1){
			Vec2 curTilePos = Vec2(_tile_x, _tile_y);
			_mapLayer->setTileGID(_mapLayer->getTileGIDAt(curTilePos) + 2, curTilePos);
			curTilePos = Vec2(_tile_x, _tile_y-1);
			_mapLayer->setTileGID(_mapLayer->getTileGIDAt(curTilePos) + 2, curTilePos);
			curTilePos = Vec2(_tile_x-1, _tile_y - 1);
			_mapLayer->setTileGID(_mapLayer->getTileGIDAt(curTilePos) + 2, curTilePos);
			curTilePos = Vec2(_tile_x - 1, _tile_y);
			_mapLayer->setTileGID(_mapLayer->getTileGIDAt(curTilePos) + 2, curTilePos);
			return RETAIN;
		}
		else if (_life == 0)
		{
			SceneGame *theGame = dynamic_cast<SceneGame *>(this->getParent());
			theGame->gameOver();
			return DESTROY;
		}
	}
	return RETAIN;
}

b2Body * TileHome::createBody()
{
	return _collDetector->createBoxBody(this, b2_staticBody, getPositionX(), getPositionY(), MAPDATA->getTile_w_in_scene()*2.0f, MAPDATA->getTile_h_in_scene()*2.0f);
}

