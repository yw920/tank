#include "TankStateNormal.h"
#include "Bullet.h"
#include "MapData.h"
#include "TankStateRebirth.h"
#include <2d/CCParticleSystemQuad.h>
#include "TankSprite.h"
#include <audio/include/SimpleAudioEngine.h>
#include <platform/CCFileUtils.h>

USING_NS_CC;


void TankStateNormal::fire()
{
	TankState::fire();
}

bool TankStateNormal::beAttacked()
{
	if (_tank->getTag() == NodeWithCollision::COLL_FELLOWS)
	{
		TankSprite *tankSprite = dynamic_cast<TankSprite *>(_tank);
		if (!tankSprite->killLife()) return false;
		CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(FileUtils::getInstance()->fullPathForFilename("audio/addlife.wav").c_str());
		return TankState::beAttacked();
	}
	return true;
}

TankStateNormal::TankStateNormal(TankState *oldState)
{
	_tank = oldState->getTank();
}

