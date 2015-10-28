#ifndef _BULLET_H_
#define _BULLET_H_

#include <stdio.h>
#include "NodeWithColl.h"
#include "TankNode.h"
class MapLayer;

#define CREATE_FUNC_BULLET(class_name)\
static class_name* create(CollisionDetector *detector, TankNode *tank){\
	class_name *curRef = new class_name();\
    if (curRef && curRef->init(detector, tank)){\
	       curRef->autorelease();\
	       return curRef;\
						    }\
    CC_SAFE_DELETE(curRef);\
    return NULL;\
}

class Bullet : public NodeWithCollision
{
public:
    virtual bool init(CollisionDetector *detector, TankNode *tank);
	CREATE_FUNC_BULLET(Bullet);
	virtual COLL_RET onCollision(cocos2d::Node* other);
    
   
    //所属碰撞的类别
    virtual int getCategory(){
        return COLL_BULLET;
    }
	void setTank(TankNode *tank){
		_tank = tank;
	}

	virtual b2Body *createBody();
	virtual ~Bullet();
private:
	//子弹速度
	float _speed;

	TankNode *_tank;
	cocos2d::Size _size;
    
};

#endif 
