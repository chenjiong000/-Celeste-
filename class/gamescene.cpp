#include "gamescene.h"

USING_NS_CC;

Scene* gamescene::createScene()
{
	return gamescene::create();
}



bool gamescene::init()
{
	//////////////////////////////
	// 1. super init first
	if (!Scene::init())
	{
		return false;
	}
	//if(!this->initWithPhysics()) //使用物理引擎
		//return false;
	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
	Common::loadMarioAnimation("walkRight.png", 16, 32, 10, ANIM_MARIO_RUN_RIGHT, 0.05);  //加载动画资源
	Common::loadMarioAnimation("walkLeft.png", 16, 32, 10, ANIM_MARIO_RUN_LEFT, 0.05,0,1);  //向左动画顺序反了
	Common::loadMarioSpriteFrame("walkLeft.png", 16, 32, 10, FRAME_MARIO_INMIDAIR_LEFT,2,0);
	Common::loadMarioSpriteFrame("walkRight.png", 16, 32, 10, FRAME_MARIO_INMIDAIR_RIGHT,2,0);

	ValueVector vl = { cocos2d::Value(2),cocos2d::Value(1),cocos2d::Value(8) };
	Common::loadMarioClimbAnimation("Sprite_Mario_Left.png", 16, 32, vl, ANIM_MARIO_LEFT_CLIMB, 0.15,1);
	ValueVector vr = { cocos2d::Value(13),cocos2d::Value(14),cocos2d::Value(7) };
	Common::loadMarioClimbAnimation("Sprite_Mario_Right.png", 16, 32, vr, ANIM_MARIO_RIGHT_CLIMB, 0.15);

	initMap(1);
	inintMario();
	camerapoint = Vec2(0, 0);  //移动相机，记录相机点，精灵getposition-相机点，判定是否精灵是否在视界内。
	_KeyboardDirLeft = false;    //按下多个方向键时，释放一个方向键，则精灵依旧有其他方向，而不是停下
	_KeyboardDirRight = false;
	spinteghost = -1;    // 减少残影数量
	ghostinterval = 5;
	auto listener = EventListenerKeyboard::create();
	listener->onKeyPressed = CC_CALLBACK_2(gamescene::onKeyPressed, this);
	listener->onKeyReleased = CC_CALLBACK_2(gamescene::onKeyReleased, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
	scheduleUpdate();



	//auto emitter3 = ParticleSnow::create();
	//emitter3->setPosition(512, 768);
	//addChild(emitter3, 10);

	return true;
}

void gamescene::inintMario() {
	_mario = mario::create();
	_mario->setPosition(60, 85);
	//_mario->setPosition(420, 280);
	//_mario->setScale(3);
	//_mario->_runDir = DIR_NONE;
	//_mariobody = PhysicsBody::createBox(Size(16 * 3, 32 * 3),PhysicsMaterial(0.1f, 1.0f, 0.0f));
	//_mariobody = physicsBody;
	//_mariobody->setDynamic(false);
	//physicsBody->setVelocity(Vec2(0, 50));
	//_mario->addComponent(_mariobody);

	this->addChild(_mario,2);
	//_mario->
}
void gamescene::initMap(int idx)
{
	auto bg = Sprite::create("bg3.jpg");
	bg->setPosition(480,320);
	addChild(bg, 0);
	// 根据地图索引，创建地图
	std::string m = "MarioMap" + std::to_string(idx) + ".tmx";
	auto mymap = TMXTiledMap::create("celeste1.tmx");
	_map = mymap;
	//mymap->setScale(768/mymap->getContentSize().height);
	if (mymap == nullptr)
		log("eroor map");
	addChild(mymap, 1);
	mymap->setPosition(0, 0);
	_mapidx = idx;
}

void gamescene::update(float dt) {
	movescene();  //移动地图
	if (_KeyboardClimb && canClimb(dt)>=3)
	{
		if (!_mario->_isclimb) {
			_mario->_isclimb = true;
			_mario->_speedDown = 0;  //重置下降速度
			_mario->updateMario();
		}
		if (canClimb(dt) == 4) {
			float rate = dt / _mario->_climbup_time;
			int rightdir = _mario->_faceTo == DIR_RIGHT ? 1 : -1;
			log("%f", rate);
			//rate = 1;
			//_mario->setDisplayFrame(
				//SpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(FRAME_MARIO_INMIDAIR_RIGHT));
			_mario->runAction(MoveBy::create(0, 
				Vec2((dt * _mario->_speed+ _mario->getContentSize().width)* rate* rightdir, rate*_mario->getContentSize().height/2)));
			return;
		}
		if (_KeyboardClimbDir == DIR_NONE && _mario->_ClimbDir != DIR_NONE) {
			_mario->_ClimbDir = DIR_NONE;
			_mario->updateMario();
			log("stopupdate");
		}
		else if (_KeyboardClimbDir == DIR_UP) {
			if (_mario->_ClimbDir == DIR_NONE) {
				_mario->_ClimbDir = DIR_UP;
				_mario->updateMario();
				log("upupdate");
			}
			else {
				_mario->_ClimbDir = DIR_UP;
			}
			_mario->climbUp(dt);
		}
		else if (_KeyboardClimbDir == DIR_DOWN) {
			if (_mario->_ClimbDir == DIR_NONE) {
				_mario->_ClimbDir = DIR_DOWN;
				_mario->updateMario();
				log("downupdate");
			}
			else {
				_mario->_ClimbDir = DIR_DOWN;
			}
			_mario->climbDown(dt);
		}
		
	}
	else {
		if (_mario->_isclimb) {
			_mario->_isclimb = false;
			_mario->updateMario();
		}
		//_mario->_isclimb
		_mario->moveUp(dt);
		_mario->moveDown(dt, canMoveDown(dt));
		if (_mario->_issprint) {
			//log("issssssssssssssssssssssss sptin:%d", _mario->_issprint);
			_mario->sprint(dt, RightBlock(dt, true), LeftBlock(dt, true));
			marioghost();
			return;
		}
		if (_KeyboardSprint&&_mario->_cansprint) {
			_mario->_speedDown = 0;  //不再跳跃，并重置重力影响
			_mario->_speedUp = 0;
			spinteghost = -1;  // 减少残影数量
			_mario->_cansprint = false;
			_mario->_issprint = true;
			_mario->LostHat();  //出现帽子
			_mario->updateMario();
			
		}

		if (_KeyboardJump) {
			_mario->jump(canMoveDown(dt));
			_mario->updateMario();
		}
		if (_KeyboardDir == DIR_NONE && _mario->_runDir != DIR_NONE) {
			this->_mario->_runDir = DIR_NONE;
			_mario->updateMario();
		}
		else if (_KeyboardDir != _mario->_runDir) {
			this->_mario->_runDir = _KeyboardDir;
			this->_mario->_faceTo = _KeyboardDir;
			_mario->updateMario();
			//_mario->move(dt * _mario->_speed, 0);
		}
		else if (_mario->_runDir != DIR_NONE) {
			if (_mario->_runDir == DIR_RIGHT) 
				_mario->runRight(dt, RightBlock(dt));
			else
				_mario->runLeft(dt, LeftBlock(dt));
		}
	}
}

void gamescene::onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event)  //按键事件
{
	//log("Key with keycode %d pressed", keyCode);
	if (int(keyCode) == 147) {
		log("sprint");
		_KeyboardSprint = true;
	}
	if (int(keyCode) == 149) {
		log("climb");
		_KeyboardClimb= true;
	}
	if (int(keyCode) == 126) {
		log("jump");
		_KeyboardJump = true;
		//_mario->jump();
	}
	if (int(keyCode) == 26) {
		log("left");
		_KeyboardDirLeft = true;
		_KeyboardDir = DIR_LEFT;
	}
	else if (int(keyCode) == 27) {
		log("right");
		_KeyboardDirRight = true;
		_KeyboardDir = DIR_RIGHT;
	}
	else if (int(keyCode) == 28) {
		//log("up");
		_KeyboardClimbUp = true;
		_KeyboardClimbDir = DIR_UP;
	}
	else if (int(keyCode) == 29) {
		//log("down");
		_KeyboardClimbDown = true;
		_KeyboardClimbDir = DIR_DOWN;
	}
}
void gamescene::onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event)
{
	//log("Key with keycode %d released", keyCode);
	if (int(keyCode) == 147) {
		log("release Sprint");
		_KeyboardSprint = false;
	}
	if (int(keyCode) == 149) {
		log("release climb");
		_KeyboardClimb = false;
	}
	if (int(keyCode) == 126) {
		log("release jump");
		_KeyboardJump = false;
		
	}
	if (int(keyCode) == 26) {
		log("left stop");
		_KeyboardDirLeft = false;
		if (_KeyboardDirRight)
			_KeyboardDir = DIR_RIGHT;
		else
			_KeyboardDir = DIR_NONE;
		//this->_mario->_runDir = DIR_NONE;
		//_mario->updateMario();
	}
	else if (int(keyCode) == 27) {
		log("right stop");
		_KeyboardDirRight = false;
		if(_KeyboardDirLeft)
			_KeyboardDir = DIR_LEFT;
		else
			_KeyboardDir = DIR_NONE;
		//this->_mario->_runDir = DIR_NONE;
		//_mario->updateMario();
	}
	else if (int(keyCode) == 28) {
		//log("up stop");
		_KeyboardClimbUp = false;
		_KeyboardClimbDir = DIR_NONE;
	}
	else if (int(keyCode) == 29) {
		//log("down stop");
		_KeyboardClimbDown = false;
		_KeyboardClimbDir = DIR_NONE;
	}
}


