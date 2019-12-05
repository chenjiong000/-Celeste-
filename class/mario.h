#pragma once
#define FRAME_MARIO_INMIDAIR_LEFT "FRAME_MARIO_INMIDAIR_LEFT"
#define FRAME_MARIO_INMIDAIR_RIGHT "FRAME_MARIO_INMIDAIR_RIGHT"
#define ANIM_MARIO_RUN_LEFT "ANIM_MARIO_RUN_LEFT"
#define ANIM_MARIO_RUN_RIGHT "ANIM_MARIO_RUN_RIGHT"
#define ANIM_MARIO_LEFT_CLIMB "ANIM_MARIO_LEFT_CLIMB"
#define ANIM_MARIO_RIGHT_CLIMB "ANIM_MARIO_RIGHT_CLIMB"

#include "common.h"
class mario : public cocos2d::Sprite
{
public:
	mario();
	~mario();

	//SceneGame* _game;

	CREATE_FUNC(mario);
	bool init();

	bool _isAir;
	bool _isclimb;
	int _speed;
	int _speedUp;   
	int _speedDown;
	int _climbspeed;
	int _gravity;  
	int _jumpspeed;
	float _climbup_time;
	float _sprintTotaltime;
	float _sprintTime;
	int _sprintlenth;
	bool _issprint;
	bool _cansprint;
	cocos2d::Sprite* hat;
	//MARIO_DIR ;
	//MARIO_DIR _faceTo;
	MARIO_DIR _runDir;
	MARIO_DIR _faceTo;
	MARIO_DIR _ClimbDir;
	void runLeft(float dt,bool);
	void runRight(float dt,bool);
	void stop();
	void jump(bool);
	void updateMario();
	void move(float x, float y);
	void mario::moveUp(float dt);
	void mario::moveDown(float dt, bool);
	void climbUp(float);
	void climbDown(float);
	void sprint(float,bool,bool);
	void LostHat();
	void RegainHat();

};
