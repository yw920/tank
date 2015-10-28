#include "TankState.h"
#include "TankNode.h"
#include "Bullet.h"
#include "MapData.h"
#include "TankSprite.h"
#include "TankStateRebirth.h"
#include <cocostudio/CocoStudio.h>
#include <audio/include/SimpleAudioEngine.h>

using namespace cocostudio;
USING_NS_CC;

//TankState::TankState(TankState *oldState)
//{
//	_tank = oldState->getTank();
//}



bool TankState::beAttacked()
{
	
	Armature* armature = Armature::create("explodeAnimation");
	// 设置当前运行动画的索引，一个“工程”可以建立多个动画
	armature->getAnimation()->playWithIndex(0);
	armature->getAnimation()->setMovementEventCallFunc([](Armature *armature, MovementEventType movementType, const std::string& movementID){
		armature->removeFromParent();
	});
	// 设置位置信息
	armature->setPosition(_tank->getPosition());
	// 添加到容器，当前运行的场景之中
	_tank->getParent()->addChild(armature);
	_tank->setState(new TankStateRebirth(this));
	
	return false;
}

void TankState::fire()
{
	if (_tank->CanFire() && 0 == _tank->getBulletNum()){
		CCLOG("TankNode sprite fire");
		Bullet *norBullet = Bullet::create(MAPDATA->getCollDetector(), _tank);
		_tank->getParent()->addChild(norBullet, -1);
		ParticleSystemQuad * particle = ParticleSystemQuad::create("bullet_smoke.plist");
		Sprite *tmpSprite = Sprite::createWithSpriteFrameName("bullet_smoke.png");
		tmpSprite->setVisible(false);
		particle->setTextureWithRect(tmpSprite->getTexture(), tmpSprite->getTextureRect());
		particle->addChild(tmpSprite);
		particle->setScale(0.2f);
		particle->setPosition(norBullet->getPosition());
		//particle->setAnchorPoint(ccp(0.5, 0.5));
		_tank->getParent()->addChild(particle, 2);
		particle->setAutoRemoveOnFinish(true);
		_tank->insertBullet(norBullet);
		if (_tank->getTag() == NodeWithCollision::COLL_FELLOWS){
			CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(FileUtils::getInstance()->fullPathForFilename("audio/bullet.wav").c_str());
		}
	}
}


void TankStateBullet::fire()
{
	int starNum = _tankSprite->getBulletAwardNum();
	if (_tankSprite->CanFire() && _tankSprite->getBulletNum() < 2){
		CCLOG("TankNode sprite fire");
		if (starNum >= 2)
		{
			CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(FileUtils::getInstance()->fullPathForFilename("audio/btndir.wav").c_str());
			_tankSprite->setBulletSpeed(_bulletSpeedBackup * 2.0f);
			if (3 == starNum)
			{
				_tankSprite->setBulletTag(NodeWithCollision::COLL_BULLET_IRON);
			}
		}
		else{
			CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(FileUtils::getInstance()->fullPathForFilename("audio/bullet.wav").c_str());
		}
		
		Bullet *norBullet = Bullet::create(MAPDATA->getCollDetector(), _tank);
		_tankSprite->getParent()->addChild(norBullet, -1);
		ParticleSystemQuad * particle = ParticleSystemQuad::create("bullet_smoke.plist");
		Sprite *tmpSprite = Sprite::createWithSpriteFrameName("bullet_smoke.png");
		tmpSprite->setVisible(false);
		particle->setTextureWithRect(tmpSprite->getTexture(), tmpSprite->getTextureRect());
		particle->addChild(tmpSprite);
		particle->setScale(0.2f);
		particle->setPosition(norBullet->getPosition());
		_tank->getParent()->addChild(particle, 2);
		particle->setAutoRemoveOnFinish(true);
		_tankSprite->insertBullet(norBullet);
	}
}

bool TankStateBullet::beAttacked()
{
	if (!_tankSprite->killLife()) return false;
	CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(FileUtils::getInstance()->fullPathForFilename("audio/addlife.wav").c_str());
	return TankState::beAttacked();
}

TankStateBullet::TankStateBullet(TankSprite *oldState)
{
	_tank = oldState;
	_tankSprite = oldState;
	_bulletSpeedBackup = _tankSprite->getBulletSpeed();
	_bulletTagBackup = _tankSprite->getBulletTag();
	
}

TankStateBullet::~TankStateBullet()
{
	_tankSprite->setBulletSpeed(_bulletSpeedBackup);
	_tankSprite->setBulletTag(_bulletTagBackup);
}

