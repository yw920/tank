#ifndef _SCENE_OVER_H_
#define _SCENE_OVER_H_
#include "cocos2d.h"
//USING_NS_CC;
class SceneOver : public cocos2d::Layer
{
public:
    virtual bool init();  
	static cocos2d::Scene* createScene(cocos2d::RenderTexture* sqr, int killNum);
	CREATE_FUNC(SceneOver);
	//重新开始游戏
	void menuRestart(cocos2d::Ref* pSender);
	//回主界面
	void menuLogin(cocos2d::Ref* pSender);
private:

};

#endif 
