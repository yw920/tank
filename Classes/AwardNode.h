#ifndef _AWARDNODE_H_
#define _AWARDNODE_H_
#include "NodeWithColl.h"


class AwardLayer;
class TankSprite;

#define CREATE_FUNC_AWARD(class_name)\
static AwardNode* create(CollisionDetector *detector, cocos2d::Vec2 pos, AwardLayer *awardLayer){\
	AwardNode *curRef = new class_name();\
    if (curRef && curRef->init(detector, pos, awardLayer)){\
	       curRef->autorelease();\
	       return curRef;\
										    }\
    CC_SAFE_DELETE(curRef);\
    return NULL;\
};


class AwardNode :public NodeWithCollision
{
public:
	virtual bool init(CollisionDetector *detector, cocos2d::Vec2 pos, AwardLayer *awardLayer);
	virtual COLL_RET onCollision(cocos2d::Node *other);
	virtual void onTrigger()=0;
	virtual b2Body * createBody();
	void setAwardLayer(AwardLayer *awardLayer){
		_awardLayer = awardLayer;
	}
	virtual ~AwardNode();
protected:
	void afterSleep(float val);
	void releaseNode(float val);
	TankSprite *_tank;
	float _sleepTime;
	float _warningTime;
	AwardLayer *_awardLayer;
	
	
	
};

class AwardBullet : public AwardNode
{
public:
	CREATE_FUNC_AWARD(AwardBullet);
	virtual bool init(CollisionDetector *detector, cocos2d::Vec2 pos, AwardLayer *awardLayer);
	virtual void onTrigger();
};

class AwardUnmove : public AwardNode
{
public:
	CREATE_FUNC_AWARD(AwardUnmove);
	virtual bool init(CollisionDetector *detector, cocos2d::Vec2 pos, AwardLayer *awardLayer);
	virtual void onTrigger();
};
#endif