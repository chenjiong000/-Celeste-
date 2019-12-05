#pragma once
#include "cocos2d.h"

class startscene : public cocos2d::Scene
{
public:
	static cocos2d::Scene* createScene();

	virtual bool init();

	// a selector callback
	// void menuCloseCallback(cocos2d::Ref* pSender);

	// implement the "static create()" method manually
	CREATE_FUNC(startscene);  //一个宏 是一个已定义的静态成员：static 参数 *create()
	void enterHelloScene(cocos2d::Ref* pSender);
	inline void onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event) { log("pressed %d", keyCode); }
	inline void onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event) { log("Released %d", keyCode); }
};