void gamescene::movescene() {
	log("moveed %f", _mario->getPosition().y);
	auto visibleSize = Director::getInstance()->getVisibleSize();
	if ((_mario->getPosition().x- camerapoint.x) > visibleSize.width) {
		unscheduleUpdate();
		auto x = Director::getInstance()->getVisibleSize().width;
		x -= _mario->getContentSize().width;
		auto camera = getDefaultCamera();
		camera->runAction(MoveBy::create(1, Vec2(x, 0)));
		camerapoint.x += x;
		scheduleUpdate();
	}
	else if ((_mario->getPosition().y-camerapoint.y) > visibleSize.height) {
		unscheduleUpdate();
		auto y= Director::getInstance()->getVisibleSize().height;
		y-= _mario->getContentSize().height;
		auto camera = getDefaultCamera();
		camera->runAction(MoveBy::create(1, Vec2(0, y)));
		_mario->runAction(MoveBy::create(0, Vec2(0, 32)));  //数值问题，跳不上，切换场景时偷偷移上去
		camerapoint.y += y;
		//Sleep(1000);
		scheduleUpdate();

		//auto move = Sequence::create(
		//	MoveBy::create(0, Vec2(0, -y)), CallFuncN::create([&](Node* pNode) {pNode->scheduleUpdate(); }), nullptr);
		//this->runAction(move);

	}
	else if ((_mario->getPosition().x- camerapoint.x) < 0) {
	}
	else if ((_mario->getPosition().y- camerapoint.y) < 0) {
	}
	else {
		return;
	}
}
void gamescene::RestartSchedule() {
	scheduleUpdate();
}

