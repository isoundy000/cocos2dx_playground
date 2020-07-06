#include "step_rain_of_chaos_game_test_BulletLifeComponentScene.h"

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
#include "ui/UIScale9Sprite.h"

#include "step_mole_AnimationComponent.h"

#include "step_mole_animation_InfoContainer.h"
#include "step_mole_CircleCollisionComponent.h"
#include "step_mole_CircleCollisionComponentConfig.h"
#include "step_rain_of_chaos_BulletLifeComponent.h"

USING_NS_CC;

namespace
{
	const int TAG_ObjectNode = 20140416;
	const int TAG_LifeTimeNode = 100;

#pragma region Clamp from c++17
	template<class T, class Compare>
	constexpr const T& clamp( const T& v, const T& lo, const T& hi, Compare comp )
	{
		return CCASSERT( !comp( hi, lo ), "" ), comp( v, lo ) ? lo : comp( hi, v ) ? hi : v;
	}

	template<class T>
	constexpr const T& clamp( const T& v, const T& lo, const T& hi )
	{
		return clamp( v, lo, hi, std::less<>() );
	}
#pragma endregion
}

namespace step_rain_of_chaos
{
	namespace game_test
	{
		BulletLifeComponentScene::BulletLifeComponentScene( const helper::FuncSceneMover& back_to_the_previous_scene_callback ) :
			helper::BackToThePreviousScene( back_to_the_previous_scene_callback )
			, mKeyboardListener( nullptr )
			, mBulletLifeArea()
			, mBulletGenerateArea()
			, mCurrentMoveSpeed( 3 )
		{}

		Scene* BulletLifeComponentScene::create( const helper::FuncSceneMover& back_to_the_previous_scene_callback )
		{
			auto ret = new ( std::nothrow ) BulletLifeComponentScene( back_to_the_previous_scene_callback );
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

		bool BulletLifeComponentScene::init()
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
				auto background_layer = LayerColor::create( Color4B( 3, 20, 70, 255 ) );
				addChild( background_layer, std::numeric_limits<int>::min() );
			}

			mCenter.set( visibleOrigin.x + visibleSize.width * 0.5f, visibleOrigin.y + visibleSize.height * 0.5f );
			const Rect dummy_stage_area(
				mCenter.x - 150.f, mCenter.y - 80.f
				, 300.f, 160.f
			);
			mBulletLifeArea.setRect(
				dummy_stage_area.origin.x - 60.f, dummy_stage_area.origin.y - 60.f
				, dummy_stage_area.size.width + 60.f + 60.f, dummy_stage_area.size.height + 60.f + 60.f
			);
			mBulletGenerateArea.setRect(
				dummy_stage_area.origin.x - 30.f, dummy_stage_area.origin.y - 30.f
				, dummy_stage_area.size.width + 30.f + 30.f, dummy_stage_area.size.height + 30.f + 30.f
			);
			
			//
			// Dummy Stage View
			//
			{
				auto sprite = ui::Scale9Sprite::createWithSpriteFrameName( "guide_01_3.png" );
				sprite->setAnchorPoint( Vec2::ZERO );
				sprite->setContentSize( dummy_stage_area.size );
				sprite->setColor( Color3B::GREEN );
				sprite->setPosition( dummy_stage_area.origin );
				addChild( sprite );
			}

			//
			// Bullet Life Area View
			//
			{
				auto sprite = ui::Scale9Sprite::createWithSpriteFrameName( "guide_01_3.png" );
				sprite->setAnchorPoint( Vec2::ZERO );
				sprite->setContentSize( mBulletLifeArea.size );
				sprite->setColor( Color3B::RED );
				sprite->setPosition( mBulletLifeArea.origin );
				addChild( sprite );
			}

			//
			// Bullet Generate Area View
			//
			{
				auto sprite = ui::Scale9Sprite::createWithSpriteFrameName( "guide_01_3.png" );
				sprite->setAnchorPoint( Vec2::ZERO );
				sprite->setContentSize( mBulletGenerateArea.size );
				sprite->setPosition( mBulletGenerateArea.origin );
				addChild( sprite );
			}

			//
			// Current Move Speed
			//
			{
				auto label = Label::createWithTTF( "", "fonts/NanumSquareR.ttf", 12, Size::ZERO, TextHAlignment::LEFT );
				label->setTag( TAG_LifeTimeNode );
				label->setAnchorPoint( Vec2( 1.f, 1.f ) );
				label->setColor( Color3B::GREEN );
				label->setPosition( Vec2(
					visibleOrigin.x + visibleSize.width
					, visibleOrigin.y + visibleSize.height
				) );
				addChild( label, std::numeric_limits<int>::max() );

				updateMoveSpeedView();
			}

