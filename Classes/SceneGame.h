#ifndef __GAME_SCENE_H__
#define __GAME_SCENE_H__

#include "cocos2d.h"
#include "Joystick.h"

class TankSprite;
class MapLayer;
class CollisionDetector;
class TankLayer;


class SceneGame : public cocos2d::Layer
{
public:
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene();

    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();
    
    // implement the "static create()" method manually
    CREATE_FUNC(SceneGame);
    
    bool touchOutsideHandle(Joystick* joystick,const cocos2d::Vec2&);
    void moveSpr(Joystick* joystick,float interval,float x , float y,JoystickEventType type);
	
	void gameOver();
private:

	TankSprite *_tankPlayer1;
	TankLayer *_tankLayer;
	MapLayer *_mapLayer;
	float _uiShed;
	float _uiShedVertical;
	cocos2d::LayerColor *_backColor;
    
    //fire按钮加事件
    cocos2d::Sprite *fireSprite;
	void menuPauseCallback(Ref* pSender);
	
    bool onTouchBegan(cocos2d::Touch* tTouch,cocos2d::Event* eEvent);
	void onTouchCancelled(cocos2d::Touch* tTouch, cocos2d::Event* eEvent);
	void onTouchEnded(cocos2d::Touch* tTouch, cocos2d::Event* eEvent);
};

#endif // __SceneGame_SCENE_H__
