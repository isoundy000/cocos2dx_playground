#include "step_rain_of_chaos_game_test_StageNodeScene.h"

#include <new>
#include <numeric>
#include <sstream>

#include "2d/CCLabel.h"
#include "2d/CCLayer.h"
#include "base/CCDirector.h"
#include "base/CCEventListenerKeyboard.h"
#include "base/CCEventDispatcher.h"
#include "base/ccUTF8.h"

#include "step_mole_CircleCollisionComponentConfig.h"
#include "step_rain_of_chaos_game_StageNode.h"
#include "step_rain_of_chaos_game_BulletManager.h"

USING_NS_CC;

namespace
{
	const int TAG_MoveSpeedView = 20140416;
	const int BulletCachingAmount = 100;
	const int TAG_FireAmountView = 20160528;
}

namespace step_rain_of_chaos
{
	namespace game_test
	{
		StageNodeScene::StageNodeScene( const helper::FuncSceneMover& back_to_the_previous_scene_callback ) :
			helper::BackToThePreviousScene( back_to_the_previous_scene_callback )
			, mKeyboardListener( nullptr )
			, mStageConfig()
			, mBulletManager( nullptr )
			, mStageNode( nullptr )
			, mCurrentMoveSpeed( 3 )
			, mCurrentFireAmount( 1 )
		{}

		Scene* StageNodeScene::create( const helper::FuncSceneMover& back_to_the_previous_scene_callback )
		{
			auto ret = new ( std::nothrow ) StageNodeScene( back_to_the_previous_scene_callback );
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

		bool StageNodeScene::init()
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
				ss << "[A] : Do Bullet";
				ss << std::endl;
				ss << std::endl;
				ss << "[Arrow Key - Up] : Move Speed - Increase";
				ss << std::endl;
				ss << "[Arrow Key - Down] : Move Speed - Decrease";
				ss << std::endl;
				ss << std::endl;
				ss << "[Arrow Key - Right] : Fire Amount - Increase";
				ss << std::endl;
				ss << "[Arrow Key - Left] : Fire Amount - Decrease";

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
				auto background_layer = LayerColor::create( Color4B( 63, 23, 14, 255 ) );
				addChild( background_layer, std::numeric_limits<int>::min() );
			}

			//
			// Current Move Speed
			//
			{
				auto label = Label::createWithTTF( "", "fonts/NanumSquareR.ttf", 12, Size::ZERO, TextHAlignment::LEFT );
				label->setTag( TAG_MoveSpeedView );
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
			// Spawn Target Count
			//
			{
				auto label = Label::createWithTTF( "Temp", "fonts/NanumSquareR.ttf", 12, Size::ZERO, TextHAlignment::LEFT );
				label->setTag( TAG_FireAmountView );
				label->setAnchorPoint( Vec2( 1.f, 1.f ) );
				label->setColor( Color3B::GREEN );
				label->setPosition( Vec2(
					visibleOrigin.x + visibleSize.width
					, visibleOrigin.y + visibleSize.height - label->getContentSize().height
				) );
				addChild( label, std::numeric_limits<int>::max() );

				updateFireAmountView();
			}

			//
			// Target Manager
			//
			{
				mBulletManager = game::BulletManager::create( BulletCachingAmount );
			}

			//
			// Stage Node
			//
			{
				mStageConfig.Build(
					visibleOrigin.x + visibleSize.width * 0.5f, visibleOrigin.y + visibleSize.height * 0.5f
					, 300.f, 160.f
				);

				mStageNode = game::StageNode::create(
					mStageConfig
					, game::StageNode::DebugConfig{ true, true }
					, mBulletManager->GetComeHomeCallback()
					, step_mole::CircleCollisionComponentConfig { true, true, true }
					, BulletCachingAmount
				);
				addChild( mStageNode );
			}

			return true;
		}

		void StageNodeScene::onEnter()
		{
			Scene::onEnter();

			assert( !mKeyboardListener );
			mKeyboardListener = EventListenerKeyboard::create();
			mKeyboardListener->onKeyPressed = CC_CALLBACK_2( StageNodeScene::onKeyPressed, this );
			getEventDispatcher()->addEventListenerWithSceneGraphPriority( mKeyboardListener, this );
		}
		void StageNodeScene::onExit()
		{
			assert( mKeyboardListener );
			getEventDispatcher()->removeEventListener( mKeyboardListener );
			mKeyboardListener = nullptr;

			Scene::onExit();
		}

		void StageNodeScene::updateMoveSpeedView()
		{
			auto label = static_cast<Label*>( getChildByTag( TAG_MoveSpeedView ) );
			label->setString( StringUtils::format( "Move Speed : %d", mCurrentMoveSpeed ) );
		}
		void StageNodeScene::updateFireAmountView()
		{
			auto label = static_cast<Label*>( getChildByTag( TAG_FireAmountView ) );
			label->setString( StringUtils::format( "Fire Count : %d", mCurrentFireAmount ) );
		}
		void StageNodeScene::onKeyPressed( EventKeyboard::KeyCode keycode, Event* /*event*/ )
		{
			switch( keycode )
			{
			case EventKeyboard::KeyCode::KEY_ESCAPE:
				helper::BackToThePreviousScene::MoveBack();
				return;

			case EventKeyboard::KeyCode::KEY_A:
			{
				Vec2 offset;

				int target_index = -1;
				for( int i = 0; i < mCurrentFireAmount; ++i )
				{
					target_index = mBulletManager->GetIdleTarget();
					if( -1 == target_index )
					{
						mBulletManager->RequestGenerate( 50 );
						mStageNode->RequestGenerate( 50 );

						target_index = mBulletManager->GetIdleTarget();
						if( -1 == target_index )
						{
							break;
						}
					}

					Vec2 dir = Vec2( mStageConfig.GetStageArea().getMaxX(), mStageConfig.GetStageArea().getMaxY() ) - mStageConfig.GetStageArea().origin;
					dir.normalize();
					dir.scale( mCurrentMoveSpeed );
					mStageNode->RequestAction( target_index, Vec2( mStageConfig.GetStageArea().origin ) + offset, dir );

					offset.y += 10.f;
				}

				return;
			}

			case EventKeyboard::KeyCode::KEY_UP_ARROW:
				mCurrentMoveSpeed += 1;
				updateMoveSpeedView();
				return;
			case EventKeyboard::KeyCode::KEY_DOWN_ARROW:
				mCurrentMoveSpeed = std::max( 1, mCurrentMoveSpeed - 1 );
				updateMoveSpeedView();
				return;

			case EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
				mCurrentFireAmount += 1;
				updateFireAmountView();
				return;
			case EventKeyboard::KeyCode::KEY_LEFT_ARROW:
				mCurrentFireAmount = std::max( 1, mCurrentFireAmount - 1 );
				updateFireAmountView();
				return;

			default:
				CCLOG( "Key Code : %d", keycode );
			}
		}
	}
}