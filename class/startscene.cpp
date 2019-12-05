#include "startscene.h"
#include "gamescene.h"
#include "Myworld.h"
#include "common.h"
#include "SimpleAudioEngine.h"
#include "cocos/ui/CocosGUI.h"
#include "SimpleAudioEngine.h"
#include<string>
#include<iostream>
#include<fstream>

//cocos2d的使用指示宏
USING_NS_CC;  

Scene* startscene::createScene()
{
	return startscene::create();
}

// Print useful error message instead of segfaulting when files are not there.
static void problemLoading(const char* filename)
{
	printf("Error while loading: %s\n", filename);
	printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in HelloWorldScene.cpp\n");
}

// on "init" you need to initialize your instance
bool startscene::init()
{
	//////////////////////////////
	// 1. super init first
	if (!Scene::init())
	{
		return false;
	}
	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	auto sprite = Sprite::create("start.jpg");
	if (sprite == nullptr)
	{
		problemLoading("'start.jpg'");
	}
	else
	{
		// position the sprite on the center of the screen
		sprite->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));
		
		// add the sprite as a child to this layer
		this->addChild(sprite, 0);
	}
	
	auto button = ui::Button::create("startgame_normal.png", "startgame_select.png", "startgame_normal.png"); //start按钮
	float scale = 1;
	button->setScale(scale);
	float x = origin.x + visibleSize.width*0.1+ button->getContentSize().width * scale / 2;
	float y = origin.y + visibleSize.height*0.9- button->getContentSize().height * scale / 2;
	button->setPosition(Vec2(x, y));

	auto listener = EventListenerKeyboard::create();
	listener->onKeyPressed = CC_CALLBACK_2(startscene::onKeyPressed, this);
	listener->onKeyReleased = CC_CALLBACK_2(startscene::onKeyReleased, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);


	button->addTouchEventListener([&](Ref* sender, ui::Widget::TouchEventType type) {
		switch (type)
		{
		case ui::Widget::TouchEventType::BEGAN:
			break;
		case ui::Widget::TouchEventType::ENDED: {
			enterHelloScene(this);
			break;
		}
		default:
			break;
		}
	});
	this->addChild(button, 1);

	//auto *CN_dict = Dictionary::createWithContentsOfFile("CN_string.xml");
	//const char* str = ((std::string *)CN_dict->objectForKey("cntest"))->c_str();

	auto audio = CocosDenshion::SimpleAudioEngine::sharedEngine();
	audio->playBackgroundMusic("celeste.mp3");
	return true;
}

void startscene::enterHelloScene(Ref* pSender)
{
	//transition scene
	Director::getInstance()->replaceScene(TransitionMoveInR::create(0, gamescene::createScene()));
	

}
