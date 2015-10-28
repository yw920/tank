#include "CollDetector.h"
#include "MyContactListener.h"
#include "GLES-Render.h"
#include "Config.h"
#include "NodeWithColl.h"
#include <Box2D/Box2D.h>

USING_NS_CC;




b2Body * CollisionDetector::createBoxBody(NodeWithCollision *node, b2BodyType type, const float&x, const float &y, const float&width, const float&height)
{
	b2BodyDef spriteBodyDef;
	spriteBodyDef.type = type;
	spriteBodyDef.position.Set(x / PTM_RATIO,
		y / PTM_RATIO);
	spriteBodyDef.userData = node;
	b2Body *spriteBody = _world->CreateBody(&spriteBodyDef);


	b2PolygonShape spriteShape;
	spriteShape.SetAsBox(width / PTM_RATIO / 2,
		height / PTM_RATIO / 2);
	b2FixtureDef spriteShapeDef;
	spriteShapeDef.shape = &spriteShape;
	spriteShapeDef.density = 10.0;
	spriteShapeDef.isSensor = true;
	spriteBody->CreateFixture(&spriteShapeDef);
	//return spriteBody;
	return spriteBody;
}


bool CollisionDetector::addBoxBoundry(NodeWithCollision *node,  float x,  float y,  float width,  float height)
{
	x = x / PTM_RATIO;
	y = y / PTM_RATIO;
	width = width / PTM_RATIO;
	height = height / PTM_RATIO;
	b2BodyDef groundBodyDef;
	groundBodyDef.type = b2_staticBody;
	groundBodyDef.position.Set(x ,
		y);
	groundBodyDef.userData = node;
	b2Body *groundBody = _world->CreateBody(&groundBodyDef);

	//地面物体定义
	//b2BodyDef groundBodyDef;
	//groundBodyDef.position.Set(0, 0);
	//groundBody = m_world->CreateBody(&groundBodyDef);

	b2EdgeShape edgeShape;
	
	b2FixtureDef groundShapeDef;
	groundShapeDef.shape = &edgeShape;
	groundShapeDef.density = 10.0;
	groundShapeDef.isSensor = true;

	edgeShape.Set(b2Vec2(-width / 2.0f, -height / 2.0f), b2Vec2(-width / 2.0f, +height / 2.0f));
	groundBody->CreateFixture(&groundShapeDef);
	edgeShape.Set(b2Vec2(+width / 2.0f, +height / 2.0f), b2Vec2(-width / 2.0f, +height / 2.0f));
	groundBody->CreateFixture(&groundShapeDef);
	edgeShape.Set(b2Vec2(+width / 2.0f, +height / 2.0f), b2Vec2(+width / 2.0f, -height / 2.0f));
	groundBody->CreateFixture(&groundShapeDef);
	edgeShape.Set(b2Vec2(-width / 2.0f, -height / 2.0f), b2Vec2(+width / 2.0f, -height / 2.0f));
	groundBody->CreateFixture(&groundShapeDef);
	

	return true;
}


b2Body *CollisionDetector::createEdgeLine(NodeWithCollision *node, float x, float y, float width, float height)
{
	x = x / PTM_RATIO;
	y = y / PTM_RATIO;
	width = width / PTM_RATIO;
	height = height / PTM_RATIO;
	b2BodyDef groundBodyDef;
	groundBodyDef.type = b2_staticBody;
	groundBodyDef.position.Set(x,
		y);
	groundBodyDef.userData = node;
	b2Body *groundBody = _world->CreateBody(&groundBodyDef);
	b2EdgeShape edgeShape;

	b2FixtureDef groundShapeDef;
	groundShapeDef.shape = &edgeShape;
	groundShapeDef.density = 10.0;
	groundShapeDef.isSensor = true;
	
	edgeShape.Set(b2Vec2(0, 0), b2Vec2(width , height));
	groundBody->CreateFixture(&groundShapeDef);

	return groundBody;
}



