#pragma once
#include "cocos2d.h"
#include "mario.h"

class Myworld : public cocos2d::Scene
{
public:
	static cocos2d::Scene* createScene();

	//cocos2d::TMXTiledMap* _map; //瓦片地图
	mario* _mario;
	cocos2d::MotionStreak* streak;
	virtual bool init();

	// a selector callback
	// void menuCloseCallback(cocos2d::Ref* pSender);
	// implement the "static create()" method manually
	CREATE_FUNC(Myworld);  //一个宏 是一个已定义的静态成员：static 参数 *create()
	void enterHelloScene(cocos2d::Ref* pSender);
	void playbgm(cocos2d::Ref* pSender, const char *m);
};

