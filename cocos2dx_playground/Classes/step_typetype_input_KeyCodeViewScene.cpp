#include "step_typetype_input_KeyCodeViewScene.h"

#include <new>
#include <numeric>
#include <sstream>

#include "step_typetype_RootScene.h"

USING_NS_CC;

namespace
{
	const int TAG_KeyCodeViewNode = 20140416;
}

namespace step_typetype
{
	namespace input
	{
		KeyCodeViewScene::KeyCodeViewScene() : mPressedKeyCount( 0 ), mKeyboardListener( nullptr ) {}

		Scene* KeyCodeViewScene::create()
		{
			auto ret = new ( std::nothrow ) KeyCodeViewScene();
			if( !ret || !ret->init() )
			{
				delete ret;
				ret = nullptr;
			}
			else
			{
				ret->autorelease();
			}

			return ret;
		}

		bool KeyCodeViewScene::init()
		{
			if( !Scene::init() )
			{
				return false;
			}

			const auto visibleSize = _director->getVisibleSize();
			const auto visibleOrigin = _director->getVisibleOrigin();


			//
			// Summury
			//
			{
				std::stringstream ss;
				ss << "+ " << getTitle();
				ss << std::endl;
				ss << std::endl;
				ss << "[ESC] : Return to Root";
				ss << std::endl;
				ss << std::endl;
				ss << "[Keyboard] : Show Key Code";

				auto label = Label::createWithTTF( ss.str(), "fonts/NanumSquareR.ttf", 10, Size::ZERO, TextHAlignment::LEFT );
				label->setAnchorPoint( Vec2( 0.f, 1.f ) );
				label->setPosition( Vec2(
					visibleOrigin.x
					, visibleOrigin.y + visibleSize.height
				) );
				addChild( label, std::numeric_limits<int>::max() );
			}

			//
			// Background
			//
			{
				auto background_layer = LayerColor::create( Color4B( 79, 10, 5, 255 ) );
				addChild( background_layer, std::numeric_limits<int>::min() );
			}

			//
			// KeyCode View
			//
			{
				auto label = Label::createWithTTF( "Press Key", "fonts/NanumSquareR.ttf", 14 );
				label->setTag( TAG_KeyCodeViewNode );
				label->setColor( Color3B::GREEN );
				label->setPosition( Vec2(
					visibleOrigin.x + ( visibleSize.width * 0.5f )
					, visibleOrigin.y + ( visibleSize.height * 0.5f )
				) );
				addChild( label );

				clearKeyCodeView();
			}

			return true;
		}

		void KeyCodeViewScene::onEnter()
		{
			Scene::onEnter();

			assert( !mKeyboardListener );
			mKeyboardListener = EventListenerKeyboard::create();
			mKeyboardListener->onKeyPressed = CC_CALLBACK_2( KeyCodeViewScene::onKeyPressed, this );
			mKeyboardListener->onKeyReleased = CC_CALLBACK_2( KeyCodeViewScene::onKeyReleased, this );
			getEventDispatcher()->addEventListenerWithSceneGraphPriority( mKeyboardListener, this );
		}
		void KeyCodeViewScene::onExit()
		{
			assert( mKeyboardListener );
			getEventDispatcher()->removeEventListener( mKeyboardListener );
			mKeyboardListener = nullptr;

			Scene::onExit();
		}

		void KeyCodeViewScene::updateKeyCodeView( cocos2d::EventKeyboard::KeyCode keycode )
		{
			auto label = static_cast<Label*>( getChildByTag( TAG_KeyCodeViewNode ) );
			label->setString( std::to_string( static_cast<int>( keycode ) ) );
		}
		void KeyCodeViewScene::clearKeyCodeView()
		{
			auto label = static_cast<Label*>( getChildByTag( TAG_KeyCodeViewNode ) );
			label->setString( "Press Key" );
		}

		void KeyCodeViewScene::onKeyPressed( EventKeyboard::KeyCode keycode, Event* /*event*/ )
		{
			if( EventKeyboard::KeyCode::KEY_ESCAPE == keycode )
			{
				_director->replaceScene( step_typetype::RootScene::create() );
				return;
			}
			else
			{
				++mPressedKeyCount;
				updateKeyCodeView( keycode );
			}
		}
		void KeyCodeViewScene::onKeyReleased( EventKeyboard::KeyCode keycode, Event* /*event*/ )
		{
			if( EventKeyboard::KeyCode::KEY_ESCAPE != keycode )
			{
				mPressedKeyCount = std::max( 0, mPressedKeyCount - 1 );
				if( 0 == mPressedKeyCount )
				{
					clearKeyCodeView();
				}
			}
		}
	}
}