bool CollisionDetector::init()
{
	
	bool doSleep = false;
	// Create a world
	b2Vec2 gravity = b2Vec2(0.0f, 0.0f);
	_world = new b2World(gravity);
	_world->SetAllowSleeping(doSleep);
	// Enable debug draw
	_debugDraw = new GLESDebugDraw(PTM_RATIO);
	_world->SetDebugDraw(_debugDraw);

	uint32 flags = 0;
	flags += b2Draw::e_shapeBit;
	_debugDraw->SetFlags(flags);

	this->schedule(schedule_selector(CollisionDetector::tick));

	_contactListener = new MyContactListener();
	_world->SetContactListener(_contactListener);

	return true;
}


void CollisionDetector::tick(float dt)
{

	_world->Step(dt, 10, 10);
	for (b2Body *b = _world->GetBodyList(); b; b = b->GetNext()) {

		if (b->GetType()!= b2_staticBody && b->GetUserData() != NULL) {
			Node *sprite = (Node *)b->GetUserData();
			if (NULL == sprite) continue;

			b2Vec2 b2Position = b2Vec2(sprite->getPosition().x / PTM_RATIO,
				sprite->getPosition().y / PTM_RATIO);
			float32 b2Angle = -1 * CC_DEGREES_TO_RADIANS(sprite->getRotation());

			b->SetTransform(b2Position, b2Angle);
		}
	}

	std::set<b2Body *> toDestroy;
	std::vector<MyContact>::iterator pos;
	for (pos = _contactListener->_contacts.begin();
		pos != _contactListener->_contacts.end(); ++pos) {
		MyContact contact = *pos;

		b2Body *bodyA = contact.fixtureA->GetBody();
		b2Body *bodyB = contact.fixtureB->GetBody();
		if (bodyA->GetUserData() != NULL && bodyB->GetUserData() != NULL) {
			Node *nodeA = (Node *)bodyA->GetUserData();
			Node *nodeB = (Node *)bodyB->GetUserData();
			try
			{
				auto *collA = dynamic_cast<NodeWithCollision *>(nodeA);
				if (nullptr == collA)
					throw std::runtime_error("NULL Pointer");
				COLL_RET ret = collA->onCollision(nodeB);
				switch (ret)
				{
				case RETAIN:
					break;
				case DESTROY:
					toDestroy.insert(bodyA);
					break;
				default:
					break;
				}
			}catch (std::exception& e)
			{
				CCLOG("error:the collision node not inherit the class NodeWithCollision. In File %s, Line %d.", __FILE__, __LINE__);
			}
            
			try{
				auto *collB = dynamic_cast<NodeWithCollision *>(nodeB);
				if (nullptr == collB)
					throw std::runtime_error("NULL Pointer");
				COLL_RET ret = collB->onCollision(nodeA);
				switch (ret)
				{
				case RETAIN:
					break;
				case DESTROY:
					toDestroy.insert(bodyB);
					break;
				default:
					break;
				}
			}catch (std::exception& e)
			{
				CCLOG("error:the collision node not implement the interface NodeWithCollision. In File %s, Line %d.", __FILE__, __LINE__);
			}
			
			
		}
	}

	for (auto body : toDestroy){
		Node *node = (Node *)body->GetUserData();
		node->removeFromParent();
		//_world->DestroyBody(body);
	}

}

CollisionDetector::~CollisionDetector()
{
	this->unschedule(schedule_selector(CollisionDetector::tick));
	for (b2Body *b = _world->GetBodyList(); b; b = b->GetNext()) {
		NodeWithCollision *sprite = (NodeWithCollision *)b->GetUserData();
		if (sprite == NULL) continue;
		sprite->setCollDetector(NULL);
	}
	delete _world;
	delete _debugDraw;
	delete _contactListener;
	
	//Layer::~Layer();
}

void CollisionDetector::destroyBody(b2Body *body)
{
	_world->DestroyBody(body);
}





