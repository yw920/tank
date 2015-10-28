#ifndef _TILE_HOME_H_
#define _TILE_HOME_H_
#include "NodeWithColl.h"
#include "CollDetector.h"
#include "MapLayer.h"
#include "TileNode.h"
#include <2d/CCTMXLayer.h>

class TileHome :public NodeWithCollision
{
public:
	CREATE_FUNC_TILE(TileHome);
	virtual bool init(CollisionDetector *detector, const int &tile_x, const int &tile_y, cocos2d::TMXLayer* mapLayer);
	virtual COLL_RET onCollision(cocos2d::Node* other);
	void setLife(int life){
		_life = life;
	}
	virtual b2Body *createBody();

	int getTileX() const { return _tile_x; }
	void setTileX(const int &val) { _tile_x = val; }
	int getTileY() const { return _tile_y; }
	void setTileY(const int &val) { _tile_y = val; }
private:
	int _life;
	int _tile_x;
	int _tile_y;
	cocos2d::TMXLayer* _mapLayer;
};
#endif