#include "graph_practice_RootScene.h"

#include <new>
#include <sstream>

#include "2d/CCLabel.h"
#include "2d/CCLayer.h"
#include "base/CCDirector.h"
#include "base/CCEventListenerKeyboard.h"
#include "base/CCEventDispatcher.h"

#include "PlayGroundScene.h"

#include "graph_practice_BasicScene.h"
#include "graph_practice_Collection01Scene.h"

USING_NS_CC;

namespace graph_practice
{
	RootScene::RootScene() : mKeyboardListener( nullptr )
	{}

	Scene* RootScene::create()
	{
		auto ret = new ( std::nothrow ) RootScene();
		if( !ret || !ret->init() )
		{
			delete ret;
			ret = nullptr;
			return nullptr;
		}
		else
		{
			ret->autorelease();
		}

		const auto visibleSize = Director::getInstance()->getVisibleSize();
		const auto visibleOrigin = Director::getInstance()->getVisibleOrigin();

		//
		// Summury
		//
		{
			std::stringstream ss;
			ss << "+ " << getTitle();
			ss << std::endl;
			ss << std::endl;
			ss << "[ESC] : Return to Playground";
			ss << std::endl;
			ss << std::endl;
			ss << "[1] : " << graph_practice::BasicScene::getTitle();
			ss << std::endl;
			ss << "[2] : " << graph_practice::Collection01Scene::getTitle();

			auto label = Label::createWithTTF( ss.str(), "fonts/arial.ttf", 12, Size::ZERO, TextHAlignment::LEFT );
			label->setPosition( Vec2(
				visibleOrigin.x + ( visibleSize.width * 0.5f )
				, visibleOrigin.y + ( visibleSize.height * 0.5f )
			) );
			ret->addChild( label );
		}

		//
		// Background
		//
		{
			auto background_layer = LayerColor::create( Color4B( 99, 1, 0, 255 ) );
			ret->addChild( background_layer, -1 );
		}

		return ret;
	}

	void RootScene::onEnter()
	{
		Scene::onEnter();

		assert( !mKeyboardListener );
		mKeyboardListener = EventListenerKeyboard::create();
		mKeyboardListener->onKeyPressed = CC_CALLBACK_2( RootScene::onKeyPressed, this );
		getEventDispatcher()->addEventListenerWithSceneGraphPriority( mKeyboardListener, this );
	}
	void RootScene::onExit()
	{
		assert( mKeyboardListener );
		getEventDispatcher()->removeEventListener( mKeyboardListener );
		mKeyboardListener = nullptr;

		Scene::onExit();
	}


	void RootScene::onKeyPressed( EventKeyboard::KeyCode keycode, Event* /*event*/ )
	{
		switch( keycode )
		{
		case EventKeyboard::KeyCode::KEY_ESCAPE:
			Director::getInstance()->replaceScene( PlayGroundScene::create() );
			return;

		case EventKeyboard::KeyCode::KEY_1:
			Director::getInstance()->replaceScene( graph_practice::BasicScene::create( helper::CreateSceneMover<RootScene>() ) );
			return;
		case EventKeyboard::KeyCode::KEY_2:
			Director::getInstance()->replaceScene( graph_practice::Collection01Scene::create( helper::CreateSceneMover<RootScene>() ) );
			return;

		default:
			CCLOG( "Key Code : %d", keycode );
		}
	}
} // namespace step01