#ifndef _TANKLAYER_H_
#define _TANKLAYER_H_

#include <stdio.h>

#include <2d/CCNode.h>
#include <unordered_set>
#include <2d/CCLabel.h>

class AwardLayer;

class TankAI;
class MapLayer;

class TankLayer : public cocos2d::Node{
public:
 

	virtual bool init();

	CREATE_FUNC(TankLayer);
    
	void initTankLayer(int tankNum, int liveNum, AwardLayer *awardLayer);
	void createAITank(float val);	
	
	void releaseAITank(TankAI *_tankAI);

	void pauseAITanks();

	void resumeAITanks(float val);

	float getTankSpeed(){
		return _curTankSpeed;
	}

	float getBulletSpeed(){
		return _curBulletSpeed;
	}

	int getDeadNum(){
		return _deadNum;
	}

	void setAwardLayer(AwardLayer *awardLayer){
		_awardLayer = awardLayer;
	}

	void setMapLayer(MapLayer *mapLayer);
	virtual ~TankLayer();
protected:
	void createOneTank();
	//AI坦克的数量
	int _tankNum;
	
	int _tankId;
	int _deadNum;
	int _coins;

	int _baseLiveNum;
	float _baseTankSpeed;
	float _baseBulletSpeed;

	int _liveNumStep;
	float _tankSpeedStep;
	float _bulletSpeedStep;

	int _curLiveNum;
	float _curTankSpeed;
	float _curBulletSpeed;

	int _curWave;
	int _waveSize;
	AwardLayer *_awardLayer;
	
	bool _isSleeping;
	MapLayer *_mapLayer;
	
	std::vector<cocos2d::Vec2> _homes;
	std::unordered_set<TankAI *> _liveTanks;
	std::vector<std::string> _tankTypes;
	cocos2d::Label *_labelKillNum;
	cocos2d::Label *_labelCoins;
};

#endif 
