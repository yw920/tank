#ifndef _TANKFACTORY_H_
#define _TANKFACTORY_H_

#include <string>
#include <unordered_map>
#include <math/Vec2.h>

class TankAI;
class CollisionDetector;
class TankLayer;

#define REG_TANK_CREATE(class_name) \
TankFactory::registerCreate(class_name::create, #class_name);

typedef TankAI* (*FactoryCreate_PTR)(CollisionDetector *dectctor, TankLayer *tankLayer, cocos2d::Vec2 tankpos);


class TankFactory
{
public:
	TankFactory();
	virtual ~TankFactory();
	static bool registerCreate(FactoryCreate_PTR tankCreate, std::string tankType);
	static TankAI *createTank(std::string tankType, CollisionDetector *dectctor, TankLayer *tankLayer, cocos2d::Vec2 tankpos);
private:
	static std::unordered_map<std::string, FactoryCreate_PTR> _classInfoMap;
};


#endif