#ifndef _TILE_IRON_H_
#define _TILE_IRON_H_
#include "TileNode.h"

class TileIron :public TileNode
{
public:
	CREATE_FUNC_TILE(TileIron);
	virtual COLL_RET onCollision(cocos2d::Node* other);

private:

};



#endif