bool gamescene::canMoveDown(float dt)
{
	CCRect rc = _mario->boundingBox();
	float yoff = dt * _mario->_speedDown;
	//float yoff = 1;
	CCPoint pt[3];
	pt[0] = ccp(rc.getMinX(), rc.getMinY() - yoff);
	pt[1] = ccp(rc.getMidX(), rc.getMinY() - yoff);
	pt[2] = ccp(rc.getMaxX(), rc.getMinY() - yoff);
	if (Common::isBlock3Point(pt, _map))
	{
		return false;
	}
	return true;
}
int gamescene::RightBlock(float dt,bool sprint)
{
	CCRect rc = _mario->boundingBox();
	float xoff = dt * _mario->_speed;
	if (sprint)
		xoff = dt * (_mario->_sprintlenth / _mario->_sprintTotaltime);  //智障bug记录，除数写为冲刺已用时，一个无穷宽的碰撞盒
	//float xoff = 1;
	CCPoint pt[3];
	pt[0] = ccp(rc.getMaxX()+xoff, rc.getMinY());  
	pt[1] = ccp(rc.getMaxX()+ xoff, rc.getMidY());
	pt[2] = ccp(rc.getMaxX()+ xoff, rc.getMaxY()-1);  //-1使之能够进入两个tiled 高度地形
	//log("right %d", Common::isBlock3Point(pt, _map));
	return Common::isBlock3Point(pt, _map);
	
}
int gamescene::LeftBlock(float dt, bool sprint)
{
	CCRect rc = _mario->boundingBox();
	float xoff = dt * _mario->_speed;
	if (sprint) {
		xoff = dt * (_mario->_sprintlenth / _mario->_sprintTotaltime);
		log("sprint off:%f", xoff);
	}
	else {
		log("run off:%f", xoff);
	}
	//float xoff = 1;
	CCPoint pt[3];
	pt[0] = ccp(rc.getMinX() - xoff, rc.getMinY());
	pt[1] = ccp(rc.getMinX() - xoff, rc.getMidY());
	pt[2] = ccp(rc.getMinX() - xoff, rc.getMaxY()-1);
	log("left:%d", Common::isBlock3Point(pt, _map));
	return Common::isBlock3Point(pt, _map);
}

