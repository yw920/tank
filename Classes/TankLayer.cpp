#include "TankLayer.h"
#include "MapLayer.h"
#include <random>
#include "TankAI.h"
#include "MapData.h"
#include "TankStateRebirth.h"
#include "TankMultiLife.h"
#include "TankAI.h"
#include "TankFactory.h"
#include "AwardLayer.h"
#include "StrategyAI.h"
#include "Config.h"
#include "MapLayer.h"
#include <2d/CCLabel.h>

using namespace std;
USING_NS_CC;

//warning 初始化这里可能有问题
void TankLayer::initTankLayer(int tankNum, int liveNum, AwardLayer *awardLayer)
{

	_awardLayer = awardLayer;
	if (_awardLayer != NULL)
	{
		_awardLayer->setTankLayer(this);
	}
	_tankNum = tankNum;

	_baseLiveNum = liveNum;
	//基准速度为15，每轮最高速度为45*3 = 35，理论最高速度为快速坦克的速度45*2 = 90
	_baseTankSpeed = 15.0f;
	//基准速度为130，每轮最高速度为130*3 = 390，理论最高速度为快速坦克的子弹速度390*1.2 = 468
	_baseBulletSpeed = 130.0f;

	_tankSpeedStep = _baseTankSpeed * 0.05f;
	_bulletSpeedStep = _baseBulletSpeed * 0.10f;
	_liveNumStep = 1;

	_curWave = 0;
	_curTankSpeed = _baseTankSpeed;
	_curBulletSpeed = _baseBulletSpeed;
	_curLiveNum = _baseLiveNum;
	_homes = MAPDATA->getEnemyHomes();
	
	_tankTypes = {"TankPlain", "TankFast", "TankAILife2", "TankAILife3" };
	_waveSize = _tankTypes.size() * _baseLiveNum;
	_tankId = 0;
	_deadNum = 0;
	_coins = 0;
	_isSleeping = false;
	
	REG_TANK_CREATE(TankPlain);
	REG_TANK_CREATE(TankFast);
	REG_TANK_CREATE(TankAILife2);
	REG_TANK_CREATE(TankAILife3);
	createAITank(0.0);
	this->schedule(schedule_selector(TankLayer::createAITank), 2, _baseLiveNum - 2, 2);

	TTFConfig ttfConfig("font/HKYuanMini.ttf", 15, GlyphCollection::DYNAMIC);
	_labelKillNum = Label::createWithTTF(ttfConfig,
		"0", TextHAlignment::RIGHT, winSize.width * 0.75);
	if (_labelKillNum) {
		_labelKillNum->setTextColor(Color4B(255, 128, 255, 255));
		_labelKillNum->setPosition(Vec2(wOrigin.x + winSize.width - 30, winSize.height - 75));
		_labelKillNum->setAnchorPoint(Vec2(0, 0.5));
		this->addChild(_labelKillNum);
	}

	_labelCoins = Label::createWithTTF(ttfConfig,
		"0", TextHAlignment::RIGHT, winSize.width * 0.75);
	if (_labelCoins) {
		_labelCoins->setTextColor(Color4B(255, 128, 255, 255));
		_labelCoins->setPosition(Vec2(wOrigin.x + winSize.width - 30, winSize.height - 115));
		_labelCoins->setAnchorPoint(Vec2(0, 0.5));
		this->addChild(_labelCoins);
	}
}

void TankLayer::createOneTank()
{
	std::random_device rd;
	if (_homes.empty()) return;
	//下次改为每个敌窝轮询地产生坦克
	int curHome = rd() % _homes.size();
	int curType = _tankId  % _waveSize;
	curType = curType / _baseLiveNum;

	//选择AI坦克类型
	TankAI* tankAI = (TankAI *)TankFactory::createTank(_tankTypes[curType], MAPDATA->getCollDetector(), this, _homes[curHome]);
	tankAI->setState(new TankStateRebirth(tankAI));
	//this->getParent()->addChild(tankAI, -1);
	this->addChild(tankAI);

	_liveTanks.insert(tankAI);
	if (_isSleeping)
	{
		tankAI->pause();
	}
	++_tankId;
}


void TankLayer::createAITank(float val)
{
	
	if (_tankId != 0 && _tankId % _waveSize == 0){
		//新的一波坦克
		++_curWave;
		_curTankSpeed = min(_baseTankSpeed + _tankSpeedStep * _curWave, _baseTankSpeed * 3.0f);
		_curBulletSpeed = min(_baseBulletSpeed + _bulletSpeedStep * _curWave, _baseBulletSpeed * 3.0f);
		for (int i = 0; i < _liveNumStep && (_curLiveNum + i) < 20; ++i)
		{
			createOneTank();
		}
		_curLiveNum = min(_curLiveNum + _liveNumStep, 20);

	}
	createOneTank();
	
}

bool TankLayer::init()
{
	if (!Node::init())
	{
		return false;
	}
	_mapLayer = nullptr;
	return true;
}

void TankLayer::releaseAITank(TankAI *_tankAI)
{
	_liveTanks.erase(_tankAI);
	++_deadNum;
	char sKillNum[20] = {0};
	sprintf(sKillNum, "%d", _deadNum);
	_labelKillNum->setString(sKillNum);
	

	_coins = _coins + _tankAI->getCoins();
	//用金币修补老巢附近的砖块
	while (_coins >= 6)
	{
		if (nullptr == _mapLayer) break;
		if (!_mapLayer->repairTile()) break;
		_coins = _coins - 6;
	}
	sprintf(sKillNum, "%d", _coins);
	_labelCoins->setString(sKillNum);
    //创建新的坦克
	createAITank(0.0);
	//生成奖励
	random_device rd;
	unsigned int godi = 100;
	godi = rd() % godi + 1;
	if (godi > 80)
	{
		if (_awardLayer != NULL)
		{
			_awardLayer->createAward(0.0f);
		}
	}
	//this->scheduleOnce(schedule_selector(TankLayer::createAITank), 1);
}

TankLayer::~TankLayer()
{
	for (auto curtank : _liveTanks)
	{
		curtank->setTankLayer(NULL);
	}
	if (_awardLayer != NULL)
	{
		_awardLayer->setTankLayer(NULL);
	}
	if (_mapLayer != nullptr)
	{
		_mapLayer->setTankLayer(nullptr);
	}
}

void TankLayer::pauseAITanks()
{
	unschedule(schedule_selector(TankLayer::resumeAITanks));
	for (auto curtank : _liveTanks)
	{
		curtank->pause();
	}
	_isSleeping = true;
	scheduleOnce(schedule_selector(TankLayer::resumeAITanks), 15.0f);
}

void TankLayer::resumeAITanks(float val)
{
	_isSleeping = false;
	for (auto curtank : _liveTanks)
	{
		curtank->resume();
	}
	
}

void TankLayer::setMapLayer(MapLayer *mapLayer)
{
	
	_mapLayer = mapLayer;
	if (_mapLayer != nullptr)
	{
		_mapLayer->setTankLayer(this);
	}
	
}

