#include "TankMultiLife.h"
#include "StrategyAI.h"
#include "StrategyLevel1.h"
#include <2d/CCSpriteFrame.h>
#include "MapData.h"

USING_NS_CC;

bool TankAILife2::init(CollisionDetector *detector, TankLayer *tankLayer, cocos2d::Vec2 tankpos)
{
	if (!TankAI::init(detector, tankLayer, tankpos)) return false;
	_speed = _speed *  1.5f;
	_strategy = (StrategyAI *)StrategyLevel1::create(this);
	this->addChild(_strategy);
	_life = 2;
	_isSleeping = false;
	_coins = 2;
	return true;
}

COLL_RET TankAILife2::onCollision(cocos2d::Node* other)
{
	if (other->getTag() == COLL_BULLET || other->getTag() == COLL_BULLET_IRON)
	{
		if (2 == _life)
		{
			_oneLifeSprite = Sprite::createWithSpriteFrameName("tank_life2_1.png");
			_oneLifeSprite->setScale(_scale);


			//¸Ä±ämy_heroµÄÍ¼Æ¬

			this->setTexture(_oneLifeSprite->getTexture());
			this->addChild(_oneLifeSprite);
			--_life;
			if (!_isSleeping)
			{
				scheduleOnce(schedule_selector(TankAILife2::resumeLife), 2.0f);
			}
			return RETAIN;
		}
	}
	return TankAI::onCollision(other);
}

void TankAILife2::resumeLife(float val)
{
	_oneLifeSprite->removeFromParent();
	_oneLifeSprite = NULL;
	if (1 == _life )
	{
		_life = 2;
	}
}

void TankAILife2::pause()
{
	TankAI::pause();
	_isSleeping = true;

}

void TankAILife2::resume()
{
	TankAI::resume();
	_isSleeping = false;
	if (1 == _life)
	{
		scheduleOnce(schedule_selector(TankAILife2::resumeLife), 2.0f);
	}
}

bool TankAILife3::init(CollisionDetector *detector, TankLayer *tankLayer, cocos2d::Vec2 tankpos)
{
	if (!TankAI::init(detector, tankLayer, tankpos)) return false;
	_speed = _speed *  1.3f;
	_strategy = StrategyAI::create(this);
	this->addChild(_strategy);
	_life = 3;
	_coins = 3;
	return true;
}

COLL_RET TankAILife3::onCollision(cocos2d::Node* other)
{
	if (other->getTag() == COLL_BULLET || other->getTag() == COLL_BULLET_IRON)
	{
		if (3 == _life)
		{
			_twoLifeSprite = Sprite::createWithSpriteFrameName("tank_life3_2.png");
			_twoLifeSprite->setScale(_scale);

			this->setTexture(_twoLifeSprite->getTexture());
			this->addChild(_twoLifeSprite);
			--_life;
			if (!_isSleeping)
			{
				scheduleOnce(schedule_selector(TankAILife3::resumeLife), 3.0f);
			}
			return RETAIN;
		}else if (2 == _life)
		{
			_oneLifeSprite = Sprite::createWithSpriteFrameName("tank_life3_1.png");
			_oneLifeSprite->setScale(_scale);

			this->setTexture(_oneLifeSprite->getTexture());
			this->addChild(_oneLifeSprite);
			--_life;
			if (!_isSleeping)
			{
				scheduleOnce(schedule_selector(TankAILife3::resumeLife2), 3.0f);
			}
			return RETAIN;
		}
	}
	return TankAI::onCollision(other);
}

void TankAILife3::resumeLife2(float val)
{
	if (_life == 1)
	{
		_oneLifeSprite->removeFromParent();
		_oneLifeSprite = NULL;
		++_life;
		if (!_isSleeping)
		{
			scheduleOnce(schedule_selector(TankAILife3::resumeLife), 3.0f);
		}
		
	}
}

void TankAILife3::resumeLife(float val)
{
	if (_life == 2)
	{
		_twoLifeSprite->removeFromParent();
		_twoLifeSprite = NULL;
		++_life;
	}
}


void TankAILife3::pause()
{
	TankAI::pause();
	_isSleeping = true;

}

void TankAILife3::resume()
{
	TankAI::resume();
	_isSleeping = false;
	if (1 == _life)
	{
		_oneLifeSprite->removeFromParent();
		_oneLifeSprite = NULL;
		++_life;
		if (!_isSleeping)
		{
			scheduleOnce(schedule_selector(TankAILife3::resumeLife), 3.0f);
		}
	}else if(_life == 2)
	{
		_twoLifeSprite->removeFromParent();
		_twoLifeSprite = NULL;
		++_life;
	}
}
