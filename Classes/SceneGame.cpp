#include "SceneGame.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"

#include "MapLayer.h"
#include "TankSprite.h"
#include "NodeWithColl.h"
#include "MyContactListener.h"
#include "GLES-Render.h"
#include "Config.h"
#include "CollDetector.h"
#include "TankLayer.h"
#include <exception>
#include <2d/CCSpriteFrameCache.h>
#include <Box2D/Box2D.h>
#include <audio/include/SimpleAudioEngine.h>
#include "MapData.h"
#include "TankStateRebirth.h"
#include "ScenePause.h"
#include "SceneOver.h"
#include "AwardLayer.h"

USING_NS_CC;

using namespace cocostudio::timeline;
using namespace CocosDenshion;

Scene* SceneGame::createScene()
{
	// 'scene' is an autorelease object
	//collision detect is used with physics;
	//auto scene = Scene::createWithPhysics();
	auto scene = Scene::create();

	auto layer = SceneGame::create();
	
	// add layer as a child to scene
	scene->addChild(layer);

	// return the scene
	return scene;
}






// on "init" you need to initialize your instance
bool SceneGame::init()
{
	//////////////////////////////
	// 1. super init first
	if (!Layer::init())
	{
		return false;
	}

	this->setGlobalZOrder(1.0f);

	CollisionDetector *collDetector = CollisionDetector::create();
	collDetector->setName("collDetector");
	this->addChild(collDetector);
	//创建一个颜色层
	/*_backColor = LayerColor::create(Color4B(192, 192, 192, 255));
	this->addChild(_backColor);
	_backColor->setGlobalZOrder(-20.0f);*/

	//加载地图
	_mapLayer = MapLayer::create();
	this->addChild(_mapLayer, 0);
	_mapLayer->initWithMap(2);

	//精灵帧类,提高效率
	SpriteFrameCache *frameCache = SpriteFrameCache::getInstance();


	

	frameCache->addSpriteFramesWithFile("images.plist");
	//frameCache->addSpriteFramesWithFile("explodeAnimation0.plist");
	frameCache->addSpriteFrame(SpriteFrame::create("tank_fast.png", Rect(0, 0, 64, 64)), "tank_fast.png");
	frameCache->addSpriteFrame(SpriteFrame::create("tank_life2_2.png", Rect(0, 0, 64, 64)), "tank_life2_2.png");
	frameCache->addSpriteFrame(SpriteFrame::create("tank_life2_1.png", Rect(0, 0, 64, 64)), "tank_life2_1.png");
	frameCache->addSpriteFrame(SpriteFrame::create("tank_life3_3.png", Rect(0, 0, 64, 64)), "tank_life3_3.png");
	frameCache->addSpriteFrame(SpriteFrame::create("tank_life3_2.png", Rect(0, 0, 64, 64)), "tank_life3_2.png");
	frameCache->addSpriteFrame(SpriteFrame::create("tank_life3_1.png", Rect(0, 0, 64, 64)), "tank_life3_1.png");
	frameCache->addSpriteFrame(SpriteFrame::create("award_bullet.png", Rect(0, 0, 64, 64)), "award_bullet.png");
	frameCache->addSpriteFrame(SpriteFrame::create("award_unmove.png", Rect(0, 0, 64, 64)), "award_unmove.png");
	frameCache->addSpriteFrame(SpriteFrame::create("bullet_smoke.png", Rect(0, 0, 57, 57)), "bullet_smoke.png");
	frameCache->addSpriteFrame(SpriteFrame::create("bullet_exp.png", Rect(0, 0, 32, 32)), "bullet_exp.png");
	frameCache->addSpriteFrame(SpriteFrame::create("bullet2.png", Rect(0, 0, 25, 25)), "bullet2.png");
	//bullet_exp.png
	//bullet_exp.png

	// 加载资源文件 png 图片，plist 文件和 ExportJson 动画文件，一般而言可以在场景运行之初加载此资源
	cocostudio::ArmatureDataManager::getInstance()->addArmatureFileInfo("explodeAnimation0.png", "explodeAnimation0.plist", "explodeAnimation.ExportJson");


	_tankPlayer1 = TankSprite::create(collDetector, MAPDATA->getHomePos());
	_tankPlayer1->setState(new TankStateRebirth(_tankPlayer1));
	this->addChild(_tankPlayer1, -1);

	AwardLayer *awardLayer = AwardLayer::create();
	this->addChild(awardLayer);
	_tankLayer = TankLayer::create();
	_tankLayer->initTankLayer(20, 10, awardLayer);
	_tankLayer->setMapLayer(_mapLayer);
	this->addChild(_tankLayer, -1);


	//方向和射击的分水岭
	_uiShed = winSize.width * 0.6;
	_uiShedVertical = winSize.height - 50;
	//菜单和控制的分水岭
	////添加摇杆
	Joystick *joystick = Joystick::create("joystickBg.png", 70.0f, "joystickHandle.png", 50.0f);
	joystick->setPosition(50, 50);
	joystick->setHandleEventListener(JOYSTICK_CALLBACK(SceneGame::moveSpr, this));
	joystick->setTouchOutsideCallback(CC_CALLBACK_2(SceneGame::touchOutsideHandle, this));
	this->addChild(joystick, 2);
	joystick->setVisible(false);
	joystick->setCascadeOpacityEnabled(true);
	joystick->setOpacity(88);
	
	
    //添加射击按钮
    fireSprite= Sprite::create("fireButtonBg.png");
    fireSprite->setScale(0.4f);
    fireSprite->setPosition(wSize.width*0.9, 50);
	fireSprite->setCascadeOpacityEnabled(true);
	fireSprite->setVisible(false);
	fireSprite->setOpacity(88);

    //添加touch事件
    auto listener = EventListenerTouchOneByOne::create();
    listener->onTouchBegan = CC_CALLBACK_2(SceneGame::onTouchBegan, this);
	listener->onTouchCancelled = CC_CALLBACK_2(SceneGame::onTouchCancelled, this);
	listener->onTouchEnded = CC_CALLBACK_2(SceneGame::onTouchEnded, this);
    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, fireSprite);
    this->addChild(fireSprite,2);

	
	//添加杀敌标签
	TTFConfig ttfConfig("font/HKYuanMini.ttf", 10, GlyphCollection::DYNAMIC);
	auto labelkill = Label::createWithTTF(ttfConfig,
		"KILL", TextHAlignment::RIGHT, winSize.width * 0.1);
	if (labelkill) {
		labelkill->setTextColor(Color4B(128, 255, 255, 255));
		labelkill->setPosition(Vec2(wOrigin.x + winSize.width - 30, _uiShedVertical - 10));
		labelkill->setAnchorPoint(Vec2(0, 0.5));
		this->addChild(labelkill);
	}


	//添加得分标签
	auto labelScore = Label::createWithTTF(ttfConfig,
		"COIN", TextHAlignment::RIGHT, winSize.width * 0.1);
	if (labelScore) {
		labelScore->setTextColor(Color4B(128, 255, 255, 255));
		labelScore->setPosition(Vec2(wOrigin.x + winSize.width - 30, _uiShedVertical - 50));
		labelScore->setAnchorPoint(Vec2(0, 0.5));
		this->addChild(labelScore);
	}
	//播放音效
	SimpleAudioEngine::getInstance()->playEffect(FileUtils::getInstance()->fullPathForFilename("audio/start.wav").c_str());	SimpleAudioEngine::getInstance()->preloadEffect(FileUtils::getInstance()->fullPathForFilename("audio/award.wav").c_str());
	SimpleAudioEngine::getInstance()->preloadEffect(FileUtils::getInstance()->fullPathForFilename("audio/sound3.wav").c_str());
	SimpleAudioEngine::getInstance()->preloadEffect(FileUtils::getInstance()->fullPathForFilename("audio/bullet.wav").c_str());
	SimpleAudioEngine::getInstance()->preloadEffect(FileUtils::getInstance()->fullPathForFilename("audio/btndir.wav").c_str());
	SimpleAudioEngine::getInstance()->preloadEffect(FileUtils::getInstance()->fullPathForFilename("audio/tankbomb.wav").c_str());
	SimpleAudioEngine::getInstance()->preloadEffect(FileUtils::getInstance()->fullPathForFilename("audio/addlife.wav").c_str());
	SimpleAudioEngine::getInstance()->preloadEffect(FileUtils::getInstance()->fullPathForFilename("audio/blast.wav").c_str());

	//添加暂停按钮
	MenuItemImage *pCloseItem = MenuItemImage::create(
		"CloseNormal.png",
		"CloseSelected.png",
		CC_CALLBACK_1(SceneGame::menuPauseCallback,this)
		);

	pCloseItem->setPosition(Vec2(wOrigin.x + winSize.width - pCloseItem->getContentSize().width / 2,
		_uiShedVertical + pCloseItem->getContentSize().height - 10));

	// create menu, it's an autorelease object
	Menu* pMenu = Menu::create(pCloseItem, NULL);
	pMenu->setPosition(Vec2(0.0f, 0.0f));
	this->addChild(pMenu, 1);	//SimpleAudioEngine::getInstance()->preloadEffect("audio/move.wav");		
	
	return true;
}

