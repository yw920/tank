#include "Bullet.h"
#include "MapLayer.h"
#include <cocos2d.h>
#include <math/CCGeometry.h>
#include "MapData.h"

USING_NS_CC;



b2Body * Bullet::createBody()
{
	return _collDetector->createBoxBody(this, b2_dynamicBody, this->getPositionX(), this->getPositionY(), _size.width, _size.height);
}

bool Bullet::init(CollisionDetector *detector, TankNode *tank)
{
	_tank = tank;

	_speed = _tank->getBulletSpeed();

	Vec2 startPos = tank->getPosition();

	Sprite *bullet = Sprite::createWithSpriteFrameName("bullet2.png");
	float tmpScale = MAPDATA->getTile_w_in_scene() *0.6f / bullet->getContentSize().width;
	_size = bullet->getContentSize() * tmpScale;
	bullet->setScale(tmpScale);

	float tileWidth = MAPDATA->getTile_w_in_scene();
	float maxdis = MAPDATA->getMaxDistanceInMap();
	float duration = maxdis / _speed;

	this->addChild(bullet, -1);
	this->setColor(cocos2d::Color3B(255, 0, 0));

	MoveBy *moveAction;
	switch (tank->getDirection()) {
	case TUP:
		this->setPosition(startPos.x, startPos.y + tileWidth);
		moveAction = MoveBy::create(duration, Vec2(0, maxdis));
		
		break;
	case TDOWN:
		this->setPosition(startPos.x, startPos.y - tileWidth);
		moveAction = MoveBy::create(duration, Vec2(0, -maxdis));
		break;
	case TLEFT:
		this->setPosition(startPos.x - tileWidth, startPos.y);
		moveAction = MoveBy::create(duration, Vec2(-maxdis, 0));
		break;
	case TRIGHT:
		this->setPosition(startPos.x + tileWidth, startPos.y);
		moveAction = MoveBy::create(duration, Vec2(maxdis, 0));
		break;
	default:
		break;
	}

	if (!NodeWithCollision::init(detector))
	{
		return false;
	}
	this->setTag(_tank->getBulletTag());
	this->runAction(moveAction);
	
	return true;
}


COLL_RET Bullet::onCollision(cocos2d::Node *other)
{
	const int otherTag = other->getTag();
	if (otherTag == COLL_TILE_GRASS
		|| otherTag == COLL_TILE_WATER
		|| otherTag == COLL_REBIRTH
		|| otherTag == COLL_AWARDS)
		return RETAIN;
	if (COLL_EDGE_BOTTOM == otherTag
		||COLL_EDGE_LEFT == otherTag
		||COLL_EDGE_RIGHT == otherTag
		||COLL_EDGE_TOP == otherTag)
	{
		return DESTROY;
	}
	
	if (COLL_BULLET == _tag || COLL_BULLET_IRON == _tag)
	{
		
		if (otherTag != COLL_FELLOWS &&otherTag != COLL_BULLET &&otherTag != COLL_BULLET_IRON)
		{
			ParticleSystemQuad * particle = ParticleSystemQuad::create("bullet_exp.plist");
			Sprite *tmpSprite = Sprite::createWithSpriteFrameName("bullet_exp.png");
			tmpSprite->setVisible(false);
			particle->setTextureWithRect(tmpSprite->getTexture(), tmpSprite->getTextureRect());
			particle->addChild(tmpSprite);
			particle->setScale(0.05f);
			particle->setPosition(this->getPosition());
			this->getParent()->addChild(particle, 2);
			particle->setAutoRemoveOnFinish(true);
			return DESTROY;
		}
	}
	else if (COLL_BULLET_ENEMIES == _tag)
	{   
		if (otherTag != COLL_ENEMIES && otherTag != COLL_BULLET_ENEMIES)
		{
			
			ParticleSystemQuad * particle = ParticleSystemQuad::create("bullet_exp.plist");
			Sprite *tmpSprite = Sprite::createWithSpriteFrameName("bullet_exp.png");
			tmpSprite->setVisible(false);
			particle->setTextureWithRect(tmpSprite->getTexture(), tmpSprite->getTextureRect());
			particle->addChild(tmpSprite);
			particle->setScale(0.05f);
			particle->setPosition(getPosition());
			this->getParent()->addChild(particle, 2);
			particle->setAutoRemoveOnFinish(true);
			return DESTROY;
		}
	}	
	return RETAIN;
}

Bullet::~Bullet()
{
	if (_tank != NULL){
		_tank->eraseBullet(this);
	}
}
