#include "Myworld.h"
#include "HelloWorldScene.h"
#include "common.h"
#include "SimpleAudioEngine.h"
#include "cocos/ui/CocosGUI.h"
#include "SimpleAudioEngine.h"
#include<string>
#include<iostream>
#include<fstream>


USING_NS_CC;

Scene* Myworld::createScene()
{
	return Myworld::create();
}

// Print useful error message instead of segfaulting when files are not there.
static void problemLoading(const char* filename)
{
	printf("Error while loading: %s\n", filename);
	printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in HelloWorldScene.cpp\n");
}

// on "init" you need to initialize your instance
bool Myworld::init()
{
	//////////////////////////////
	// 1. super init first
	if (!Scene::init())
	{
		return false;
	}
	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
	auto sprite = Sprite::create("HelloWorld.png");  //cocos图标
	if (sprite == nullptr)
	{
		problemLoading("'HelloWorld.png'");
	}
	else
	{
		// position the sprite on the center of the screen
		sprite->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));
		sprite->setScale(0.5);
		//sprite->setOpacity(30);
		// add the sprite as a child to this layer
		//this->addChild(sprite, 0);
	}
	
	auto scaleTo = ScaleTo::create(1.5f, 1);
	auto fadein = FadeIn::create(1.5f);
	//auto mySpawn = Spawn::createWithTwoActions(scaleTo, fadein);
	//sprite->runAction(mySpawn);
	streak = MotionStreak::create(1,50,20, Color3B::WHITE,"axe.png");
	streak->setPosition(Vec2( origin.x, visibleSize.height / 2 + origin.y));
	this->addChild(streak, 1);
	streak->runAction(MoveBy::create(2,Vec2(400,0)));

	auto button = ui::Button::create("startbutton.png", "startbutton.png", "startbutton.png"); //start按钮
	float scale = 0.2;
	button->setScale(scale);
	float x = origin.x + visibleSize.width - button->getContentSize().width* scale / 2;
	float y = origin.y + button->getContentSize().height* scale / 2;
	button->setPosition(Vec2(100, 200));
	button->addTouchEventListener([&](Ref* sender, ui::Widget::TouchEventType type) {
		switch (type)
		{
		case ui::Widget::TouchEventType::BEGAN:
			break;
		case ui::Widget::TouchEventType::ENDED: {
			//enterHelloScene(this);
			//button->runAction(JumpBy::create(0,Vec2(10, 20),50,1));
			std::string bgm("OnLand.ogg");
			playbgm(this,bgm.c_str());
			log("button clicked");
			break;
		}
		default:
			break;
		}
	});
	this->addChild(button, 1);
	button->runAction(JumpBy::create(0, Vec2(50, 10), 100, 2));
	//auto *CN_dict = Dictionary::createWithContentsOfFile("CN_string.xml");
	//const char* str = ((std::string *)CN_dict->objectForKey("cntest"))->c_str();

	std::string str("中文");
	auto myLabel = Label::createWithSystemFont(str, "Arial", 16);  //label组件及xml
	myLabel->setPosition(200,200);
	myLabel->enableShadow();
	this->addChild(myLabel, 1);
	try {
		auto audio = CocosDenshion::SimpleAudioEngine::sharedEngine();
		audio->playBackgroundMusic("celeste.mp3");
	}
	catch (...) {
		log("error while loading music");
	}


	return true;
}

void Myworld::enterHelloScene(Ref* pSender)
{
	//transition scene
	Director::getInstance()->replaceScene(TransitionSlideInR::create(1, HelloWorld::createScene()));

}
void Myworld::playbgm(Ref* pSender,const char *m) {
	
	try {
		
		auto audio = CocosDenshion::SimpleAudioEngine::getInstance();
		audio->playBackgroundMusic("OnLand.ogg");
		log("background m");
	}
	catch (...) {
		log("error while loading music");
	}
}

