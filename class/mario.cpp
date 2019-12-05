#include "mario.h"
USING_NS_CC;
mario::mario()
{
}
mario::~mario()
{
}

bool mario::init()
{
	cocos2d::Sprite::init();
	hat = Sprite::create("MarioHat.png");
	addChild(hat);
	hat->setScale(0.5);
	hat->setOpacity(0);
	hat->setPosition(Vec2(0, 40));
	_isAir = false;
	_isclimb = false;
	_issprint = false;
	_cansprint = true;
	_speed = 100;
	_jumpspeed = 200;  //60֡��35�����ص����ң����ھ���߶�32
	_speedUp = 0;
	_speedDown = 0;
	_climbspeed = 50;
	_gravity = 10;  // �������ٶ�
	_climbup_time = 0.3;
	_sprintTime = 0;
	_sprintTotaltime = 0.3;
	_sprintlenth = 80;
	_runDir = DIR_NONE;
	_ClimbDir = DIR_NONE;
	_faceTo = DIR_RIGHT;
	updateMario();
	return true;
}

void mario::updateMario() {
	this->stopAllActions();
	if (this->_isclimb) {
		if (this->_ClimbDir==DIR_UP|| this->_ClimbDir == DIR_DOWN) {
			CCAnimation* animation = CCAnimationCache::sharedAnimationCache()->animationByName(
				this->_faceTo == DIR_LEFT ? ANIM_MARIO_LEFT_CLIMB : ANIM_MARIO_RIGHT_CLIMB);
			CCAnimate* animate = CCAnimate::create(animation);
			CCRepeatForever* rep = CCRepeatForever::create(animate);
			this->runAction(rep);
		}
		else 
			this->setDisplayFrameWithAnimationName(
				this->_faceTo == DIR_LEFT ? ANIM_MARIO_LEFT_CLIMB : ANIM_MARIO_RIGHT_CLIMB, 0);
		return;
	}
	if (this->_isAir)
	{
		// ����
		if (this->_faceTo == DIR_LEFT)
			this->setDisplayFrame(
				SpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(FRAME_MARIO_INMIDAIR_LEFT));
		else
			this->setDisplayFrame(
				SpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(FRAME_MARIO_INMIDAIR_RIGHT));
	}
	else // ����
	{
		if (this->_runDir == DIR_NONE)
		{
			if (this->_faceTo == DIR_LEFT)
			{
				// ��ʾ������ߵ�ͼƬ
				this->setDisplayFrameWithAnimationName(ANIM_MARIO_RUN_LEFT, 2);
			}
			else
			{
				// ��ʾ�����ҵ�ͼƬ
				this->setDisplayFrameWithAnimationName(ANIM_MARIO_RUN_RIGHT, 2);
			}
		}
		else
		{
			if (this->_runDir == DIR_LEFT)
			{
				// �����������ߵĶ���
				CCAnimation* animation = CCAnimationCache::sharedAnimationCache()->animationByName(ANIM_MARIO_RUN_LEFT);
				CCAnimate* animate = CCAnimate::create(animation);
				CCRepeatForever* rep = CCRepeatForever::create(animate);
				this->runAction(rep);
			}
			else
			{
				// �������ұ����ߵĶ���
				CCAnimation* animation = CCAnimationCache::sharedAnimationCache()->animationByName(ANIM_MARIO_RUN_RIGHT);
				CCAnimate* animate = CCAnimate::create(animation);
				CCRepeatForever* rep = CCRepeatForever::create(animate);
				this->runAction(rep);
			}
		}
	}



}
void mario::move(float x, float y) {
	this->runAction(MoveBy::create(0, Vec2(x, y)));
}
void mario::runRight(float dt, bool RightBlock) {
	if(!RightBlock)
		this->runAction(MoveBy::create(0, Vec2(dt * _speed, 0)));
}
void mario::runLeft(float dt, bool LeftBlock) {
	if (!LeftBlock)
		this->runAction(MoveBy::create(0, Vec2(dt * -_speed, 0)));
}

void mario::jump(bool canmovedown)
{
	if (this->_isAir || canmovedown) // �������������ϾͲ�����
		return;
	//this->_speedUp = NUMBER_MARIO_START_JUMP_SPEED;
	_isAir = true;
	this->_speedUp = _jumpspeed;

}
void mario::moveUp(float dt)
{
	if (_speedUp <= 0)
		return;
	setPositionY(getPositionY() + dt * _speedUp);
	_speedUp -= _gravity;
}
void mario::moveDown(float dt,bool canmovedown)
{
	// ���ж��Ƿ��������������»���������ô�������½�
	if (_speedUp > 0)
		return;
	if (!canmovedown)
	{
		//land(); // �������½
		if (_isAir) {
			_isAir = false;
			updateMario();
		}
		//if (_isAir) {  //ÿ֡�����£��������ʱδ������̣����³�̽���������
		//_isAir = false;
		if(!_issprint)
			_cansprint = true;  //��̽���,���ÿɳ��
		_speedDown = 0;
		//
		//}
		return;
	}
	setPositionY(getPositionY() - dt * _speedDown);
	if (!_isAir&& _speedDown>200) {
		this->_isAir = true;  //����Ծ���ߴ�������״̬Ҳ��Ϊ����
		updateMario();
	}
	if(_speedDown<1000)  //�ߴ����²�һֱ����
		_speedDown += _gravity;
}
void mario::climbUp(float dt) {
	this->runAction(MoveBy::create(0, Vec2(0,dt * _climbspeed)));
	//log("climpup");
}
void mario::climbDown(float dt) {
	this->runAction(MoveBy::create(0, Vec2(0, dt * -_climbspeed)));
	//log("climpdown");
}
void mario::sprint(float dt, bool rightblock, bool leftblock) {
	if (_issprint) {
		if (_sprintTime > _sprintTotaltime) {
			_issprint = false;
			_sprintTime = 0;
			RegainHat();   //ñ��͸��
			//hat->
			//if(!_isAir)
				//_cansprint = true;  //��̽��������ܳ�̣��ڵ��棩�������ٴγ��
		}
		else {
			_sprintTime += dt;
			_isAir = true;  //ƽ�س���߼�������
			updateMario();
			float rate = dt / _sprintTotaltime;
			if (_faceTo == DIR_RIGHT && !rightblock)
				rate *= 1;
			else if (_faceTo == DIR_LEFT && !leftblock)
				rate *= -1;
			else {
				rate = 0;
				_issprint = false;   //��ײ����ǰ��ֹ���
				_sprintTime = 0;
				RegainHat();
			}
			//rate *= _faceTo == DIR_RIGHT ? 1 : -1;
			log("sprint moveby %f,%f", rate * _sprintlenth, rate * _sprintlenth);
			runAction(MoveBy::create(0, Vec2(rate * _sprintlenth, std::abs(rate) * _sprintlenth)));
		}
	}
	else
		return;
}
void mario::LostHat() {
	hat->setOpacity(0);
}
void mario::RegainHat() {
	hat->setOpacity(0);
}

