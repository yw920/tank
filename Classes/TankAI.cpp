#include "TankAI.h"
#include <random>
#include "TileNode.h"
#include "MapData.h"
#include "TankLayer.h"
#include "StrategyAI.h"
#include "StrategyLevel1.h"
#include "cocostudio/CocoStudio.h"
#include <audio/include/SimpleAudioEngine.h>
USING_NS_CC;

using namespace cocostudio;



bool TankAI::init(CollisionDetector *detector, TankLayer *tankLayer, cocos2d::Vec2 tankpos)
{
	if (!TankNode::init(detector, tankpos)) return false;
	_coins = 1;
	//_sleepScheduleKey = _IDStr + "AISleep";
	this->_tankdir = TUP;
	_speed = 15.0f;
	_tankLayer = tankLayer;
	_bulletTag = COLL_BULLET_ENEMIES;
	if (_tankLayer != NULL)
	{
		_speed = _tankLayer->getTankSpeed();
		_bulletSpeed = _tankLayer->getBulletSpeed();
	}
	this->setTag(COLL_ENEMIES);
	return true;
}

COLL_RET TankAI::onCollision(cocos2d::Node* other)
{
	COLL_RET collRet = _strategy->onCollision(other);
	if (collRet != UNKNOWN) return collRet;
	if (other->getTag() == COLL_BULLET || other->getTag() == COLL_BULLET_IRON)
	{
		if (_state->beAttacked()){
			// ����ֱ��ʹ�� explodeAnimation ��������Ϣ������ explodeAnimation.ExportJson �У����䴴������Ŀ�������Ӧ  
			Armature* armature = Armature::create("explodeAnimation");
			// ���õ�ǰ���ж�����������һ�������̡����Խ����������
			armature->getAnimation()->playWithIndex(0);
			armature->getAnimation()->setMovementEventCallFunc([](Armature *armature, MovementEventType movementType, const std::string& movementID){
				armature->removeFromParent();
			});
			// ����λ����Ϣ
			armature->setPosition(getPosition());
			// ��ӵ���������ǰ���еĳ���֮��
			this->getParent()->addChild(armature);
			CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(FileUtils::getInstance()->fullPathForFilename("audio/tankbomb.wav").c_str());
			return DESTROY;
		}
	}
	return TankNode::onCollision(other);

}

void TankAI::stopTank(const TankDirection old_dir)
{
	if (old_dir == _tankdir)
	{
		_strategy->stopTank(old_dir);
	}
}


TankAI::~TankAI()
{
	if (_tankLayer != NULL)
	{
		_tankLayer->releaseAITank(this);
	}
	_strategy->removeFromParent();

}

void TankAI::pause()
{
	Node::pause();
	_strategy->pause();
}

void TankAI::resume()
{
	Node::resume();
	_strategy->resume();
}


bool TankPlain::init(CollisionDetector *detector, TankLayer *tankLayer, cocos2d::Vec2 tankpos)
{
	if (!TankAI::init(detector, tankLayer, tankpos)) return false;
	_strategy = (StrategyAI*)StrategyLevel1::create(this);
	this->addChild(_strategy);
	return true;
}

bool TankFast::init(CollisionDetector *detector, TankLayer *tankLayer, cocos2d::Vec2 tankpos)
{
	if (!TankAI::init(detector, tankLayer, tankpos)) return false;
	_speed = _speed *  2.0f;
	_bulletSpeed = _bulletSpeed * 1.2f;
	_coins = 2;
	_strategy = StrategyAI::create(this);
	this->addChild(_strategy);
	return true;
}


