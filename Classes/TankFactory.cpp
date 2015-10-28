#include "TankAI.h"
#include "CollDetector.h"
#include "TankLayer.h"
using namespace std;

unordered_map<string, FactoryCreate_PTR> TankFactory::_classInfoMap;

TankAI * TankFactory::createTank(string tankType, CollisionDetector *dectctor, TankLayer *tankLayer, cocos2d::Vec2 tankpos)
{
	if (_classInfoMap.find(tankType) != _classInfoMap.end()){
		return _classInfoMap[tankType](dectctor, tankLayer,  tankpos);
	}
	return NULL;
}

bool TankFactory::registerCreate(FactoryCreate_PTR tankCreate, std::string tankType)
{
	_classInfoMap[tankType] = tankCreate;
	return true;
}

