#ifndef _TILEHOME_WALL_H_
#define _TILEHOME_WALL_H_

#include "TileNode.h"
class TileHomeWall :public TileNode
{
public:
	CREATE_FUNC_TILE(TileHomeWall);
	//virtual bool init();
	virtual bool init(CollisionDetector *detector, const int &tile_x, const int &tile_y, cocos2d::TMXLayer* mapLayer);
	virtual COLL_RET onCollision(cocos2d::Node* other);

protected:
	virtual int getCategory(){
		return COLL_TILE_BRICK;
	}
	int _life;

};
#endif