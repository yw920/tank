#include "TankSprite.h"
#include <2d/CCTMXLayer.h>
#include <2d/CCLayer.h>
#include <cocos2d.h>
#include <audio/include/SimpleAudioEngine.h>
#include <cocostudio/CocoStudio.h>

#include <math.h>
#include "MapLayer.h"
#include "TileNode.h"
#include "Bullet.h"
#include "MapData.h"
#include "SceneGame.h"


USING_NS_CC;
using namespace cocostudio;



void TankSprite::onMove(const float& x, const float &y)
{
	//通过x，y计算朝那个方向运动,每次前进0.5个单位
	//right
	float duration = 0.1f;
	if ((x  >  0) && (x - y) > 0 && (x + y) > 0){	    
		int rtn = rotateWithAdjust(TRIGHT, duration);
		
	
	}
	//left
	else if ((x < 0) && (x + y) < 0 && (x - y) < 0) {
		int rtn = rotateWithAdjust(TLEFT, duration);
		
		
	}
	//up
	else if ((y > 0) && (y - x) > 0 && (y + x) > 0){
		int rtn = rotateWithAdjust(TUP, duration);
		
		
		
		
	}
	//down
	else if ((y < 0) && (y - x) < 0 && (y + x) < 0) {
		int rtn = rotateWithAdjust(TDOWN, duration);
		
	}
}

void TankSprite::stopTank(const TankDirection dir)
{
	if (dir == _tankdir)
	{
		this->stopAllActionsByTag(ACTION_MOVE);
	}
}

bool TankSprite::init(CollisionDetector *dectctor, cocos2d::Vec2 tankpos)
{
	if (!TankNode::init(dectctor, tankpos)) return false;
	this->_tankdir = TUP;
	this->_speed = 4;
	_life = 3;
	_bulletTag = COLL_BULLET;
	setTag(COLL_FELLOWS);
	return true;
}


COLL_RET TankSprite::onCollision(cocos2d::Node* other)
{
	

	if (other->getTag() == COLL_BULLET_ENEMIES)
	{
		if (_state->beAttacked())
			return DESTROY;
	}

	return TankNode::onCollision(other);
}

void TankSprite::setBulletAwardNum(int num)
{
	_bulletAwardNum = num;
	
}

bool TankSprite::killLife()
{
	CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(FileUtils::getInstance()->fullPathForFilename("audio/tankbomb.wav").c_str());
	if (--_life == 0)
	{
		Armature* armature = Armature::create("explodeAnimation");
		// 设置当前运行动画的索引，一个“工程”可以建立多个动画
		armature->getAnimation()->playWithIndex(0);
		armature->getAnimation()->setMovementEventCallFunc([&](Armature *armature, MovementEventType movementType, const std::string& movementID){
			armature->removeFromParent();
			SceneGame *theGame = dynamic_cast<SceneGame *>(this->getParent());
			theGame->gameOver();
		});
		// 设置位置信息
		armature->setPosition(getPosition());
		// 添加到容器，当前运行的场景之中
		getParent()->addChild(armature);
		this->setVisible(false);
		
		return false;
	}
	return true;
}

void TankSprite::move(TankDirection dir, const float &step, const float &duration)
{
	TankNode::move(dir, step, duration);
}
