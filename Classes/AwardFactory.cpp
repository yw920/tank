#include "AwardNode.h"
#include "CollDetector.h"
#include "AwardFactory.h"
using namespace std;

unordered_map<string, AwardFactoryCreate_PTR> AwardFactory::_classInfoMap;

AwardNode * AwardFactory::create(string tankType, CollisionDetector *dectctor, cocos2d::Vec2 pos, AwardLayer *layer)
{
	if (_classInfoMap.find(tankType) != _classInfoMap.end()){
		return _classInfoMap[tankType](dectctor, pos, layer);
	}
	return NULL;
}

bool AwardFactory::registerCreate(AwardFactoryCreate_PTR createFunc, string paraType)
{
	_classInfoMap[paraType] = createFunc;
	return true;
}

