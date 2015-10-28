#ifndef _TILE_GRASS_H_
#define _TILE_GRASS_H_
#include "TileNode.h"

class TileGrass :public TileNode
{
public:
	CREATE_FUNC_TILE(TileGrass);
	virtual COLL_RET onCollision(cocos2d::Node* other){
		return RETAIN;
	};
	virtual int getCategory(){
		return COLL_TILE_GRASS;
	}

private:

};


#endif