bool SceneGame::onTouchBegan(Touch* tTouch,Event* eEvent){
	Vec2 curLocation = tTouch->getLocation();
	if (curLocation.x > _uiShed && curLocation.y < _uiShedVertical)
	{
		fireSprite->setPosition(curLocation);
		fireSprite->setVisible(true);
		_tankPlayer1->fire();
		return true;
	}
 
    return false;
    
}

void SceneGame::onTouchCancelled(cocos2d::Touch* tTouch, cocos2d::Event* eEvent)
{
	onTouchEnded(tTouch, eEvent);
}

void SceneGame::onTouchEnded(cocos2d::Touch* tTouch, cocos2d::Event* eEvent)
{
	fireSprite->setVisible(false);
}


bool SceneGame::touchOutsideHandle(Joystick* joystick, const cocos2d::Vec2& position)
{

	if (position.x <= _uiShed && position.y < _uiShedVertical)
	  {
		  joystick->setPosition(position);
		  joystick->setVisible(true);
		 
	  }
	  return true;
	
}

void SceneGame::moveSpr(Joystick *joystick, float interval, float x, float y, JoystickEventType  type){
	switch (type)
	{
	case JoystickEventType::JET_TOUCH_BEGIN:
		break;

	case JoystickEventType::JET_TOUCH_MOVE:
		assert(_tankPlayer1 != NULL);
		if (joystick->isVisible()){
			_tankPlayer1->onMove(x, y);
		}
		break;

	case JoystickEventType::JET_TOUCH_END:
		joystick->setVisible(false);
		break;
	}

}



