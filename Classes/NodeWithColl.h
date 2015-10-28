#ifndef _NODE_WITH_COLL_H
#define _NODE_WITH_COLL_H
#define PTM_RATIO 32.0
#include <2d/CCSprite.h>
#include "CollDetector.h"
enum COLL_RET{
	RETAIN,
	DESTROY,
	UNKNOWN
};

#define CREATE_FUNC_NWC(class_name)\
static class_name* create(CollisionDetector *detector){\
	class_name *curRef = new class_name();\
    if (curRef && curRef->init(detector)){\
	       curRef->autorelease();\
	       return curRef;\
						    }\
    CC_SAFE_DELETE(curRef);\
    return NULL;\
}


class NodeWithCollision : public cocos2d::Sprite
{
public:
	virtual bool init(CollisionDetector *detector);
	//CREATE_FUNC(NodeWithCollision);
	static const int COLL_FELLOWS = 0x0001;
	static const int COLL_ENEMIES = 0xFFFE;
	static const int COLL_TILES = 0x0004;
	static const int COLL_TILE_GRASS = 0x0005;
	static const int COLL_TILE_WATER = 0x0006;
	static const int COLL_TILE_HOME = 0x0007;
	static const int COLL_TILE_BRICK = 0x0008;
    static const int COLL_BULLET = 0x0010;
	static const int COLL_BULLET_IRON = 0x0011;
	static const int COLL_BULLET_ENEMIES = 0xFFEF;
	static const int COLL_EDGE_LEFT = 0x0014;
	static const int COLL_EDGE_RIGHT = 0x0015;
	static const int COLL_EDGE_TOP = 0x0016;
	static const int COLL_EDGE_BOTTOM = 0x0017;
	static const int COLL_REBIRTH = 0x0020;
	static const int COLL_AWARDS = 0x0030;
	//static const int COLL_ALL = 0x000F;
	//if return true, the node will be destroyed.
	virtual b2Body *createBody() = 0;
	virtual COLL_RET onCollision(cocos2d::Node* other) = 0;
	void setCollDetector(CollisionDetector *val) { _collDetector = val; }
	virtual ~NodeWithCollision();
protected:
	CollisionDetector *_collDetector;
	b2Body *_body;

};


#endif