#ifndef _AWARDFACTORY_H_
#define _AWARDFACTORY_H_



#include <string>
#include <unordered_map>
#include <math/Vec2.h>

class AwardNode;
class CollisionDetector;
class TankLayer;

#define REG_AWARD_CREATE(class_name) \
AwardFactory::registerCreate(class_name::create, #class_name);

typedef AwardNode* (*AwardFactoryCreate_PTR)(CollisionDetector *dectctor, cocos2d::Vec2 tankpos, AwardLayer *tankLayer);


class AwardFactory
{
public:
	AwardFactory();
	virtual ~AwardFactory();
	static bool registerCreate(AwardFactoryCreate_PTR createFunc, std::string paraType);
	static AwardNode *create(std::string tankType, CollisionDetector *dectctor, cocos2d::Vec2 pos, AwardLayer *layer);
private:
	static std::unordered_map<std::string, AwardFactoryCreate_PTR> _classInfoMap;
};


#endif