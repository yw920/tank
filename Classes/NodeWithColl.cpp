#include "NodeWithColl.h"



bool NodeWithCollision::init(CollisionDetector *detector)
{
	if (!Sprite::init())
	{
		return false;
	}
	_collDetector = detector;
	_body = createBody();
	return true;
}


NodeWithCollision::~NodeWithCollision()
{
	if (_collDetector != NULL){
		_collDetector->destroyBody(_body);
	}
}

