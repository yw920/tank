#ifndef _TANKMULTILIFE_H_
#define _TANKMULTILIFE_H_
#include "TankAI.h"
#include <string>
class TankAILife2 : public TankAI
{
public:
	CREATE_FUNC_TANKAI(TankAILife2);
	virtual bool init(CollisionDetector *detector, TankLayer *tankLayer, cocos2d::Vec2 tankpos);
	virtual COLL_RET onCollision(cocos2d::Node* other);
	virtual const char* getPicName() override{
		return "tank_life2_2.png";
	}
	virtual void pause();
	virtual void resume();

	void resumeLife(float val);
private:
	cocos2d::Sprite *_oneLifeSprite;
	bool _isSleeping;
};

class TankAILife3 :public TankAI
{
public:
	CREATE_FUNC_TANKAI(TankAILife3);
	virtual bool init(CollisionDetector *detector, TankLayer *tankLayer, cocos2d::Vec2 tankpos);
	virtual COLL_RET onCollision(cocos2d::Node* other);
	virtual const char* getPicName() override{
		return "tank_life3_3.png";
	}

	virtual void pause();
	virtual void resume();

	void resumeLife2(float val);
	void resumeLife(float val);
private:
	cocos2d::Sprite *_oneLifeSprite;
	cocos2d::Sprite *_twoLifeSprite;
	bool _isSleeping;
};

#endif