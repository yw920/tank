#include "StrategyAI.h"
//#include "TankAI.h"
StrategyAI* StrategyAI::create(TankAI *tankAI)
{
	StrategyAI *strategyAI = new StrategyAI();
	if (strategyAI && strategyAI->init(tankAI))
	{
		strategyAI->autorelease();
		return strategyAI;
	}

	CC_SAFE_DELETE(strategyAI);
	return NULL;
}

bool StrategyAI::init(TankAI *tankAI)
{
	if (!Node::init())
	{
		return false;
	}
	_tankAI = tankAI;
	_maxSleepTime = 2.0f;
	_isSleeping = false;
	std::random_device rd;
	this->schedule(schedule_selector(StrategyAI::updateFire), 1.0f + 2.0f * (rd() % 100 + 1.0f) / 100.0f, CC_REPEAT_FOREVER, 0);
	this->schedule(schedule_selector(StrategyAI::updateDir), 5.0f, CC_REPEAT_FOREVER, 0);
	_tankAI->setDuration(5.0f);
	updateDir(5.0f);
	return true;
}


void StrategyAI::updateFire(float val)
{
	_tankAI->fire();
	
}

void StrategyAI::stopTank(const TankDirection old_dir)
{
	
	     _tankAI->stopAllActionsByTag(TankNode::ACTION_MOVE); 
		if (_isSleeping) return;
		_isSleeping = true;
		std::random_device rd;

		//改变坦克方向

		switch (old_dir)
		{
		case TLEFT:
		{
			int godi = rd() % 5;
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
				_tarDir = TDOWN;
				break;
			}

		}
		break;
		case TUP:
		{
			int godi = rd() % 4;
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
		//让坦克随机休息一段时间

		float sleepTime = (rd() % 100 + 1.0f) / 100.0f * _maxSleepTime + 0.5f;

		scheduleOnce(schedule_selector(StrategyAI::afterSleep), sleepTime);
}

void StrategyAI::updateDir(float val)
{
	//3,2,2,1
	_tankAI->stopAllActionsByTag(TankNode::ACTION_MOVE);
	std::random_device rd;
	int godi = rd() % 8;
	TankDirection tarDir;
	switch (godi)
	{
	case 0:
		tarDir = TUP;

		break;
	case 1:
	case 2:
		tarDir = TLEFT;

		break;
	case 3:
	case 4:
		tarDir = TRIGHT;

		break;
	default:
		tarDir = TDOWN;
		break;
	}
	_tankAI->rotateWithAdjust(tarDir, 0.1f);
}

void StrategyAI::afterSleep(float val)
{
	_tankAI->rotateWithAdjust(_tarDir, 0.1f);
	_isSleeping = false;
}

void StrategyAI::pause()
{
	Node::pause();
	_isSleeping = true;
}

void StrategyAI::resume()
{
	Node::resume();
	_isSleeping = false;
}



