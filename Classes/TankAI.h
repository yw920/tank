#ifndef _TANK_AI_
#define _TANK_AI_
#include "TankNode.h"
#include "TankFactory.h"
#include <string>
class TankLayer;
class StrategyAI;

#define CREATE_FUNC_TANKAI(class_name)\
static TankAI* create(CollisionDetector *detector, TankLayer *tankLayer, cocos2d::Vec2 tankpos){\
	TankAI *curRef = new class_name();\
    if (curRef && curRef->init(detector, tankLayer, tankpos)){\
	       curRef->autorelease();\
	       return curRef;\
								    }\
    CC_SAFE_DELETE(curRef);\
    return NULL;\
};

class TankAI :public TankNode
{
public:
	virtual bool init(CollisionDetector *detector, TankLayer *tankLayer, cocos2d::Vec2 tankpos);
	virtual COLL_RET onCollision(cocos2d::Node* other);
	virtual void stopTank(const TankDirection dir);
	virtual int getCoins(){
		return _coins;
	}
	virtual ~TankAI();
	void setTankLayer(TankLayer * tankLayer){
		_tankLayer = tankLayer;
	}

	StrategyAI *getStrategy(){
		return _strategy;
	}
	virtual void pause();
	virtual void resume();
protected:
	int _coins;
	TankLayer *_tankLayer;
	StrategyAI *_strategy;
};


class TankPlain :public TankAI
{
public:
	CREATE_FUNC_TANKAI(TankPlain);
	virtual const char* getPicName() override{
		return "en1r.png";
	}
	virtual bool init(CollisionDetector *detector, TankLayer *tankLayer, cocos2d::Vec2 tankpos);
};


class TankFast : public TankAI
{
public:
	CREATE_FUNC_TANKAI(TankFast);
	virtual bool init(CollisionDetector *detector, TankLayer *tankLayer, cocos2d::Vec2 tankpos);
	virtual const char* getPicName() override{
		return "tank_fast.png";
	}

};
#endif