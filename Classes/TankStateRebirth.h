#ifndef _TANKSTATE_REBIRTH_H
#define _TANKSTATE_REBIRTH_H
#include "TankState.h"
#include <2d/CCAction.h>

class FinishRebirthTimer;
class TankStateRebirth :public TankState
{
	friend class FinishRebirthTimer;
public:
	virtual void fire();
	virtual bool beAttacked();
	virtual COLL_RET onConcreteCollision(cocos2d::Node* other);
	TankStateRebirth(TankState *oldState);
	~TankStateRebirth();
	void changeState();
    TankStateRebirth(TankNode *tank);
    
private:
	FinishRebirthTimer *_myTimer;
	int _tagBackup;
    void init();
};

class FinishRebirthTimer :public cocos2d::Node{
public:
	CREATE_FUNC(FinishRebirthTimer);
	void initTimer(TankStateRebirth *state, float dtime);
	void finish(float val);
private:
	TankStateRebirth *_state;


};
#endif