int gamescene::canClimb(float dt) {
	if (_mario->_faceTo == DIR_LEFT && LeftBlock(dt))
		return LeftBlock(dt);
	else if (_mario->_faceTo == DIR_RIGHT && RightBlock(dt))
		return RightBlock(dt);
	else
		return 0;
}

bool gamescene::canClimbup(float dt)
{
	CCRect rc = _mario->boundingBox();
	float xoff = dt * _mario->_speed;
	//float xoff = 1;
	CCPoint pt[3];
	pt[0] = ccp(rc.getMaxX() + xoff, rc.getMinY());
	pt[1] = ccp(rc.getMaxX() + xoff, rc.getMidY());
	pt[2] = ccp(rc.getMaxX() + xoff, rc.getMaxY());
	//log("%d", Common::isBlock3Point(pt, _map));
	if (Common::isBlock3Point(pt, _map)==4)
	{
		return true;
	}
	return false;
}
void gamescene::marioghost() {
	spinteghost += 1;
	if (spinteghost % ghostinterval != 0)
		return;
	Sprite *ghost;
	if (this->_mario->_faceTo == DIR_LEFT)
		ghost = Sprite::createWithSpriteFrame(
			SpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(FRAME_MARIO_INMIDAIR_LEFT));
	else
		ghost = Sprite::create("walkRightGhost.png");
	addChild(ghost,2);
	ghost->setPosition(this->_mario->getPosition());
	ghost->setOpacity(150);
	//ghost->setColor(Color3B( 240, 255, 255));
	//auto f = CallFuncN::create([&](Node* pNode) {pNode->removeFromParent(); });
	//auto callback = CallFunc::create(f(ghost));
	auto fade = Sequence::create(
		FadeOut::create(0.3), CallFuncN::create([&](Node* pNode) {pNode->removeFromParent(); }) ,  nullptr);
	ghost->runAction(fade);
	//removeChild(ghost);
}

