#ifndef _GAME_PAUSE_H_
#define _GAME_PAUSE_H_
#include "cocos2d.h"
//USING_NS_CC;
class Gamepause : public cocos2d::Layer
{
public:
    virtual bool init();  
	static cocos2d::Scene* createScene(cocos2d::RenderTexture* sqr);
    CREATE_FUNC(Gamepause);
	//继续游戏
	void menuContinueCallback(cocos2d::Ref* pSender);
	//重新开始游戏
	void menuRestart(cocos2d::Ref* pSender);
	//回主界面
	void menuLogin(cocos2d::Ref* pSender);
private:

};

#endif 
