#ifndef _TANK_STATE_H_
#define _TANK_STATE_H_
class TankNode;
class TankSprite;
#include "NodeWithColl.h"
class TankState{
public:
	virtual void fire();
	virtual bool beAttacked();
	virtual COLL_RET onConcreteCollision(cocos2d::Node* other){
		return UNKNOWN;
	}
	TankNode *getTank(){
		return _tank;
	}
	virtual ~TankState(){};
	//TankState(TankState *oldState);
protected:
    TankNode *_tank;
};


class TankStateBullet: public TankState{
public:
	virtual void fire();
	virtual bool beAttacked();
	TankStateBullet(TankSprite *oldState);
	virtual ~TankStateBullet();
private:
	float _bulletSpeedBackup;
	int _bulletTagBackup;
	TankSprite *_tankSprite;
};

#endif