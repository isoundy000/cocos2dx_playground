#include "Step01_Sound_Basic.h"

#include <new>
#include <sstream>

#include "audio/include/AudioEngine.h"

#include "Step01_RootScene.h"

USING_NS_CC;

namespace step01
{
	namespace sound
	{
		BasicScene::BasicScene() : mKeyboardListener( nullptr ) {}
		BasicScene::~BasicScene()
		{
			mKeyboardListener->release();
		}

		Scene* BasicScene::create()
		{
			auto ret = new ( std::nothrow ) BasicScene();
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

			return ret;
		}

		bool BasicScene::init()
		{
			if( !Scene::init() )
			{
				return false;
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
				ss << "[ESC] : Return to Root";
				ss << std::endl;
				ss << std::endl;
				ss << "[SPACE BAR] : Play Sound";

				auto label = Label::createWithTTF( ss.str(), "fonts/arial.ttf", 9, Size::ZERO, TextHAlignment::LEFT );
				label->setColor( Color3B::GREEN );
				label->setAnchorPoint( Vec2( 0.f, 1.f ) );
				label->setPosition( Vec2(
					visibleOrigin.x
					, visibleOrigin.y + visibleSize.height
				) );
				addChild( label, 9999 );
			}

			//
			// Keyboard Listener
			//
			{
				mKeyboardListener = EventListenerKeyboard::create();
				mKeyboardListener->onKeyPressed = CC_CALLBACK_2( BasicScene::onKeyPressed, this );
				mKeyboardListener->retain();
			}

			//
			// Json View
			//
			{
				auto label = Label::createWithTTF( "Press Space Bar", "fonts/arial.ttf", 9, Size::ZERO, TextHAlignment::CENTER );
				label->setAnchorPoint( Vec2( 0.5f, 0.5f ) );
				label->setPosition( Vec2(
					visibleOrigin.x + ( visibleSize.width * 0.5f )
					, visibleOrigin.y + ( visibleSize.height * 0.5f )
				) );
				addChild( label, 0 );
			}

			return true;
		}

		void BasicScene::onEnter()
		{
			Scene::onEnter();
			assert( mKeyboardListener );
			getEventDispatcher()->addEventListenerWithFixedPriority( mKeyboardListener, 1 );
		}
		void BasicScene::onExit()
		{
			assert( mKeyboardListener );
			getEventDispatcher()->removeEventListener( mKeyboardListener );
			Node::onExit();
		}

		void BasicScene::updateForExit( float /*dt*/ )
		{
			Director::getInstance()->replaceScene( RootScene::create() );
		}
		void BasicScene::onKeyPressed( EventKeyboard::KeyCode keycode, Event* /*event*/ )
		{
			if( EventKeyboard::KeyCode::KEY_SPACE == keycode )
			{
				experimental::AudioEngine::play2d( "sounds/fx/jump_001.ogg" );
			}

			if( EventKeyboard::KeyCode::KEY_ESCAPE == keycode )
			{
				if( isScheduled( schedule_selector( BasicScene::updateForExit ) ) )
				{
					return;
				}
				else
				{
					scheduleOnce( schedule_selector( BasicScene::updateForExit ), 0.f );
				}
			}
		}
	}
}
