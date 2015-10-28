#include "SceneOver.h"
#include "SceneGame.h"
#include "Config.h"
//#include <cocos/base/ccMacros.h>

USING_NS_CC;
//传入一个CCrenderTexture 
//相当于一个正在运行的游戏的截图作为这个暂停对话框的背景 
//这样就看起来像是对话框在游戏界面之上，一般游戏当中都是这样子写的。
Scene* SceneOver::createScene(RenderTexture* sqr, int killNum)
{

    Scene *scene = Scene::create();
	SceneOver *layer = SceneOver::create();
		scene->addChild(layer,1);//把游戏层放上面，我们还要在这上面放按钮


	//增加部分：使用Game界面中截图的sqr纹理图片创建Sprite
	//并将Sprite添加到GamePause场景层中
	//得到窗口的大小
	
	Sprite *back_spr = Sprite::createWithTexture(sqr->getSprite()->getTexture());  
	back_spr->setPosition(Vec2(winSize.width / 2, winSize.height / 2)); //放置位置,这个相对于中心位置。
	back_spr->setFlippedY(true);            //翻转，因为UI坐标和OpenGL坐标不同
	back_spr->setColor(Color3B::GRAY); //图片颜色变灰色
	scene->addChild(back_spr);


	//添加游戏暂停背景小图，用来放按钮
	Sprite *back_small_spr = Sprite::create("back_over.png");
	back_small_spr->setPosition(Vec2(winSize.width / 2, winSize.height / 2)); //放置位置,这个相对于中心位置。
	scene->addChild(back_small_spr);

	char strBuffer[20] = { 0 };
	sprintf(strBuffer, "%d", killNum);
	TTFConfig ttfConfig("font/HKYuanMini.ttf", 35, GlyphCollection::DYNAMIC);
	auto labelKillNum = Label::createWithTTF(ttfConfig,
		strBuffer, TextHAlignment::RIGHT, winSize.width * 0.75);
	if (labelKillNum) {
		labelKillNum->setTextColor(Color4B(255, 128, 255, 255));
		labelKillNum->setPosition(Vec2(winSize.width / 2 + 15, winSize.height / 2 + 25));
		labelKillNum->setAnchorPoint(Vec2(0, 0.5));
		scene->addChild(labelKillNum);
	}


    return scene;
}

bool SceneOver::init()
{

    if ( !Layer::init() )
    {
        return false;
    }
	//重新开始游戏按钮
	MenuItemImage *pRestartItem = MenuItemImage::create(
		"pause_restart.png",
		"pause_restart.png",
		CC_CALLBACK_1(SceneOver::menuRestart, this)
	);

	pRestartItem->setPosition(Vec2(winSize.width / 2, winSize.height / 2 - 20));

	//回主界面
	MenuItemImage *pLoginItem = MenuItemImage::create(
		"pause_login.png",
		"pause_login.png",
		CC_CALLBACK_1(SceneOver::menuLogin, this)
	);

	pLoginItem->setPosition(Vec2(winSize.width / 2, winSize.height / 2 - 70));

	// create menu, it's an autorelease object
	Menu* pMenu = Menu::create(pRestartItem,pLoginItem,NULL);
	pMenu->setPosition(Vec2(0.0f, 0.0f));
	this->addChild(pMenu, 2);


    return true;
}

//重新开始游戏
void  SceneOver::menuRestart(Ref* pSender)
{
	Director::getInstance()->replaceScene(SceneGame::createScene());
}
//回主界面
void  SceneOver::menuLogin(Ref* pSender)
{

}






