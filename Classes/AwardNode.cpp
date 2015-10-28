#include "AwardNode.h"
#include "MapData.h"
#include "AwardLayer.h"
#include "TankState.h"
#include "TankSprite.h"
#include <audio/include/SimpleAudioEngine.h>
#include <platform/CCFileUtils.h>

USING_NS_CC;

COLL_RET AwardNode::onCollision(cocos2d::Node *other)
{
	if (COLL_FELLOWS == other->getTag())
	{
		_tank = dynamic_cast<TankSprite *>(other);
		CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(FileUtils::getInstance()->fullPathForFilename("audio/award.wav").c_str());
		onTrigger();
		return DESTROY;
	}
	return RETAIN;
}

bool AwardNode::init(CollisionDetector *detector, cocos2d::Vec2 pos, AwardLayer *awardLayer)
{
	this->setPosition(pos);
	if (!NodeWithCollision::init(detector))
	{
		return false;
	}
	_awardLayer = awardLayer;
	_sleepTime = 10.0f;
	_warningTime = 5.0f;
	this->setTag(COLL_AWARDS);
	scheduleOnce(schedule_selector(AwardNode::afterSleep), _sleepTime);
	CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(FileUtils::getInstance()->fullPathForFilename("audio/sound3.wav").c_str());
	return true;
}


b2Body * AwardNode::createBody()
{
	return _collDetector->createBoxBody(this, b2_staticBody, getPositionX(), getPositionY(), MAPDATA->getTile_w_in_scene(), MAPDATA->getTile_h_in_scene() );
}

void AwardNode::afterSleep(float val)
{
	this->setCascadeOpacityEnabled(true);
	FadeTo *fadeLight = FadeTo::create(0.5, 50);
	FadeTo *fadeDense = FadeTo::create(0.5, 100);
	//_tank->runAction(fadeIn);
	//vector<FiniteTimeAction*> repeatActions{fadeDense, fadeLight};
	RepeatForever *fadeForever = RepeatForever::create(Sequence::createWithTwoActions(fadeLight, fadeDense));
	runAction(fadeForever);
	scheduleOnce(schedule_selector(AwardNode::releaseNode), _warningTime);
}

void AwardNode::releaseNode(float val)
{
	this->removeFromParent();
}

AwardNode::~AwardNode()
{
	if (_awardLayer != NULL)
	{
		_awardLayer->releaseAward(this);
	}
}



void AwardBullet::onTrigger()
{
	int awardNum = _tank->getBulletAwardNum();
	if (awardNum == 0)
	{
		_tank->setState(new TankStateBullet(_tank));
		_tank->setBulletAwardNum(1);
	}
	else if (awardNum == 1)
	{
		_tank->setBulletAwardNum(2);
	}else if (awardNum  == 2)
	{
		_tank->setBulletAwardNum(3);
	}
	
}

bool AwardBullet::init(CollisionDetector *detector, cocos2d::Vec2 pos, AwardLayer *awardLayer)
{
	Sprite *node = Sprite::createWithSpriteFrameName("award_bullet.png");
	float scale = MAPDATA->getTile_w_in_scene() * 2.0f / node->getContentSize().width;
	node->setScale(scale);
	this->addChild(node, 0);
	if (!AwardNode::init(detector, pos, awardLayer))
	{
		return false;
	}
	return true;
}


bool AwardUnmove::init(CollisionDetector *detector, cocos2d::Vec2 pos, AwardLayer *awardLayer)
{
	Sprite *node = Sprite::createWithSpriteFrameName("award_unmove.png");
	float scale = MAPDATA->getTile_w_in_scene() * 2.0f / node->getContentSize().width;
	node->setScale(scale);
	this->addChild(node, 0);
	if (!AwardNode::init(detector, pos, awardLayer))
	{
		return false;
	}
	return true;
}

void AwardUnmove::onTrigger()
{
	if (_awardLayer != NULL)
	{
		_awardLayer->pauseAITanks();
	}
	
}
