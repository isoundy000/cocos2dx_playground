#include "Research_Input_KeyConfigScene.h"

#include <sstream>

#include "RootScene.h"
#include "CPG_InputDelegator.h"
#include "CPG_Input_BasicCollector.h"
#include "CPG_InputKeyMap.h"

USING_NS_CC;

namespace Research
{
	namespace Input
	{
		Scene* KeyConfigScene::create()
		{
			auto ret = new ( std::nothrow ) KeyConfigScene();
			if( !ret || !ret->init() )
			{
				delete ret;
				ret = nullptr;
				return nullptr;
			}

			const auto visibleSize = Director::getInstance()->getVisibleSize();
			const Vec2 origin = Director::getInstance()->getVisibleOrigin();

			//
			// summury
			//
			{
				std::stringstream ss;
				ss << "+ Input : Key Config Scene";
				ss << "\n";
				ss << "\n";
				ss << "[ESC] : Exit";
				ss << "\n";
				ss << "\n";
				ss << "<Config File Path> : " << cocos2d::FileUtils::getInstance()->getWritablePath();

				auto label = Label::createWithTTF( ss.str(), "fonts/arial.ttf", 9, Size::ZERO, TextHAlignment::LEFT );
				label->setColor( Color3B::GREEN );
				label->setAnchorPoint( Vec2( 0.f, 1.f ) );
				label->setPosition( Vec2(
					origin.x
					, origin.y + visibleSize.height
				) );
				ret->addChild( label, 9999 );
			}

			//
			// input
			//
			{
				auto input_delegator = CPG::Input::Delegator::create( "research_input_allowedKeysTest_allowed_keys.json" );
				ret->addChild( input_delegator, 0 );

				auto key_map = CPG::Input::KeyMap::create( "research_input_keyconfigscene_keymap.json" );

				ret->input_collector = CPG::Input::BasicCollector::create( key_map );
				input_delegator->addInputCollector( ret->input_collector );
			}

			//
			// Setup Key Config Controls
			//
			{
				ret->keymap_config_helper.load( "research_input_keyconfigscene_keymap.json" );

				for( const auto& h : ret->keymap_config_helper.getContainer() )
					CCLOG( "%s", h.name );
			}


			ret->autorelease();
			ret->scheduleUpdate();

			return ret;
		}

		void KeyConfigScene::update( float dt )
		{
			if( input_collector->getKeyStatus( cocos2d::EventKeyboard::KeyCode::KEY_ESCAPE ) )
			{
				Director::getInstance()->replaceScene( RootScene::create() );
			}
		}
	}
}