//暂停界面
void SceneGame::menuPauseCallback(Ref* pSender)
{
	//得到窗口的大小
	RenderTexture *renderTexture = RenderTexture::create(winSize.width, winSize.height);

	//遍历当前类的所有子节点信息，画入renderTexture中。
	//这里类似截图。
	renderTexture->begin();		//_backColor->visit();
	this->getParent()->visit();		//MAPDATA->getWaterLayer()->getParent()->visit();
	renderTexture->end();

	//将游戏界面暂停，压入场景堆栈。并切换到GamePause界面
	Director::getInstance()->pushScene(Gamepause::createScene(renderTexture));
}
void SceneGame::gameOver()
{

	SimpleAudioEngine::getInstance()->playEffect(FileUtils::getInstance()->fullPathForFilename("audio/gameover.wav").c_str());
	//得到窗口的大小
	RenderTexture *renderTexture = RenderTexture::create(winSize.width, winSize.height);

	//遍历当前类的所有子节点信息，画入renderTexture中。
	//这里类似截图。
	renderTexture->begin();
	this->_mapLayer->visit();
	renderTexture->end();
	//将游戏界面暂停，压入场景堆栈。并切换到GamePause界面
	Director::getInstance()->pushScene(SceneOver::createScene(renderTexture, _tankLayer->getDeadNum()));
}