			//
			// Object Node
			//
			{
				auto object_node = Node::create();
				object_node->setTag( TAG_ObjectNode );
				object_node->setPosition( Vec2(
					static_cast<int>( visibleOrigin.x + dummy_stage_area.origin.x + ( dummy_stage_area.size.width * 0.5f ) )
					, static_cast<int>( visibleOrigin.y + dummy_stage_area.origin.y + ( dummy_stage_area.size.height * 0.5f ) )
				) );
				addChild( object_node, 1 );

				// Pivot
				{
					auto pivot = Sprite::createWithSpriteFrameName( "helper_pivot.png" );
					pivot->setScale( _director->getContentScaleFactor() );
					object_node->addChild( pivot, std::numeric_limits<int>::max() );
				}

				// View
				auto view_node = Sprite::createWithSpriteFrameName( "step_mole_target_wait_0.png" );
				view_node->setAnchorPoint( Vec2( 0.5f, 0.f ) );
				view_node->setScale( _director->getContentScaleFactor() );
				view_node->setPositionY( -18.f );
				object_node->addChild( view_node );

				// Animation Component
				auto animation_component = step_mole::AnimationComponent::create( step_mole::animation::GetObjectInfoContainer() );
				view_node->addComponent( animation_component );

				// Collision Component
				auto circle_collision_component = step_mole::CircleCollisionComponent::create( 30.f, Vec2::ZERO, step_mole::CircleCollisionComponentConfig{ true, true, true } );
				object_node->addComponent( circle_collision_component );

				// Object Component
				object_node->addComponent( step_rain_of_chaos::BulletLifeComponent::create( mBulletLifeArea, animation_component, circle_collision_component, nullptr ) );
			}

			return true;
		}

		void BulletLifeComponentScene::onEnter()
		{
			Scene::onEnter();

			assert( !mKeyboardListener );
			mKeyboardListener = EventListenerKeyboard::create();
			mKeyboardListener->onKeyPressed = CC_CALLBACK_2( BulletLifeComponentScene::onKeyPressed, this );
			getEventDispatcher()->addEventListenerWithSceneGraphPriority( mKeyboardListener, this );
		}
		void BulletLifeComponentScene::onExit()
		{
			assert( mKeyboardListener );
			getEventDispatcher()->removeEventListener( mKeyboardListener );
			mKeyboardListener = nullptr;

			Scene::onExit();
		}

		void BulletLifeComponentScene::updateMoveSpeedView()
		{
			auto life_time_node = static_cast<Label*>( getChildByTag( TAG_LifeTimeNode ) );
			life_time_node->setString( StringUtils::format( "Move Speed : %d", mCurrentMoveSpeed ) );
		}

		void BulletLifeComponentScene::onKeyPressed( EventKeyboard::KeyCode keycode, Event* /*event*/ )
		{
			switch( keycode )
			{
			case EventKeyboard::KeyCode::KEY_ESCAPE:
				helper::BackToThePreviousScene::MoveBack();
				return;

			case EventKeyboard::KeyCode::KEY_1:
			{
				Vec2 pivot_vector( mBulletGenerateArea.size.width * 0.5f, mBulletGenerateArea.size.height * 0.5f );
				Vec2 direction_vector = pivot_vector;

				static std::mt19937 randomEngine( std::random_device{}() );
				std::uniform_real_distribution<> dist( 0, 360 );
				direction_vector.rotate( Vec2::ZERO, CC_DEGREES_TO_RADIANS( dist( randomEngine ) ) );				
				direction_vector.x = clamp( direction_vector.x, -pivot_vector.x, pivot_vector.x );
				direction_vector.y = clamp( direction_vector.y, -pivot_vector.y, pivot_vector.y );

				const auto start_position = mCenter + direction_vector;

				direction_vector.normalize();
				direction_vector.scale( mCurrentMoveSpeed );

				auto object_node = getChildByTag( TAG_ObjectNode );
				object_node->setPosition( start_position );
				auto animation_component = static_cast<step_rain_of_chaos::BulletLifeComponent*>( object_node->getComponent( step_rain_of_chaos::BulletLifeComponent::GetStaticName() ) );
				animation_component->ProcessStart( -direction_vector );
			}
			return;

			case EventKeyboard::KeyCode::KEY_2:
			{
				auto object_node = getChildByTag( TAG_ObjectNode );
				auto animation_component = static_cast<step_rain_of_chaos::BulletLifeComponent*>( object_node->getComponent( step_rain_of_chaos::BulletLifeComponent::GetStaticName() ) );
				animation_component->ProcessBoom();
			}
			return;

			case EventKeyboard::KeyCode::KEY_UP_ARROW:
				mCurrentMoveSpeed += 1;
				updateMoveSpeedView();
				return;

			case EventKeyboard::KeyCode::KEY_DOWN_ARROW:
				mCurrentMoveSpeed = std::max( 1, mCurrentMoveSpeed - 1 );
				updateMoveSpeedView();
				return;

			default:
				CCLOG( "Key Code : %d", keycode );
			}
		}
	}
}
