#ifndef _TANKNODE_H_
#define _TANKNODE_H_

#include <2d/CCNode.h>
#include <2d/CCSprite.h>
#include <2d/CCTMXTiledMap.h>
#include <2d/CCActionInterval.h>
#include <math/CCGeometry.h>
#include "NodeWithColl.h"
#include "TankState.h"
#include <unordered_set>

class MapLayer;
class Bullet;

enum TankKind{
	kBorn = 1,
	kPlusStarOne,
	kPlusStarTwo,
	kPlusStarThree
};

//3 －当前方向＝相反方向
enum TankDirection{
	TLEFT = 0,
	TUP = 1,
	TDOWN = 2,
	TRIGHT = 3,
	TSIZE
};

#define CREATE_FUNC_TANK(class_name)\
static class_name* create(CollisionDetector *detector, cocos2d::Vec2 tankpos){\
	class_name *curRef = new class_name();\
    if (curRef && curRef->init(detector, tankpos)){\
	       curRef->autorelease();\
	       return curRef;\
				    }\
    CC_SAFE_DELETE(curRef);\
    return NULL;\
}


class TankNode : public NodeWithCollision
{

public:
	static float ROTATION_TANK[TSIZE];

	virtual bool init(CollisionDetector *dectctor, cocos2d::Vec2 tankpos);

	virtual b2Body *createBody();

	virtual COLL_RET onCollision(cocos2d::Node* other);

	//射击事件
	virtual void fire();
	virtual ~TankNode();


	//移动方向函数
	virtual void move(TankDirection dir, const float &step, const float &duration);
	void move();

	cocos2d::Vec2 getAdjustPos(TankDirection targetDir);
	cocos2d::Vec2 getAdjustPos();

	int rotateWithAdjust(TankDirection targetDir, float duration);
	virtual void stopTank(const TankDirection dir){}
	void afterRotate(float val);
	void adjusting(float val);

	TankDirection getDirection(){
		return _tankdir;
	}
	void setDirection(TankDirection dir){
		_tankdir = dir;
	}

	TankDirection getOtherTankDirection(cocos2d::Vec2 otherWorldXY);
	TankDirection getOtherDirection(int tile_x, int tile_y);

	void setState(TankState *tankState);

	void insertBullet(Bullet *bullet){
		_bullets.insert(bullet);
	}
	bool CanFire(){
		return _canFire;
	}

	int getBulletNum(){
		return _bullets.size();
	}
	void eraseBullet(Bullet *aBullet){
		_bullets.erase(aBullet);
	}

	void setRefX(float x){
		_ref_x = x;
	}
	void setRefY(float y){
		_ref_y = y;
	}

	float getBulletSpeed(){
		return _bulletSpeed;
	}

	void setBulletSpeed(float aSpeed){
		_bulletSpeed = aSpeed;
	}

	int getBulletTag(){
		return _bulletTag;
	}

	void setBulletTag(int tag){
		_bulletTag = tag;
	}

	void setDuration(float atime){
		_duration = atime;
	}

	static const int ACTION_MOVE = 0x0001;
	static const int ACTION_FADE = 0x0004;
	static const int ACTION_ROTATE = 0x0008;

	//void finishRebirth(float val);
protected:
	
	virtual const char* getPicName(){
		return "p1.png";
	}
	//坦克的生命值
	int _life;
	//坦克的实际大小（像素）
	cocos2d::Size _size;
	//与地图的比例
	float _scale;
	//坦克方向
	TankDirection _tankdir;

	//坦克速度
	float _speed;
	//子弹速度
	float _bulletSpeed;
	//
	float _duration;

	float _ref_x;
	float _ref_y;

	bool _canFire;

	//current map instance

	std::string _IDStr;
	TankState *_state;

	//bool _canFire;

	int _bulletTag;
	std::unordered_set<Bullet *> _bullets;


};
#endif