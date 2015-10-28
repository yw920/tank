#ifndef _TANK_STATE_NORMAL_H_
#define _TANK_STATE_NORMAL_H_
#include "TankState.h"
class TankStateNormal : public TankState
{
public:
	virtual void fire();
	virtual bool beAttacked();
	TankStateNormal(TankState *oldState);
	
	//virtual ~TankStateNormal(){}
};

#endif