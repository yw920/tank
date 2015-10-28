#include "TankStateRebirth.h"
#include "TankNode.h"
#include "TankSprite.h"

#include "MapData.h"
#include "TankStateNormal.h"
#include <vector>

USING_NS_CC;
using namespace std;


COLL_RET TankStateRebirth::onConcreteCollision(cocos2d::Node* other)
{
	if (_tank->getTag() == NodeWithCollision::COLL_FELLOWS){
		if (other->getTag() == NodeWithCollision::COLL_BULLET_ENEMIES)
		{
			if (beAttacked())
				return DESTROY;
		}
		else if (other->getTag() == NodeWithCollision::COLL_ENEMIES)
		{
			_tank->stopTank(_tank->getOtherTankDirection(other->getPosition()));
		}
	}


	return UNKNOWN;
}

void TankStateRebirth::fire()
{
	TankState::fire();
}

bool TankStateRebirth::beAttacked()
{
	return false;
}

TankStateRebirth::TankStateRebirth(TankState *oldState)
{
	_tank = oldState->getTank();
	
    init();
}

TankStateRebirth::TankStateRebirth(TankNode *tank)
{
    _tank = tank;
    init();
}

void TankStateRebirth::init()
{
    //CCLOG("Opacity: %hhu", _tank->getOpacity());
	_tagBackup = _tank->getTag();
    _tank->setOpacity(0x00);
	_tank->stopAllActions();
	float dtime = 3.0f;
    //CCLOG("afterSetOpacity: %hhu", _tank->getOpacity());
    if (_tank->getTag() == NodeWithCollision::COLL_FELLOWS) {
        Vec2 homexy = MAPDATA->getHomePos();
        _tank->setPosition(homexy);
        _tank->setRefX(homexy.x);
        _tank->setRefY(homexy.y);
        _tank->setDirection(TUP);
		TankSprite *tankSprite = dynamic_cast<TankSprite *>(_tank);
		tankSprite->setBulletAwardNum(0);
    }else if(_tank->getTag() == NodeWithCollision::COLL_ENEMIES){
		dtime = 1.5f;
    }

    
    FadeTo *fadeLight = FadeTo::create(0.5, 50);
    FadeTo *fadeDense = FadeTo::create(0.5, 100);
    //_tank->runAction(fadeIn);
    //vector<FiniteTimeAction*> repeatActions{fadeDense, fadeLight};
    RepeatForever *fadeForever = RepeatForever::create(Sequence::createWithTwoActions(fadeDense, fadeLight));
    fadeForever->setTag(TankNode::ACTION_FADE);
    _tank->runAction(fadeForever);
    _myTimer = FinishRebirthTimer::create();
	_myTimer->initTimer(this, dtime);
    _tank->addChild(_myTimer);
	_tank->setTag(NodeWithCollision::COLL_REBIRTH);



}


TankStateRebirth::~TankStateRebirth()
{
	_tank->stopAllActionsByTag(TankNode::ACTION_FADE);
	_tank->setOpacity(255);
	_tank->setTag(_tagBackup);
	_myTimer->removeFromParent();
}


void TankStateRebirth::changeState()
{
	_tank->setState(new TankStateNormal(this));
	static int count = 0;
	CCLOG("change State %d", ++count);
	
}


void FinishRebirthTimer::finish(float val)
{
	
	_state->changeState();
}

void FinishRebirthTimer::initTimer(TankStateRebirth *state, float dtime)
{
	_state = state;
	scheduleOnce(schedule_selector(FinishRebirthTimer::finish), dtime);
}
