#ifndef _MAP_LAYER_H_
#define _MAP_LAYER_H_

#include <2d/CCNode.h>
#include <2d/CCTMXTiledMap.h>
#include <2d/CCTMXObjectGroup.h>
#include <math/CCGeometry.h>
#include "NodeWithColl.h"
#include <vector>
#include <queue>


class TankSprite;
class TileNode;
class TankLayer;

#define CREATE_FUNC_EDGE(class_name)\
static class_name* create(CollisionDetector *detector, cocos2d::Vec2 pos){\
	class_name *curRef = new class_name();\
    if (curRef && curRef->init(detector, pos)){\
	       curRef->autorelease();\
	       return curRef;\
						    }\
    CC_SAFE_DELETE(curRef);\
    return NULL;\
}

class EdgeLine : public NodeWithCollision
{
public:
	virtual bool init(CollisionDetector *detector, cocos2d::Vec2 pos);
	virtual COLL_RET onCollision(cocos2d::Node* other){
		return RETAIN;
	};
};

class EdgeHorizontal : public EdgeLine
{
public:
	CREATE_FUNC_EDGE(EdgeHorizontal);
	virtual b2Body *createBody();
};

class EdgeVertical : public EdgeLine
{
public:
	CREATE_FUNC_EDGE(EdgeVertical);
	virtual b2Body *createBody();
};

struct DestroyedTile
{
	int key;
	int x;
	int y;
	uint32_t gid;
	DestroyedTile(int x_, int y_, uint32_t gid_, int key_) :x(x_), y(y_), gid(gid_), key(key_)
	{
	}
	DestroyedTile() :x(0), y(0), key(0), gid(0)
	{
	}
};

class CmpTile{
public:
	bool operator()(DestroyedTile &tile1, DestroyedTile &tile2){
		return tile1.key > tile2.key;
	}
};


class MapLayer : public cocos2d::Node 
{
public:

	virtual bool init();

	CREATE_FUNC(MapLayer);

	virtual COLL_RET onCollision(cocos2d::Node* other){
		return RETAIN;
	};

	
	void initWithMap(int leve);


	cocos2d::TMXObjectGroup* Objects() const { return _objects; }
	void Objects(cocos2d::TMXObjectGroup* val) { _objects = val; }

	void destroyTile(TileNode *tile);

	bool repairTile();

	void setTankLayer(TankLayer *tankLayer){
		_tankLayer = tankLayer;
	}
	virtual ~MapLayer();


private:
	void initHome();
	TileNode *createTile(uint32_t gid, int x, int y);
	int _leve;
	
	cocos2d::TMXTiledMap *_gameMap;

	cocos2d::TMXObjectGroup* _objects;
	
	
	
	EdgeLine *_top_edge;
	EdgeLine *_bottom_edge;
	EdgeLine *_left_edge;
	EdgeLine *_right_edge;
	TankLayer *_tankLayer;
	std::priority_queue <DestroyedTile, std::vector<DestroyedTile>, CmpTile> _destroyedTiles;

};
#endif