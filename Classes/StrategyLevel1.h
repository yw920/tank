#ifndef _STRATEGYLEVEL1_H_
#define _STRATEGYLEVEL1_H_

//#include <2d/CCNode.h>
#include "StrategyAI.h"


class StrategyLevel1 : public StrategyAI{
public:
	static StrategyLevel1* create(TankAI *tankAI);
	virtual bool init(TankAI *tankAI);
	virtual COLL_RET onCollision(cocos2d::Node* other);
	virtual void stopTank(const TankDirection old_dir);

protected:
	//virtual void updateFire(float val);
	virtual void updateDir(float val);
	int _otherTag;
	float _baseSleepTime;
	//virtual void afterSleep(float val);
};
#endif