#include "step02_collision_BasicScene.h"

#include <new>
#include <sstream>

#include "ui/UIButton.h"

#include "Step02_RootScene.h"

USING_NS_CC;

const int TAG_Player = 20140416;
const int TAG_Bullet = 20200209;

namespace step02
{
	namespace collision
	{
		BasicScene::BasicScene() : mKeyboardListener( nullptr )
		{}

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

				auto label = Label::createWithTTF( ss.str(), "fonts/arial.ttf", 9, Size::ZERO, TextHAlignment::LEFT );
				label->setAnchorPoint( Vec2( 0.f, 1.f ) );
				label->setPosition( Vec2(
					visibleOrigin.x
					, visibleOrigin.y + visibleSize.height
				) );
				addChild( label, 9999 );
			}
			
			//
			// Background
			//
			{
				auto background_layer = LayerColor::create( Color4B( 15, 49, 101, 255 ) );
				addChild( background_layer, 0 );
			}

			//
			// Player
			//
			{
				auto button = ui::Button::create( "guide_01_1.png", "guide_01_2.png", "guide_01_4.png", ui::Widget::TextureResType::PLIST );
				button->setTag( TAG_Player );
				button->setScale9Enabled( true );
				button->setPosition( Vec2(
					visibleOrigin.x + ( visibleSize.width * 0.5f )
					, visibleOrigin.y + ( visibleSize.height * 0.3f )
				) );
				addChild( button, 100 );
				{
					auto player_node = Sprite::createWithSpriteFrameName( "actor001_run_01.png" );
					player_node->setScale( 2.f );
					button->addChild( player_node );
					{
						auto animation_object = Animation::create();
						animation_object->setDelayPerUnit( 0.2f );
						animation_object->addSpriteFrame( SpriteFrameCache::getInstance()->getSpriteFrameByName( "actor001_run_01.png" ) );
						animation_object->addSpriteFrame( SpriteFrameCache::getInstance()->getSpriteFrameByName( "actor001_run_02.png" ) );
						animation_object->addSpriteFrame( SpriteFrameCache::getInstance()->getSpriteFrameByName( "actor001_run_03.png" ) );
						animation_object->addSpriteFrame( SpriteFrameCache::getInstance()->getSpriteFrameByName( "actor001_run_04.png" ) );

						auto animate_action = Animate::create( animation_object );

						auto repeat_action = RepeatForever::create( animate_action );

						player_node->runAction( repeat_action );
					}

					const Size margin( 3.f, 3.f );
					button->setContentSize( player_node->getBoundingBox().size + margin );
					player_node->setPosition(
						button->getContentSize().width * 0.5f
						, button->getContentSize().height * 0.5f
					);
				}
			}

			//
			// Bullet
			//
			{
				auto player_node = Sprite::createWithSpriteFrameName( "bullet001_01.png" );
				player_node->setTag( TAG_Bullet );
				player_node->setPosition( Vec2(
					visibleOrigin.x + ( visibleSize.width * 0.5f )
					, visibleOrigin.y + ( visibleSize.height * 0.7f )
				) );
				addChild( player_node, 100 );
				{
					auto animation_object = Animation::create();
					animation_object->setDelayPerUnit( 0.1f );
					animation_object->addSpriteFrame( SpriteFrameCache::getInstance()->getSpriteFrameByName( "bullet001_01.png" ) );
					animation_object->addSpriteFrame( SpriteFrameCache::getInstance()->getSpriteFrameByName( "bullet001_02.png" ) );

					auto animate_action = Animate::create( animation_object );

					auto repeat_action = RepeatForever::create( animate_action );

					player_node->runAction( repeat_action );
				}
			}

			return true;
		}

		void BasicScene::onEnter()
		{
			Scene::onEnter();
			mKeyboardListener = EventListenerKeyboard::create();
			mKeyboardListener->onKeyPressed = CC_CALLBACK_2( BasicScene::onKeyPressed, this );
			getEventDispatcher()->addEventListenerWithFixedPriority( mKeyboardListener, 1 );
		}
		void BasicScene::onExit()
		{
			assert( mKeyboardListener );
			getEventDispatcher()->removeEventListener( mKeyboardListener );
			mKeyboardListener = nullptr;
			Node::onExit();
		}

		void BasicScene::onKeyPressed( EventKeyboard::KeyCode keycode, Event* /*event*/ )
		{
			if( EventKeyboard::KeyCode::KEY_ESCAPE == keycode )
			{
				Director::getInstance()->replaceScene( step02::RootScene::create() );
			}
		}
	}
}
