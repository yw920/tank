#ifndef _TILE_BRICK_H_
#define _TILE_BRICK_H_
#include "TileNode.h"

class TileBrick :public TileNode
{
public:

	virtual bool init(CollisionDetector *detector, const int &tile_x, const int &tile_y, cocos2d::TMXLayer* mapLayer);
	CREATE_FUNC_TILE(TileBrick);
	virtual COLL_RET onCollision(cocos2d::Node* other);
	

protected:
	virtual int getCategory(){
		return COLL_TILE_BRICK;
	}
	int _life;

};



#endif