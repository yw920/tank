#ifndef _STRATEGYAI_H_
#define _STRATEGYAI_H_

//#include <2d/CCNode.h>
#include "NodeWithColl.h"
#include "TankAI.h"


class StrategyAI: public cocos2d::Node{
public:
	static StrategyAI* create(TankAI *tankAI);
	virtual bool init(TankAI *tankAI);
	virtual COLL_RET onCollision(cocos2d::Node* other){
		return UNKNOWN;
	}
	virtual void stopTank(const TankDirection old_dir);

	virtual void afterSleep(float val);
	virtual void pause();
	virtual void resume();

protected:
	virtual void updateFire(float val);
	virtual void updateDir(float val);
	
	TankAI *_tankAI;
	TankDirection _tarDir;
	bool _isSleeping;
	float _maxSleepTime;
};
#endif