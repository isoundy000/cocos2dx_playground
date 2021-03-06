#pragma once

#include "2d/CCScene.h"
#include "ui/UIWidget.h"

namespace step_clickclick
{
	namespace button
	{
		class BasicScene : public cocos2d::Scene
		{
		private:
			BasicScene();

		public:
			static const char* getTitle() { return "Button : Basic"; }
			static cocos2d::Scene* create();

			bool init() override;
			void onEnter() override;
			void onExit() override;

		private:
			void onButton( Ref* sender, cocos2d::ui::Widget::TouchEventType touch_event_type );
			void updateView_ButtonStatus( const char* status_string );
			void updateView_TouchPosition( const cocos2d::Vec2 touch_position );

			void updateForExit( float dt );
			void onKeyPressed( cocos2d::EventKeyboard::KeyCode keycode, cocos2d::Event* /*event*/ );

		private:
			cocos2d::EventListenerKeyboard* mKeyboardListener;
		};
	}
}
