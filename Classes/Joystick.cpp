#include "Joystick.h"

USING_NS_CC;

Joystick* Joystick::create(const char *fnBg,float bgRadius,const char *fnHandle,float handleRadius)
{
    Joystick *joystick = new Joystick();
    if (joystick && joystick->init(fnBg,bgRadius,fnHandle,handleRadius))
    {
        joystick->autorelease();
        return joystick;
    }

    CC_SAFE_DELETE(joystick);
    return NULL;
}

bool Joystick::init(const char *fnBg,float bgRadius,const char *fnHandle,float handleRadius)
{
	//创建底盘精灵
	_bg = Sprite::create(fnBg);
	if(!_bg)
		return false;
	this->addChild(_bg);

	//创建摇杆精灵
	_handle = Sprite::create(fnHandle);
	if(!_handle)
		return false;
	this->addChild(_handle);
	
	//初始化参数
	_bgRadius     = bgRadius;
	_handleRadius = handleRadius;
	_handlePos    = Vec2(0.0f,0.0f);
	_touchEventCallback = nullptr;
	_touchOutsideCallback = nullptr;

	//设置底盘的大小
	float bgDiameter = bgRadius * 2;
	Size oriBgSize = _bg->getContentSize();
	_bg->setScaleX(bgDiameter / oriBgSize.width);
	_bg->setScaleY(bgDiameter / oriBgSize.height);

	//设置摇杆的大小
	float handleDiameter = handleRadius * 2;
	Size oriHandleSize = _handle->getContentSize();
	_handle->setScaleX(handleDiameter / oriHandleSize.width);
	_handle->setScaleY(handleDiameter / oriHandleSize.height);

	//设置定时器
	this->schedule(schedule_selector(Joystick::callHandleEvent));

	//设置触碰事件
	auto listener = EventListenerTouchOneByOne::create();
	listener->onTouchBegan     = CC_CALLBACK_2(Joystick::onTouchBegan,this);
	listener->onTouchMoved     = CC_CALLBACK_2(Joystick::onTouchMoved,this);
	listener->onTouchEnded     = CC_CALLBACK_2(Joystick::onTouchEnded,this);
	listener->onTouchCancelled = CC_CALLBACK_2(Joystick::onTouchCancelled,this);

	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener,this);

	return true;

}

bool Joystick::onTouchBegan(Touch *pTouch, Event *pEvent)
{
	Vec2 point = this->convertTouchToNodeSpaceAR(pTouch);
	
	//判断触点是否在摇杆上
	if(_touchOutsideCallback && _touchOutsideCallback(this,pTouch->getLocation()))
	{
		_bMove = true;

		setHandlePosition(pTouch->getLocation());

		point   = this->convertTouchToNodeSpaceAR(pTouch);
		
		Vec2 pos = _handle->getPosition();

		if(_touchEventCallback)
			_touchEventCallback(this,0.0f,pos.x/_bgRadius,pos.y/_bgRadius,JoystickEventType::JET_TOUCH_BEGIN);

		return true;
	}

	return false;
}

void Joystick::onTouchMoved(Touch *pTouch, Event *pEvent)
{
	setHandlePosition(pTouch->getLocation());
}

void Joystick::onTouchEnded(Touch *pTouch, Event *pEvent)
{
	Vec2 pos = _handle->getPosition();

	if(_touchEventCallback)
		_touchEventCallback(this,0.0f,pos.x/_bgRadius,pos.y/_bgRadius,JoystickEventType::JET_TOUCH_END);

	_handle->setPosition(Vec2(0.0f,0.0f));

	_bMove = false;
}

void Joystick::onTouchCancelled(Touch *pTouch, Event *pEvent)
{
	Vec2 pos = _handle->getPosition();

	if(_touchEventCallback)
		_touchEventCallback(this,0.0f,pos.x/_bgRadius,pos.y/_bgRadius,JoystickEventType::JET_TOUCH_END);
	
	_handle->setPosition(Vec2(0.0f,0.0f));

	_bMove = false;
}

void Joystick::callHandleEvent(float interval)
{
	if(_bMove)
	{
		//调用摇杆事件处理方法
		Vec2 point = _handle->getPosition();

		if(_touchEventCallback)
			_touchEventCallback(this,interval,point.x/_bgRadius,point.y/_bgRadius,JoystickEventType::JET_TOUCH_MOVE);
	}
}

void Joystick::setHandlePosition(const cocos2d::Vec2& position)
{
	//将摇杆限制在底盘的范围内
	Vec2 point = this->convertToNodeSpaceAR(position);

	if(point.x*point.x+point.y*point.y > _bgRadius*_bgRadius)
	{
		point.normalize();
		point = point * _bgRadius;
	}

	_handle->setPosition(point);
}