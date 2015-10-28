#include "StrategyLevel1.h"
#include "TankNode.h"
#include "MapData.h"

StrategyLevel1* StrategyLevel1::create(TankAI *tankAI)
{
	StrategyLevel1 *strategyAI = new StrategyLevel1();
	if (strategyAI && strategyAI->init(tankAI))
	{
		strategyAI->autorelease();
		return strategyAI;
	}

	CC_SAFE_DELETE(strategyAI);
	return NULL;
}

bool StrategyLevel1::init(TankAI *tankAI)
{
	if (!StrategyAI::init(tankAI))
	{
		return false;
	}
	_baseSleepTime = 2.0f;
	_maxSleepTime = 2.0f;
	return true;
}


COLL_RET StrategyLevel1::onCollision(cocos2d::Node* other)
{
	_otherTag = 0;
	const int OTHER_TAG = other->getTag();
	if (NodeWithCollision::COLL_TILE_BRICK == OTHER_TAG
		|| NodeWithCollision::COLL_TILE_HOME == OTHER_TAG
		|| NodeWithCollision::COLL_FELLOWS == OTHER_TAG)
	{
		_otherTag = OTHER_TAG;
	}

	return UNKNOWN;
}

void StrategyLevel1::updateDir(float val)
{
	_tankAI->stopAllActionsByTag(TankNode::ACTION_MOVE);
	TankDirection old_dir = _tankAI->getDirection();
	if (TDOWN == old_dir)
	{
		_tankAI->move();
		return;
	}
	if (TUP == old_dir)
	{
		StrategyAI::updateDir(0.0f);
		return;
	}
	int tankX = MAPDATA->getNearestTileX(_tankAI->getPosition().x);
	int homeX = MAPDATA->getHomeTileXStart() + 2;
	if ((tankX < homeX && TRIGHT == old_dir)
		|| (tankX >= homeX && TLEFT == old_dir))
	{
		_tankAI->move();
		return;
	}
	std::random_device rd;
	int godi = rd() % 8;
	TankDirection tarDir;
	if (TRIGHT == old_dir)
	{
		
		switch (godi)
		{
		case 0:
			tarDir = TUP;
			break;
		case 1:
			tarDir = TRIGHT;
			break;
		case 2:
		case 3:
		case 4:
			tarDir = TLEFT;
			break;
		default:
			tarDir = TDOWN;
			break;
		}
		
	}
	else{
		switch (godi)
		{
		case 0:
			tarDir = TUP;
			break;
		case 1:
			tarDir = TLEFT;
			break;
		case 2:
		case 3:
		case 4:
			tarDir = TRIGHT;
			break;
		default:
			tarDir = TDOWN;
			break;
		}
	}
	_tankAI->rotateWithAdjust(tarDir, 0.1f);

}

void StrategyLevel1::stopTank(const TankDirection old_dir)
{
	_tankAI->stopAllActionsByTag(TankNode::ACTION_MOVE);
	std::random_device rd;
	if (_isSleeping) return;
	_isSleeping = true;
	if (_otherTag != 0)
	{
		
		//_tankAI->fire();
		float sleepTime = (rd() % 100 + 1.0f) / 100.0f * _maxSleepTime + _baseSleepTime;
		_tarDir = old_dir;
		int godi = rd() % 6;
		switch (godi){
		case 0:
			_tarDir = (TankDirection)((_tarDir + 1) % TSIZE);
			break;
		case 1:
			_tarDir = (TankDirection)((_tarDir + 2) % TSIZE);
			break;
		case 2:
			_tarDir = (TankDirection)((_tarDir + 3) % TSIZE);
			break;
		default:
			break;
		}
		scheduleOnce(schedule_selector(StrategyAI::afterSleep), sleepTime);
		return;
	}

	switch (old_dir)
	{
	case TLEFT:
	{
		int godi = rd() % 6;
		switch (godi)
		{
		case 0:
			_tarDir = TUP;
			break;
		case 1:
		case 2:
			_tarDir = TRIGHT;
			break;
		default:
			_tarDir = TDOWN;
			break;
		}
	}
	break;
	case TRIGHT:
	{
		int godi = rd() % 6;
		switch (godi)
		{
		case 0:
			_tarDir = TUP;
			break;
		case 1:
		case 2:
			_tarDir = TLEFT;
			break;
		default:
			_tarDir = TDOWN;
			break;
		}

	}
	break;
	case TUP:
	{
		int godi = rd() % 5;
		switch (godi)
		{
		case 0:
			_tarDir = TRIGHT;
			break;
		case 1:
			_tarDir = TLEFT;
			break;
		default:
			_tarDir = TDOWN;
			break;
		}

	}
	break;

	case TDOWN:
	{
		int godi = rd() % 5;
		switch (godi)
		{
		case 0:
			_tarDir = TUP;
			break;
		case 1:
		case 2:
			_tarDir = TLEFT;
			break;
		default:
			_tarDir = TRIGHT;
			break;
		}

	}
	break;
	}

	float sleepTime = (rd() % 100 + 1.0f) / 100.0f * _maxSleepTime + 0.5f;
	scheduleOnce(schedule_selector(StrategyAI::afterSleep), sleepTime);
}


