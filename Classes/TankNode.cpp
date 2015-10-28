#include "TankNode.h"
#include <2d/CCTMXLayer.h>
#include <2d/CCLayer.h>
#include <cocos2d.h>

#include <math.h>
#include "MapLayer.h"
#include "TileNode.h"
#include "TileHome.h"
#include "Bullet.h"
#include <string>
#include <sstream>
#include "MapData.h"

USING_NS_CC;
using namespace std;

b2Body * TankNode::createBody()
{
	return _collDetector->createBoxBody(this, b2_dynamicBody, getPositionX(), getPositionY(), MAPDATA->getTile_w_in_scene() * 2.0f, MAPDATA->getTile_h_in_scene() * 2.0f);
}


bool TankNode::init(CollisionDetector *dectctor, cocos2d::Vec2 tankpos)
{
	this->setPosition(tankpos);
	if (!NodeWithCollision::init(dectctor))
	{
		return false;
	}
	_bullets.clear();
	_ref_x = tankpos.x;
	_state = NULL;
	_ref_y = tankpos.y;
	_bulletSpeed = 130.0f;
	_canFire = true;
	_speed = 3.0f;
	Sprite *TankNode = Sprite::createWithSpriteFrameName(getPicName());
	//_scale = scale;
	_scale = MAPDATA->getTile_w_in_scene() * 2.0f / TankNode->getContentSize().width;
	TankNode->setScale(_scale);
	
	this->addChild(TankNode, 0);
	this->setCascadeOpacityEnabled(true);
	_duration = 0.3f;
	_life = 1;

	stringstream ss;
	ss << _ID;
	_IDStr = ss.str();

	schedule(schedule_selector(TankNode::adjusting), 0.0f);

	return true;
}

float TankNode::ROTATION_TANK[TSIZE] = { -90.0f, 0.0f, 180.0f, 90.0f };

cocos2d::Vec2 TankNode::getAdjustPos(TankDirection targetDir){
	float tankX = getPositionX();
	float tankY = getPositionY();
	if (_tankdir == targetDir) return Vec2(tankX, tankY);
	if (3 - _tankdir == targetDir) return Vec2(tankX, tankY);
	switch (targetDir)
	{
	case TLEFT:
	case TRIGHT:
		tankY = MAPDATA->getNearestWorldY(tankY);
		break;
	case TUP:
	case TDOWN:
		tankX = MAPDATA->getNearestWorldX(tankX);
		break;
	default:
		break;
	}
	
	return  Vec2(tankX, tankY);
}

cocos2d::Vec2 TankNode::getAdjustPos()
{
	float tankX = getPositionX();
	float tankY = getPositionY();
	switch (_tankdir)
	{
	case TLEFT:
	case TRIGHT:
		tankY = MAPDATA->getNearestWorldY(tankY);
		break;
	case TUP:
	case TDOWN:
		tankX = MAPDATA->getNearestWorldX(tankX);
		break;
	default:
		break;
	}
	return  Vec2(tankX, tankY);
}


void TankNode::move(TankDirection dir, const float &step, const float &duration)
{
	cocos2d::MoveBy* curAction;
	switch (dir)
	{
	case TLEFT:
		curAction = MoveBy::create(duration, Vec2(-step, 0.0f));
		
		break;
	case TRIGHT:
		curAction = MoveBy::create(duration, Vec2(step, 0.0f));
		
		break;
	case TUP:
		curAction = MoveBy::create(duration, Vec2(0.0f, step));
	

		break;
	case TDOWN:
		curAction = MoveBy::create(duration, Vec2(0.0f, -step));
		break;
	}
	//CCLOG("action reference4: %d", _curAction->getReferenceCount());
	curAction->setTag(ACTION_MOVE);
	this->runAction(curAction);
	//CCLOG("action reference5: %d", _curAction->getReferenceCount());
}

void TankNode::move()
{
	move(_tankdir, _duration*_speed, _duration);
}

void TankNode::fire()
{
	assert(_state != NULL);
	_state->fire();

}

int TankNode::rotateWithAdjust(TankDirection targetDir, float duration)
{
	_canFire = false;
	unschedule(schedule_selector(TankNode::adjusting));
	if (targetDir == _tankdir)
	{
		afterRotate(1.0f);
		return -1;
	}
	stopAllActionsByTag(ACTION_ROTATE);
	adjusting(0.0);
	Vec2 adjustPos = getAdjustPos(targetDir);
	Spawn *spawn = Spawn::createWithTwoActions(RotateTo::create(duration, ROTATION_TANK[targetDir]), MoveTo::create(duration, adjustPos));
	spawn->setTag(ACTION_ROTATE);
	_ref_x = adjustPos.x;
	_ref_y = adjustPos.y;
	_tankdir = targetDir;
	runAction(spawn);
	scheduleOnce(schedule_selector(TankNode::afterRotate), duration*0.98f);
	return 0;
}

void TankNode::afterRotate(float val)
{
	_canFire = true;
	schedule(schedule_selector(TankNode::adjusting), 0.0f);
	adjusting(0.0);
	move();
}

