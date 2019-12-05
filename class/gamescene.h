#pragma once
#include "cocos2d.h"
#include "mario.h"
#include"common.h"

class gamescene : public cocos2d::Scene
{
public:
	static cocos2d::Scene* createScene();

	cocos2d::TMXTiledMap* _map; //瓦片地图
	int _mapidx;
	mario* _mario;
	cocos2d::PhysicsBody* _mariobody;
	MARIO_DIR _KeyboardDir;
	MARIO_DIR _KeyboardClimbDir;
	bool _KeyboardDirLeft;
	bool _KeyboardDirRight;
	bool _KeyboardClimbUp;
	bool _KeyboardClimbDown;
	bool _KeyboardJump;
	bool _KeyboardClimb;
	bool _KeyboardSprint;
	int spinteghost;  
	int ghostinterval;
	//bool _KeyboardDir;
	virtual bool init();
	cocos2d::Point camerapoint;

	// a selector callback
	// void menuCloseCallback(cocos2d::Ref* pSender);
	// implement the "static create()" method manually
	CREATE_FUNC(gamescene);  //一个宏 是一个已定义的静态成员：static 参数 *create()

	void inintMario();
	void initMap(int idx);
	void onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);
	void onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);
	void update(float dt);
	void movescene();
	bool canMoveDown(float dt);
	//bool canMoveRight(float dt);
	//bool canMoveLeft(float dt);
	int RightBlock(float dt, bool sprint = false);
	int LeftBlock(float dt, bool sprint = false);
	int canClimb(float dt);
	bool canClimbup(float dt);
	void marioghost();
	void RestartSchedule();

};

