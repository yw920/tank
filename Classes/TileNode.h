#ifndef _TILE_NODE_H_
#define _TILE_NODE_H_

#include "NodeWithColl.h"
#include "CollDetector.h"
#include "MapLayer.h"
#include <2d/CCTMXLayer.h>


#define CREATE_FUNC_TILE(class_name)\
static class_name* create(CollisionDetector *detector, const int &tile_x, const int &tile_y, cocos2d::TMXLayer* mapLayer){\
	class_name *curTile = new class_name();\
    if (curTile && curTile->init(detector, tile_x, tile_y, mapLayer)){\
	       curTile->autorelease();\
	       return curTile;\
		    }\
    CC_SAFE_DELETE(curTile);\
    return NULL;\
}

class TileNode : public NodeWithCollision
{
public:
	virtual bool init(CollisionDetector *detector, const int &tile_x, const int &tile_y, cocos2d::TMXLayer* mapLayer);

	CREATE_FUNC_TILE(TileNode);

	virtual COLL_RET onCollision(cocos2d::Node* other){
		return RETAIN;
	};

	virtual b2Body *createBody();
	
	int getTileX() const { return _tile_x; }
	void setTileX(const int &val) { _tile_x = val; }
	int getTileY() const { return _tile_y; }
	void setTileY(const int &val) { _tile_y = val; }
	uint32_t getGid(){
		return _gid;
	}
private:
	
protected:
	virtual int getCategory(){
		return COLL_TILES;
	}
	int _tile_x;
	int _tile_y;
	uint32_t _gid;
	cocos2d::TMXLayer* _mapLayer;

};

#endif