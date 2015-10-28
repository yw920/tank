#ifndef _AWARDLAYER_H_
#define _AWARDLAYER_H_

#include <stdio.h>
#include <2d/CCNode.h>
#include <unordered_set>

class TankLayer;

class AwardNode;

//struct Vec2_equal_to
//{	// functor for operator==
//	bool operator()(const cocos2d::Vec2& _Left, const cocos2d::Vec2& _Right) const
//	{	// apply operator== to operands
//		return (_Left.x == _Right.x && _Left.y == _Right.y);
//	}
//};
//
//struct Vec2_hash
//{	// hash functor for plain old data
//	unsigned int operator()(const cocos2d::Vec2& _Keyval) const
//	{	// hash _Keyval to size_t value by pseudorandomizing transform
//		float x = _Keyval.x;
//		float y = _Keyval.y;
//		unsigned int *kx = (unsigned int *)&(x);
//		unsigned int *ky = (unsigned int *)&y;
//		return (*kx) ^ (*ky);
//	}
//};
class AwardLayer : public cocos2d::Node{
public:
	void releaseAward(AwardNode *awardNode);

	void createAward(float val);
	virtual bool init();

	CREATE_FUNC(AwardLayer);


	virtual ~AwardLayer();
	void setTankLayer(TankLayer *tankLayer){
		_tankLayer = tankLayer;
	}

	void pauseAITanks();


protected:
	std::vector<cocos2d::Vec2> _homes;
	std::unordered_set<AwardNode *> _awards;
	std::vector<std::string> _types;
	TankLayer *_tankLayer;
	unsigned int _lastPos;
};

#endif 