void TankNode::adjusting(float val)
{
	switch (_tankdir)
	{
	case TLEFT:
	case TRIGHT:
		setPositionY(_ref_y);
		break;
	case TUP:
	case TDOWN:
		//tankX = MAPDATA->getNearestWorldX(tankX);
		setPositionX(_ref_x);
		break;
	}
	setRotation(ROTATION_TANK[_tankdir]);
}

void TankNode::setState(TankState *tankState)
{
	if (nullptr != _state)
	{
		CC_SAFE_DELETE(_state);
	}
	
	_state = tankState;

}

TankNode::~TankNode()
{
	if (nullptr != _state)
	{
		CC_SAFE_DELETE(_state);
	}
	for (auto bullet:_bullets)
	{
		bullet->setTank(NULL);
	}
	unscheduleAllCallbacks();
	stopAllActions();
}

COLL_RET TankNode::onCollision(cocos2d::Node* other)
{
	COLL_RET subret = _state->onConcreteCollision(other);
	if (subret != UNKNOWN) return subret;
	const int OTHER_TAG = other->getTag();
	if (OTHER_TAG == COLL_EDGE_BOTTOM)
	{
		stopTank(TDOWN);
	}
	else if (OTHER_TAG == COLL_EDGE_TOP)
	{
		stopTank(TUP);
	}
	else if (OTHER_TAG == COLL_EDGE_LEFT)
	{
		stopTank(TLEFT);
	}
	else if (OTHER_TAG == COLL_EDGE_RIGHT)
	{
		stopTank(TRIGHT);
	}
	else if (OTHER_TAG == COLL_TILES || OTHER_TAG == COLL_TILE_WATER || OTHER_TAG == COLL_TILE_BRICK)
	{
		TileNode *curTile = (TileNode *)other;
		TankDirection otherDir = getOtherDirection(curTile->getTileX(), curTile->getTileY());
		if (otherDir != TSIZE)
		{
			stopTank(otherDir);
		}
		
	}
	else if (OTHER_TAG == COLL_FELLOWS || OTHER_TAG == COLL_ENEMIES || OTHER_TAG == COLL_REBIRTH)
	{
		stopTank(getOtherTankDirection(other->getPosition()));
		return RETAIN;
	}
	else if (OTHER_TAG == COLL_TILE_HOME)
	{
		TileHome *tileHome = (TileHome *)other;
		int tank_tile_x = MAPDATA->getNearestTileX(getPositionX());
		int tank_tile_y = MAPDATA->getNearestTileY(getPositionY());
		int home_x = tileHome->getTileX();
		int home_y = tileHome->getTileY();
		if (tank_tile_x + 2 == home_x && (tank_tile_y == home_y || tank_tile_y + 1 == home_y))
		{
			stopTank(TRIGHT);
		}
		else if (tank_tile_x - 2 == home_x && (tank_tile_y == home_y || tank_tile_y + 1 == home_y))
		{
			stopTank(TLEFT);
		}
		else if (tank_tile_y + 2 == home_y && (tank_tile_x >= home_x - 1 && tank_tile_x <= home_x + 1))
		{
			stopTank(TDOWN);
		}
		return RETAIN;
	}
	return RETAIN;
}

TankDirection TankNode::getOtherTankDirection(Vec2 otherWorldXY)
{
	int other_x = MAPDATA->getNearestTileX(otherWorldXY.x);
	int other_y = MAPDATA->getNearestTileY(otherWorldXY.y);
	if (getOtherDirection(other_x-1, other_y-1) == _tankdir) return _tankdir;
	if (getOtherDirection(other_x - 1, other_y) == _tankdir) return _tankdir;
	if (getOtherDirection(other_x, other_y-1) == _tankdir) return _tankdir;
	return getOtherDirection(other_x, other_y);
}

TankDirection TankNode::getOtherDirection(int otherTileX, int otherTileY)
{
	int tank_tile_x = MAPDATA->getNearestTileX(getPositionX());
	int tank_tile_y = MAPDATA->getNearestTileY(getPositionY());
	
	if (tank_tile_x - otherTileX == 2)
	{
		//left of tank
		if (tank_tile_y == otherTileY || tank_tile_y - 1 == otherTileY)
		{
			return TLEFT;
		}

	}

	else if (otherTileX - tank_tile_x == 1)
	{
		//right of tank
		if (tank_tile_y == otherTileY || tank_tile_y - 1 == otherTileY)
		{
			return TRIGHT;
		}

	}


	else if (tank_tile_y - otherTileY == 2)
	{
		//up of tank
		if (tank_tile_x == otherTileX || tank_tile_x - 1 == otherTileX)
		{
			return TUP;
		}


	}


	else if (otherTileY - tank_tile_y == 1){
		//down of tank
		if (tank_tile_x == otherTileX || tank_tile_x - 1 == otherTileX)
		{
			return TDOWN;
		}
	}
	return TSIZE;
}

//void TankNode::finishRebirth(float val)
//{
//	_tank->stopAction(_fadeForever);
//	runAction(FadeTo::create(0.1, 255));
//	setState(new TankStateNormal(this));
//}

