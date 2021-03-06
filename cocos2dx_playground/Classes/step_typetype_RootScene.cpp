#include "step_typetype_RootScene.h"

#include <new>
#include <numeric>
#include <sstream>

#include "PlayGroundScene.h"

#include "step_typetype_sprite_TextureTypeScene.h"
#include "step_typetype_sprite_AnchorPointScene.h"

#include "step_typetype_input_KeyCodeViewScene.h"

#include "step_typetype_sound_BasicScene.h"

#include "step_typetype_ScheduleScene.h"

#include "step_typetype_game_test_StageScene.h"
#include "step_typetype_game_TitleScene.h"

USING_NS_CC;

namespace step_typetype
{
	RootScene::RootScene() : mKeyboardListener( nullptr ) {}

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
			ss << "[1] " << step_typetype::sprite::TextureTypeScene::getTitle();
			ss << std::endl;
			ss << "[2] " << step_typetype::sprite::AnchorPointScene::getTitle();
			ss << std::endl;
			ss << std::endl;
			ss << "[3] " << step_typetype::input::KeyCodeViewScene::getTitle();
			ss << std::endl;
			ss << std::endl;
			ss << "[4] " << step_typetype::sound::BasicScene::getTitle();
			ss << std::endl;
			ss << std::endl;
			ss << "[5] " << step_typetype::ScheduleScene::getTitle();
			ss << std::endl;
			ss << std::endl;
			ss << "=============================";
			ss << std::endl;
			ss << std::endl;
			ss << "[A] " << step_typetype::game_test::StageScene::getTitle();
			ss << std::endl;
			ss << std::endl;
			ss << "=============================";
			ss << std::endl;
			ss << std::endl;
			ss << "[SPACE] " << step_typetype::game::TitleScene::getTitle();

			auto label = Label::createWithTTF( ss.str(), "fonts/NanumSquareR.ttf", 12, Size::ZERO, TextHAlignment::LEFT );
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
			ret->addChild( background_layer, std::numeric_limits<int>::min() );
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
			_director->replaceScene( PlayGroundScene::create() );
			break;

		case EventKeyboard::KeyCode::KEY_1:
			_director->replaceScene( step_typetype::sprite::TextureTypeScene::create() );
			break;
		case EventKeyboard::KeyCode::KEY_2:
			_director->replaceScene( step_typetype::sprite::AnchorPointScene::create() );
			break;

		case EventKeyboard::KeyCode::KEY_3:
			_director->replaceScene( step_typetype::input::KeyCodeViewScene::create() );
			break;

		case EventKeyboard::KeyCode::KEY_4:
			_director->replaceScene( step_typetype::sound::BasicScene::create() );
			break;

		case EventKeyboard::KeyCode::KEY_5:
			_director->replaceScene( step_typetype::ScheduleScene::create() );
			break;

		case EventKeyboard::KeyCode::KEY_A:
			_director->replaceScene( step_typetype::game_test::StageScene::create() );
			break;

		case EventKeyboard::KeyCode::KEY_SPACE:
			_director->replaceScene( step_typetype::game::TitleScene::create() );
			break;

		default:
			CCLOG( "Key Code : %d", keycode );
		}
	}
} // namespace step01
