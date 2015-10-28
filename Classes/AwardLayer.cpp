
#include "AwardLayer.h"
#include <random>
#include "TankAI.h"
#include "MapData.h"
#include "TankStateRebirth.h"
#include "AwardNode.h"
#include <vector>
#include <algorithm>
#include "AwardNode.h"
#include "AwardFactory.h"
#include "TankLayer.h"

using namespace std;
USING_NS_CC;


bool AwardLayer::init()
{
	if (!Node::init())
	{
		return false;
	}
	_homes = MAPDATA->getAwardHomes();
	_types = {"AwardBullet", "AwardUnmove"};
	_tankLayer = NULL;
	_lastPos = 0;
	
	REG_AWARD_CREATE(AwardBullet);
	REG_AWARD_CREATE(AwardUnmove);
	
	//this->schedule(schedule_selector(AwardLayer::initAITank), 1, _liveNum - 1, 2);
	return true;
}

void AwardLayer::releaseAward(AwardNode *awardNode)
{
	//_homes.emplace_back(awardNode->getPosition());
	_awards.erase(awardNode);
}

AwardLayer::~AwardLayer()
{
	for (auto &award:_awards)
	{
		award->setAwardLayer(NULL);
	}
	if (_tankLayer != NULL)
	{
		_tankLayer->setAwardLayer(NULL);
	}
}

void AwardLayer::createAward(float val)
{
	std::random_device rd;
	if (_homes.empty()) return;
	if (_types.empty()) return;
	//下次改为每个敌窝轮询地产生坦克
	unsigned int curHome;
	do 
	{
		curHome = rd() % _homes.size();
	} while (curHome == _lastPos);
	
	//选择AI奖励类型
	unsigned int curType =_types.size(); 
	curType = rd() % curType;
	
	AwardNode* myAward = AwardFactory::create(_types[curType], MAPDATA->getCollDetector(), _homes[curHome], this);
	this->addChild(myAward, 0);
	_lastPos = curHome;
	_awards.insert(myAward);
}

void AwardLayer::pauseAITanks()
{
	if (_tankLayer != NULL)
	{
		_tankLayer->pauseAITanks();
	}
}


