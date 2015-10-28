#ifndef _COLL_DETECTOR_H
#define _COLL_DETECTOR_H
#define PTM_RATIO 32.0
#include <2d/CCNode.h>
#include <2d/CCSprite.h>
#include <Box2D/Dynamics/b2Body.h>

class b2World;
class b2Body;
class b2Fixture;
class b2MouseJoint;
class GLESDebugDraw;
class MyContactListener;
class NodeWithCollision;

class CollisionDetector : public cocos2d::Node
{
public:
	virtual bool init();
	CREATE_FUNC(CollisionDetector);

	
	
	//solid
	b2Body* createBoxBody(NodeWithCollision *node, b2BodyType type, const float&x, const float &y, const float&width, const float&height);
	//hollow
	bool addBoxBoundry(NodeWithCollision *node, float x, float y, float width, float height);
	b2Body* createEdgeLine(NodeWithCollision *node, float x, float y, float width, float height);

	//void removeBody(NodeWithCollision *node);
	virtual ~CollisionDetector();
	void destroyBody(b2Body *body);
	void spriteDone(cocos2d::Node* sender);
private:
	void tick(float dt);
	MyContactListener *_contactListener;
	GLESDebugDraw *_debugDraw;
	b2World *_world;


};
#endif