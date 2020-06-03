#include "step_mole_game_test_ObjectActionScene.h"

#include <new>
#include <numeric>
#include <sstream>

#include "2d/CCLabel.h"
#include "2d/CCLayer.h"
#include "2d/CCSprite.h"
#include "base/CCDirector.h"
#include "base/CCEventListenerKeyboard.h"
#include "base/CCEventDispatcher.h"
#include "base/ccUTF8.h"

#include "step_mole_AnimationComponent.h"

#include "step_mole_animation_InfoContainer.h"
#include "step_mole_CircleCollisionComponent.h"
#include "step_mole_CircleCollisionComponentConfig.h"
#include "step_mole_ObjectComponent.h"

USING_NS_CC;

namespace
{
	const int TAG_ObjectNode = 20140416;
	const int TAG_LifeTimeNode = 100;
	const int TAG_ViewNode = 200;
}

namespace step_mole
{
	namespace game_test
	{
		ObjectActionScene::ObjectActionScene( const helper::FuncSceneMover& back_to_the_previous_scene_callback ) :
			helper::BackToThePreviousScene( back_to_the_previous_scene_callback )
			, mKeyboardListener( nullptr )
			, mCurrentLifeTime( 3 )
		{}

		Scene* ObjectActionScene::create( const helper::FuncSceneMover& back_to_the_previous_scene_callback )
		{
			auto ret = new ( std::nothrow ) ObjectActionScene( back_to_the_previous_scene_callback );
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

		bool ObjectActionScene::init()
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
				ss << "[1] : Object - Process Start";
				ss << std::endl;
				ss << "[2] : Object - Kill";
				ss << std::endl;
				ss << std::endl;
				ss << "[Arrow Key - Up] : Life Time - Increase";
				ss << std::endl;
				ss << "[Arrow Key - Down] : Life Time - Decrease";

				auto label = Label::createWithTTF( ss.str(), "fonts/arial.ttf", 9, Size::ZERO, TextHAlignment::LEFT );
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
				auto background_layer = LayerColor::create( Color4B( 3, 20, 70, 255 ) );
				addChild( background_layer, -1 );
			}

			//
			// Current Life Time
			//
			{
				auto label = Label::createWithTTF( "", "fonts/arial.ttf", 12, Size::ZERO, TextHAlignment::LEFT );
				label->setTag( TAG_LifeTimeNode );
				label->setAnchorPoint( Vec2( 1.f, 1.f ) );
				label->setColor( Color3B::GREEN );
				label->setPosition( Vec2(
					visibleOrigin.x + visibleSize.width
					, visibleOrigin.y + visibleSize.height
				) );
				addChild( label, std::numeric_limits<int>::max() );

				updateLifeTimeView();
			}

			//
			// Object Node
			//
			{
				auto object_node = Node::create();
				object_node->setTag( TAG_ObjectNode );
				object_node->setPosition( Vec2(
					static_cast<int>( visibleOrigin.x + ( visibleSize.width * 0.5f ) )
					, static_cast<int>( visibleOrigin.y + ( visibleSize.height * 0.5f ) )
				) );
				addChild( object_node, 0 );

				// Pivot
				{
					auto pivot = Sprite::createWithSpriteFrameName( "helper_pivot.png" );
					pivot->setScale( 2.f );
					object_node->addChild( pivot, std::numeric_limits<int>::max() );
				}

				// View
				auto view_node = Sprite::createWithSpriteFrameName( "step_mole_target_wait_0.png" );
				view_node->setTag( TAG_ViewNode );
				view_node->setAnchorPoint( Vec2( 0.5f, 0.f ) );
				view_node->setScale( 2.f );
				view_node->setPositionY( -18.f );
				object_node->addChild( view_node );

				// Animation Component
				auto animation_component = step_mole::AnimationComponent::create( step_mole::animation::GetObjectInfoContainer() );
				view_node->addComponent( animation_component );

				// Collision Component
				auto circle_collision_component = step_mole::CircleCollisionComponent::create( 30.f, Vec2::ZERO, CircleCollisionComponentConfig{ true, true, true } );
				object_node->addComponent( circle_collision_component );

				// Object Component
				object_node->addComponent( step_mole::ObjectComponent::create( animation_component, circle_collision_component, nullptr ) );
			}

			return true;
		}

		void ObjectActionScene::onEnter()
		{
			Scene::onEnter();

			assert( !mKeyboardListener );
			mKeyboardListener = EventListenerKeyboard::create();
			mKeyboardListener->onKeyPressed = CC_CALLBACK_2( ObjectActionScene::onKeyPressed, this );
			getEventDispatcher()->addEventListenerWithFixedPriority( mKeyboardListener, 1 );
		}
		void ObjectActionScene::onExit()
		{
			assert( mKeyboardListener );
			getEventDispatcher()->removeEventListener( mKeyboardListener );
			mKeyboardListener = nullptr;

			Node::onExit();
		}

		void ObjectActionScene::updateLifeTimeView()
		{
			auto life_time_node = static_cast<Label*>( getChildByTag( TAG_LifeTimeNode ) );
			life_time_node->setString( StringUtils::format( "Life Time : %d", mCurrentLifeTime ) );
		}

		void ObjectActionScene::onKeyPressed( EventKeyboard::KeyCode keycode, Event* /*event*/ )
		{
			switch( keycode )
			{
			case EventKeyboard::KeyCode::KEY_ESCAPE:
				helper::BackToThePreviousScene::MoveBack();
				return;

			case EventKeyboard::KeyCode::KEY_1:
			{
				auto object_node = getChildByTag( TAG_ObjectNode );
				auto animation_component = static_cast<step_mole::ObjectComponent*>( object_node->getComponent( step_mole::ObjectComponent::GetStaticName() ) );
				animation_component->ProcessStart( mCurrentLifeTime );
			}
			return;

			case EventKeyboard::KeyCode::KEY_2:
			{
				auto object_node = getChildByTag( TAG_ObjectNode );
				auto animation_component = static_cast<step_mole::ObjectComponent*>( object_node->getComponent( step_mole::ObjectComponent::GetStaticName() ) );
				animation_component->ProcessDamage();
			}
			return;

			case EventKeyboard::KeyCode::KEY_UP_ARROW:
				mCurrentLifeTime += 1;
				updateLifeTimeView();
				return;

			case EventKeyboard::KeyCode::KEY_DOWN_ARROW:
				mCurrentLifeTime = std::max( 1, mCurrentLifeTime - 1 );
				updateLifeTimeView();
				return;

			default:
				CCLOG( "Key Code : %d", keycode );
			}
		}
	}
}