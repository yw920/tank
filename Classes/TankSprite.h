#ifndef _TANK_SPRITE_H_
#define _TANK_SPRITE_H_

#include <2d/CCNode.h>
#include <2d/CCSprite.h>
#include <2d/CCTMXTiledMap.h>
#include <2d/CCActionInterval.h>
#include <math/CCGeometry.h>
#include "NodeWithColl.h"
#include "TankNode.h"


class TankSprite: public TankNode
{
    
public:
	
       
    CREATE_FUNC_TANK(TankSprite);
    
    //射击事件
	//virtual void fire();
	virtual bool init(CollisionDetector *dectctor, cocos2d::Vec2 tankpos);
   
    //移动方向函数
    void onMove(const float& x, const float &y);
	void moveUpdate(float val);
	virtual void move(TankDirection dir, const float &step, const float &duration);
	virtual COLL_RET onCollision(cocos2d::Node* other);

	int getBulletAwardNum(){
		return _bulletAwardNum;
	}

	bool killLife();

	void setBulletAwardNum(int num);
protected:
	virtual void stopTank(const TankDirection dir);
	
    TankKind _tankKind;
	
	
	float _lastX;
	float _lastY;

	int _bulletAwardNum;

};
#endif

