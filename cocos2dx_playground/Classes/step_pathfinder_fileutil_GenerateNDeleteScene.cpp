#include "step_pathfinder_fileutil_GenerateNDeleteScene.h"

#include <new>
#include <numeric>
#include <sstream>

#include "2d/CCLabel.h"
#include "2d/CCLayer.h"
#include "base/CCDirector.h"
#include "base/CCEventListenerKeyboard.h"
#include "base/CCEventDispatcher.h"
#include "platform/CCFileUtils.h"

USING_NS_CC;

namespace
{
	const char* FontPath = "fonts/arial.ttf";

	const int TAG_FileStatus = 20140416;

	const char* FilePath_for_Generate_N_Delete = "step_pathfinder_fileutil_GenerateNDeleteScene.txt";
}

namespace step_pathfinder
{
	namespace fileutil
	{
		GenerateNDeleteScene::GenerateNDeleteScene( const helper::FuncSceneMover& back_to_the_previous_scene_callback ) :
			helper::BackToThePreviousScene( back_to_the_previous_scene_callback )
			, mKeyboardListener( nullptr )
		{}

		Scene* GenerateNDeleteScene::create( const helper::FuncSceneMover& back_to_the_previous_scene_callback )
		{
			auto ret = new ( std::nothrow ) GenerateNDeleteScene( back_to_the_previous_scene_callback );
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

		bool GenerateNDeleteScene::init()
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

				auto label = Label::createWithTTF( ss.str(), FontPath, 9, Size::ZERO, TextHAlignment::LEFT );
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
				auto background_layer = LayerColor::create( Color4B( 41, 0, 61, 255 ) );
				addChild( background_layer, std::numeric_limits<int>::min() );
			}

			//
			// Target Path
			//
			{
				const float MAX_LINE_WIDTH = visibleSize.width * 0.8f;

				// Title
				{
					auto label = Label::createWithTTF( "<Target Path>", FontPath, 9 );
					label->setAnchorPoint( Vec2( 0.f, 0.f ) );
					label->setPosition( Vec2(
						visibleOrigin.x + visibleSize.width * 0.1f
						, visibleOrigin.y + visibleSize.height * 0.7f
					) );
					addChild( label );
				}

				// Path
				{
					auto label = Label::createWithTTF( cocos2d::FileUtils::getInstance()->getWritablePath().c_str(), FontPath, 9 );
					label->setAnchorPoint( Vec2( 0.f, 1.f ) );
					label->setColor( Color3B::GREEN );
					label->setMaxLineWidth( MAX_LINE_WIDTH );
					label->setPosition( Vec2(
						visibleOrigin.x + visibleSize.width * 0.1f
						, visibleOrigin.y + visibleSize.height * 0.7f
					) );
					addChild( label );
				}
			}

			//
			// File Name
			//
			{
				// Title
				{
					auto label = Label::createWithTTF( "<File Name>", FontPath, 9 );
					label->setAnchorPoint( Vec2( 0.f, 0.f ) );
					label->setPosition( Vec2(
						visibleOrigin.x + visibleSize.width * 0.1f
						, visibleOrigin.y + visibleSize.height * 0.5f
					) );
					addChild( label );
				}

				// Name
				{
					auto label = Label::createWithTTF( FilePath_for_Generate_N_Delete, FontPath, 9 );
					label->setAnchorPoint( Vec2( 0.f, 1.f ) );
					label->setColor( Color3B::GREEN );
					label->setPosition( Vec2(
						visibleOrigin.x + visibleSize.width * 0.1f
						, visibleOrigin.y + visibleSize.height * 0.5f
					) );
					addChild( label );
				}
			}

			//
			// File Status
			//
			{
				// Title
				{
					auto label = Label::createWithTTF( "<File Status>", FontPath, 9 );
					label->setAnchorPoint( Vec2( 0.f, 0.f ) );
					label->setPosition( Vec2(
						visibleOrigin.x + visibleSize.width * 0.6f
						, visibleOrigin.y + visibleSize.height * 0.5f
					) );
					addChild( label );
				}

				// Status
				{
					auto label = Label::createWithTTF( "", FontPath, 9 );
					label->setTag( TAG_FileStatus );
					label->setAnchorPoint( Vec2( 0.f, 1.f ) );
					label->setColor( Color3B::RED );
					label->setPosition( Vec2(
						visibleOrigin.x + visibleSize.width * 0.6f
						, visibleOrigin.y + visibleSize.height * 0.5f
					) );
					addChild( label );
				}

				updateFileStatus();
			}

			return true;
		}

		void GenerateNDeleteScene::onEnter()
		{
			Scene::onEnter();

			assert( !mKeyboardListener );
			mKeyboardListener = EventListenerKeyboard::create();
			mKeyboardListener->onKeyPressed = CC_CALLBACK_2( GenerateNDeleteScene::onKeyPressed, this );
			getEventDispatcher()->addEventListenerWithSceneGraphPriority( mKeyboardListener, this );
		}
		void GenerateNDeleteScene::onExit()
		{
			assert( mKeyboardListener );
			getEventDispatcher()->removeEventListener( mKeyboardListener );
			mKeyboardListener = nullptr;

			Node::onExit();
		}

		void GenerateNDeleteScene::updateFileStatus()
		{
			std::string path( std::move( cocos2d::FileUtils::getInstance()->getWritablePath() ) );
			path.append( FilePath_for_Generate_N_Delete );

			auto label = static_cast<Label*>( getChildByTag( TAG_FileStatus ) );

			if( cocos2d::FileUtils::getInstance()->isFileExist( path ) )
			{
				label->setString( "Exist" );
			}
			else
			{
				label->setString( "Nothing" );
			}
		}

		void GenerateNDeleteScene::onKeyPressed( EventKeyboard::KeyCode keycode, Event* /*event*/ )
		{
			if( EventKeyboard::KeyCode::KEY_ESCAPE == keycode )
			{
				helper::BackToThePreviousScene::MoveBack();
				return;
			}
		}
	}
}
