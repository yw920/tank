#ifndef _TILE_WATER_H_
#define _TILE_WATER_H_
#include "TileNode.h"

class TileWater :public TileNode
{
public:
	CREATE_FUNC_TILE(TileWater);
	virtual COLL_RET onCollision(cocos2d::Node* other){
		return RETAIN;
	};
	virtual bool init(CollisionDetector *detector, const int &tile_x, const int &tile_y, cocos2d::TMXLayer* mapLayer);
	virtual int getCategory(){
		return COLL_TILE_WATER;
	}
	void changeTileId(float dt);
private:
	uint32_t _curTid;
};


#endif